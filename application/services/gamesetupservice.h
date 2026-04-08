#ifndef GAMESETUPSERVICE_H
#define GAMESETUPSERVICE_H

#include "../../config/gameconfig.h"
#include "../models/gamestate.h"

/**
 * @brief Serwis odpowiedzialny za inicjalizację gry.
 *
 * Klasa tworzy kompletny stan gry na podstawie konfiguracji,
 * w tym:
 * - planszę,
 * - drużyny,
 * - rozmieszczenie jednostek,
 * - początkowe ustawienia rozgrywki.
 */
class GameSetupService
{
public:
    /**
     * @brief Tworzy nową grę na podstawie konfiguracji.
     *
     * Metoda inicjalizuje pełny stan gry gotowy do rozpoczęcia rozgrywki.
     *
     * @param config Konfiguracja gry (mapa, jednostki itp.).
     * @return Gotowy do użycia stan gry.
     */
    static GameState createGame(const GameConfig& config);
};

#endif // GAMESETUPSERVICE_H
