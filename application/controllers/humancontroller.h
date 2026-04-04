#ifndef HUMANCONTROLLER_H
#define HUMANCONTROLLER_H

#include "interfaces/Iplayercontroller.h"

class HumanController : public IPlayerController
{
public:
    void performTurn(GameState& gameState, BattleEngine& battleEngine) override;
};

#endif // HUMANCONTROLLER_H
