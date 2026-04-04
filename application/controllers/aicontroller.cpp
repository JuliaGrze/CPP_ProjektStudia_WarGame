#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "interfaces/iplayercontroller.h"

class AIController : public IPlayerController
{
public:
    void performTurn(GameState& gameState, BattleEngine& battleEngine) override;
};

#endif // AICONTROLLER_H
