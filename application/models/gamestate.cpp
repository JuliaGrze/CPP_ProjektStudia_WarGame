#include "gamestate.h"

namespace
{
/**
 * @brief Sprawdza, czy dana pozycja znajduje się w podanej kolekcji pól.
 *
 * Funkcja pomocnicza używana do sprawdzania, czy wskazane współrzędne
 * należą do listy pozycji dostępnych dla ruchu, ataku lub leczenia.
 *
 * @param positions Kolekcja pozycji.
 * @param x Współrzędna X sprawdzanego pola.
 * @param y Współrzędna Y sprawdzanego pola.
 * @return true, jeśli pozycja istnieje w kolekcji, w przeciwnym razie false.
 */
bool containsPosition(const QVector<QPair<int, int>>& positions, int x, int y)
{
    for (const auto& position : positions)
    {
        if (position.first == x && position.second == y)
            return true;
    }

    return false;
}

/**
 * @brief Zwraca nazwę strony konfliktu w formie tekstowej.
 *
 * @param side Strona konfliktu.
 * @return Nazwa strony wyświetlana użytkownikowi.
 */
QString sideDisplayName(TeamSide side)
{
    return side == TeamSide::Player ? "Niebiescy" : "Czerwoni";
}
}

/**
 * @brief Domyślny konstruktor stanu gry.
 *
 * Tworzy planszę o rozmiarze 10x10 oraz inicjalizuje
 * domyślne drużyny gracza i przeciwnika.
 */
GameState::GameState()
    : m_board(10, 10),
    m_playerTeam("Niebiescy", TeamSide::Player),
    m_enemyTeam("Czerwoni", TeamSide::Enemy)
{
}

/**
 * @brief Zwraca planszę gry.
 *
 * @return Referencja do planszy.
 */
Board& GameState::getBoard() { return m_board; }

/**
 * @brief Zwraca stałą planszę gry.
 *
 * @return Stała referencja do planszy.
 */
const Board& GameState::getBoard() const { return m_board; }

/**
 * @brief Ustawia planszę gry.
 *
 * @param board Nowa plansza.
 */
void GameState::setBoard(const Board& board) { m_board = board; }

/**
 * @brief Zwraca drużynę gracza.
 *
 * @return Referencja do drużyny gracza.
 */
Team& GameState::getPlayerTeam() { return m_playerTeam; }

/**
 * @brief Zwraca stałą drużynę gracza.
 *
 * @return Stała referencja do drużyny gracza.
 */
const Team& GameState::getPlayerTeam() const { return m_playerTeam; }

/**
 * @brief Zwraca drużynę przeciwnika.
 *
 * @return Referencja do drużyny przeciwnika.
 */
Team& GameState::getEnemyTeam() { return m_enemyTeam; }

/**
 * @brief Zwraca stałą drużynę przeciwnika.
 *
 * @return Stała referencja do drużyny przeciwnika.
 */
const Team& GameState::getEnemyTeam() const { return m_enemyTeam; }

/**
 * @brief Zwraca numer aktualnej tury.
 *
 * @return Numer aktualnej tury.
 */
int GameState::getCurrentTurn() const { return m_currentTurn; }

/**
 * @brief Ustawia numer aktualnej tury.
 *
 * @param turn Nowy numer tury.
 */
void GameState::setCurrentTurn(int turn) { m_currentTurn = turn; }

/**
 * @brief Zwraca aktualnie aktywną stronę.
 *
 * @return Aktywna strona konfliktu.
 */
TeamSide GameState::getCurrentSide() const { return m_currentSide; }

/**
 * @brief Ustawia aktualnie aktywną stronę.
 *
 * @param side Nowa aktywna strona.
 */
void GameState::setCurrentSide(TeamSide side) { m_currentSide = side; }

/**
 * @brief Zwraca drużynę aktualnie aktywnej strony.
 *
 * @return Referencja do aktualnej drużyny.
 */
Team& GameState::getCurrentTeam()
{
    return m_currentSide == TeamSide::Player ? m_playerTeam : m_enemyTeam;
}

/**
 * @brief Zwraca stałą drużynę aktualnie aktywnej strony.
 *
 * @return Stała referencja do aktualnej drużyny.
 */
const Team& GameState::getCurrentTeam() const
{
    return m_currentSide == TeamSide::Player ? m_playerTeam : m_enemyTeam;
}

/**
 * @brief Przechodzi do następnej tury.
 *
 * Metoda:
 * - przełącza aktywną stronę,
 * - zwiększa numer tury po zakończeniu pełnej rundy,
 * - czyści zaznaczenia i podświetlenia,
 * - resetuje punkty akcji,
 * - resetuje zasoby jednostek aktywnej strony,
 * - zapisuje komunikat do logu.
 */
void GameState::nextTurn()
{
    if (m_gameFinished)
        return;

    if (m_currentSide == TeamSide::Player)
        m_currentSide = TeamSide::Enemy;
    else
    {
        m_currentSide = TeamSide::Player;
        ++m_currentTurn;
    }

    clearSelectedPosition();
    clearAllHighlights();
    resetTurnActionPoints();
    resetCurrentSideUnitsForTurn();

    m_lastActionMessage = QString("Rozpoczęła się tura %1. AP drużyny: %2.")
                              .arg(m_currentSide == TeamSide::Player ? "Niebieskich" : "Czerwonych")
                              .arg(m_currentTurnActionPoints);

    addLogEntry(m_lastActionMessage);
}

/**
 * @brief Ustawia aktualnie zaznaczoną pozycję.
 *
 * @param x Współrzędna X.
 * @param y Współrzędna Y.
 */
void GameState::setSelectedPosition(int x, int y)
{
    m_selectedX = x;
    m_selectedY = y;
}

/**
 * @brief Czyści aktualnie zaznaczoną pozycję.
 */
void GameState::clearSelectedPosition()
{
    m_selectedX = -1;
    m_selectedY = -1;
}

/**
 * @brief Sprawdza, czy istnieje aktualnie zaznaczone pole.
 *
 * @return true, jeśli istnieje zaznaczenie, w przeciwnym razie false.
 */
bool GameState::hasSelectedPosition() const
{
    return m_selectedX >= 0 && m_selectedY >= 0;
}

/**
 * @brief Zwraca współrzędną X zaznaczonego pola.
 *
 * @return Współrzędna X.
 */
int GameState::getSelectedX() const { return m_selectedX; }

/**
 * @brief Zwraca współrzędną Y zaznaczonego pola.
 *
 * @return Współrzędna Y.
 */
int GameState::getSelectedY() const { return m_selectedY; }

/**
 * @brief Ustawia listę dostępnych pól ruchu.
 *
 * @param positions Lista pozycji dostępnych do ruchu.
 */
void GameState::setAvailableMovePositions(const QVector<QPair<int, int>>& positions)
{
    m_availableMovePositions = positions;
}

/**
 * @brief Zwraca listę dostępnych pól ruchu.
 *
 * @return Lista dostępnych pozycji.
 */
const QVector<QPair<int, int>>& GameState::getAvailableMovePositions() const
{
    return m_availableMovePositions;
}

/**
 * @brief Czyści listę dostępnych pól ruchu.
 */
void GameState::clearAvailableMovePositions()
{
    m_availableMovePositions.clear();
}

/**
 * @brief Sprawdza, czy wskazane pole jest dostępne do ruchu.
 *
 * @param x Współrzędna X pola.
 * @param y Współrzędna Y pola.
 * @return true, jeśli pole jest dostępne do ruchu.
 */
bool GameState::isMovePositionAvailable(int x, int y) const
{
    return containsPosition(m_availableMovePositions, x, y);
}

/**
 * @brief Ustawia listę zablokowanych pól ruchu.
 *
 * @param positions Lista zablokowanych pozycji.
 */
void GameState::setBlockedMovePositions(const QVector<QPair<int, int>>& positions)
{
    m_blockedMovePositions = positions;
}

/**
 * @brief Zwraca listę zablokowanych pól ruchu.
 *
 * @return Lista zablokowanych pozycji.
 */
const QVector<QPair<int, int>>& GameState::getBlockedMovePositions() const
{
    return m_blockedMovePositions;
}

/**
 * @brief Czyści listę zablokowanych pól ruchu.
 */
void GameState::clearBlockedMovePositions()
{
    m_blockedMovePositions.clear();
}

/**
 * @brief Sprawdza, czy wskazane pole jest oznaczone jako zablokowane.
 *
 * @param x Współrzędna X pola.
 * @param y Współrzędna Y pola.
 * @return true, jeśli pole jest zablokowane.
 */
bool GameState::isBlockedMovePosition(int x, int y) const
{
    return containsPosition(m_blockedMovePositions, x, y);
}

/**
 * @brief Ustawia listę pól możliwych do zaatakowania.
 *
 * @param positions Lista celów ataku.
 */
void GameState::setAttackablePositions(const QVector<QPair<int, int>>& positions)
{
    m_attackablePositions = positions;
}

/**
 * @brief Zwraca listę pól możliwych do zaatakowania.
 *
 * @return Lista celów ataku.
 */
const QVector<QPair<int, int>>& GameState::getAttackablePositions() const
{
    return m_attackablePositions;
}

/**
 * @brief Czyści listę pól możliwych do zaatakowania.
 */
void GameState::clearAttackablePositions()
{
    m_attackablePositions.clear();
}

/**
 * @brief Sprawdza, czy wskazane pole jest możliwe do zaatakowania.
 *
 * @param x Współrzędna X pola.
 * @param y Współrzędna Y pola.
 * @return true, jeśli pole jest celem ataku.
 */
bool GameState::isAttackablePosition(int x, int y) const
{
    return containsPosition(m_attackablePositions, x, y);
}

/**
 * @brief Ustawia listę pól możliwych do leczenia.
 *
 * @param positions Lista celów leczenia.
 */
void GameState::setHealablePositions(const QVector<QPair<int, int>>& positions)
{
    m_healablePositions = positions;
}

/**
 * @brief Zwraca listę pól możliwych do leczenia.
 *
 * @return Lista celów leczenia.
 */
const QVector<QPair<int, int>>& GameState::getHealablePositions() const
{
    return m_healablePositions;
}

/**
 * @brief Czyści listę pól możliwych do leczenia.
 */
void GameState::clearHealablePositions()
{
    m_healablePositions.clear();
}

/**
 * @brief Sprawdza, czy wskazane pole jest możliwe do leczenia.
 *
 * @param x Współrzędna X pola.
 * @param y Współrzędna Y pola.
 * @return true, jeśli pole jest celem leczenia.
 */
bool GameState::isHealablePosition(int x, int y) const
{
    return containsPosition(m_healablePositions, x, y);
}

/**
 * @brief Czyści wszystkie podświetlenia akcji.
 */
void GameState::clearAllHighlights()
{
    clearAvailableMovePositions();
    clearBlockedMovePositions();
    clearAttackablePositions();
    clearHealablePositions();
}

/**
 * @brief Ustawia ostatni komunikat akcji.
 *
 * Dodatkowo zapisuje komunikat do logu bitwy.
 *
 * @param message Treść komunikatu.
 */
void GameState::setLastActionMessage(const QString& message)
{
    m_lastActionMessage = message;
    addLogEntry(message);
}

/**
 * @brief Zwraca ostatni komunikat akcji.
 *
 * @return Treść ostatniego komunikatu.
 */
QString GameState::getLastActionMessage() const
{
    return m_lastActionMessage;
}

/**
 * @brief Dodaje wpis do logu bitwy.
 *
 * Pusty lub biały tekst nie jest dodawany.
 *
 * @param message Treść wpisu.
 */
void GameState::addLogEntry(const QString& message)
{
    if (!message.trimmed().isEmpty())
    {
        m_battleLog.append(QString("[Tura %1] %2")
                               .arg(m_currentTurn)
                               .arg(message));
    }
}

/**
 * @brief Zwraca log bitwy.
 *
 * @return Lista wpisów logu.
 */
const QStringList& GameState::getBattleLog() const
{
    return m_battleLog;
}

/**
 * @brief Zwraca aktualną liczbę punktów akcji.
 *
 * @return Aktualna liczba AP.
 */
int GameState::getCurrentTurnActionPoints() const
{
    return m_currentTurnActionPoints;
}

/**
 * @brief Zwraca maksymalną liczbę punktów akcji.
 *
 * @return Maksymalna liczba AP.
 */
int GameState::getMaxTurnActionPoints() const
{
    return m_maxTurnActionPoints;
}

/**
 * @brief Sprawdza, czy drużyna ma jeszcze punkty akcji.
 *
 * @return true, jeśli liczba AP jest większa od zera.
 */
bool GameState::hasTurnActionPoints() const
{
    return m_currentTurnActionPoints > 0;
}

/**
 * @brief Zużywa określoną liczbę punktów akcji.
 *
 * Liczba AP nie spadnie poniżej zera.
 *
 * @param amount Liczba punktów do zużycia.
 */
void GameState::consumeTurnActionPoints(int amount)
{
    if (amount <= 0)
        return;

    m_currentTurnActionPoints -= amount;
    if (m_currentTurnActionPoints < 0)
        m_currentTurnActionPoints = 0;
}

/**
 * @brief Resetuje punkty akcji do wartości maksymalnej.
 */
void GameState::resetTurnActionPoints()
{
    m_currentTurnActionPoints = m_maxTurnActionPoints;
}

/**
 * @brief Sprawdza, czy gra została zakończona.
 *
 * @return true, jeśli gra się zakończyła.
 */
bool GameState::isGameFinished() const
{
    return m_gameFinished;
}

/**
 * @brief Zwraca zwycięską stronę.
 *
 * @return Zwycięska strona konfliktu.
 */
TeamSide GameState::getWinnerSide() const
{
    return m_winnerSide;
}

/**
 * @brief Aktualizuje stan zwycięstwa.
 *
 * Metoda sprawdza, czy któraś ze stron utraciła wszystkie jednostki.
 * W przypadku zakończenia gry ustawia zwycięzcę, komunikat końcowy
 * oraz dodaje podsumowanie do logu.
 *
 * @return true, jeśli gra została zakończona, w przeciwnym razie false.
 */
bool GameState::updateVictoryState()
{
    if (m_gameFinished)
        return true;

    const bool playerAlive = m_playerTeam.hasUnitsAlive();
    const bool enemyAlive = m_enemyTeam.hasUnitsAlive();

    if (playerAlive && enemyAlive)
        return false;

    m_gameFinished = true;
    m_winnerSide = playerAlive ? TeamSide::Player : TeamSide::Enemy;
    m_lastActionMessage = QString("Koniec gry. Wygrywają %1.")
                              .arg(m_winnerSide == TeamSide::Player ? "Niebiescy" : "Czerwoni");
    addLogEntry(m_lastActionMessage);
    addLogEntry(getPostGameSummaryText());

    return true;
}

/**
 * @brief Resetuje zasoby jednostek aktualnej strony na początku tury.
 */
void GameState::resetCurrentSideUnitsForTurn()
{
    const auto units = getCurrentTeam().getUnits();

    for (const auto& unit : units)
    {
        if (unit && unit->isAlive())
            unit->resetTurnResources();
    }
}

/**
 * @brief Zwraca referencję do statystyk wskazanej strony.
 *
 * @param side Strona konfliktu.
 * @return Referencja do statystyk tej strony.
 */
TeamBattleStats& GameState::getStatsForSideInternal(TeamSide side)
{
    return side == TeamSide::Player ? m_playerStats : m_enemyStats;
}

/**
 * @brief Zwraca stałą referencję do statystyk wskazanej strony.
 *
 * @param side Strona konfliktu.
 * @return Stała referencja do statystyk tej strony.
 */
const TeamBattleStats& GameState::getStatsForSideInternal(TeamSide side) const
{
    return side == TeamSide::Player ? m_playerStats : m_enemyStats;
}

/**
 * @brief Rejestruje oddanie strzału przez wskazaną stronę.
 *
 * @param side Strona, która oddała strzał.
 */
void GameState::recordShotFired(TeamSide side)
{
    getStatsForSideInternal(side).shotsFired++;
}

/**
 * @brief Rejestruje udane trafienie i zadane obrażenia.
 *
 * @param side Strona, która trafiła.
 * @param damage Liczba zadanych obrażeń.
 */
void GameState::recordSuccessfulHit(TeamSide side, int damage)
{
    TeamBattleStats& stats = getStatsForSideInternal(side);
    stats.hits++;
    stats.damageDealt += damage;
}

/**
 * @brief Rejestruje zniszczenie jednostki przeciwnika.
 *
 * @param side Strona, która zniszczyła jednostkę.
 */
void GameState::recordUnitDestroyed(TeamSide side)
{
    getStatsForSideInternal(side).unitsDestroyed++;
}

/**
 * @brief Zwraca statystyki wskazanej strony.
 *
 * @param side Strona konfliktu.
 * @return Kopia statystyk wskazanej strony.
 */
TeamBattleStats GameState::getStatsForSide(TeamSide side) const
{
    return getStatsForSideInternal(side);
}

/**
 * @brief Oblicza procent skuteczności trafień dla wskazanej strony.
 *
 * @param side Strona konfliktu.
 * @return Procent skutecznych trafień.
 */
int GameState::getAccuracyPercent(TeamSide side) const
{
    const TeamBattleStats& stats = getStatsForSideInternal(side);

    if (stats.shotsFired <= 0)
        return 0;

    return (stats.hits * 100) / stats.shotsFired;
}

/**
 * @brief Zwraca liczbę strat wskazanej strony.
 *
 * @param side Strona konfliktu.
 * @return Liczba utraconych jednostek.
 */
int GameState::getLossesForSide(TeamSide side) const
{
    const Team& team = side == TeamSide::Player ? m_playerTeam : m_enemyTeam;
    return team.getUnitsCount() - team.getAliveUnitsCount();
}

/**
 * @brief Buduje tekstowe podsumowanie po zakończeniu gry.
 *
 * @return Tekstowe podsumowanie bitwy.
 */
QString GameState::getPostGameSummaryText() const
{
    const TeamBattleStats playerStats = getStatsForSide(TeamSide::Player);
    const TeamBattleStats enemyStats = getStatsForSide(TeamSide::Enemy);

    return QString(
               "PODSUMOWANIE BITWY\n"
               "Liczba tur: %1\n"
               "Niebiescy -> straty: %2, obrażenia: %3, trafienia: %4/%5, skuteczność: %6%%, zniszczone jednostki: %7\n"
               "Czerwoni -> straty: %8, obrażenia: %9, trafienia: %10/%11, skuteczność: %12%%, zniszczone jednostki: %13")
        .arg(m_currentTurn)
        .arg(getLossesForSide(TeamSide::Player))
        .arg(playerStats.damageDealt)
        .arg(playerStats.hits)
        .arg(playerStats.shotsFired)
        .arg(getAccuracyPercent(TeamSide::Player))
        .arg(playerStats.unitsDestroyed)
        .arg(getLossesForSide(TeamSide::Enemy))
        .arg(enemyStats.damageDealt)
        .arg(enemyStats.hits)
        .arg(enemyStats.shotsFired)
        .arg(getAccuracyPercent(TeamSide::Enemy))
        .arg(enemyStats.unitsDestroyed);
}

/**
 * @brief Buduje HTML-owe podsumowanie po zakończeniu gry.
 *
 * @return Fragment HTML z podsumowaniem bitwy.
 */
QString GameState::getPostGameSummaryHtml() const
{
    const TeamBattleStats playerStats = getStatsForSide(TeamSide::Player);
    const TeamBattleStats enemyStats = getStatsForSide(TeamSide::Enemy);

    return QString(
               "<div style='text-align:center;'>"
               "<div style='font-size:18px; font-weight:800; color:#f9fafb;'>PODSUMOWANIE BITWY</div>"
               "<div style='margin-top:8px; color:#fbbf24; font-weight:700;'>Liczba tur: %1</div>"
               "<div style='margin-top:14px; color:#60a5fa; font-weight:800;'>NIEBIESCY</div>"
               "<div style='line-height:1.7; color:#dbeafe;'>"
               "Straty: %2<br/>"
               "Zadane obrażenia: %3<br/>"
               "Trafienia: %4 / %5<br/>"
               "Skuteczność: %6%%<br/>"
               "Zniszczone jednostki: %7"
               "</div>"
               "<div style='margin-top:14px; color:#f87171; font-weight:800;'>CZERWONI</div>"
               "<div style='line-height:1.7; color:#fee2e2;'>"
               "Straty: %8<br/>"
               "Zadane obrażenia: %9<br/>"
               "Trafienia: %10 / %11<br/>"
               "Skuteczność: %12%%<br/>"
               "Zniszczone jednostki: %13"
               "</div>"
               "</div>")
        .arg(m_currentTurn)
        .arg(getLossesForSide(TeamSide::Player))
        .arg(playerStats.damageDealt)
        .arg(playerStats.hits)
        .arg(playerStats.shotsFired)
        .arg(getAccuracyPercent(TeamSide::Player))
        .arg(playerStats.unitsDestroyed)
        .arg(getLossesForSide(TeamSide::Enemy))
        .arg(enemyStats.damageDealt)
        .arg(enemyStats.hits)
        .arg(enemyStats.shotsFired)
        .arg(getAccuracyPercent(TeamSide::Enemy))
        .arg(enemyStats.unitsDestroyed);
}
