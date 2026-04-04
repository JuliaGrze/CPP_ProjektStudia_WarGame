#include "gamesetupservice.h"

#include "../models/board.h"
#include "../models/tile.h"
#include "../models/enums/terraintype.h"
#include "../helpers/unitplacementhelper.h"

GameState GameSetupService::createGame(const GameConfig& config)
{
    GameState state;

    int boardSize = config.mapSize;
    if (boardSize != 10 && boardSize != 15 && boardSize != 20)
        boardSize = 10;

    Board board(boardSize, boardSize);

    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = 0; x < board.getWidth(); ++x)
        {
            Tile* tile = board.getTile(x, y);
            if (tile)
                tile->setTerrain(TerrainType::Plain);
        }
    }

    if (config.mapVariant == "Las i wzgórza")
    {
        for (int i = 1; i < boardSize - 1; i += 3)
        {
            Tile* forestTile = board.getTile(boardSize / 3, i);
            Tile* mountainTile = board.getTile((boardSize / 3) * 2, i);

            if (forestTile)
                forestTile->setTerrain(TerrainType::Forest);

            if (mountainTile)
                mountainTile->setTerrain(TerrainType::Mountain);
        }
    }
    else if (config.mapVariant == "Rzeka i most")
    {
        int riverColumn = boardSize / 2;

        for (int y = 0; y < boardSize; ++y)
        {
            Tile* waterTile = board.getTile(riverColumn, y);
            if (waterTile)
                waterTile->setTerrain(TerrainType::Water);
        }

        Tile* bridgeTile1 = board.getTile(riverColumn, boardSize / 2);
        Tile* bridgeTile2 = board.getTile(riverColumn, boardSize / 2 - 1);

        if (bridgeTile1)
            bridgeTile1->setTerrain(TerrainType::Plain);

        if (bridgeTile2)
            bridgeTile2->setTerrain(TerrainType::Plain);
    }
    else if (config.mapVariant == "Zabudowania")
    {
        for (int y = 2; y < boardSize - 2; y += 4)
        {
            for (int x = boardSize / 2 - 1; x <= boardSize / 2 + 1; ++x)
            {
                Tile* buildingTile = board.getTile(x, y);
                if (buildingTile)
                    buildingTile->setTerrain(TerrainType::Building);
            }
        }
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
