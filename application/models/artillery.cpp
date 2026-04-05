#include "artillery.h"

Artillery::Artillery(TeamSide side)
    : Unit("Artyleria", side, 80, 52, 2, 5, 2, 4, 2, 3, 76, 5, 4, false, 0, true, false)
{
}

QString Artillery::getUnitType() const
{
    return "Artillery";
}
