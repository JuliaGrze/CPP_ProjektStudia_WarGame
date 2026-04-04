#ifndef UNITPLACEMENTHELPER_H
#define UNITPLACEMENTHELPER_H
#include "../models/board.h"
#include "../models/team.h"
#include "../../config/teamComposition.h"

class UnitPlacementHelper
{
public:
    static void placeTeams(Board& board,
                           Team& playerTeam,
                           Team& enemyTeam,
                           const TeamComposition& playerComp,
                           const TeamComposition& enemyComp);

private:
    static QVector<QPair<int,int>> generateRandomPositions(Board& board, int fromCol, int toCol);

    static void placeSingleTeam(Board& board,
                                Team& team,
                                const TeamComposition& comp,
                                TeamSide side,
                                const QVector<QPair<int,int>>& positions);
};

#endif // UNITPLACEMENTHELPER_H
