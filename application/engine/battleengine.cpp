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
    int steps;
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

    return gameState.hasTurnActionPoints();
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

    if (!gameState.hasTurnActionPoints() || unit->getCurrentMovementPoints() <= 0)
    {
        gameState.clearAvailableMovePositions();
        gameState.clearBlockedMovePositions();
        return;
    }

    const int moveRange = unit->getCurrentMovementPoints();

    QQueue<MoveNode> queue;
    QSet<QString> visited;
    QSet<QString> availableSet;
    QSet<QString> blockedSet;

    queue.enqueue({ startX, startY, 0 });
    visited.insert(makeKey(startX, startY));

    const int directions[4][2] =
        {
            { 1, 0 },
            { -1, 0 },
            { 0, 1 },
            { 0, -1 }
        };

    while (!queue.isEmpty())
    {
        MoveNode current = queue.dequeue();

        if (current.steps >= moveRange)
            continue;

        for (const auto& direction : directions)
        {
            const int nextX = current.x + direction[0];
            const int nextY = current.y + direction[1];

            if (!gameState.getBoard().isInsideBoard(nextX, nextY))
                continue;

            const QString key = makeKey(nextX, nextY);
            const Tile* nextTile = gameState.getBoard().getTile(nextX, nextY);
            if (!nextTile)
                continue;

            const bool isPlain = nextTile->getTerrain() == TerrainType::Plain;
            const bool isOccupied = nextTile->isOccupied();

            if (!isPlain || isOccupied)
            {
                if (!blockedSet.contains(key))
                {
                    blockedPositions.append({ nextX, nextY });
                    blockedSet.insert(key);
                }
                continue;
            }

            if (visited.contains(key))
                continue;

            visited.insert(key);

            if (!availableSet.contains(key))
            {
                availablePositions.append({ nextX, nextY });
                availableSet.insert(key);
            }

            queue.enqueue({ nextX, nextY, current.steps + 1 });
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

    if (targetTile->getTerrain() != TerrainType::Plain)
        return false;

    Unit* unit = sourceTile->getUnit();
    if (!unit)
        return false;

    if (!gameState.hasTurnActionPoints())
    {
        gameState.setLastActionMessage("Drużyna nie ma już punktów akcji w tej turze.");
        return false;
    }

    const int movementCost = calculateDistance(
        sourceTile->getX(), sourceTile->getY(), targetX, targetY
        );

    if (movementCost > unit->getCurrentMovementPoints())
    {
        gameState.setLastActionMessage(
            QString("%1 nie ma wystarczającej liczby punktów ruchu.")
                .arg(unit->getName())
            );
        return false;
    }

    sourceTile->removeUnit();
    targetTile->setUnit(unit);

    gameState.consumeTurnActionPoints(1);
    unit->consumeMovementPoints(movementCost);

    gameState.setSelectedPosition(targetX, targetY);

    if (gameState.hasTurnActionPoints() && unit->getCurrentMovementPoints() > 0)
        calculateMoveHighlights(gameState, targetX, targetY);
    else
    {
        gameState.clearSelectedPosition();
        gameState.clearAvailableMovePositions();
        gameState.clearBlockedMovePositions();
    }

    gameState.setLastActionMessage(
        QString("%1 przemieścił się na pole (%2, %3). AP drużyny: %4/%5. MP jednostki: %6/%7.")
            .arg(unit->getName())
            .arg(targetX)
            .arg(targetY)
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints())
            .arg(unit->getCurrentMovementPoints())
            .arg(unit->getMovementPoints())
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
        QString("Wybrano jednostkę: %1. AP drużyny: %2/%3. MP jednostki: %4/%5.")
            .arg(unit->getName())
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints())
            .arg(unit->getCurrentMovementPoints())
            .arg(unit->getMovementPoints())
        );
}

void BattleEngine::endTurn(GameState& gameState)
{
    gameState.nextTurn();
}
