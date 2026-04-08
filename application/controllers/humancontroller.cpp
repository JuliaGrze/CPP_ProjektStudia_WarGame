#include "humancontroller.h"

#include "../models/gamestate.h"
#include "../engine/battleengine.h"

/**
 * @brief Wykonuje turę gracza sterowanego przez użytkownika.
 *
 * W przypadku gracza sterowanego ręcznie logika wykonywania akcji
 * realizowana jest poprzez interakcje użytkownika z interfejsem
 * (np. kliknięcia na planszy).
 *
 * Metoda nie zawiera bezpośredniej logiki sterowania,
 * dlatego pozostaje pusta.
 *
 * @param gameState Aktualny stan gry.
 * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
 */
void HumanController::performTurn(GameState& gameState, BattleEngine& battleEngine)
{
    Q_UNUSED(gameState);
    Q_UNUSED(battleEngine);
}
