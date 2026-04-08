#include "gamecontroller.h"

#include "../services/gamesetupservice.h"

/**
 * @brief Konstruktor kontrolera gry.
 *
 * Inicjalizuje obiekt kontrolera. W tym miejscu nie jest jeszcze
 * tworzony stan gry – następuje to dopiero w metodzie startGame().
 *
 * @param parent Wskaźnik na obiekt nadrzędny Qt.
 */
GameController::GameController(QObject* parent)
    : QObject(parent)
{
}

/**
 * @brief Rozpoczyna nową rozgrywkę.
 *
 * Tworzy stan gry na podstawie przekazanej konfiguracji,
 * resetuje punkty akcji oraz jednostki dla aktualnej tury,
 * a następnie ustawia komunikat startowy.
 *
 * Jeśli pierwsza tura należy do przeciwnika, wywoływana jest jego logika.
 *
 * @param config Konfiguracja gry.
 */
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
    emit stateChanged();
}

/**
 * @brief Obsługuje turę gracza sterowanego automatycznie.
 *
 * Metoda wykonuje kolejne akcje przeciwnika do momentu:
 * - zakończenia gry,
 * - zmiany aktywnej strony.
 *
 * Po każdej akcji sprawdzany jest stan gry oraz to,
 * czy przeciwnik nadal ma turę.
 */
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

/**
 * @brief Obsługuje kliknięcie pola planszy przez użytkownika.
 *
 * Przekazuje zdarzenie do silnika gry, a następnie uruchamia
 * ewentualną turę przeciwnika i informuje interfejs o zmianie stanu.
 *
 * @param x Współrzędna X klikniętego pola.
 * @param y Współrzędna Y klikniętego pola.
 */
void GameController::handleTileClick(int x, int y)
{
    if (m_state.isGameFinished())
        return;

    m_engine.handleTileClick(m_state, x, y);
    processAiTurnIfNeeded();
    emit stateChanged();
}

/**
 * @brief Kończy aktualną turę.
 *
 * Przełącza stronę aktywną, uruchamia ewentualną turę przeciwnika
 * oraz informuje interfejs użytkownika o zmianie stanu gry.
 */
void GameController::endTurn()
{
    if (m_state.isGameFinished())
        return;

    m_engine.endTurn(m_state);
    processAiTurnIfNeeded();
    emit stateChanged();
}

/**
 * @brief Zwraca aktualny stan gry.
 *
 * @return Referencja do obiektu stanu gry.
 */
const GameState& GameController::getGameState() const
{
    return m_state;
}

/**
 * @brief Zwraca konfigurację aktualnej rozgrywki.
 *
 * @return Referencja do konfiguracji gry.
 */
const GameConfig& GameController::getGameConfig() const
{
    return m_config;
}
