#include "battleinteractionservice.h"

#include "../models/tile.h"
#include "../models/unit.h"

void BattleInteractionService::handleTileClick(GameState& gameState, int x, int y)
{
    Tile* tile = gameState.getBoard().getTile(x, y);
    if (!tile)
        return;

    if (!tile->isOccupied())
    {
        return;
    }

    Unit* unit = tile->getUnit();
    if (!unit)
        return;

    if (unit->getSide() != gameState.getCurrentSide())
    {
        return;
    }

    if (gameState.hasSelectedPosition() &&
        gameState.getSelectedX() == x &&
        gameState.getSelectedY() == y)
    {
        gameState.clearSelectedPosition();
        return;
    }

    gameState.setSelectedPosition(x, y);
}
