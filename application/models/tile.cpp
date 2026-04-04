#include "tile.h"

Tile::Tile() = default;

Tile::Tile(int x, int y, TerrainType terrain)
    : m_x(x), m_y(y), m_terrain(terrain)
{
}

int Tile::getX() const
{
    return m_x;
}

int Tile::getY() const
{
    return m_y;
}

TerrainType Tile::getTerrain() const
{
    return m_terrain;
}

void Tile::setTerrain(TerrainType terrain)
{
    m_terrain = terrain;
}

bool Tile::isOccupied() const
{
    return m_unit != nullptr;
}

Unit* Tile::getUnit() const
{
    return m_unit;
}

void Tile::setUnit(Unit* unit)
{
    m_unit = unit;
}

void Tile::removeUnit()
{
    m_unit = nullptr;
}

bool Tile::isWalkable() const
{
    return m_terrain != TerrainType::Water && m_terrain != TerrainType::Building;
}
