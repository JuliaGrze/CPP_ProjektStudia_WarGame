#ifndef BOARD_H
#define BOARD_H

#include <QVector>
#include "tile.h"

class Board
{
public:
    Board();
    Board(int width, int height);

    void initialize(int width, int height, TerrainType defaultTerrain = TerrainType::Plain);

    int getWidth() const;
    int getHeight() const;

    bool isInsideBoard(int x, int y) const;

    Tile* getTile(int x, int y);
    const Tile* getTile(int x, int y) const;

private:
    int m_width = 0;
    int m_height = 0;
    QVector<QVector<Tile>> m_tiles;
};

#endif // BOARD_H
