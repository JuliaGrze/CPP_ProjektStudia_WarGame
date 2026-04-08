#ifndef TILE_H
#define TILE_H

#include "enums/terraintype.h"

class Unit;

/**
 * @brief Reprezentuje pojedyncze pole planszy.
 *
 * Klasa przechowuje informacje o:
 * - położeniu pola (współrzędne),
 * - typie terenu,
 * - jednostce znajdującej się na polu.
 *
 * Dodatkowo udostępnia metody określające właściwości pola,
 * takie jak możliwość przejścia, koszt ruchu czy bonusy.
 */
class Tile
{
public:
    /**
     * @brief Domyślny konstruktor pola.
     */
    Tile();

    /**
     * @brief Tworzy pole o podanych współrzędnych i typie terenu.
     *
     * @param x Współrzędna X pola.
     * @param y Współrzędna Y pola.
     * @param terrain Typ terenu pola.
     */
    Tile(int x, int y, TerrainType terrain);

    /**
     * @brief Zwraca współrzędną X pola.
     */
    int getX() const;

    /**
     * @brief Zwraca współrzędną Y pola.
     */
    int getY() const;

    /**
     * @brief Zwraca typ terenu pola.
     */
    TerrainType getTerrain() const;

    /**
     * @brief Ustawia typ terenu pola.
     *
     * @param terrain Nowy typ terenu.
     */
    void setTerrain(TerrainType terrain);

    /**
     * @brief Sprawdza, czy pole jest zajęte przez jednostkę.
     *
     * @return true, jeśli na polu znajduje się jednostka.
     */
    bool isOccupied() const;

    /**
     * @brief Zwraca wskaźnik na jednostkę znajdującą się na polu.
     *
     * @return Wskaźnik na jednostkę lub nullptr.
     */
    Unit* getUnit() const;

    /**
     * @brief Ustawia jednostkę na polu.
     *
     * @param unit Wskaźnik na jednostkę.
     */
    void setUnit(Unit* unit);

    /**
     * @brief Usuwa jednostkę z pola.
     */
    void removeUnit();

    /**
     * @brief Sprawdza, czy pole jest przechodnie.
     *
     * @return true, jeśli można wejść na pole.
     */
    bool isWalkable() const;

    /**
     * @brief Zwraca koszt ruchu dla tego pola.
     *
     * @return Koszt wejścia na pole.
     */
    int getMovementCost() const;

    /**
     * @brief Zwraca bonus obronny wynikający z terenu.
     *
     * @return Wartość bonusu osłony.
     */
    int getCoverBonus() const;

    /**
     * @brief Zwraca bonus do zasięgu wynikający z terenu.
     *
     * @return Wartość bonusu zasięgu.
     */
    int getRangeBonus() const;

private:
    int m_x = 0;                      ///< Współrzędna X pola.
    int m_y = 0;                      ///< Współrzędna Y pola.
    TerrainType m_terrain = TerrainType::Plain; ///< Typ terenu.
    Unit* m_unit = nullptr;          ///< Jednostka znajdująca się na polu.
};

#endif // TILE_H
