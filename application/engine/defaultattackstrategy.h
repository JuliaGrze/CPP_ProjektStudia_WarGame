#ifndef DEFAULTATTACKSTRATEGY_H
#define DEFAULTATTACKSTRATEGY_H

#include "iattackstrategy.h"

class DefaultAttackStrategy : public IAttackStrategy
{
public:
    int calculateDamage(const Unit& attacker,
                        const Unit& defender,
                        TerrainType defenderTerrain) const override;
};

#endif // DEFAULTATTACKSTRATEGY_H
