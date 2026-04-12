#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "interfaces/Iplayercontroller.h"
#include "../services/aiturnservice.h"

/**
 * @brief Kontroler odpowiedzialny za sterowanie przeciwnikiem komputerowym.
 *
 * Klasa implementuje interfejs IPlayerController i reprezentuje
 * automatyczne wykonywanie tury przez przeciwnika.
 *
 * Sama klasa nie przechowuje już złożonej logiki decyzyjnej.
 * Zamiast tego deleguje całą analizę i wybór akcji do serwisu
 * AITurnService, dzięki czemu zachowuje pojedynczą odpowiedzialność.
 */
class AIController : public IPlayerController
{
public:
    /**
     * @brief Wykonuje turę przeciwnika komputerowego.
     *
     * Metoda deleguje logikę wyboru akcji do serwisu AITurnService.
     *
     * @param gameState Aktualny stan gry.
     * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
     */
    void performTurn(GameState& gameState, BattleEngine& battleEngine) override;

private:
    AITurnService m_aiTurnService; ///< Serwis realizujący logikę decyzji AI.
};

#endif // AICONTROLLER_H
