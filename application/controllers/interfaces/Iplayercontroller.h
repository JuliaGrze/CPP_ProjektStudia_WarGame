#ifndef IPLAYERCONTROLLER_H
#define IPLAYERCONTROLLER_H

class GameState;
class BattleEngine;

/**
 * @brief Interfejs kontrolera gracza.
 *
 * Klasa abstrakcyjna definiująca sposób wykonywania tury przez gracza.
 * Może być implementowana zarówno przez gracza sterowanego przez użytkownika,
 * jak i przez gracza sterowanego automatycznie (komputerowo).
 */
class IPlayerController
{
public:
    /**
     * @brief Wirtualny destruktor interfejsu.
     */
    virtual ~IPlayerController() = default;

    /**
     * @brief Wykonuje turę gracza.
     *
     * Metoda powinna zawierać logikę wykonywania akcji przez gracza
     * w trakcie jednej tury, wykorzystując aktualny stan gry oraz
     * silnik walki.
     *
     * @param gameState Aktualny stan gry.
     * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji (np. ruch, atak).
     */
    virtual void performTurn(GameState& gameState, BattleEngine& battleEngine) = 0;
};

#endif // IPLAYERCONTROLLER_H
