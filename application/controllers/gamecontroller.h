#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>

#include "../models/gamestate.h"
#include "../../config/gameconfig.h"
#include "../engine/battleengine.h"
#include "../controllers/aicontroller.h"

/**
 * @brief Kontroler zarządzający przebiegiem gry.
 *
 * Klasa odpowiada za sterowanie logiką rozgrywki, w tym:
 * - inicjalizację gry,
 * - obsługę akcji użytkownika,
 * - zarządzanie turami,
 * - współpracę z silnikiem walki,
 * - wywoływanie ruchów gracza sterowanego automatycznie.
 *
 * Pełni rolę pośrednika pomiędzy interfejsem użytkownika a logiką gry.
 */
class GameController : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor kontrolera gry.
     *
     * @param parent Wskaźnik na obiekt nadrzędny Qt.
     */
    explicit GameController(QObject* parent = nullptr);

    /**
     * @brief Rozpoczyna nową grę na podstawie konfiguracji.
     *
     * @param config Konfiguracja rozgrywki.
     */
    void startGame(const GameConfig& config);

    /**
     * @brief Obsługuje kliknięcie pola planszy.
     *
     * Przekazuje zdarzenie do silnika gry, który interpretuje je
     * jako ruch, atak lub wybór jednostki.
     *
     * @param x Współrzędna X klikniętego pola.
     * @param y Współrzędna Y klikniętego pola.
     */
    void handleTileClick(int x, int y);

    /**
     * @brief Kończy aktualną turę.
     *
     * Przełącza aktywną stronę i ewentualnie uruchamia turę
     * gracza sterowanego automatycznie.
     */
    void endTurn();

    /**
     * @brief Zwraca aktualny stan gry.
     *
     * @return Referencja do stanu gry.
     */
    const GameState& getGameState() const;

    /**
     * @brief Zwraca aktualną konfigurację gry.
     *
     * @return Referencja do konfiguracji gry.
     */
    const GameConfig& getGameConfig() const;

signals:
    /**
     * @brief Sygnał informujący o zmianie stanu gry.
     *
     * Emitowany po wykonaniu akcji wpływającej na stan gry,
     * co umożliwia odświeżenie interfejsu użytkownika.
     */
    void stateChanged();

private:
    /**
     * @brief Obsługuje turę gracza sterowanego automatycznie, jeśli jest wymagana.
     *
     * Metoda sprawdza, czy aktualna tura należy do przeciwnika
     * i w razie potrzeby uruchamia jego logikę działania.
     */
    void processAiTurnIfNeeded();

private:
    GameConfig m_config;        ///< Aktualna konfiguracja gry.
    GameState m_state;          ///< Aktualny stan rozgrywki.
    BattleEngine m_engine;      ///< Silnik odpowiedzialny za wykonywanie akcji.
    AIController m_aiController; ///< Kontroler gracza sterowanego automatycznie.
};

#endif // GAMECONTROLLER_H
