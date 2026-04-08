#include "battleuiformatter.h"

#include "../models/gamestate.h"
#include "../models/tile.h"
#include "../models/unit.h"

/**
 * @brief Zwraca nazwę terenu w formie tekstowej.
 *
 * Metoda konwertuje wartość wyliczeniową typu terenu
 * na nazwę czytelną dla użytkownika.
 *
 * @param terrain Typ terenu.
 * @return Nazwa terenu wyświetlana w interfejsie.
 */
QString BattleUiFormatter::terrainToDisplayName(TerrainType terrain)
{
    switch (terrain)
    {
    case TerrainType::Plain:    return "Równina";
    case TerrainType::Forest:   return "Las";
    case TerrainType::Mountain: return "Wzgórze";
    case TerrainType::Water:    return "Woda";
    case TerrainType::Building: return "Budynek";
    }

    return "Nieznany";
}

/**
 * @brief Buduje domyślny tekst informacji o polu.
 *
 * Tekst ten jest wyświetlany, gdy użytkownik nie zaznaczył
 * jeszcze żadnego pola planszy.
 *
 * @return Domyślny komunikat informacyjny.
 */
QString BattleUiFormatter::buildDefaultTileInfoText()
{
    return "Brak wybranego pola.\n"
           "Kliknij swoją jednostkę,\n"
           "aby zobaczyć informacje o terenie.";
}

/**
 * @brief Buduje tekst opisujący wskazane pole planszy.
 *
 * Metoda przygotowuje tekst z podstawowymi informacjami
 * o terenie, koszcie ruchu, osłonie, bonusie zasięgu
 * oraz możliwości przejścia przez pole.
 *
 * @param tile Wskaźnik na pole planszy.
 * @return Tekst opisujący pole planszy.
 */
QString BattleUiFormatter::buildTileInfoText(const Tile* tile)
{
    if (!tile)
        return buildDefaultTileInfoText();

    const QString terrainName = terrainToDisplayName(tile->getTerrain());
    const QString walkableText = tile->isWalkable() ? "tak" : "nie";

    return QString("Teren: %1\n"
                   "Koszt ruchu: %2\n"
                   "Osłona: +%3\n"
                   "Bonus zasięgu: +%4\n"
                   "Przechodnie: %5")
        .arg(terrainName)
        .arg(tile->getMovementCost())
        .arg(tile->getCoverBonus())
        .arg(tile->getRangeBonus())
        .arg(walkableText);
}

/**
 * @brief Buduje HTML z informacją o aktualnej turze.
 *
 * Jeśli gra została zakończona, metoda zwraca komunikat
 * o końcu gry i zwycięzcy. W przeciwnym razie zwraca
 * informację o numerze tury i aktywnej stronie.
 *
 * @param currentTurn Numer aktualnej tury.
 * @param currentSide Aktualnie aktywna strona.
 * @param gameFinished Informacja, czy gra została zakończona.
 * @param winnerSide Strona zwycięska.
 * @return Fragment HTML z informacją o turze.
 */
QString BattleUiFormatter::buildTurnInfoHtml(
    int currentTurn,
    TeamSide currentSide,
    bool gameFinished,
    TeamSide winnerSide)
{
    if (gameFinished)
    {
        const bool playerWon = winnerSide == TeamSide::Player;
        const QString winnerText = playerWon ? "ZWYCIĘSTWO: NIEBIESCY" : "ZWYCIĘSTWO: CZERWONI";
        const QString winnerColor = playerWon ? "#60a5fa" : "#f87171";

        return QString(
                   "<div style='text-align:center;'>"
                   "<div style='font-size:18px; font-weight:800; color:#f9fafb;'>Koniec gry</div>"
                   "<div style='font-size:14px; font-weight:800; color:%1; margin-top:4px;'>%2</div>"
                   "<div style='font-size:12px; color:#d1d5db; margin-top:4px;'>Liczba tur: %3</div>"
                   "</div>"
                   )
            .arg(winnerColor)
            .arg(winnerText)
            .arg(currentTurn);
    }

    const bool isPlayerTurn = currentSide == TeamSide::Player;
    const QString sideText = isPlayerTurn ? "NIEBIESCY" : "CZERWONI";
    const QString sideColor = isPlayerTurn ? "#60a5fa" : "#f87171";

    return QString(
               "<div style='text-align:center;'>"
               "<div style='font-size:18px; font-weight:800; color:#f9fafb;'>Tura %1</div>"
               "<div style='font-size:14px; font-weight:700; color:%2; margin-top:4px;'>Ruch: %3</div>"
               "</div>"
               )
        .arg(currentTurn)
        .arg(sideColor)
        .arg(sideText);
}

/**
 * @brief Buduje HTML z informacjami o obu drużynach.
 *
 * Metoda przygotowuje podsumowanie liczby jednostek,
 * aktualnej strony, punktów akcji oraz statystyk trafień.
 *
 * @param playerCount Liczba jednostek drużyny gracza.
 * @param enemyCount Liczba jednostek drużyny przeciwnika.
 * @param currentSide Aktualnie aktywna strona.
 * @param currentAp Aktualna liczba punktów akcji.
 * @param maxAp Maksymalna liczba punktów akcji.
 * @param playerStats Statystyki drużyny gracza.
 * @param enemyStats Statystyki drużyny przeciwnika.
 * @param playerAccuracy Skuteczność drużyny gracza.
 * @param enemyAccuracy Skuteczność drużyny przeciwnika.
 * @return Fragment HTML z informacjami o drużynach.
 */
QString BattleUiFormatter::buildTeamsInfoHtml(
    int playerCount,
    int enemyCount,
    TeamSide currentSide,
    int currentAp,
    int maxAp,
    const TeamBattleStats& playerStats,
    const TeamBattleStats& enemyStats,
    int playerAccuracy,
    int enemyAccuracy)
{
    const bool isPlayerTurn = currentSide == TeamSide::Player;
    const QString currentSideText = isPlayerTurn ? "NIEBIESCY" : "CZERWONI";
    const QString currentSideColor = isPlayerTurn ? "#60a5fa" : "#f87171";

    return QString(
               "<div style='text-align:center;'>"
               "<span style='color:#60a5fa; font-weight:700;'>Niebiescy: %1</span>"
               "<br/>"
               "<span style='color:#f87171; font-weight:700;'>Czerwoni: %2</span>"
               "<br/><br/>"
               "<span style='color:#d1d5db;'>Aktualny ruch:</span> "
               "<span style='color:%3; font-weight:800; font-size:16px;'>%4</span>"
               "<br/>"
               "<span style='color:#fbbf24; font-weight:700;'>AP drużyny:</span> %5 / %6"
               "<br/><br/>"
               "<span style='color:#60a5fa; font-weight:700;'>Niebiescy trafienia:</span> %7 / %8 (%9%%)"
               "<br/>"
               "<span style='color:#f87171; font-weight:700;'>Czerwoni trafienia:</span> %10 / %11 (%12%%)"
               "</div>"
               )
        .arg(playerCount)
        .arg(enemyCount)
        .arg(currentSideColor)
        .arg(currentSideText)
        .arg(currentAp)
        .arg(maxAp)
        .arg(playerStats.hits)
        .arg(playerStats.shotsFired)
        .arg(playerAccuracy)
        .arg(enemyStats.hits)
        .arg(enemyStats.shotsFired)
        .arg(enemyAccuracy);
}

/**
 * @brief Buduje HTML informujący o braku zaznaczonej jednostki.
 *
 * @return Fragment HTML z komunikatem o braku wyboru.
 */
QString BattleUiFormatter::buildNoUnitSelectedNameHtml()
{
    return
        "<div style='text-align:center;'>"
        "<span style='font-size:12px; color:#9ca3af;'>PANEL JEDNOSTKI</span><br/>"
        "<span style='font-size:18px; font-weight:800; color:#e5e7eb;'>Brak wyboru</span>"
        "</div>";
}

/**
 * @brief Buduje HTML ze statystykami wyświetlanymi przy braku wyboru jednostki.
 *
 * @param currentAp Aktualna liczba punktów akcji.
 * @param maxAp Maksymalna liczba punktów akcji.
 * @param currentTurn Numer aktualnej tury.
 * @param playerStats Statystyki drużyny gracza.
 * @param enemyStats Statystyki drużyny przeciwnika.
 * @return Fragment HTML ze statystykami ogólnymi.
 */
QString BattleUiFormatter::buildNoUnitSelectedStatsHtml(
    int currentAp,
    int maxAp,
    int currentTurn,
    const TeamBattleStats& playerStats,
    const TeamBattleStats& enemyStats)
{
    return QString(
               "<div style='line-height:1.8; color:#9ca3af; text-align:center;'>"
               "AP drużyny: <b>%1 / %2</b><br/>"
               "Tura: <b>%3</b><br/>"
               "Niebiescy trafienia: <b>%4 / %5</b><br/>"
               "Czerwoni trafienia: <b>%6 / %7</b><br/>"
               "Kliknij swoją jednostkę, a potem puste pole, niebieskiego sojusznika do leczenia albo fioletowego przeciwnika."
               "</div>"
               )
        .arg(currentAp)
        .arg(maxAp)
        .arg(currentTurn)
        .arg(playerStats.hits)
        .arg(playerStats.shotsFired)
        .arg(enemyStats.hits)
        .arg(enemyStats.shotsFired);
}

/**
 * @brief Buduje HTML z nazwą zaznaczonej jednostki.
 *
 * Oprócz nazwy jednostki wyświetlana jest również informacja
 * o drużynie, do której jednostka należy.
 *
 * @param unit Wskaźnik na zaznaczoną jednostkę.
 * @return Fragment HTML z nazwą jednostki.
 */
QString BattleUiFormatter::buildSelectedUnitNameHtml(const Unit* unit)
{
    if (!unit)
        return buildNoUnitSelectedNameHtml();

    const bool isPlayerUnit = unit->getSide() == TeamSide::Player;
    const QString teamText = isPlayerUnit ? "DRUŻYNA: NIEBIESCY" : "DRUŻYNA: CZERWONI";
    const QString teamColor = isPlayerUnit ? "#60a5fa" : "#f87171";

    return QString(
               "<div style='text-align:center;'>"
               "<span style='font-size:12px; color:#9ca3af;'>WYBRANA JEDNOSTKA</span><br/>"
               "<span style='font-size:18px; font-weight:800; color:white;'>%1</span><br/>"
               "<span style='font-size:13px; font-weight:700; color:%2;'>%3</span>"
               "</div>"
               )
        .arg(unit->getName())
        .arg(teamColor)
        .arg(teamText);
}

/**
 * @brief Buduje HTML ze szczegółowymi statystykami zaznaczonej jednostki.
 *
 * Metoda przygotowuje informacje o zdrowiu, ataku, zasięgu,
 * ruchu, pancerzu, celności, unikach, leczeniu oraz liczbie
 * dostępnych oznaczonych pól i aktualnych punktach akcji drużyny.
 *
 * @param unit Wskaźnik na zaznaczoną jednostkę.
 * @param moveCount Liczba dostępnych pól ruchu.
 * @param attackCount Liczba dostępnych celów ataku.
 * @param healCount Liczba dostępnych celów leczenia.
 * @param currentAp Aktualna liczba punktów akcji.
 * @param maxAp Maksymalna liczba punktów akcji.
 * @param currentTurn Numer aktualnej tury.
 * @return Fragment HTML ze statystykami jednostki.
 */
QString BattleUiFormatter::buildSelectedUnitStatsHtml(
    const Unit* unit,
    int moveCount,
    int attackCount,
    int healCount,
    int currentAp,
    int maxAp,
    int currentTurn)
{
    if (!unit)
        return "";

    return QString(
               "<div style='line-height:1.7;'>"
               "<span style='color:#fca5a5; font-weight:700;'>HP:</span> %1 / %2<br/>"
               "<span style='color:#fdba74; font-weight:700;'>Atak:</span> %3<br/>"
               "<span style='color:#86efac; font-weight:700;'>Zasięg:</span> %4 - %5<br/>"
               "<span style='color:#93c5fd; font-weight:700;'>Ruch:</span> do %6 pól<br/>"
               "<span style='color:#fbbf24; font-weight:700;'>Koszt ataku:</span> %7 AP<br/>"
               "<span style='color:#c4b5fd; font-weight:700;'>Pancerz:</span> %8<br/>"
               "<span style='color:#f9a8d4; font-weight:700;'>Celność:</span> %9<br/>"
               "<span style='color:#a7f3d0; font-weight:700;'>Unik:</span> %10<br/>"
               "<span style='color:#38bdf8; font-weight:700;'>Leczenie:</span> %11<br/>"
               "<span style='color:#d1d5db;'>Ruch wykonany wcześniej:</span> %12<br/>"
               "<span style='color:#d1d5db;'>Atak / leczenie wcześniej:</span> %13<br/>"
               "<span style='color:#22c55e; font-weight:700;'>Zielone pola:</span> %14<br/>"
               "<span style='color:#a855f7; font-weight:700;'>Fioletowe cele ataku:</span> %15<br/>"
               "<span style='color:#38bdf8; font-weight:700;'>Niebieskie cele leczenia:</span> %16<br/>"
               "<span style='color:#cbd5e1;'>AP drużyny:</span> %17 / %18<br/>"
               "<span style='color:#fbbf24;'>Tura:</span> %19"
               "</div>"
               )
        .arg(unit->getHealth())
        .arg(unit->getMaxHealth())
        .arg(unit->getDamage())
        .arg(unit->getMinRange())
        .arg(unit->getRange())
        .arg(unit->getMovementPoints())
        .arg(unit->getAttackCost())
        .arg(unit->getArmor())
        .arg(unit->getAccuracy())
        .arg(unit->getEvasion())
        .arg(unit->canHeal() ? QString::number(unit->getHealAmount()) : "-")
        .arg(unit->hasMovedThisTurn() ? "tak" : "nie")
        .arg(unit->hasActedThisTurn() ? "tak" : "nie")
        .arg(moveCount)
        .arg(attackCount)
        .arg(healCount)
        .arg(currentAp)
        .arg(maxAp)
        .arg(currentTurn);
}

/**
 * @brief Buduje HTML z podsumowaniem po zakończeniu gry.
 *
 * Metoda przygotowuje rozbudowane podsumowanie wyników bitwy,
 * zawierające zwycięzcę, liczbę tur oraz statystyki obu drużyn.
 *
 * @param gameState Końcowy stan gry.
 * @return Fragment HTML z podsumowaniem rozgrywki.
 */
QString BattleUiFormatter::buildPostGameSummaryHtml(const GameState& gameState)
{
    const TeamBattleStats playerStats = gameState.getStatsForSide(TeamSide::Player);
    const TeamBattleStats enemyStats = gameState.getStatsForSide(TeamSide::Enemy);

    const bool playerWon = gameState.getWinnerSide() == TeamSide::Player;
    const QString winnerColor = playerWon ? "#60a5fa" : "#f87171";
    const QString winnerText = playerWon ? "Wygrywają Niebiescy" : "Wygrywają Czerwoni";

    return QString(
               "<div style='font-family:Segoe UI, Arial, sans-serif; width:100%%;'>"

               "<div style='text-align:center; margin-bottom:20px;'>"
               "<div style='font-size:30px; font-weight:900; color:#f9fafb;'>PODSUMOWANIE BITWY</div>"
               "<div style='margin-top:10px; font-size:20px; font-weight:800; color:%1;'>%2</div>"
               "<div style='margin-top:8px; font-size:16px; color:#fbbf24; font-weight:800;'>Liczba tur: %3</div>"
               "</div>"

               "<div style='text-align:center;'>"
               "<table style='margin-left:auto; margin-right:auto; border-collapse:separate; border-spacing:22px 0;'>"
               "<tr>"

               "<td valign='top' "
               "style='min-width:300px; max-width:300px; background-color:#111827; border:2px solid #2563eb; border-radius:16px; padding:18px 22px;'>"
               "<div style='text-align:center; font-size:24px; font-weight:900; color:#60a5fa; margin-bottom:14px;'>NIEBIESCY</div>"
               "<div style='font-size:18px; line-height:2.0; color:#e5e7eb;'>"
               "<b style='color:#93c5fd;'>Straty:</b> %4<br/>"
               "<b style='color:#93c5fd;'>Zadane obrażenia:</b> %5<br/>"
               "<b style='color:#93c5fd;'>Trafienia:</b> %6 / %7<br/>"
               "<b style='color:#93c5fd;'>Skuteczność:</b> %8%%<br/>"
               "<b style='color:#93c5fd;'>Zniszczone jednostki:</b> %9"
               "</div>"
               "</td>"

               "<td valign='top' "
               "style='min-width:300px; max-width:300px; background-color:#111827; border:2px solid #dc2626; border-radius:16px; padding:18px 22px;'>"
               "<div style='text-align:center; font-size:24px; font-weight:900; color:#f87171; margin-bottom:14px;'>CZERWONI</div>"
               "<div style='font-size:18px; line-height:2.0; color:#e5e7eb;'>"
               "<b style='color:#fca5a5;'>Straty:</b> %10<br/>"
               "<b style='color:#fca5a5;'>Zadane obrażenia:</b> %11<br/>"
               "<b style='color:#fca5a5;'>Trafienia:</b> %12 / %13<br/>"
               "<b style='color:#fca5a5;'>Skuteczność:</b> %14%%<br/>"
               "<b style='color:#fca5a5;'>Zniszczone jednostki:</b> %15"
               "</div>"
               "</td>"

               "</tr>"
               "</table>"
               "</div>"

               "<div style='margin-top:24px; text-align:center; color:#9ca3af; font-size:14px;'>"
               "Wybierz, co chcesz zrobić dalej."
               "</div>"

               "</div>"
               )
        .arg(winnerColor)
        .arg(winnerText)
        .arg(gameState.getCurrentTurn())
        .arg(gameState.getLossesForSide(TeamSide::Player))
        .arg(playerStats.damageDealt)
        .arg(playerStats.hits)
        .arg(playerStats.shotsFired)
        .arg(gameState.getAccuracyPercent(TeamSide::Player))
        .arg(playerStats.unitsDestroyed)
        .arg(gameState.getLossesForSide(TeamSide::Enemy))
        .arg(enemyStats.damageDealt)
        .arg(enemyStats.hits)
        .arg(enemyStats.shotsFired)
        .arg(gameState.getAccuracyPercent(TeamSide::Enemy))
        .arg(enemyStats.unitsDestroyed);
}
