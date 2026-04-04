#ifndef IATTACKSTRATEGY_H
#define IATTACKSTRATEGY_H

#include "../models/enums/terraintype.h"

class Unit;

class IAttackStrategy
{
public:
    virtual ~IAttackStrategy() = default;

    virtual int calculateDamage(const Unit& attacker,
                                const Unit& defender,
                                TerrainType defenderTerrain) const = 0;
};

#endif // IATTACKSTRATEGY_H
