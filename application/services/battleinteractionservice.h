#ifndef BATTLEINTERACTIONSERVICE_H
#define BATTLEINTERACTIONSERVICE_H

#include "../models/gamestate.h"

class BattleInteractionService
{
public:
    static void handleTileClick(GameState& gameState, int x, int y);
};

#endif // BATTLEINTERACTIONSERVICE_H
