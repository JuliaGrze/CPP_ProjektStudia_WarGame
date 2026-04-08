#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "interfaces/iplayercontroller.h"

/**
 * @brief Kontroler gracza sterowanego automatycznie.
 *
 * Klasa implementuje interfejs IPlayerController i odpowiada za podejmowanie
 * decyzji przez gracza sterowanego przez system (komputerowo).
 * Zawiera logikę wyboru akcji wykonywanych w trakcie tury.
 */
class AIController : public IPlayerController
{
public:
    /**
     * @brief Wykonuje turę gracza sterowanego automatycznie.
     *
     * Metoda analizuje aktualny stan gry i podejmuje decyzje dotyczące
     * ruchów oraz akcji jednostek przy użyciu silnika walki.
     *
     * @param gameState Aktualny stan gry.
     * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
     */
    void performTurn(GameState& gameState, BattleEngine& battleEngine) override;
};

#endif // AICONTROLLER_H
