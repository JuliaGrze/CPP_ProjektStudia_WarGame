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

private:
    Board m_board;
    Team m_playerTeam;
    Team m_enemyTeam;
    int m_currentTurn = 1;
    TeamSide m_currentSide = TeamSide::Player;

    int m_selectedX = -1;
    int m_selectedY = -1;
};

#endif // GAMESTATE_H
