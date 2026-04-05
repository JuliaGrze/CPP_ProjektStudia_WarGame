#include "gamestate.h"

namespace
{
bool containsPosition(const QVector<QPair<int, int>>& positions, int x, int y)
{
    for (const auto& position : positions)
    {
        if (position.first == x && position.second == y)
            return true;
    }

    return false;
}
}

GameState::GameState()
    : m_board(10, 10),
    m_playerTeam("Niebiescy", TeamSide::Player),
    m_enemyTeam("Czerwoni", TeamSide::Enemy)
{
}

Board& GameState::getBoard() { return m_board; }
const Board& GameState::getBoard() const { return m_board; }
void GameState::setBoard(const Board& board) { m_board = board; }

Team& GameState::getPlayerTeam() { return m_playerTeam; }
const Team& GameState::getPlayerTeam() const { return m_playerTeam; }

Team& GameState::getEnemyTeam() { return m_enemyTeam; }
const Team& GameState::getEnemyTeam() const { return m_enemyTeam; }

int GameState::getCurrentTurn() const { return m_currentTurn; }
void GameState::setCurrentTurn(int turn) { m_currentTurn = turn; }

TeamSide GameState::getCurrentSide() const { return m_currentSide; }
void GameState::setCurrentSide(TeamSide side) { m_currentSide = side; }

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

int GameState::getSelectedX() const { return m_selectedX; }
int GameState::getSelectedY() const { return m_selectedY; }

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
    return containsPosition(m_availableMovePositions, x, y);
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
    return containsPosition(m_blockedMovePositions, x, y);
}

void GameState::setAttackablePositions(const QVector<QPair<int, int>>& positions)
{
    m_attackablePositions = positions;
}

const QVector<QPair<int, int>>& GameState::getAttackablePositions() const
{
    return m_attackablePositions;
}

void GameState::clearAttackablePositions()
{
    m_attackablePositions.clear();
}

bool GameState::isAttackablePosition(int x, int y) const
{
    return containsPosition(m_attackablePositions, x, y);
}

void GameState::setHealablePositions(const QVector<QPair<int, int>>& positions)
{
    m_healablePositions = positions;
}

const QVector<QPair<int, int>>& GameState::getHealablePositions() const
{
    return m_healablePositions;
}

void GameState::clearHealablePositions()
{
    m_healablePositions.clear();
}

bool GameState::isHealablePosition(int x, int y) const
{
    return containsPosition(m_healablePositions, x, y);
}

void GameState::clearAllHighlights()
{
    clearAvailableMovePositions();
    clearBlockedMovePositions();
    clearAttackablePositions();
    clearHealablePositions();
}

void GameState::setLastActionMessage(const QString& message)
{
    m_lastActionMessage = message;
    addLogEntry(message);
}

QString GameState::getLastActionMessage() const
{
    return m_lastActionMessage;
}

void GameState::addLogEntry(const QString& message)
{
    if (!message.trimmed().isEmpty())
        m_battleLog.append(message);
}

const QStringList& GameState::getBattleLog() const
{
    return m_battleLog;
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

bool GameState::isGameFinished() const
{
    return m_gameFinished;
}

TeamSide GameState::getWinnerSide() const
{
    return m_winnerSide;
}

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

    return true;
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
