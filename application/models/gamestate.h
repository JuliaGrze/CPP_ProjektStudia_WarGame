#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.h"
#include "team.h"
#include "enums/teamside.h"

#include <QVector>
#include <QPair>
#include <QString>
#include <QStringList>

/**
 * @brief Struktura przechowująca statystyki drużyny w trakcie bitwy.
 */
struct TeamBattleStats
{
    int shotsFired = 0;     /**< Liczba oddanych strzałów. */
    int hits = 0;           /**< Liczba trafień. */
    int damageDealt = 0;    /**< Łączne zadane obrażenia. */
    int unitsDestroyed = 0; /**< Liczba zniszczonych jednostek przeciwnika. */
};

/**
 * @brief Reprezentuje pełny stan gry.
 *
 * Klasa przechowuje wszystkie informacje potrzebne do prowadzenia rozgrywki:
 * - planszę i drużyny,
 * - aktualną turę i aktywną stronę,
 * - zaznaczenia i dostępne akcje,
 * - punkty akcji,
 * - logi i komunikaty,
 * - statystyki oraz stan zakończenia gry.
 *
 * Jest centralnym modelem aplikacji (Model w architekturze MVC/MVVM).
 */
class GameState
{
public:
    GameState();

    /** @name Plansza */
    ///@{
    Board& getBoard();
    const Board& getBoard() const;
    void setBoard(const Board& board);
    ///@}

    /** @name Drużyny */
    ///@{
    Team& getPlayerTeam();
    const Team& getPlayerTeam() const;

    Team& getEnemyTeam();
    const Team& getEnemyTeam() const;
    ///@}

    /** @name Tura i strona */
    ///@{
    int getCurrentTurn() const;
    void setCurrentTurn(int turn);

    TeamSide getCurrentSide() const;
    void setCurrentSide(TeamSide side);

    void nextTurn();
    ///@}

    /** @name Zaznaczenie */
    ///@{
    void setSelectedPosition(int x, int y);
    void clearSelectedPosition();
    bool hasSelectedPosition() const;
    int getSelectedX() const;
    int getSelectedY() const;
    ///@}

    /** @name Ruch */
    ///@{
    void setAvailableMovePositions(const QVector<QPair<int, int>>& positions);
    const QVector<QPair<int, int>>& getAvailableMovePositions() const;
    void clearAvailableMovePositions();
    bool isMovePositionAvailable(int x, int y) const;
    ///@}

    /** @name Zablokowane pola */
    ///@{
    void setBlockedMovePositions(const QVector<QPair<int, int>>& positions);
    const QVector<QPair<int, int>>& getBlockedMovePositions() const;
    void clearBlockedMovePositions();
    bool isBlockedMovePosition(int x, int y) const;
    ///@}

    /** @name Atak */
    ///@{
    void setAttackablePositions(const QVector<QPair<int, int>>& positions);
    const QVector<QPair<int, int>>& getAttackablePositions() const;
    void clearAttackablePositions();
    bool isAttackablePosition(int x, int y) const;
    ///@}

    /** @name Leczenie */
    ///@{
    void setHealablePositions(const QVector<QPair<int, int>>& positions);
    const QVector<QPair<int, int>>& getHealablePositions() const;
    void clearHealablePositions();
    bool isHealablePosition(int x, int y) const;
    ///@}

    /**
     * @brief Usuwa wszystkie oznaczenia (ruch, atak, leczenie).
     */
    void clearAllHighlights();

    /** @name Komunikaty i log */
    ///@{
    void setLastActionMessage(const QString& message);
    QString getLastActionMessage() const;

    void addLogEntry(const QString& message);
    const QStringList& getBattleLog() const;
    ///@}

    /** @name Punkty akcji */
    ///@{
    int getCurrentTurnActionPoints() const;
    int getMaxTurnActionPoints() const;
    bool hasTurnActionPoints() const;
    void consumeTurnActionPoints(int amount = 1);
    void resetTurnActionPoints();
    ///@}

    /** @name Stan gry */
    ///@{
    bool isGameFinished() const;
    TeamSide getWinnerSide() const;
    bool updateVictoryState();
    ///@}

    /**
     * @brief Resetuje stan jednostek aktualnej strony na nową turę.
     */
    void resetCurrentSideUnitsForTurn();

    /** @name Statystyki */
    ///@{
    void recordShotFired(TeamSide side);
    void recordSuccessfulHit(TeamSide side, int damage);
    void recordUnitDestroyed(TeamSide side);

    TeamBattleStats getStatsForSide(TeamSide side) const;
    int getAccuracyPercent(TeamSide side) const;
    int getLossesForSide(TeamSide side) const;
    QString getPostGameSummaryText() const;
    QString getPostGameSummaryHtml() const;
    ///@}

private:
    Team& getCurrentTeam();
    const Team& getCurrentTeam() const;
    TeamBattleStats& getStatsForSideInternal(TeamSide side);
    const TeamBattleStats& getStatsForSideInternal(TeamSide side) const;

private:
    Board m_board;                      /**< Plansza gry. */
    Team m_playerTeam;                  /**< Drużyna gracza. */
    Team m_enemyTeam;                   /**< Drużyna przeciwnika. */
    int m_currentTurn = 1;              /**< Aktualny numer tury. */
    TeamSide m_currentSide = TeamSide::Player; /**< Aktualnie aktywna strona. */

    int m_selectedX = -1;               /**< Współrzędna X zaznaczenia. */
    int m_selectedY = -1;               /**< Współrzędna Y zaznaczenia. */

    QVector<QPair<int, int>> m_availableMovePositions; /**< Dostępne pola ruchu. */
    QVector<QPair<int, int>> m_blockedMovePositions;   /**< Zablokowane pola. */
    QVector<QPair<int, int>> m_attackablePositions;    /**< Cele ataku. */
    QVector<QPair<int, int>> m_healablePositions;      /**< Cele leczenia. */

    QString m_lastActionMessage = "Kliknij swoją jednostkę, aby ją wybrać."; /**< Ostatni komunikat. */
    QStringList m_battleLog; /**< Historia zdarzeń w bitwie. */

    int m_maxTurnActionPoints = 6;      /**< Maksymalna liczba AP na turę. */
    int m_currentTurnActionPoints = 6;  /**< Aktualna liczba AP. */

    bool m_gameFinished = false;        /**< Czy gra została zakończona. */
    TeamSide m_winnerSide = TeamSide::Player; /**< Zwycięska strona. */

    TeamBattleStats m_playerStats; /**< Statystyki gracza. */
    TeamBattleStats m_enemyStats;  /**< Statystyki przeciwnika. */
};

#endif // GAMESTATE_H
