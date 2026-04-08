#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include "tile.h"

/**
 * @brief Reprezentuje planszę gry.
 *
 * Klasa przechowuje wymiary planszy oraz siatkę pól (`Tile`),
 * na których odbywa się rozgrywka.
 *
 * Umożliwia inicjalizację planszy, sprawdzanie granic
 * oraz pobieranie dostępu do wybranego pola.
 */
class Board
{
public:
    /**
     * @brief Domyślny konstruktor planszy.
     */
    Board();

    /**
     * @brief Tworzy planszę o podanych wymiarach.
     *
     * @param width Szerokość planszy.
     * @param height Wysokość planszy.
     */
    Board(int width, int height);

    /**
     * @brief Inicjalizuje planszę o wskazanych wymiarach i typie terenu.
     *
     * Metoda tworzy siatkę pól i ustawia domyślny teren
     * dla wszystkich pozycji planszy.
     *
     * @param width Szerokość planszy.
     * @param height Wysokość planszy.
     * @param defaultTerrain Domyślny typ terenu dla pól planszy.
     */
    void initialize(int width, int height, TerrainType defaultTerrain = TerrainType::Plain);

    /**
     * @brief Zwraca szerokość planszy.
     *
     * @return Szerokość planszy.
     */
    int getWidth() const;

    /**
     * @brief Zwraca wysokość planszy.
     *
     * @return Wysokość planszy.
     */
    int getHeight() const;

    /**
     * @brief Sprawdza, czy wskazane współrzędne znajdują się na planszy.
     *
     * @param x Współrzędna X.
     * @param y Współrzędna Y.
     * @return true, jeśli pole znajduje się na planszy, w przeciwnym razie false.
     */
    bool isInsideBoard(int x, int y) const;

    /**
     * @brief Zwraca wskaźnik na pole planszy.
     *
     * @param x Współrzędna X pola.
     * @param y Współrzędna Y pola.
     * @return Wskaźnik na pole planszy lub nullptr, jeśli współrzędne są niepoprawne.
     */
    Tile* getTile(int x, int y);

    /**
     * @brief Zwraca stały wskaźnik na pole planszy.
     *
     * @param x Współrzędna X pola.
     * @param y Współrzędna Y pola.
     * @return Stały wskaźnik na pole planszy lub nullptr, jeśli współrzędne są niepoprawne.
     */
    const Tile* getTile(int x, int y) const;

private:
    int m_width = 0;                  ///< Szerokość planszy.
    int m_height = 0;                 ///< Wysokość planszy.
    QVector<QVector<Tile>> m_tiles;   ///< Dwuwymiarowa siatka pól planszy.
};

#endif // BOARD_H
