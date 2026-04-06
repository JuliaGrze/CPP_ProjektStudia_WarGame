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
        QString("Rozpoczęła się tura Niebieskich. AP drużyny: %1. Wybierz jednostkę, potem pole ruchu, sojusznika do leczenia albo przeciwnika.")
            .arg(m_state.getCurrentTurnActionPoints()));

    processAiTurnIfNeeded();
}

void GameController::processAiTurnIfNeeded()
{
    if (m_state.isGameFinished())
        return;

    while (!m_state.isGameFinished() && m_state.getCurrentSide() == TeamSide::Enemy)
    {
        m_aiController.performTurn(m_state, m_engine);

        if (m_state.isGameFinished())
            return;

        if (m_state.getCurrentSide() == TeamSide::Enemy)
            m_engine.endTurn(m_state);
    }
}

void GameController::handleTileClick(int x, int y)
{
    if (m_state.isGameFinished())
        return;

    m_engine.handleTileClick(m_state, x, y);
    processAiTurnIfNeeded();
}

void GameController::endTurn()
{
    if (m_state.isGameFinished())
        return;

    m_engine.endTurn(m_state);
    processAiTurnIfNeeded();
}

const GameState& GameController::getGameState() const
{
    return m_state;
}

const GameConfig& GameController::getGameConfig() const
{
    return m_config;
}
