#ifndef BATTLEUIFORMATTER_H
#define BATTLEUIFORMATTER_H

#include <QString>

#include "../models/enums/teamside.h"
#include "../models/enums/terraintype.h"
#include "../models/gamestate.h"

class Tile;
class Unit;
class GameState;

/**
 * @brief Klasa pomocnicza do formatowania danych dla interfejsu bitwy.
 *
 * Zawiera statyczne metody odpowiedzialne za generowanie tekstów
 * oraz fragmentów HTML wykorzystywanych w interfejsie użytkownika.
 *
 * Oddziela logikę prezentacji od logiki gry, co poprawia czytelność
 * i utrzymywalność kodu.
 */
class BattleUiFormatter
{
public:
    /**
     * @brief Zwraca nazwę terenu w formie tekstowej.
     *
     * @param terrain Typ terenu.
     * @return Tekstowa reprezentacja terenu.
     */
    static QString terrainToDisplayName(TerrainType terrain);

    /**
     * @brief Buduje domyślny tekst informacji o polu.
     *
     * @return Tekst wyświetlany, gdy nie wybrano żadnego pola.
     */
    static QString buildDefaultTileInfoText();

    /**
     * @brief Buduje tekst informacji o wybranym polu planszy.
     *
     * @param tile Wskaźnik na pole planszy.
     * @return Tekst opisujący pole i ewentualną jednostkę.
     */
    static QString buildTileInfoText(const Tile* tile);

    /**
     * @brief Buduje HTML z informacją o turze.
     *
     * @param currentTurn Numer aktualnej tury.
     * @param currentSide Aktualnie aktywna strona.
     * @param gameFinished Informacja, czy gra została zakończona.
     * @param winnerSide Strona zwycięska (jeśli gra zakończona).
     * @return HTML reprezentujący informacje o turze.
     */
    static QString buildTurnInfoHtml(
        int currentTurn,
        TeamSide currentSide,
        bool gameFinished,
        TeamSide winnerSide
        );

    /**
     * @brief Buduje HTML z informacjami o drużynach.
     *
     * @param playerCount Liczba jednostek gracza.
     * @param enemyCount Liczba jednostek przeciwnika.
     * @param currentSide Aktualnie aktywna strona.
     * @param currentAp Aktualna liczba punktów akcji.
     * @param maxAp Maksymalna liczba punktów akcji.
     * @param playerStats Statystyki drużyny gracza.
     * @param enemyStats Statystyki drużyny przeciwnika.
     * @param playerAccuracy Skuteczność gracza.
     * @param enemyAccuracy Skuteczność przeciwnika.
     * @return HTML z informacjami o drużynach.
     */
    static QString buildTeamsInfoHtml(
        int playerCount,
        int enemyCount,
        TeamSide currentSide,
        int currentAp,
        int maxAp,
        const TeamBattleStats& playerStats,
        const TeamBattleStats& enemyStats,
        int playerAccuracy,
        int enemyAccuracy
        );

    /**
     * @brief Buduje HTML informujący o braku zaznaczonej jednostki.
     *
     * @return HTML z komunikatem.
     */
    static QString buildNoUnitSelectedNameHtml();

    /**
     * @brief Buduje HTML ze statystykami przy braku zaznaczonej jednostki.
     *
     * @param currentAp Aktualna liczba punktów akcji.
     * @param maxAp Maksymalna liczba punktów akcji.
     * @param currentTurn Numer aktualnej tury.
     * @param playerStats Statystyki drużyny gracza.
     * @param enemyStats Statystyki drużyny przeciwnika.
     * @return HTML ze statystykami.
     */
    static QString buildNoUnitSelectedStatsHtml(
        int currentAp,
        int maxAp,
        int currentTurn,
        const TeamBattleStats& playerStats,
        const TeamBattleStats& enemyStats
        );

    /**
     * @brief Buduje HTML z nazwą zaznaczonej jednostki.
     *
     * @param unit Wskaźnik na jednostkę.
     * @return HTML z nazwą jednostki.
     */
    static QString buildSelectedUnitNameHtml(const Unit* unit);

    /**
     * @brief Buduje HTML ze statystykami zaznaczonej jednostki.
     *
     * @param unit Wskaźnik na jednostkę.
     * @param moveCount Liczba wykonanych ruchów.
     * @param attackCount Liczba wykonanych ataków.
     * @param healCount Liczba wykonanych akcji leczenia.
     * @param currentAp Aktualna liczba punktów akcji.
     * @param maxAp Maksymalna liczba punktów akcji.
     * @param currentTurn Numer aktualnej tury.
     * @return HTML ze statystykami jednostki.
     */
    static QString buildSelectedUnitStatsHtml(
        const Unit* unit,
        int moveCount,
        int attackCount,
        int healCount,
        int currentAp,
        int maxAp,
        int currentTurn
        );

    /**
     * @brief Buduje HTML podsumowania po zakończeniu gry.
     *
     * @param gameState Stan gry po zakończeniu.
     * @return HTML zawierający podsumowanie rozgrywki.
     */
    static QString buildPostGameSummaryHtml(const GameState& gameState);
};

#endif // BATTLEUIFORMATTER_H
