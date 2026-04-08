#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include "../models/gamestate.h"
#include "attackresolver.h"

class Unit;
class Tile;

/**
 * @brief Silnik obsługujący logikę przebiegu bitwy.
 *
 * Klasa odpowiada za interpretację akcji wykonywanych podczas walki,
 * takich jak:
 * - wybór jednostki,
 * - wyznaczanie dostępnych akcji,
 * - ruch,
 * - atak,
 * - leczenie,
 * - kończenie tury.
 *
 * Współpracuje ze stanem gry i aktualizuje go zgodnie z zasadami rozgrywki.
 */
class BattleEngine
{
public:
    /**
     * @brief Tworzy silnik bitwy.
     */
    BattleEngine() = default;

    /**
     * @brief Obsługuje kliknięcie pola planszy.
     *
     * Metoda interpretuje kliknięcie użytkownika w zależności od aktualnego
     * stanu gry, zaznaczenia jednostki oraz możliwych akcji.
     *
     * @param gameState Aktualny stan gry.
     * @param x Współrzędna X klikniętego pola.
     * @param y Współrzędna Y klikniętego pola.
     */
    void handleTileClick(GameState& gameState, int x, int y);

    /**
     * @brief Kończy aktualną turę.
     *
     * Metoda przełącza aktywną stronę, resetuje odpowiednie dane tury
     * oraz przygotowuje stan gry do dalszej rozgrywki.
     *
     * @param gameState Aktualny stan gry.
     */
    void endTurn(GameState& gameState);

private:
    /**
     * @brief Sprawdza, czy jednostka może zostać zaznaczona.
     *
     * @param gameState Aktualny stan gry.
     * @param unit Jednostka, która ma zostać sprawdzona.
     * @return true, jeśli jednostkę można zaznaczyć, w przeciwnym razie false.
     */
    bool canSelectUnit(const GameState& gameState, const Unit* unit) const;

    /**
     * @brief Wyznacza i zapisuje dostępne akcje dla zaznaczonej jednostki.
     *
     * Metoda oblicza możliwe pola ruchu, ataku oraz leczenia
     * dla jednostki znajdującej się na wskazanej pozycji.
     *
     * @param gameState Aktualny stan gry.
     * @param x Współrzędna X zaznaczonej jednostki.
     * @param y Współrzędna Y zaznaczonej jednostki.
     */
    void calculateActionHighlights(GameState& gameState, int x, int y) const;

    /**
     * @brief Próbuje wykonać ruch zaznaczoną jednostką.
     *
     * @param gameState Aktualny stan gry.
     * @param targetX Współrzędna X pola docelowego.
     * @param targetY Współrzędna Y pola docelowego.
     * @return true, jeśli ruch został wykonany, w przeciwnym razie false.
     */
    bool tryMoveSelectedUnit(GameState& gameState, int targetX, int targetY);

    /**
     * @brief Próbuje wykonać atak zaznaczoną jednostką.
     *
     * @param gameState Aktualny stan gry.
     * @param targetX Współrzędna X celu ataku.
     * @param targetY Współrzędna Y celu ataku.
     * @return true, jeśli atak został wykonany, w przeciwnym razie false.
     */
    bool tryAttackSelectedUnit(GameState& gameState, int targetX, int targetY);

    /**
     * @brief Próbuje wykonać leczenie zaznaczoną jednostką.
     *
     * @param gameState Aktualny stan gry.
     * @param targetX Współrzędna X celu leczenia.
     * @param targetY Współrzędna Y celu leczenia.
     * @return true, jeśli leczenie zostało wykonane, w przeciwnym razie false.
     */
    bool tryHealSelectedUnit(GameState& gameState, int targetX, int targetY);

    /**
     * @brief Usuwa pokonaną jednostkę z pola planszy.
     *
     * @param tile Pole planszy, z którego ma zostać usunięta jednostka.
     */
    void clearDefeatedUnitFromTile(Tile& tile) const;

    /**
     * @brief Kończy akcję i w razie potrzeby kończy turę.
     *
     * Metoda wykonuje działania porządkowe po akcji jednostki
     * oraz sprawdza, czy możliwe są jeszcze dalsze ruchy w bieżącej turze.
     *
     * @param gameState Aktualny stan gry.
     */
    void finishActionAndMaybeEndTurn(GameState& gameState);

    /**
     * @brief Sprawdza, czy aktywna strona ma jakąkolwiek dostępną akcję.
     *
     * @param gameState Aktualny stan gry.
     * @return true, jeśli istnieje możliwa akcja, w przeciwnym razie false.
     */
    bool hasAnyAvailableAction(const GameState& gameState) const;

    /**
     * @brief Sprawdza, czy wskazana jednostka może wykonać jakąkolwiek akcję.
     *
     * @param gameState Aktualny stan gry.
     * @param x Współrzędna X jednostki.
     * @param y Współrzędna Y jednostki.
     * @param unit Jednostka, która ma zostać sprawdzona.
     * @return true, jeśli jednostka ma dostępną akcję, w przeciwnym razie false.
     */
    bool unitHasAnyAvailableAction(const GameState& gameState, int x, int y, const Unit& unit) const;

    /**
     * @brief Zwraca powód, dla którego jednostka nie może wykonać akcji.
     *
     * @param gameState Aktualny stan gry.
     * @param x Współrzędna X jednostki.
     * @param y Współrzędna Y jednostki.
     * @param unit Jednostka, której dotyczy sprawdzenie.
     * @return Komunikat opisujący przyczynę braku dostępnej akcji.
     */
    QString getUnitUnavailableReason(const GameState& gameState, int x, int y, const Unit& unit) const;

    /**
     * @brief Oblicza odległość pomiędzy dwoma polami planszy.
     *
     * @param x1 Współrzędna X pierwszego pola.
     * @param y1 Współrzędna Y pierwszego pola.
     * @param x2 Współrzędna X drugiego pola.
     * @param y2 Współrzędna Y drugiego pola.
     * @return Odległość pomiędzy wskazanymi polami.
     */
    int calculateDistance(int x1, int y1, int x2, int y2) const;

    /**
     * @brief Oblicza najniższy koszt ruchu pomiędzy dwoma polami.
     *
     * Metoda uwzględnia właściwości jednostki, pozycję początkową i docelową
     * oraz aktualny stan planszy.
     *
     * @param gameState Aktualny stan gry.
     * @param unit Jednostka wykonująca ruch.
     * @param startX Współrzędna X pola początkowego.
     * @param startY Współrzędna Y pola początkowego.
     * @param targetX Współrzędna X pola docelowego.
     * @param targetY Współrzędna Y pola docelowego.
     * @return Najniższy koszt ruchu pomiędzy wskazanymi polami.
     */
    int calculateLowestMoveCost(const GameState& gameState,
                                const Unit& unit,
                                int startX,
                                int startY,
                                int targetX,
                                int targetY) const;

private:
    AttackResolver m_attackResolver; ///< Obiekt odpowiedzialny za rozstrzyganie ataków.
};

#endif // BATTLEENGINE_H
