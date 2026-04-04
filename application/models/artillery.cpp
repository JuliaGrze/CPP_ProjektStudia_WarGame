#include "artillery.h"

Artillery::Artillery(TeamSide side)
    : Unit("Artyleria", side, 80, 50, 5, 1, 4)
{
}

QString Artillery::getUnitType() const
{
    return "Artillery";
}
