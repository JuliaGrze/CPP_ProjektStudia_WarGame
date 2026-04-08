#ifndef UNITPLACEMENTHELPER_H
#define UNITPLACEMENTHELPER_H

#include "../models/board.h"
#include "../models/team.h"
#include "../../config/teamComposition.h"

/**
 * @brief Klasa pomocnicza odpowiedzialna za rozmieszczanie jednostek na planszy.
 *
 * Zawiera metody służące do:
 * - generowania pozycji startowych,
 * - rozmieszczania jednostek obu drużyn,
 * - przypisywania jednostek do odpowiednich pól planszy.
 *
 * Klasa działa statycznie i nie wymaga tworzenia instancji.
 */
class UnitPlacementHelper
{
public:
    /**
     * @brief Rozmieszcza obie drużyny na planszy.
     *
     * Metoda generuje pozycje startowe i umieszcza jednostki
     * drużyny gracza oraz drużyny przeciwnika zgodnie z przekazanym składem.
     *
     * @param board Plansza, na której rozmieszczane są jednostki.
     * @param playerTeam Drużyna gracza.
     * @param enemyTeam Drużyna przeciwnika.
     * @param playerComp Skład drużyny gracza.
     * @param enemyComp Skład drużyny przeciwnika.
     */
    static void placeTeams(Board& board,
                           Team& playerTeam,
                           Team& enemyTeam,
                           const TeamComposition& playerComp,
                           const TeamComposition& enemyComp);

private:
    /**
     * @brief Generuje losowe pozycje startowe z uwzględnieniem strony planszy.
     *
     * Metoda przygotowuje listę pól odpowiednich do rozmieszczenia jednostek
     * dla wskazanej strony konfliktu.
     *
     * @param board Plansza gry.
     * @param side Strona, dla której generowane są pozycje.
     * @param neededCount Liczba wymaganych pozycji.
     * @return Lista wygenerowanych współrzędnych.
     */
    static QVector<QPair<int, int>> generateWeightedRandomPositions(Board& board,
                                                                    TeamSide side,
                                                                    int neededCount);

    /**
     * @brief Rozmieszcza pojedynczą drużynę na planszy.
     *
     * Metoda umieszcza jednostki wskazanej drużyny na przekazanych pozycjach
     * zgodnie z określonym składem zespołu.
     *
     * @param board Plansza gry.
     * @param team Drużyna, która ma zostać rozmieszczona.
     * @param comp Skład rozmieszczanej drużyny.
     * @param side Strona, do której należy drużyna.
     * @param positions Lista pozycji startowych.
     */
    static void placeSingleTeam(Board& board,
                                Team& team,
                                const TeamComposition& comp,
                                TeamSide side,
                                const QVector<QPair<int, int>>& positions);
};

#endif // UNITPLACEMENTHELPER_H
