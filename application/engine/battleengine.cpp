#include "battleengine.h"

#include "../models/tile.h"
#include "../models/unit.h"

bool BattleEngine::canSelectUnit(const GameState& gameState, const Unit* unit) const
{
    if (!unit)
        return false;

    return unit->getSide() == gameState.getCurrentSide();
}

int BattleEngine::calculateDistance(int x1, int y1, int x2, int y2) const
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

QVector<QPair<int, int>> BattleEngine::calculateAvailableMovePositions(const GameState& gameState, int x, int y) const
{
    QVector<QPair<int, int>> positions;

    const Tile* startTile = gameState.getBoard().getTile(x, y);
    if (!startTile || !startTile->isOccupied() || !startTile->getUnit())
        return positions;

    const Unit* unit = startTile->getUnit();
    const int moveRange = unit->getMovementPoints();

    for (int row = 0; row < gameState.getBoard().getHeight(); ++row)
    {
        for (int col = 0; col < gameState.getBoard().getWidth(); ++col)
        {
            if (col == x && row == y)
                continue;

            const Tile* tile = gameState.getBoard().getTile(col, row);
            if (!tile)
                continue;

            if (!tile->isWalkable())
                continue;

            if (tile->isOccupied())
                continue;

            if (calculateDistance(x, y, col, row) <= moveRange)
                positions.append({col, row});
        }
    }

    return positions;
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

    if (!sourceTile->isOccupied() || targetTile->isOccupied())
        return false;

    if (!targetTile->isWalkable())
        return false;

    Unit* unit = sourceTile->getUnit();
    if (!unit)
        return false;

    sourceTile->removeUnit();
    targetTile->setUnit(unit);

    gameState.setSelectedPosition(targetX, targetY);
    gameState.setAvailableMovePositions(
        calculateAvailableMovePositions(gameState, targetX, targetY)
        );

    return true;
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
    if (!canSelectUnit(gameState, unit))
        return;

    if (gameState.hasSelectedPosition() &&
        gameState.getSelectedX() == x &&
        gameState.getSelectedY() == y)
    {
        gameState.clearSelectedPosition();
        gameState.clearAvailableMovePositions();
        return;
    }

    gameState.setSelectedPosition(x, y);
    gameState.setAvailableMovePositions(
        calculateAvailableMovePositions(gameState, x, y)
        );
}

void BattleEngine::endTurn(GameState& gameState)
{
    gameState.nextTurn();
}
