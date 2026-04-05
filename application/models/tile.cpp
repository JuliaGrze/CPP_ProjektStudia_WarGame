#include "tile.h"

Tile::Tile() = default;
Tile::Tile(int x, int y, TerrainType terrain) : m_x(x), m_y(y), m_terrain(terrain) {}

int Tile::getX() const { return m_x; }
int Tile::getY() const { return m_y; }

TerrainType Tile::getTerrain() const { return m_terrain; }
void Tile::setTerrain(TerrainType terrain) { m_terrain = terrain; }

bool Tile::isOccupied() const { return m_unit != nullptr; }
Unit* Tile::getUnit() const { return m_unit; }
void Tile::setUnit(Unit* unit) { m_unit = unit; }
void Tile::removeUnit() { m_unit = nullptr; }

bool Tile::isWalkable() const
{
    return m_terrain != TerrainType::Water;
}

int Tile::getMovementCost() const
{
    switch (m_terrain)
    {
    case TerrainType::Plain: return 1;
    case TerrainType::Forest: return 2;
    case TerrainType::Mountain: return 3;
    case TerrainType::Building: return 2;
    case TerrainType::Water: return 9999;
    }

    return 1;
}

int Tile::getCoverBonus() const
{
    switch (m_terrain)
    {
    case TerrainType::Plain: return 0;
    case TerrainType::Forest: return 15;
    case TerrainType::Mountain: return 10;
    case TerrainType::Building: return 20;
    case TerrainType::Water: return 0;
    }

    return 0;
}

int Tile::getRangeBonus() const
{
    switch (m_terrain)
    {
    case TerrainType::Mountain: return 1;
    default: return 0;
    }
}
