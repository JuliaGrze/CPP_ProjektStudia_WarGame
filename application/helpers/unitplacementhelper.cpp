#include "unitplacementhelper.h"

#include "../models/infantry.h"
#include "../models/tank.h"
#include "../models/artillery.h"
#include "../models/medic.h"

#include <QRandomGenerator>

void UnitPlacementHelper::placeTeams(Board& board,
                                     Team& playerTeam,
                                     Team& enemyTeam,
                                     const TeamComposition& playerComp,
                                     const TeamComposition& enemyComp)
{
    int width = board.getWidth();

    // lewa strona: 0–2
    QVector<QPair<int,int>> playerPositions = generateRandomPositions(board, 0, 2);

    // prawa strona: ostatnie 3 kolumny
    QVector<QPair<int,int>> enemyPositions = generateRandomPositions(board, width - 3, width - 1);

    placeSingleTeam(board, playerTeam, playerComp, TeamSide::Player, playerPositions);
    placeSingleTeam(board, enemyTeam, enemyComp, TeamSide::Enemy, enemyPositions);
}

QVector<QPair<int,int>> UnitPlacementHelper::generateRandomPositions(Board& board, int fromCol, int toCol)
{
    QVector<QPair<int,int>> positions;

    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = fromCol; x <= toCol; ++x)
        {
            Tile* tile = board.getTile(x, y);
            if (tile && tile->isWalkable())
                positions.append({x, y});
        }
    }

    // shuffle
    for (int i = positions.size() - 1; i > 0; --i)
    {
        int j = QRandomGenerator::global()->bounded(i + 1);
        positions.swapItemsAt(i, j);
    }

    return positions;
}

void UnitPlacementHelper::placeSingleTeam(Board& board,
                                          Team& team,
                                          const TeamComposition& comp,
                                          TeamSide side,
                                          const QVector<QPair<int,int>>& positions)
{
    int index = 0;

    auto placeUnit = [&](std::shared_ptr<Unit> unit)
    {
        team.addUnit(unit);

        while (index < positions.size())
        {
            int x = positions[index].first;
            int y = positions[index].second;
            ++index;

            Tile* tile = board.getTile(x, y);
            if (tile && !tile->isOccupied() && tile->isWalkable())
            {
                tile->setUnit(unit.get());
                break;
            }
        }
    };

    for (int i = 0; i < comp.infantry; ++i)
        placeUnit(std::make_shared<Infantry>(side));

    for (int i = 0; i < comp.tank; ++i)
        placeUnit(std::make_shared<Tank>(side));

    for (int i = 0; i < comp.artillery; ++i)
        placeUnit(std::make_shared<Artillery>(side));

    for (int i = 0; i < comp.medic; ++i)
        placeUnit(std::make_shared<Medic>(side));
}
