#include "battleengine.h"

#include "../models/tile.h"
#include "../models/unit.h"
#include "../models/enums/terraintype.h"

#include <QQueue>
#include <QSet>
#include <QPair>
#include <QHash>
#include <limits>
#include <cmath>

namespace
{
struct MoveNode
{
    int x;
    int y;
    int spentCost;
};

QString makeKey(int x, int y)
{
    return QString("%1_%2").arg(x).arg(y);
}
}

bool BattleEngine::canSelectUnit(const GameState& gameState, const Unit* unit) const
{
    if (!unit || !unit->isAlive())
        return false;

    if (unit->getSide() != gameState.getCurrentSide())
        return false;

    return true;
}

int BattleEngine::calculateDistance(int x1, int y1, int x2, int y2) const
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

int BattleEngine::calculateLowestMoveCost(const GameState& gameState,
                                          const Unit& unit,
                                          int startX,
                                          int startY,
                                          int targetX,
                                          int targetY) const
{
    if (startX == targetX && startY == targetY)
        return 0;

    QQueue<MoveNode> queue;
    QHash<QString, int> bestCost;

    queue.enqueue({ startX, startY, 0 });
    bestCost.insert(makeKey(startX, startY), 0);

    const int directions[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

    while (!queue.isEmpty())
    {
        const MoveNode current = queue.dequeue();

        for (const auto& direction : directions)
        {
            const int nextX = current.x + direction[0];
            const int nextY = current.y + direction[1];

            if (!gameState.getBoard().isInsideBoard(nextX, nextY))
                continue;

            const Tile* nextTile = gameState.getBoard().getTile(nextX, nextY);
            if (!nextTile || !nextTile->isWalkable())
                continue;

            if (nextTile->isOccupied() && !(nextX == targetX && nextY == targetY))
                continue;

            const int nextCost = current.spentCost + nextTile->getMovementCost() * unit.getMoveCostPerTile();
            const QString key = makeKey(nextX, nextY);

            if (bestCost.contains(key) && bestCost.value(key) <= nextCost)
                continue;

            bestCost.insert(key, nextCost);

            if (nextX == targetX && nextY == targetY)
                return nextCost;

            queue.enqueue({ nextX, nextY, nextCost });
        }
    }

    return std::numeric_limits<int>::max();
}

void BattleEngine::calculateActionHighlights(GameState& gameState, int startX, int startY) const
{
    QVector<QPair<int, int>> availablePositions;
    QVector<QPair<int, int>> blockedPositions;
    QVector<QPair<int, int>> attackablePositions;
    QVector<QPair<int, int>> healablePositions;

    const Tile* startTile = gameState.getBoard().getTile(startX, startY);
    if (!startTile || !startTile->isOccupied() || !startTile->getUnit())
    {
        gameState.clearAllHighlights();
        return;
    }

    const Unit* unit = startTile->getUnit();
    const Board& board = gameState.getBoard();

    if (unit->canMoveNow())
    {
        const int maxAffordableCost = gameState.getCurrentTurnActionPoints();

        QQueue<MoveNode> queue;
        QSet<QString> availableSet;
        QSet<QString> blockedSet;
        QHash<QString, int> bestCost;

        queue.enqueue({ startX, startY, 0 });
        bestCost.insert(makeKey(startX, startY), 0);

        const int directions[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

        while (!queue.isEmpty())
        {
            const MoveNode current = queue.dequeue();

            for (const auto& direction : directions)
            {
                const int nextX = current.x + direction[0];
                const int nextY = current.y + direction[1];

                if (!board.isInsideBoard(nextX, nextY))
                    continue;

                const Tile* nextTile = board.getTile(nextX, nextY);
                if (!nextTile)
                    continue;

                const QString key = makeKey(nextX, nextY);

                if (!nextTile->isWalkable())
                {
                    if (!blockedSet.contains(key))
                    {
                        blockedPositions.append({ nextX, nextY });
                        blockedSet.insert(key);
                    }
                    continue;
                }

                if (nextTile->isOccupied())
                {
                    if (!blockedSet.contains(key))
                    {
                        blockedPositions.append({ nextX, nextY });
                        blockedSet.insert(key);
                    }
                    continue;
                }

                const int nextCost = current.spentCost + nextTile->getMovementCost() * unit->getMoveCostPerTile();
                if (nextCost > maxAffordableCost)
                    continue;

                if (bestCost.contains(key) && bestCost.value(key) <= nextCost)
                    continue;

                bestCost.insert(key, nextCost);

                if (calculateDistance(startX, startY, nextX, nextY) <= unit->getMovementPoints())
                {
                    if (!availableSet.contains(key))
                    {
                        availablePositions.append({ nextX, nextY });
                        availableSet.insert(key);
                    }
                }

                queue.enqueue({ nextX, nextY, nextCost });
            }
        }
    }

    if (unit->canAttackNow() && unit->getAttackCost() <= gameState.getCurrentTurnActionPoints())
    {
        for (int y = 0; y < board.getHeight(); ++y)
        {
            for (int x = 0; x < board.getWidth(); ++x)
            {
                const Tile* targetTile = board.getTile(x, y);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive() || targetUnit->getSide() == unit->getSide())
                    continue;

                if (m_attackResolver.canAttack(*unit,
                                               startX,
                                               startY,
                                               *targetUnit,
                                               x,
                                               y,
                                               startTile->getTerrain()))
                {
                    attackablePositions.append({ x, y });
                }
            }
        }
    }

    if (unit->canHealNow() && unit->getAttackCost() <= gameState.getCurrentTurnActionPoints())
    {
        for (int y = 0; y < board.getHeight(); ++y)
        {
            for (int x = 0; x < board.getWidth(); ++x)
            {
                const Tile* targetTile = board.getTile(x, y);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive() || targetUnit->getSide() != unit->getSide())
                    continue;

                if (!targetUnit->isDamaged())
                    continue;

                const int distance = calculateDistance(startX, startY, x, y);
                if (distance >= unit->getMinRange() && distance <= unit->getRange())
                    healablePositions.append({ x, y });
            }
        }
    }

    gameState.setAvailableMovePositions(availablePositions);
    gameState.setBlockedMovePositions(blockedPositions);
    gameState.setAttackablePositions(attackablePositions);
    gameState.setHealablePositions(healablePositions);
}

bool BattleEngine::tryMoveSelectedUnit(GameState& gameState, int targetX, int targetY)
{
    if (!gameState.hasSelectedPosition())
        return false;

    if (!gameState.isMovePositionAvailable(targetX, targetY))
    {
        gameState.setLastActionMessage("To pole nie jest dostępne do ruchu. Kliknij zielone pole.");
        return true;
    }

    Tile* sourceTile = gameState.getBoard().getTile(
        gameState.getSelectedX(),
        gameState.getSelectedY());

    Tile* targetTile = gameState.getBoard().getTile(targetX, targetY);

    if (!sourceTile || !targetTile || !sourceTile->isOccupied() || targetTile->isOccupied() || !targetTile->isWalkable())
        return false;

    Unit* unit = sourceTile->getUnit();
    if (!unit || !unit->canMoveNow())
        return false;

    const int movementDistance = calculateDistance(sourceTile->getX(), sourceTile->getY(), targetX, targetY);
    if (movementDistance <= 0 || movementDistance > unit->getMovementPoints())
        return false;

    const int movementCost = calculateLowestMoveCost(
        gameState,
        *unit,
        sourceTile->getX(),
        sourceTile->getY(),
        targetX,
        targetY);

    if (movementCost == std::numeric_limits<int>::max())
    {
        gameState.setLastActionMessage("Nie znaleziono poprawnej ścieżki ruchu.");
        return true;
    }

    if (movementCost > gameState.getCurrentTurnActionPoints())
    {
        gameState.setLastActionMessage(
            QString("Za mało punktów akcji. Ruch %1 kosztuje %2 AP.")
                .arg(unit->getName())
                .arg(movementCost));
        return true;
    }

    sourceTile->removeUnit();
    targetTile->setUnit(unit);

    unit->markMoved();

    gameState.consumeTurnActionPoints(movementCost);
    gameState.setSelectedPosition(targetX, targetY);
    calculateActionHighlights(gameState, targetX, targetY);

    gameState.setLastActionMessage(
        QString("%1 przesunął się. Koszt ruchu: %2 AP. Pozostałe AP drużyny: %3/%4. Możesz dalej działać tą samą jednostką, jeśli starczy AP.")
            .arg(unit->getName())
            .arg(movementCost)
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints()));

    finishActionAndMaybeEndTurn(gameState);
    return true;
}

void BattleEngine::clearDefeatedUnitFromTile(Tile& tile) const
{
    Unit* unit = tile.getUnit();
    if (unit && !unit->isAlive())
        tile.removeUnit();
}

bool BattleEngine::tryHealSelectedUnit(GameState& gameState, int targetX, int targetY)
{
    if (!gameState.hasSelectedPosition())
        return false;

    Tile* healerTile = gameState.getBoard().getTile(gameState.getSelectedX(), gameState.getSelectedY());
    Tile* targetTile = gameState.getBoard().getTile(targetX, targetY);

    if (!healerTile || !targetTile || !healerTile->isOccupied() || !targetTile->isOccupied())
        return false;

    Unit* healer = healerTile->getUnit();
    Unit* target = targetTile->getUnit();

    if (!healer || !target || !healer->canHealNow() || !healer->canHeal())
        return false;

    if (healer->getSide() != target->getSide())
        return false;

    if (!gameState.isHealablePosition(targetX, targetY))
    {
        if (!target->isDamaged())
            gameState.setLastActionMessage("Ta jednostka nie wymaga leczenia.");
        else
            gameState.setLastActionMessage("Nie możesz uleczyć tego celu. Kliknij niebiesko oznaczonego sojusznika.");
        return true;
    }

    const int actionCost = healer->getAttackCost();
    if (actionCost > gameState.getCurrentTurnActionPoints())
    {
        gameState.setLastActionMessage(
            QString("Za mało punktów akcji. Leczenie kosztuje %1 AP.")
                .arg(actionCost));
        return true;
    }

    const int before = target->getHealth();
    target->heal(healer->getHealAmount());
    const int healed = target->getHealth() - before;

    healer->markActed();
    gameState.consumeTurnActionPoints(actionCost);
    gameState.setSelectedPosition(healerTile->getX(), healerTile->getY());
    calculateActionHighlights(gameState, healerTile->getX(), healerTile->getY());

    gameState.setLastActionMessage(
        QString("%1 uleczył %2 o %3 HP. Stan celu: %4/%5 HP. Pozostałe AP drużyny: %6/%7. Możesz wykonać kolejną akcję, jeśli starczy AP.")
            .arg(healer->getName())
            .arg(target->getName())
            .arg(healed)
            .arg(target->getHealth())
            .arg(target->getMaxHealth())
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints()));

    finishActionAndMaybeEndTurn(gameState);
    return true;
}

bool BattleEngine::tryAttackSelectedUnit(GameState& gameState, int targetX, int targetY)
{
    if (!gameState.hasSelectedPosition())
        return false;

    Tile* attackerTile = gameState.getBoard().getTile(
        gameState.getSelectedX(),
        gameState.getSelectedY());
    Tile* defenderTile = gameState.getBoard().getTile(targetX, targetY);

    if (!attackerTile || !defenderTile || !attackerTile->isOccupied() || !defenderTile->isOccupied())
        return false;

    Unit* attacker = attackerTile->getUnit();
    Unit* defender = defenderTile->getUnit();

    if (!attacker || !defender)
        return false;

    if (!attacker->canAttackNow())
        return false;

    if (attacker->getSide() == defender->getSide())
        return false;

    const int attackCost = attacker->getAttackCost();
    if (attackCost > gameState.getCurrentTurnActionPoints())
    {
        gameState.setLastActionMessage(
            QString("Za mało punktów akcji. Atak jednostki %1 kosztuje %2 AP.")
                .arg(attacker->getName())
                .arg(attackCost));
        return true;
    }

    const int distance = calculateDistance(attackerTile->getX(), attackerTile->getY(), defenderTile->getX(), defenderTile->getY());
    if (!gameState.isAttackablePosition(targetX, targetY))
    {
        gameState.setLastActionMessage(
            QString("Cel jest poza zasięgiem lub niedostępny. %1 atakuje na dystansie %2-%3 pól, a cel jest %4 pól dalej. Kliknij fioletowo oznaczonego przeciwnika.")
                .arg(attacker->getName())
                .arg(attacker->getMinRange())
                .arg(attacker->getRange())
                .arg(distance));
        return true;
    }

    const AttackResult result = m_attackResolver.resolveAttack(
        *attacker,
        attackerTile->getX(),
        attackerTile->getY(),
        attackerTile->getTerrain(),
        *defender,
        defenderTile->getX(),
        defenderTile->getY(),
        defenderTile->getTerrain());

    if (!result.attackPerformed)
    {
        gameState.setLastActionMessage(result.message);
        return true;
    }

    attacker->markActed();
    gameState.consumeTurnActionPoints(attackCost);
    clearDefeatedUnitFromTile(*defenderTile);
    gameState.setSelectedPosition(attackerTile->getX(), attackerTile->getY());
    calculateActionHighlights(gameState, attackerTile->getX(), attackerTile->getY());

    gameState.setLastActionMessage(
        QString("%1 Koszt ataku: %2 AP. Pozostałe AP drużyny: %3/%4. Jeśli starczy AP, możesz działać dalej tą jednostką.")
            .arg(result.message)
            .arg(attackCost)
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints()));

    gameState.updateVictoryState();
    finishActionAndMaybeEndTurn(gameState);
    return true;
}

bool BattleEngine::unitHasAnyAvailableAction(const GameState& gameState, int x, int y, const Unit& unit) const
{
    if (!unit.isAlive() || unit.getSide() != gameState.getCurrentSide())
        return false;

    const int currentAp = gameState.getCurrentTurnActionPoints();
    if (currentAp <= 0)
        return false;

    const Tile* unitTile = gameState.getBoard().getTile(x, y);
    if (!unitTile)
        return false;

    const Board& board = gameState.getBoard();

    if (unit.canAttackNow() && unit.getAttackCost() <= currentAp)
    {
        for (int targetY = 0; targetY < board.getHeight(); ++targetY)
        {
            for (int targetX = 0; targetX < board.getWidth(); ++targetX)
            {
                const Tile* targetTile = board.getTile(targetX, targetY);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive())
                    continue;

                if (targetUnit->getSide() == unit.getSide())
                    continue;

                if (m_attackResolver.canAttack(unit,
                                               x,
                                               y,
                                               *targetUnit,
                                               targetX,
                                               targetY,
                                               unitTile->getTerrain()))
                {
                    return true;
                }
            }
        }
    }

    if (unit.canHealNow() && unit.getAttackCost() <= currentAp)
    {
        for (int targetY = 0; targetY < board.getHeight(); ++targetY)
        {
            for (int targetX = 0; targetX < board.getWidth(); ++targetX)
            {
                const Tile* targetTile = board.getTile(targetX, targetY);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive())
                    continue;

                if (targetUnit->getSide() != unit.getSide())
                    continue;

                if (!targetUnit->isDamaged())
                    continue;

                const int distance = calculateDistance(x, y, targetX, targetY);
                if (distance >= unit.getMinRange() && distance <= unit.getRange())
                    return true;
            }
        }
    }

    if (unit.canMoveNow())
    {
        for (int targetY = 0; targetY < board.getHeight(); ++targetY)
        {
            for (int targetX = 0; targetX < board.getWidth(); ++targetX)
            {
                const Tile* targetTile = board.getTile(targetX, targetY);
                if (!targetTile || targetTile->isOccupied() || !targetTile->isWalkable())
                    continue;

                const int distance = calculateDistance(x, y, targetX, targetY);
                if (distance <= 0 || distance > unit.getMovementPoints())
                    continue;

                const int moveCost = calculateLowestMoveCost(gameState, unit, x, y, targetX, targetY);
                if (moveCost != std::numeric_limits<int>::max() && moveCost <= currentAp)
                    return true;
            }
        }
    }

    return false;
}

bool BattleEngine::hasAnyAvailableAction(const GameState& gameState) const
{
    const Board& board = gameState.getBoard();

    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = 0; x < board.getWidth(); ++x)
        {
            const Tile* tile = board.getTile(x, y);
            if (!tile || !tile->isOccupied() || !tile->getUnit())
                continue;

            const Unit* unit = tile->getUnit();
            if (!unit)
                continue;

            if (unitHasAnyAvailableAction(gameState, x, y, *unit))
                return true;
        }
    }

    return false;
}

void BattleEngine::finishActionAndMaybeEndTurn(GameState& gameState)
{
    if (gameState.updateVictoryState())
        return;

    if (!gameState.hasTurnActionPoints())
    {
        endTurn(gameState);
        return;
    }

    if (!hasAnyAvailableAction(gameState))
    {
        gameState.setLastActionMessage(
            QString("Drużyna %1 nie ma już dostępnych akcji. Tura kończy się automatycznie.")
                .arg(gameState.getCurrentSide() == TeamSide::Player ? "Niebieskich" : "Czerwonych"));
        endTurn(gameState);
    }
}

void BattleEngine::handleTileClick(GameState& gameState, int x, int y)
{
    if (gameState.isGameFinished())
        return;

    Tile* tile = gameState.getBoard().getTile(x, y);
    if (!tile)
        return;

    if (!gameState.hasSelectedPosition())
    {
        if (!tile->isOccupied())
        {
            gameState.setLastActionMessage("Najpierw wybierz swoją jednostkę.");
            return;
        }

        Unit* unit = tile->getUnit();
        if (!unit)
            return;

        if (unit->getSide() != gameState.getCurrentSide())
        {
            gameState.setLastActionMessage("To jest jednostka przeciwnika. Najpierw kliknij swoją jednostkę, potem fioletowy cel, aby zaatakować.");
            return;
        }

        if (!canSelectUnit(gameState, unit))
            return;

        gameState.setSelectedPosition(x, y);
        calculateActionHighlights(gameState, x, y);

        gameState.setLastActionMessage(
            QString("Wybrano jednostkę: %1. Zielone pola = ruch, fioletowe = cele ataku, niebieskie = leczenie. AP drużyny: %2/%3. Ruch: do %4 pól. Koszt ataku: %5 AP.")
                .arg(unit->getName())
                .arg(gameState.getCurrentTurnActionPoints())
                .arg(gameState.getMaxTurnActionPoints())
                .arg(unit->getMovementPoints())
                .arg(unit->getAttackCost()));
        return;
    }

    Tile* selectedTile = gameState.getBoard().getTile(
        gameState.getSelectedX(),
        gameState.getSelectedY());
    Unit* selectedUnit = selectedTile ? selectedTile->getUnit() : nullptr;

    if (!selectedUnit)
    {
        gameState.clearSelectedPosition();
        gameState.clearAllHighlights();
        return;
    }

    if (gameState.getSelectedX() == x && gameState.getSelectedY() == y)
    {
        gameState.clearSelectedPosition();
        gameState.clearAllHighlights();
        gameState.setLastActionMessage("Odznaczono jednostkę.");
        return;
    }

    if (!tile->isOccupied())
    {
        tryMoveSelectedUnit(gameState, x, y);
        return;
    }

    Unit* clickedUnit = tile->getUnit();
    if (!clickedUnit)
        return;

    if (clickedUnit->getSide() == gameState.getCurrentSide())
    {
        if (selectedUnit->canHeal() && gameState.isHealablePosition(x, y))
        {
            tryHealSelectedUnit(gameState, x, y);
            return;
        }

        if (canSelectUnit(gameState, clickedUnit))
        {
            gameState.setSelectedPosition(x, y);
            calculateActionHighlights(gameState, x, y);

            gameState.setLastActionMessage(
                QString("Wybrano jednostkę: %1. Zielone pola = ruch, fioletowe = cele ataku, niebieskie = leczenie. AP drużyny: %2/%3. Ruch: do %4 pól. Koszt ataku: %5 AP.")
                    .arg(clickedUnit->getName())
                    .arg(gameState.getCurrentTurnActionPoints())
                    .arg(gameState.getMaxTurnActionPoints())
                    .arg(clickedUnit->getMovementPoints())
                    .arg(clickedUnit->getAttackCost()));
        }
        return;
    }

    tryAttackSelectedUnit(gameState, x, y);
}

void BattleEngine::endTurn(GameState& gameState)
{
    if (!gameState.isGameFinished())
        gameState.nextTurn();
}
