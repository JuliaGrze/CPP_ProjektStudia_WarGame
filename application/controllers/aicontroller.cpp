#include "aicontroller.h"

#include "../models/gamestate.h"
#include "../engine/battleengine.h"

/**
 * @brief Wykonuje turę przeciwnika sterowanego automatycznie.
 *
 * Metoda nie zawiera już własnej logiki wyboru akcji.
 * Odpowiada jedynie za delegowanie decyzji do serwisu AITurnService.
 *
 * @param gameState Aktualny stan gry.
 * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
 */
void AIController::performTurn(GameState& gameState, BattleEngine& battleEngine)
{
    m_aiTurnService.performTurn(gameState, battleEngine);
}
