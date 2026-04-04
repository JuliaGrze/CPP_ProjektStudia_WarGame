#include "humancontroller.h"

#include "../models/gamestate.h"
#include "../engine/battleengine.h"

void HumanController::performTurn(GameState& gameState, BattleEngine& battleEngine)
{
    Q_UNUSED(gameState);
    Q_UNUSED(battleEngine);
    // Na razie ruch gracza obsługiwany jest przez kliknięcia w UI.
}
