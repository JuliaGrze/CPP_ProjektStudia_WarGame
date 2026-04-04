#include "gamecontroller.h"

#include "../services/gamesetupservice.h"

GameController::GameController() = default;

void GameController::startGame(const GameConfig& config)
{
    m_config = config;
    m_state = GameSetupService::createGame(config);
    m_state.resetTurnActionPoints();
    m_state.resetCurrentSideUnitsForTurn();
    m_state.setLastActionMessage(
        QString("Rozpoczęła się tura Niebieskich. AP drużyny: %1. Wybierz jednostkę, potem pole ruchu albo przeciwnika.")
            .arg(m_state.getCurrentTurnActionPoints())
        );
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
