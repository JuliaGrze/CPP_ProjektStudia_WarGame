#ifndef HUMANCONTROLLER_H
#define HUMANCONTROLLER_H

#include "interfaces/Iplayercontroller.h"

/**
 * @brief Kontroler gracza sterowanego przez użytkownika.
 *
 * Klasa implementuje interfejs IPlayerController i odpowiada
 * za obsługę tury gracza wykonującego akcje ręcznie
 * poprzez interfejs użytkownika.
 */
class HumanController : public IPlayerController
{
public:
    /**
     * @brief Wykonuje turę gracza sterowanego przez użytkownika.
     *
     * W przypadku gracza manualnego logika tury jest realizowana
     * poprzez interakcje użytkownika (np. kliknięcia na planszy),
     * dlatego metoda może pełnić rolę pomocniczą lub pozostawać pusta.
     *
     * @param gameState Aktualny stan gry.
     * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
     */
    void performTurn(GameState& gameState, BattleEngine& battleEngine) override;
};

#endif // HUMANCONTROLLER_H
