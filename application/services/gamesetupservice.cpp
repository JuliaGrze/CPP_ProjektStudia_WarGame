#include "gamesetupservice.h"

#include "../models/board.h"
#include "../models/tile.h"
#include "../models/enums/terraintype.h"
#include "../helpers/unitplacementhelper.h"

#include <QRandomGenerator>
#include <algorithm>

namespace
{
void setTerrainIfPossible(Board& board, int x, int y, TerrainType terrain)
{
    Tile* tile = board.getTile(x, y);
    if (tile)
        tile->setTerrain(terrain);
}

void fillBoardWithPlain(Board& board)
{
    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = 0; x < board.getWidth(); ++x)
        {
            Tile* tile = board.getTile(x, y);
            if (tile)
                tile->setTerrain(TerrainType::Plain);
        }
    }
}

void generateForestAndMountains(Board& board)
{
    const int size = board.getWidth();
    const int centerX = size / 2;
    const int centerY = size / 2;

    // 2-3 skupiska lasu
    const int forestClusters = (size >= 15) ? 3 : 2;
    for (int i = 0; i < forestClusters; ++i)
    {
        const int clusterX = QRandomGenerator::global()->bounded(2, size - 2);
        const int clusterY = QRandomGenerator::global()->bounded(2, size - 2);

        for (int dy = -1; dy <= 1; ++dy)
        {
            for (int dx = -1; dx <= 1; ++dx)
            {
                if (QRandomGenerator::global()->bounded(100) < 75)
                    setTerrainIfPossible(board, clusterX + dx, clusterY + dy, TerrainType::Forest);
            }
        }
    }

    // wzgórza bardziej w środkowej części planszy
    const int mountainCount = (size >= 15) ? 5 : 3;
    for (int i = 0; i < mountainCount; ++i)
    {
        const int x = std::clamp(centerX + QRandomGenerator::global()->bounded(-3, 4), 1, size - 2);
        const int y = std::clamp(centerY + QRandomGenerator::global()->bounded(-4, 5), 1, size - 2);
        setTerrainIfPossible(board, x, y, TerrainType::Mountain);
    }
}

void generateRiverAndBridge(Board& board)
{
    const int size = board.getWidth();
    int riverColumn = size / 2 + QRandomGenerator::global()->bounded(-1, 2);

    riverColumn = std::clamp(riverColumn, 2, size - 3);

    for (int y = 0; y < size; ++y)
    {
        setTerrainIfPossible(board, riverColumn, y, TerrainType::Water);

        // lekki meander rzeki
        if (y % 4 == 2)
            riverColumn = std::clamp(riverColumn + QRandomGenerator::global()->bounded(-1, 2), 2, size - 3);
    }

    const int bridgeY = size / 2;
    setTerrainIfPossible(board, riverColumn, bridgeY, TerrainType::Plain);

    if (bridgeY - 1 >= 0)
        setTerrainIfPossible(board, riverColumn, bridgeY - 1, TerrainType::Plain);
}

void generateBuildings(Board& board)
{
    const int size = board.getWidth();
    const int centerX = size / 2;
    const int centerY = size / 2;

    // główne zabudowania bliżej środka
    for (int y = centerY - 2; y <= centerY + 2; y += 2)
    {
        for (int x = centerX - 2; x <= centerX + 2; ++x)
        {
            if (QRandomGenerator::global()->bounded(100) < 65)
                setTerrainIfPossible(board, x, y, TerrainType::Building);
        }
    }

    // trochę lasu wokół, żeby teren był bardziej taktyczny
    for (int i = 0; i < size / 3; ++i)
    {
        const int x = QRandomGenerator::global()->bounded(1, size - 1);
        const int y = QRandomGenerator::global()->bounded(1, size - 1);

        Tile* tile = board.getTile(x, y);
        if (tile && tile->getTerrain() == TerrainType::Plain)
        {
            if (QRandomGenerator::global()->bounded(100) < 35)
                tile->setTerrain(TerrainType::Forest);
        }
    }
}
}

GameState GameSetupService::createGame(const GameConfig& config)
{
    GameState state;

    int boardSize = config.mapSize;
    if (boardSize != 10 && boardSize != 15 && boardSize != 20)
        boardSize = 10;

    Board board(boardSize, boardSize);
    fillBoardWithPlain(board);

    if (config.mapVariant == "Las i wzgórza")
    {
        generateForestAndMountains(board);
    }
    else if (config.mapVariant == "Rzeka i most")
    {
        generateRiverAndBridge(board);
    }
    else if (config.mapVariant == "Zabudowania")
    {
        generateBuildings(board);
    }

    UnitPlacementHelper::placeTeams(
        board,
        state.getPlayerTeam(),
        state.getEnemyTeam(),
        config.playerTeam,
        config.enemyTeam
        );

    state.setBoard(board);

    return state;
}
