#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>

#include "../models/gamestate.h"
#include "../../config/gameconfig.h"
#include "../engine/battleengine.h"
#include "../controllers/aicontroller.h"
#include "../controllers/humancontroller.h"
#include "../controllers/interfaces/Iplayercontroller.h"

/**
 * @brief Kontroler zarządzający przebiegiem całej rozgrywki.
 *
 * Klasa odpowiada za:
 * - inicjalizację nowej gry,
 * - przekazywanie kliknięć użytkownika do silnika walki,
 * - kończenie tur,
 * - wybór kontrolera odpowiedzialnego za aktualną stronę konfliktu,
 * - uruchamianie logiki gracza ręcznego lub sztucznej inteligencji.
 *
 * Dzięki wykorzystaniu interfejsu IPlayerController klasa stosuje
 * polimorfizm na poziomie sterowania turą:
 * - HumanController reprezentuje sterowanie ręczne,
 * - AIController reprezentuje sterowanie automatyczne.
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
     * Metoda tworzy nowy stan gry, resetuje zasoby tury
     * oraz uruchamia logikę kontrolera dla strony rozpoczynającej grę.
     *
     * @param config Konfiguracja rozgrywki.
     */
    void startGame(const GameConfig& config);

    /**
     * @brief Obsługuje kliknięcie pola planszy przez użytkownika.
     *
     * Kliknięcie jest przekazywane do silnika walki tylko wtedy,
     * gdy aktualna tura należy do gracza sterowanego ręcznie.
     * Po wykonaniu akcji metoda uruchamia logikę kolejnego kontrolera,
     * jeśli strona aktywna się zmieniła.
     *
     * @param x Współrzędna X klikniętego pola.
     * @param y Współrzędna Y klikniętego pola.
     */
    void handleTileClick(int x, int y);

    /**
     * @brief Kończy aktualną turę.
     *
     * Metoda przełącza aktywną stronę, resetuje stan nowej tury
     * i uruchamia odpowiedni kontroler.
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
     * Emitowany po wykonaniu operacji wpływającej na rozgrywkę,
     * co pozwala odświeżyć interfejs użytkownika.
     */
    void stateChanged();

private:
    /**
     * @brief Zwraca kontroler odpowiedzialny za aktualną stronę konfliktu.
     *
     * Dla strony gracza zwracany jest HumanController,
     * a dla strony przeciwnika AIController.
     *
     * @return Wskaźnik na aktywny kontroler.
     */
    IPlayerController* getCurrentController();

    /**
     * @brief Zwraca kontroler odpowiedzialny za aktualną stronę konfliktu.
     *
     * Wersja tylko do odczytu.
     *
     * @return Wskaźnik na aktywny kontroler.
     */
    const IPlayerController* getCurrentController() const;

    /**
     * @brief Uruchamia kontroler aktualnej strony konfliktu.
     *
     * Metoda wykorzystuje polimorfizm interfejsu IPlayerController
     * i wywołuje metodę performTurn(...) na odpowiednim kontrolerze.
     *
     * W przypadku AI może to skutkować automatycznym wykonaniem ruchu,
     * natomiast dla HumanController metoda pełni rolę formalnego wejścia
     * w turę gracza ręcznego.
     */
    void processCurrentTurn();

    /**
     * @brief Uruchamia kolejne akcje AI, dopóki aktywna jest strona przeciwnika.
     *
     * Metoda jest potrzebna, ponieważ AI wykonuje swoje akcje automatycznie,
     * a gracz ręczny działa przez interakcję z interfejsem.
     */
    void processAiTurnLoopIfNeeded();

private:
    GameConfig m_config;               ///< Aktualna konfiguracja gry.
    GameState m_state;                 ///< Aktualny stan rozgrywki.
    BattleEngine m_engine;             ///< Silnik odpowiedzialny za wykonywanie akcji.

    HumanController m_humanController; ///< Kontroler gracza sterowanego ręcznie.
    AIController m_aiController;       ///< Kontroler gracza sterowanego automatycznie.
};

#endif // GAMECONTROLLER_H
