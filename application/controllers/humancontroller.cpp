#include "humancontroller.h"

#include "../models/gamestate.h"
#include "../engine/battleengine.h"

/**
 * @brief Wykonuje turę gracza sterowanego ręcznie.
 *
 * W przypadku gracza sterowanego przez użytkownika wykonywanie akcji
 * odbywa się poprzez kliknięcia w interfejsie użytkownika,
 * a nie przez automatyczną logikę kontrolera.
 *
 * Metoda pozostaje celowo pusta, ale jej istnienie ma znaczenie
 * architektoniczne – dzięki niej GameController może korzystać
 * ze wspólnego interfejsu IPlayerController zarówno dla człowieka,
 * jak i dla przeciwnika sterowanego automatycznie.
 *
 * @param gameState Aktualny stan gry.
 * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
 */
void HumanController::performTurn(GameState& gameState, BattleEngine& battleEngine)
{
    Q_UNUSED(gameState);
    Q_UNUSED(battleEngine);
}
