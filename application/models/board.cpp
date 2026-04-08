#include "board.h"

/**
 * @brief Domyślny konstruktor planszy.
 */
Board::Board() = default;

/**
 * @brief Konstruktor tworzący planszę o podanych wymiarach.
 *
 * Automatycznie inicjalizuje planszę domyślnym terenem.
 *
 * @param width Szerokość planszy.
 * @param height Wysokość planszy.
 */
Board::Board(int width, int height)
{
    initialize(width, height);
}

/**
 * @brief Inicjalizuje planszę o zadanych wymiarach.
 *
 * Tworzy siatkę pól (`Tile`) i ustawia ich współrzędne
 * oraz domyślny typ terenu.
 *
 * @param width Szerokość planszy.
 * @param height Wysokość planszy.
 * @param defaultTerrain Domyślny typ terenu dla wszystkich pól.
 */
void Board::initialize(int width, int height, TerrainType defaultTerrain)
{
    m_width = width;
    m_height = height;
    m_tiles.clear();
    m_tiles.resize(m_height);

    for (int row = 0; row < m_height; ++row)
    {
        m_tiles[row].reserve(m_width);
        for (int col = 0; col < m_width; ++col)
        {
            m_tiles[row].append(Tile(col, row, defaultTerrain));
        }
    }
}

/**
 * @brief Zwraca szerokość planszy.
 */
int Board::getWidth() const
{
    return m_width;
}

/**
 * @brief Zwraca wysokość planszy.
 */
int Board::getHeight() const
{
    return m_height;
}

/**
 * @brief Sprawdza, czy współrzędne znajdują się w granicach planszy.
 *
 * @param x Współrzędna X.
 * @param y Współrzędna Y.
 * @return true jeśli współrzędne są poprawne.
 */
bool Board::isInsideBoard(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

/**
 * @brief Zwraca wskaźnik na pole planszy.
 *
 * @param x Współrzędna X pola.
 * @param y Współrzędna Y pola.
 * @return Wskaźnik na pole lub nullptr, jeśli poza planszą.
 */
Tile* Board::getTile(int x, int y)
{
    if (!isInsideBoard(x, y))
        return nullptr;

    return &m_tiles[y][x];
}

/**
 * @brief Zwraca stały wskaźnik na pole planszy.
 *
 * @param x Współrzędna X pola.
 * @param y Współrzędna Y pola.
 * @return Stały wskaźnik na pole lub nullptr, jeśli poza planszą.
 */
const Tile* Board::getTile(int x, int y) const
{
    if (!isInsideBoard(x, y))
        return nullptr;

    return &m_tiles[y][x];
}
