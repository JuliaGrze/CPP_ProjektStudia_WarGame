#include "gamestate.h"

GameState::GameState()
    : m_board(10, 10)
{
}

Board& GameState::getBoard()
{
    return m_board;
}

const Board& GameState::getBoard() const
{
    return m_board;
}

void GameState::setBoard(const Board& board)
{
    m_board = board;
}

Team& GameState::getPlayerTeam()
{
    return m_playerTeam;
}

const Team& GameState::getPlayerTeam() const
{
    return m_playerTeam;
}

Team& GameState::getEnemyTeam()
{
    return m_enemyTeam;
}

const Team& GameState::getEnemyTeam() const
{
    return m_enemyTeam;
}

int GameState::getCurrentTurn() const
{
    return m_currentTurn;
}

void GameState::setCurrentTurn(int turn)
{
    m_currentTurn = turn;
}

TeamSide GameState::getCurrentSide() const
{
    return m_currentSide;
}

void GameState::setCurrentSide(TeamSide side)
{
    m_currentSide = side;
}

void GameState::nextTurn()
{
    if (m_currentSide == TeamSide::Player)
        m_currentSide = TeamSide::Enemy;
    else
    {
        m_currentSide = TeamSide::Player;
        ++m_currentTurn;
    }

    clearSelectedPosition();
    clearAvailableMovePositions();
}

void GameState::setSelectedPosition(int x, int y)
{
    m_selectedX = x;
    m_selectedY = y;
}

void GameState::clearSelectedPosition()
{
    m_selectedX = -1;
    m_selectedY = -1;
}

bool GameState::hasSelectedPosition() const
{
    return m_selectedX >= 0 && m_selectedY >= 0;
}

int GameState::getSelectedX() const
{
    return m_selectedX;
}

int GameState::getSelectedY() const
{
    return m_selectedY;
}

void GameState::setAvailableMovePositions(const QVector<QPair<int, int>>& positions)
{
    m_availableMovePositions = positions;
}

const QVector<QPair<int, int>>& GameState::getAvailableMovePositions() const
{
    return m_availableMovePositions;
}

void GameState::clearAvailableMovePositions()
{
    m_availableMovePositions.clear();
}

bool GameState::isMovePositionAvailable(int x, int y) const
{
    for (const auto& position : m_availableMovePositions)
    {
        if (position.first == x && position.second == y)
            return true;
    }

    return false;
}
