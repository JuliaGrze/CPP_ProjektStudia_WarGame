#ifndef AITURNSERVICE_H
#define AITURNSERVICE_H

#include "../models/enums/teamside.h"

class GameState;
class BattleEngine;
class Unit;

/**
 * @brief Serwis odpowiedzialny za wykonywanie tury sztucznej inteligencji.
 *
 * Klasa zawiera logikę podejmowania decyzji przez przeciwnika komputerowego.
 * Odpowiada za:
 * - wyszukiwanie jednostek na planszy,
 * - wybór możliwego ataku,
 * - wybór najlepszego ruchu,
 * - ocenę pól planszy.
 *
 * Dzięki wydzieleniu tej logiki z AIController kontroler pozostaje
 * cienką warstwą delegującą, a właściwe decyzje AI są realizowane
 * przez osobny serwis domenowy.
 */
class AITurnService
{
public:
    /**
     * @brief Wykonuje jedną akcję AI w ramach aktualnej tury.
     *
     * Serwis najpierw próbuje wykonać atak, a jeśli nie jest to możliwe,
     * wybiera najlepszy dostępny ruch. Jeśli żadna akcja nie może zostać
     * wykonana, kończy turę przeciwnika.
     *
     * @param gameState Aktualny stan gry.
     * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
     */
    void performTurn(GameState& gameState, BattleEngine& battleEngine) const;

private:
    /**
     * @brief Sprawdza, czy bezpiecznie zaznaczono jednostkę na planszy.
     *
     * Metoda symuluje kliknięcie pola i weryfikuje, czy wskazana jednostka
     * została poprawnie ustawiona jako aktualnie zaznaczona.
     *
     * @param gameState Aktualny stan gry.
     * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
     * @param x Współrzędna X jednostki.
     * @param y Współrzędna Y jednostki.
     * @return true, jeśli zaznaczenie się powiodło.
     */
    bool selectUnitSafely(GameState& gameState, BattleEngine& battleEngine, int x, int y) const;

    /**
     * @brief Próbuje znaleźć i wykonać możliwy atak.
     *
     * @param gameState Aktualny stan gry.
     * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
     * @return true, jeśli wykonano atak.
     */
    bool tryPerformAttack(GameState& gameState, BattleEngine& battleEngine) const;

    /**
     * @brief Próbuje znaleźć i wykonać najlepszy dostępny ruch.
     *
     * @param gameState Aktualny stan gry.
     * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
     * @return true, jeśli wykonano ruch.
     */
    bool tryPerformMove(GameState& gameState, BattleEngine& battleEngine) const;

    /**
     * @brief Oblicza ocenę pola dla ruchu AI.
     *
     * Niższy wynik oznacza lepszy wybór. Ocena uwzględnia przede wszystkim:
     * - odległość od najbliższego przeciwnika,
     * - osłonę zapewnianą przez teren.
     *
     * @param gameState Aktualny stan gry.
     * @param moveX Współrzędna X analizowanego pola.
     * @param moveY Współrzędna Y analizowanego pola.
     * @return Wynik oceny pola.
     */
    int scoreMovePosition(const GameState& gameState, int moveX, int moveY) const;
};

#endif // AITURNSERVICE_H
