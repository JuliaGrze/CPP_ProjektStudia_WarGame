#include "aicontroller.h"

#include "../engine/attackresolver.h"
#include "../engine/battleengine.h"
#include "../models/gamestate.h"
#include "../models/board.h"
#include "../models/tile.h"
#include "../models/unit.h"
#include "../models/enums/teamside.h"

#include <QList>
#include <QtGlobal>
#include <limits>

namespace
{
struct UnitPosition
{
    int x = -1;
    int y = -1;
    Unit* unit = nullptr;
};

QList<UnitPosition> findUnits(const GameState& gameState, TeamSide side)
{
    QList<UnitPosition> positions;
    const Board& board = gameState.getBoard();

    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = 0; x < board.getWidth(); ++x)
        {
            const Tile* tile = board.getTile(x, y);
            if (!tile || !tile->isOccupied() || !tile->getUnit())
                continue;

            if (tile->getUnit()->getSide() == side && tile->getUnit()->isAlive())
                positions.append({ x, y, tile->getUnit() });
        }
    }

    return positions;
}

int manhattan(int x1, int y1, int x2, int y2)
{
    return qAbs(x1 - x2) + qAbs(y1 - y2);
}
}

void AIController::performTurn(GameState& gameState, BattleEngine& battleEngine)
{
    AttackResolver resolver;

    while (gameState.getCurrentSide() == TeamSide::Enemy &&
           gameState.hasTurnActionPoints() &&
           !gameState.isGameFinished())
    {
        const QList<UnitPosition> enemies = findUnits(gameState, TeamSide::Enemy);
        const QList<UnitPosition> players = findUnits(gameState, TeamSide::Player);

        if (enemies.isEmpty() || players.isEmpty())
            break;

        bool actionDone = false;

        for (const UnitPosition& enemyPos : enemies)
        {
            if (!enemyPos.unit || !enemyPos.unit->isAlive())
                continue;

            const Tile* enemyTile = gameState.getBoard().getTile(enemyPos.x, enemyPos.y);
            if (!enemyTile)
                continue;

            for (const UnitPosition& playerPos : players)
            {
                if (!playerPos.unit || !playerPos.unit->isAlive())
                    continue;

                if (resolver.canAttack(*enemyPos.unit,
                                       enemyPos.x,
                                       enemyPos.y,
                                       *playerPos.unit,
                                       playerPos.x,
                                       playerPos.y,
                                       enemyTile->getTerrain()))
                {
                    battleEngine.handleTileClick(gameState, enemyPos.x, enemyPos.y);
                    battleEngine.handleTileClick(gameState, playerPos.x, playerPos.y);
                    actionDone = true;
                    break;
                }
            }

            if (actionDone)
                break;
        }

        if (actionDone)
            continue;

        for (const UnitPosition& enemyPos : enemies)
        {
            if (!enemyPos.unit || !enemyPos.unit->isAlive())
                continue;

            if (!enemyPos.unit->canMoveNow())
                continue;

            battleEngine.handleTileClick(gameState, enemyPos.x, enemyPos.y);
            const auto movePositions = gameState.getAvailableMovePositions();

            if (movePositions.isEmpty())
                continue;

            int bestIndex = -1;
            int bestScore = std::numeric_limits<int>::max();

            for (int i = 0; i < movePositions.size(); ++i)
            {
                const auto& move = movePositions[i];
                int nearest = std::numeric_limits<int>::max();

                for (const UnitPosition& playerPos : players)
                {
                    if (!playerPos.unit || !playerPos.unit->isAlive())
                        continue;

                    nearest = qMin(nearest, manhattan(move.first, move.second, playerPos.x, playerPos.y));
                }

                const Tile* tile = gameState.getBoard().getTile(move.first, move.second);
                const int coverBonus = tile ? tile->getCoverBonus() : 0;
                const int score = nearest * 10 - coverBonus;

                if (score < bestScore)
                {
                    bestScore = score;
                    bestIndex = i;
                }
            }

            if (bestIndex >= 0)
            {
                battleEngine.handleTileClick(gameState,
                                             movePositions[bestIndex].first,
                                             movePositions[bestIndex].second);
                actionDone = true;
                break;
            }
        }

        if (!actionDone)
            battleEngine.endTurn(gameState);
    }
}
