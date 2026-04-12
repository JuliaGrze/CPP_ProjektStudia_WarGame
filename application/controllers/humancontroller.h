#ifndef HUMANCONTROLLER_H
#define HUMANCONTROLLER_H

#include "interfaces/Iplayercontroller.h"

/**
 * @brief Kontroler gracza sterowanego przez użytkownika.
 *
 * Klasa implementuje interfejs IPlayerController i reprezentuje
 * ręczne sterowanie turą. W przeciwieństwie do AIController
 * nie wykonuje automatycznie ruchów, ponieważ akcje gracza
 * realizowane są przez interakcję z interfejsem użytkownika.
 *
 * Obiekt tej klasy nadal bierze udział w architekturze gry,
 * ponieważ GameController wybiera go polimorficznie jako aktywny
 * kontroler strony gracza.
 */
class HumanController : public IPlayerController
{
public:
    /**
     * @brief Wykonuje turę gracza sterowanego ręcznie.
     *
     * W praktyce metoda nie wykonuje automatycznych akcji.
     * Jej zadaniem jest formalne reprezentowanie wejścia
     * w turę gracza ręcznego.
     *
     * @param gameState Aktualny stan gry.
     * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
     */
    void performTurn(GameState& gameState, BattleEngine& battleEngine) override;
};

#endif // HUMANCONTROLLER_H
