#ifndef BATTLEBOARDSERVICE_H
#define BATTLEBOARDSERVICE_H

#include <QGridLayout>
#include <QWidget>
#include <functional>

#include "../../config/gameconfig.h"
#include "../models/gamestate.h"

/**
 * @brief Klasa pomocnicza odpowiedzialna za rysowanie planszy bitwy.
 *
 * Zawiera statyczne metody służące do budowania widoku planszy
 * na podstawie aktualnego stanu gry i konfiguracji rozgrywki.
 *
 * Odpowiada za tworzenie pól planszy w interfejsie użytkownika
 * oraz podpinanie obsługi kliknięć.
 */
class BattleBoardService
{
public:
    /**
     * @brief Rysuje planszę bitwy w podanym układzie siatki.
     *
     * Metoda odtwarza wizualną reprezentację planszy na podstawie:
     * - aktualnego stanu gry,
     * - konfiguracji mapy,
     * - funkcji obsługującej kliknięcie pola.
     *
     * @param grid Układ siatki, do którego dodawane są pola planszy.
     * @param boardContainer Kontener przechowujący planszę.
     * @param gameState Aktualny stan gry.
     * @param config Konfiguracja rozgrywki.
     * @param onTileClicked Funkcja wywoływana po kliknięciu pola planszy.
     */
    static void drawBoard(QGridLayout* grid,
                          QWidget* boardContainer,
                          const GameState& gameState,
                          const GameConfig& config,
                          const std::function<void(int, int)>& onTileClicked);
};

#endif // BATTLEBOARDSERVICE_H
