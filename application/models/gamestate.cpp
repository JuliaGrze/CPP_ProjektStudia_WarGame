#include "gamestate.h"

GameState::GameState()
    : m_playerTeam("Niebiescy", TeamSide::Player),
    m_enemyTeam("Czerwoni", TeamSide::Enemy)
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

TeamSide GameState::getCurrentSide() const
{
    return m_currentSide;
}

bool GameState::isGameFinished() const
{
    return m_gameFinished;
}

void GameState::setBoard(const Board& board)
{
    m_board = board;
}

void GameState::setPlayerTeam(const Team& team)
{
    m_playerTeam = team;
}

void GameState::setEnemyTeam(const Team& team)
{
    m_enemyTeam = team;
}

void GameState::nextTurn()
{
    switchCurrentTeam();

    if (m_currentSide == TeamSide::Player)
        ++m_currentTurn;
}

void GameState::switchCurrentTeam()
{
    if (m_currentSide == TeamSide::Player)
        m_currentSide = TeamSide::Enemy;
    else
        m_currentSide = TeamSide::Player;
}

bool GameState::checkVictory()
{
    if (!m_playerTeam.hasUnitsAlive() || !m_enemyTeam.hasUnitsAlive())
    {
        m_gameFinished = true;
        return true;
    }

    return false;
}

void GameState::reset()
{
    m_board = Board();
    m_playerTeam = Team("Niebiescy", TeamSide::Player);
    m_enemyTeam = Team("Czerwoni", TeamSide::Enemy);
    m_currentTurn = 1;
    m_currentSide = TeamSide::Player;
    m_gameFinished = false;
}
