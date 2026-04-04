#ifndef TILE_H
#define TILE_H

#include "enums/terraintype.h"

class Unit;

class Tile
{
public:
    Tile();
    Tile(int x, int y, TerrainType terrain);

    int getX() const;
    int getY() const;

    TerrainType getTerrain() const;
    void setTerrain(TerrainType terrain);

    bool isOccupied() const;
    Unit* getUnit() const;
    void setUnit(Unit* unit);
    void removeUnit();

    bool isWalkable() const;

private:
    int m_x = 0;
    int m_y = 0;
    TerrainType m_terrain = TerrainType::Plain;
    Unit* m_unit = nullptr;
};

#endif // TILE_H
