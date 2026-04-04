#include "gamecontroller.h"

#include "../services/gamesetupservice.h"

GameController::GameController() = default;

void GameController::startGame(const GameConfig& config)
{
    m_config = config;
    m_state = GameSetupService::createGame(config);
}

void GameController::handleTileClick(int x, int y)
{
    m_engine.handleTileClick(m_state, x, y);
}

void GameController::endTurn()
{
    m_engine.endTurn(m_state);
}

const GameState& GameController::getGameState() const
{
    return m_state;
}

const GameConfig& GameController::getGameConfig() const
{
    return m_config;
}
