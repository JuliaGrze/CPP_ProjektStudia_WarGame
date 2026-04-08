#include "tile.h"

/**
 * @brief Domyślny konstruktor pola.
 */
Tile::Tile() = default;

/**
 * @brief Konstruktor pola z parametrami.
 *
 * @param x Współrzędna X pola.
 * @param y Współrzędna Y pola.
 * @param terrain Typ terenu pola.
 */
Tile::Tile(int x, int y, TerrainType terrain)
    : m_x(x), m_y(y), m_terrain(terrain) {}

/**
 * @brief Zwraca współrzędną X pola.
 */
int Tile::getX() const { return m_x; }

/**
 * @brief Zwraca współrzędną Y pola.
 */
int Tile::getY() const { return m_y; }

/**
 * @brief Zwraca typ terenu pola.
 */
TerrainType Tile::getTerrain() const { return m_terrain; }

/**
 * @brief Ustawia typ terenu pola.
 *
 * @param terrain Nowy typ terenu.
 */
void Tile::setTerrain(TerrainType terrain) { m_terrain = terrain; }

/**
 * @brief Sprawdza, czy pole jest zajęte przez jednostkę.
 *
 * @return true, jeśli pole zawiera jednostkę.
 */
bool Tile::isOccupied() const { return m_unit != nullptr; }

/**
 * @brief Zwraca jednostkę znajdującą się na polu.
 *
 * @return Wskaźnik na jednostkę lub nullptr.
 */
Unit* Tile::getUnit() const { return m_unit; }

/**
 * @brief Ustawia jednostkę na polu.
 *
 * @param unit Wskaźnik na jednostkę.
 */
void Tile::setUnit(Unit* unit) { m_unit = unit; }

/**
 * @brief Usuwa jednostkę z pola.
 */
void Tile::removeUnit() { m_unit = nullptr; }

/**
 * @brief Sprawdza, czy pole jest przechodnie.
 *
 * @return true, jeśli można wejść na pole.
 */
bool Tile::isWalkable() const
{
    return m_terrain != TerrainType::Water;
}

/**
 * @brief Zwraca koszt ruchu dla danego terenu.
 *
 * @return Koszt wejścia na pole.
 */
int Tile::getMovementCost() const
{
    switch (m_terrain)
    {
    case TerrainType::Plain: return 1;
    case TerrainType::Forest: return 2;
    case TerrainType::Mountain: return 3;
    case TerrainType::Building: return 2;
    case TerrainType::Water: return 9999; ///< Bardzo wysoki koszt – praktycznie nieprzechodnie
    case TerrainType::Bridge: return 1;
    }

    return 1;
}

/**
 * @brief Zwraca bonus do obrony wynikający z terenu.
 *
 * @return Wartość bonusu osłony.
 */
int Tile::getCoverBonus() const
{
    switch (m_terrain)
    {
    case TerrainType::Plain: return 0;
    case TerrainType::Forest: return 15;
    case TerrainType::Mountain: return 10;
    case TerrainType::Building: return 20;
    case TerrainType::Water: return 0;
    case TerrainType::Bridge: return 0;
    }

    return 0;
}

/**
 * @brief Zwraca bonus do zasięgu wynikający z terenu.
 *
 * @return Wartość bonusu zasięgu.
 */
int Tile::getRangeBonus() const
{
    switch (m_terrain)
    {
    case TerrainType::Mountain: return 1;
    default: return 0;
    }
}
