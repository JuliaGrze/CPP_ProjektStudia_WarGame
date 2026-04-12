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
 * Metoda tworzy stan gry na podstawie przekazanej konfiguracji,
 * resetuje punkty akcji oraz jednostki dla aktualnej tury,
 * ustawia komunikat startowy, a następnie uruchamia kontroler
 * odpowiedzialny za stronę rozpoczynającą grę.
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
        QString("Rozpoczęła się tura Niebieskich. AP drużyny: %1. "
                "Wybierz jednostkę, potem pole ruchu, sojusznika do leczenia albo przeciwnika.")
            .arg(m_state.getCurrentTurnActionPoints()));

    processCurrentTurn();
    emit stateChanged();
}

/**
 * @brief Zwraca kontroler odpowiedzialny za aktualną stronę konfliktu.
 *
 * @return Wskaźnik na aktywny kontroler.
 */
IPlayerController* GameController::getCurrentController()
{
    if (m_state.getCurrentSide() == TeamSide::Player)
        return &m_humanController;

    return &m_aiController;
}

/**
 * @brief Zwraca kontroler odpowiedzialny za aktualną stronę konfliktu.
 *
 * @return Wskaźnik na aktywny kontroler.
 */
const IPlayerController* GameController::getCurrentController() const
{
    if (m_state.getCurrentSide() == TeamSide::Player)
        return &m_humanController;

    return &m_aiController;
}

/**
 * @brief Uruchamia kontroler aktualnej strony konfliktu.
 *
 * Metoda wykorzystuje polimorfizm interfejsu IPlayerController
 * i wywołuje performTurn(...) na kontrolerze przypisanym
 * do aktualnej strony konfliktu.
 *
 * Dla HumanController metoda nie wykonuje automatycznych działań,
 * ponieważ ruchy gracza są realizowane przez kliknięcia w interfejsie.
 * Dla AIController metoda inicjuje automatyczne decyzje przeciwnika.
 */
void GameController::processCurrentTurn()
{
    if (m_state.isGameFinished())
        return;

    IPlayerController* controller = getCurrentController();
    if (!controller)
        return;

    controller->performTurn(m_state, m_engine);

    processAiTurnLoopIfNeeded();
}

/**
 * @brief Obsługuje kolejne akcje AI, dopóki trwa tura przeciwnika.
 *
 * Metoda wykonuje logikę AI wielokrotnie aż do momentu:
 * - zakończenia gry,
 * - zmiany aktywnej strony,
 * - wyczerpania działań przeciwnika.
 *
 * Dzięki temu cała tura przeciwnika może zostać rozegrana automatycznie
 * bez udziału użytkownika.
 */
void GameController::processAiTurnLoopIfNeeded()
{
    if (m_state.isGameFinished())
        return;

    while (!m_state.isGameFinished() && m_state.getCurrentSide() == TeamSide::Enemy)
    {
        IPlayerController* controller = getCurrentController();
        if (!controller)
            return;

        controller->performTurn(m_state, m_engine);

        if (m_state.isGameFinished())
            return;

        if (m_state.getCurrentSide() == TeamSide::Enemy)
            m_engine.endTurn(m_state);
    }
}

/**
 * @brief Obsługuje kliknięcie pola planszy przez użytkownika.
 *
 * Kliknięcia mają znaczenie tylko podczas tury gracza sterowanego ręcznie.
 * W takim przypadku metoda przekazuje zdarzenie do silnika walki,
 * a następnie – jeśli aktywna strona się zmieni – uruchamia odpowiedni
 * kontroler dla kolejnej tury.
 *
 * @param x Współrzędna X klikniętego pola.
 * @param y Współrzędna Y klikniętego pola.
 */
void GameController::handleTileClick(int x, int y)
{
    if (m_state.isGameFinished())
        return;

    if (m_state.getCurrentSide() != TeamSide::Player)
        return;

    m_engine.handleTileClick(m_state, x, y);
    processCurrentTurn();
    emit stateChanged();
}

/**
 * @brief Kończy aktualną turę.
 *
 * Metoda przełącza stronę aktywną, a następnie uruchamia kontroler
 * odpowiedzialny za nową turę.
 */
void GameController::endTurn()
{
    if (m_state.isGameFinished())
        return;

    m_engine.endTurn(m_state);
    processCurrentTurn();
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
