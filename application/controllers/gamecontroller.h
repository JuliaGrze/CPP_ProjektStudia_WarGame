#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "../../config/gameconfig.h"
#include "../engine/battleengine.h"
#include "../models/gamestate.h"

class GameController
{
public:
    GameController();

    void startGame(const GameConfig& config);

    void handleTileClick(int x, int y);
    void endTurn();

    const GameState& getGameState() const;
    const GameConfig& getGameConfig() const;

private:
    GameConfig m_config;
    GameState m_state;
    BattleEngine m_engine;
};

#endif // GAMECONTROLLER_H
