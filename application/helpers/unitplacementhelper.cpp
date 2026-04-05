#include "unitplacementhelper.h"

#include "../models/infantry.h"
#include "../models/tank.h"
#include "../models/artillery.h"
#include "../models/medic.h"

#include <QRandomGenerator>
#include <algorithm>

namespace
{
int getNeededUnitsCount(const TeamComposition& comp)
{
    return comp.infantry + comp.tank + comp.artillery + comp.medic;
}
}

void UnitPlacementHelper::placeTeams(Board& board,
                                     Team& playerTeam,
                                     Team& enemyTeam,
                                     const TeamComposition& playerComp,
                                     const TeamComposition& enemyComp)
{
    const int playerUnitsCount = getNeededUnitsCount(playerComp);
    const int enemyUnitsCount = getNeededUnitsCount(enemyComp);

    QVector<QPair<int, int>> playerPositions =
        generateWeightedRandomPositions(board, TeamSide::Player, playerUnitsCount);

    QVector<QPair<int, int>> enemyPositions =
        generateWeightedRandomPositions(board, TeamSide::Enemy, enemyUnitsCount);

    placeSingleTeam(board, playerTeam, playerComp, TeamSide::Player, playerPositions);
    placeSingleTeam(board, enemyTeam, enemyComp, TeamSide::Enemy, enemyPositions);
}

QVector<QPair<int, int>> UnitPlacementHelper::generateWeightedRandomPositions(Board& board,
                                                                              TeamSide side,
                                                                              int neededCount)
{
    QVector<QPair<int, int>> candidates;

    const int width = board.getWidth();
    const int height = board.getHeight();

    if (width <= 0 || height <= 0 || neededCount <= 0)
        return {};

    int fromCol = 0;
    int toCol = 0;

    if (side == TeamSide::Player)
    {
        fromCol = 0;
        toCol = std::min(2, width - 1);
    }
    else
    {
        fromCol = std::max(0, width - 3);
        toCol = width - 1;
    }

    for (int y = 0; y < height; ++y)
    {
        for (int x = fromCol; x <= toCol; ++x)
        {
            Tile* tile = board.getTile(x, y);
            if (!tile)
                continue;

            if (!tile->isWalkable())
                continue;

            if (tile->isOccupied())
                continue;

            candidates.append({ x, y });
        }
    }

    for (int i = candidates.size() - 1; i > 0; --i)
    {
        const int j = QRandomGenerator::global()->bounded(i + 1);
        candidates.swapItemsAt(i, j);
    }

    if (candidates.size() > neededCount)
        candidates.resize(neededCount);

    return candidates;
}

void UnitPlacementHelper::placeSingleTeam(Board& board,
                                          Team& team,
                                          const TeamComposition& comp,
                                          TeamSide side,
                                          const QVector<QPair<int, int>>& positions)
{
    int index = 0;

    auto placeUnit = [&](std::shared_ptr<Unit> unit)
    {
        team.addUnit(unit);

        while (index < positions.size())
        {
            const int x = positions[index].first;
            const int y = positions[index].second;
            ++index;

            Tile* tile = board.getTile(x, y);
            if (tile && !tile->isOccupied() && tile->isWalkable())
            {
                tile->setUnit(unit.get());
                return;
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
