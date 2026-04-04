#include "defaultattackstrategy.h"
#include "../models/unit.h"

#include <algorithm>

int DefaultAttackStrategy::calculateDamage(const Unit& attacker,
                                           const Unit& defender,
                                           TerrainType defenderTerrain) const
{
    Q_UNUSED(defender);

    int damage = attacker.getDamage();

    switch (defenderTerrain)
    {
    case TerrainType::Forest:
        damage -= 8;
        break;
    case TerrainType::Mountain:
        damage -= 12;
        break;
    case TerrainType::Building:
        damage -= 15;
        break;
    case TerrainType::Water:
    case TerrainType::Plain:
    default:
        break;
    }

    return std::max(5, damage);
}
