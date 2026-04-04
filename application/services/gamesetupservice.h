#ifndef GAMESETUPSERVICE_H
#define GAMESETUPSERVICE_H

#include "../../config/gameconfig.h"
#include "../models/gamestate.h"

class GameSetupService
{
public:
    static GameState createGame(const GameConfig& config);
};

#endif // GAMESETUPSERVICE_H
