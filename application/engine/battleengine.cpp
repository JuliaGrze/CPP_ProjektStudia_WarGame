#include "battleengine.h"

#include "../models/tile.h"
#include "../models/unit.h"
#include "../models/enums/terraintype.h"

#include <QQueue>
#include <QSet>
#include <QPair>
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
    if (!unit)
        return false;

    if (!unit->isAlive())
        return false;

    if (unit->getSide() != gameState.getCurrentSide())
        return false;

    return true;
}

int BattleEngine::calculateDistance(int x1, int y1, int x2, int y2) const
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

void BattleEngine::calculateMoveHighlights(GameState& gameState, int startX, int startY) const
{
    QVector<QPair<int, int>> availablePositions;
    QVector<QPair<int, int>> blockedPositions;

    const Tile* startTile = gameState.getBoard().getTile(startX, startY);
    if (!startTile || !startTile->isOccupied() || !startTile->getUnit())
    {
        gameState.clearAvailableMovePositions();
        gameState.clearBlockedMovePositions();
        return;
    }

    const Unit* unit = startTile->getUnit();
    if (!unit)
    {
        gameState.clearAvailableMovePositions();
        gameState.clearBlockedMovePositions();
        return;
    }

    const int maxCostByRange = unit->getMovementPoints() * unit->getMoveCostPerTile();
    const int maxAffordableCost = gameState.getCurrentTurnActionPoints();
    const int maxAllowedCost = std::min(maxCostByRange, maxAffordableCost);

    if (maxAllowedCost <= 0)
    {
        gameState.clearAvailableMovePositions();
        gameState.clearBlockedMovePositions();
        return;
    }

    QQueue<MoveNode> queue;
    QSet<QString> availableSet;
    QSet<QString> blockedSet;
    QHash<QString, int> bestCost;

    queue.enqueue({ startX, startY, 0 });
    bestCost.insert(makeKey(startX, startY), 0);

    const int directions[4][2] =
        {
            { 1, 0 },
            { -1, 0 },
            { 0, 1 },
            { 0, -1 }
        };

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
            if (!nextTile)
                continue;

            const QString key = makeKey(nextX, nextY);

            if (!nextTile->isWalkable() || nextTile->isOccupied())
            {
                if (!blockedSet.contains(key))
                {
                    blockedPositions.append({ nextX, nextY });
                    blockedSet.insert(key);
                }
                continue;
            }

            const int nextCost = current.spentCost + unit->getMoveCostPerTile();
            if (nextCost > maxAllowedCost)
                continue;

            if (bestCost.contains(key) && bestCost.value(key) <= nextCost)
                continue;

            bestCost.insert(key, nextCost);

            if (!availableSet.contains(key))
            {
                availablePositions.append({ nextX, nextY });
                availableSet.insert(key);
            }

            queue.enqueue({ nextX, nextY, nextCost });
        }
    }

    gameState.setAvailableMovePositions(availablePositions);
    gameState.setBlockedMovePositions(blockedPositions);
}

bool BattleEngine::tryMoveSelectedUnit(GameState& gameState, int targetX, int targetY)
{
    if (!gameState.hasSelectedPosition())
        return false;

    if (!gameState.isMovePositionAvailable(targetX, targetY))
        return false;

    Tile* sourceTile = gameState.getBoard().getTile(
        gameState.getSelectedX(),
        gameState.getSelectedY()
        );

    Tile* targetTile = gameState.getBoard().getTile(targetX, targetY);

    if (!sourceTile || !targetTile)
        return false;

    if (!sourceTile->isOccupied())
        return false;

    if (targetTile->isOccupied())
        return false;

    if (!targetTile->isWalkable())
        return false;

    Unit* unit = sourceTile->getUnit();
    if (!unit)
        return false;

    const int movementDistance = calculateDistance(
        sourceTile->getX(), sourceTile->getY(), targetX, targetY
        );

    if (movementDistance <= 0)
        return false;

    if (movementDistance > unit->getMovementPoints())
    {
        gameState.setLastActionMessage(
            QString("%1 nie może przejść tak daleko w jednym ruchu.")
                .arg(unit->getName())
            );
        return false;
    }

    const int movementCost = movementDistance * unit->getMoveCostPerTile();

    if (movementCost > gameState.getCurrentTurnActionPoints())
    {
        gameState.setLastActionMessage(
            QString("Za mało punktów akcji. Ruch %1 kosztuje %2 AP.")
                .arg(unit->getName())
                .arg(movementCost)
            );
        return false;
    }

    sourceTile->removeUnit();
    targetTile->setUnit(unit);

    gameState.consumeTurnActionPoints(movementCost);
    gameState.setSelectedPosition(targetX, targetY);

    if (gameState.hasTurnActionPoints())
        calculateMoveHighlights(gameState, targetX, targetY);
    else
    {
        gameState.clearSelectedPosition();
        gameState.clearAvailableMovePositions();
        gameState.clearBlockedMovePositions();
    }

    gameState.setLastActionMessage(
        QString("%1 przemieścił się o %2 pola. Koszt ruchu: %3 AP. AP drużyny: %4/%5.")
            .arg(unit->getName())
            .arg(movementDistance)
            .arg(movementCost)
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints())
        );

    finishActionAndMaybeEndTurn(gameState);
    return true;
}

void BattleEngine::clearDefeatedUnitFromTile(Tile& tile) const
{
    Unit* unit = tile.getUnit();
    if (!unit)
        return;

    if (!unit->isAlive())
        tile.removeUnit();
}

bool BattleEngine::tryAttackSelectedUnit(GameState& gameState, int targetX, int targetY)
{
    if (!gameState.hasSelectedPosition())
        return false;

    Tile* attackerTile = gameState.getBoard().getTile(
        gameState.getSelectedX(),
        gameState.getSelectedY()
        );
    Tile* defenderTile = gameState.getBoard().getTile(targetX, targetY);

    if (!attackerTile || !defenderTile || !attackerTile->isOccupied() || !defenderTile->isOccupied())
        return false;

    Unit* attacker = attackerTile->getUnit();
    Unit* defender = defenderTile->getUnit();

    if (!attacker || !defender)
        return false;

    if (attacker->getSide() == defender->getSide())
        return false;

    const int attackCost = attacker->getAttackCost();
    if (attackCost > gameState.getCurrentTurnActionPoints())
    {
        gameState.setLastActionMessage(
            QString("Za mało punktów akcji. Atak jednostki %1 kosztuje %2 AP.")
                .arg(attacker->getName())
                .arg(attackCost)
            );
        return true;
    }

    if (!m_attackResolver.canAttack(*attacker,
                                    attackerTile->getX(),
                                    attackerTile->getY(),
                                    *defender,
                                    defenderTile->getX(),
                                    defenderTile->getY()))
    {
        gameState.setLastActionMessage(
            QString("%1 nie może zaatakować tego celu - przeciwnik jest poza zasięgiem.")
                .arg(attacker->getName())
            );
        return true;
    }

    const AttackResult result = m_attackResolver.resolveAttack(
        *attacker,
        attackerTile->getX(),
        attackerTile->getY(),
        *defender,
        defenderTile->getX(),
        defenderTile->getY(),
        defenderTile->getTerrain()
        );

    if (!result.attackPerformed)
    {
        gameState.setLastActionMessage(result.message);
        return true;
    }

    gameState.consumeTurnActionPoints(attackCost);
    clearDefeatedUnitFromTile(*defenderTile);

    gameState.clearSelectedPosition();
    gameState.clearAvailableMovePositions();
    gameState.clearBlockedMovePositions();

    gameState.setLastActionMessage(
        QString("%1 Koszt ataku: %2 AP. AP drużyny: %3/%4.")
            .arg(result.message)
            .arg(attackCost)
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints())
        );

    finishActionAndMaybeEndTurn(gameState);
    return true;
}

void BattleEngine::finishActionAndMaybeEndTurn(GameState& gameState)
{
    if (!gameState.hasTurnActionPoints())
        endTurn(gameState);
}

void BattleEngine::handleTileClick(GameState& gameState, int x, int y)
{
    Tile* tile = gameState.getBoard().getTile(x, y);
    if (!tile)
        return;

    if (!tile->isOccupied())
    {
        tryMoveSelectedUnit(gameState, x, y);
        return;
    }

    Unit* unit = tile->getUnit();
    if (!unit)
        return;

    if (gameState.hasSelectedPosition() && unit->getSide() != gameState.getCurrentSide())
    {
        if (tryAttackSelectedUnit(gameState, x, y))
            return;
    }

    if (!canSelectUnit(gameState, unit))
        return;

    if (gameState.hasSelectedPosition() &&
        gameState.getSelectedX() == x &&
        gameState.getSelectedY() == y)
    {
        gameState.clearSelectedPosition();
        gameState.clearAvailableMovePositions();
        gameState.clearBlockedMovePositions();
        gameState.setLastActionMessage("Odznaczono jednostkę.");
        return;
    }

    gameState.setSelectedPosition(x, y);
    calculateMoveHighlights(gameState, x, y);
    gameState.setLastActionMessage(
        QString("Wybrano jednostkę: %1. AP drużyny: %2/%3. Ruch: %4 pola. Koszt ataku: %5 AP.")
            .arg(unit->getName())
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints())
            .arg(unit->getMovementPoints())
            .arg(unit->getAttackCost())
        );
}

void BattleEngine::endTurn(GameState& gameState)
{
    gameState.nextTurn();
}
