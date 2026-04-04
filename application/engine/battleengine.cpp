#include "battleengine.h"

#include "../models/tile.h"
#include "../models/unit.h"
#include "../models/enums/terraintype.h"

#include <cmath>

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

void BattleEngine::calculateMoveHighlights(GameState& gameState, int x, int y) const
{
    QVector<QPair<int, int>> availablePositions;
    QVector<QPair<int, int>> blockedPositions;

    const Tile* startTile = gameState.getBoard().getTile(x, y);
    if (!startTile || !startTile->isOccupied() || !startTile->getUnit())
    {
        gameState.clearAvailableMovePositions();
        gameState.clearBlockedMovePositions();
        return;
    }

    const Unit* unit = startTile->getUnit();
    const int moveRange = unit->getMovementPoints();

    for (int row = 0; row < gameState.getBoard().getHeight(); ++row)
    {
        for (int col = 0; col < gameState.getBoard().getWidth(); ++col)
        {
            if (col == x && row == y)
                continue;

            if (calculateDistance(x, y, col, row) > moveRange)
                continue;

            const Tile* tile = gameState.getBoard().getTile(col, row);
            if (!tile)
                continue;

            const bool isPlain = tile->getTerrain() == TerrainType::Plain;
            const bool isFree = !tile->isOccupied();

            if (isPlain && isFree)
                availablePositions.append({col, row});
            else
                blockedPositions.append({col, row});
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

    sourceTile->removeUnit();
    targetTile->setUnit(unit);

    gameState.setSelectedPosition(targetX, targetY);
    calculateMoveHighlights(gameState, targetX, targetY);

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
        gameState.clearBlockedMovePositions();
        return;
    }

    gameState.setSelectedPosition(x, y);
    calculateMoveHighlights(gameState, x, y);
}

void BattleEngine::endTurn(GameState& gameState)
{
    gameState.nextTurn();
}
