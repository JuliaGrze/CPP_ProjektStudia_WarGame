#include "gamestate.h"

GameState::GameState()
    : m_board(10, 10),
    m_playerTeam("Niebiescy", TeamSide::Player),
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

Team& GameState::getCurrentTeam()
{
    return m_currentSide == TeamSide::Player ? m_playerTeam : m_enemyTeam;
}

const Team& GameState::getCurrentTeam() const
{
    return m_currentSide == TeamSide::Player ? m_playerTeam : m_enemyTeam;
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
    clearBlockedMovePositions();

    resetTurnActionPoints();
    resetCurrentSideUnitsForTurn();

    m_lastActionMessage = QString("Rozpoczęła się tura %1. AP drużyny: %2.")
                              .arg(m_currentSide == TeamSide::Player ? "Niebieskich" : "Czerwonych")
                              .arg(m_currentTurnActionPoints);
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

void GameState::setBlockedMovePositions(const QVector<QPair<int, int>>& positions)
{
    m_blockedMovePositions = positions;
}

const QVector<QPair<int, int>>& GameState::getBlockedMovePositions() const
{
    return m_blockedMovePositions;
}

void GameState::clearBlockedMovePositions()
{
    m_blockedMovePositions.clear();
}

bool GameState::isBlockedMovePosition(int x, int y) const
{
    for (const auto& position : m_blockedMovePositions)
    {
        if (position.first == x && position.second == y)
            return true;
    }

    return false;
}

void GameState::setLastActionMessage(const QString& message)
{
    m_lastActionMessage = message;
}

QString GameState::getLastActionMessage() const
{
    return m_lastActionMessage;
}

int GameState::getCurrentTurnActionPoints() const
{
    return m_currentTurnActionPoints;
}

int GameState::getMaxTurnActionPoints() const
{
    return m_maxTurnActionPoints;
}

bool GameState::hasTurnActionPoints() const
{
    return m_currentTurnActionPoints > 0;
}

void GameState::consumeTurnActionPoints(int amount)
{
    if (amount <= 0)
        return;

    m_currentTurnActionPoints -= amount;
    if (m_currentTurnActionPoints < 0)
        m_currentTurnActionPoints = 0;
}

void GameState::resetTurnActionPoints()
{
    m_currentTurnActionPoints = m_maxTurnActionPoints;
}

void GameState::resetCurrentSideUnitsForTurn()
{
    const auto units = getCurrentTeam().getUnits();

    for (const auto& unit : units)
    {
        if (unit && unit->isAlive())
            unit->resetTurnResources();
    }
}
