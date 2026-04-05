#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.h"
#include "team.h"
#include "enums/teamside.h"

#include <QVector>
#include <QPair>
#include <QString>
#include <QStringList>

class GameState
{
public:
    GameState();

    Board& getBoard();
    const Board& getBoard() const;
    void setBoard(const Board& board);

    Team& getPlayerTeam();
    const Team& getPlayerTeam() const;

    Team& getEnemyTeam();
    const Team& getEnemyTeam() const;

    int getCurrentTurn() const;
    void setCurrentTurn(int turn);

    TeamSide getCurrentSide() const;
    void setCurrentSide(TeamSide side);

    void nextTurn();

    void setSelectedPosition(int x, int y);
    void clearSelectedPosition();
    bool hasSelectedPosition() const;
    int getSelectedX() const;
    int getSelectedY() const;

    void setAvailableMovePositions(const QVector<QPair<int, int>>& positions);
    const QVector<QPair<int, int>>& getAvailableMovePositions() const;
    void clearAvailableMovePositions();
    bool isMovePositionAvailable(int x, int y) const;

    void setBlockedMovePositions(const QVector<QPair<int, int>>& positions);
    const QVector<QPair<int, int>>& getBlockedMovePositions() const;
    void clearBlockedMovePositions();
    bool isBlockedMovePosition(int x, int y) const;

    void setLastActionMessage(const QString& message);
    QString getLastActionMessage() const;

    void addLogEntry(const QString& message);
    const QStringList& getBattleLog() const;

    int getCurrentTurnActionPoints() const;
    int getMaxTurnActionPoints() const;
    bool hasTurnActionPoints() const;
    void consumeTurnActionPoints(int amount = 1);
    void resetTurnActionPoints();

    bool isGameFinished() const;
    TeamSide getWinnerSide() const;
    bool updateVictoryState();

    void resetCurrentSideUnitsForTurn();

private:
    Team& getCurrentTeam();
    const Team& getCurrentTeam() const;

private:
    Board m_board;
    Team m_playerTeam;
    Team m_enemyTeam;
    int m_currentTurn = 1;
    TeamSide m_currentSide = TeamSide::Player;

    int m_selectedX = -1;
    int m_selectedY = -1;

    QVector<QPair<int, int>> m_availableMovePositions;
    QVector<QPair<int, int>> m_blockedMovePositions;

    QString m_lastActionMessage = "Kliknij swoją jednostkę, aby ją wybrać.";
    QStringList m_battleLog;

    int m_maxTurnActionPoints = 6;
    int m_currentTurnActionPoints = 6;

    bool m_gameFinished = false;
    TeamSide m_winnerSide = TeamSide::Player;
};

#endif // GAMESTATE_H
