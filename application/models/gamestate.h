#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "board.h"
#include "team.h"
#include "enums/teamside.h"

class GameState
{
public:
    GameState();

    Board& getBoard();
    const Board& getBoard() const;

    Team& getPlayerTeam();
    const Team& getPlayerTeam() const;

    Team& getEnemyTeam();
    const Team& getEnemyTeam() const;

    int getCurrentTurn() const;
    TeamSide getCurrentSide() const;
    bool isGameFinished() const;

    void setBoard(const Board& board);
    void setPlayerTeam(const Team& team);
    void setEnemyTeam(const Team& team);

    void nextTurn();
    void switchCurrentTeam();
    bool checkVictory();
    void reset();

private:
    Board m_board;
    Team m_playerTeam;
    Team m_enemyTeam;
    int m_currentTurn = 1;
    TeamSide m_currentSide = TeamSide::Player;
    bool m_gameFinished = false;
};

#endif // GAMESTATE_H
