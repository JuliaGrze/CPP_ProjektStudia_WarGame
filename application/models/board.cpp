#include "board.h"

Board::Board() = default;

Board::Board(int width, int height)
{
    initialize(width, height);
}

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

int Board::getWidth() const
{
    return m_width;
}

int Board::getHeight() const
{
    return m_height;
}

bool Board::isInsideBoard(int x, int y) const
{
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

Tile* Board::getTile(int x, int y)
{
    if (!isInsideBoard(x, y))
        return nullptr;

    return &m_tiles[y][x];
}

const Tile* Board::getTile(int x, int y) const
{
    if (!isInsideBoard(x, y))
        return nullptr;

    return &m_tiles[y][x];
}
