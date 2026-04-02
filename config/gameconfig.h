#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <QString>
#include "teamComposition.h"

struct GameConfig
{
    QString mapVariant;
    int mapSize = 20;

    TeamComposition playerTeam;
    TeamComposition enemyTeam;

    bool isValid() const
    {
        return !mapVariant.isEmpty()
        && mapSize > 0
            && playerTeam.totalUnits() > 0
            && enemyTeam.totalUnits() > 0;
    }
};

#endif // GAMECONFIG_H
