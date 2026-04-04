#ifndef IPLAYERCONTROLLER_H
#define IPLAYERCONTROLLER_H

class GameState;
class BattleEngine;

class IPlayerController
{
public:
    virtual ~IPlayerController() = default;
    virtual void performTurn(GameState& gameState, BattleEngine& battleEngine) = 0;
};

#endif // IPLAYERCONTROLLER_H
