#include "tank.h"

Tank::Tank(TeamSide side)
    : Unit("Czolg", side, 180, 42, 1, 3, 2, 3, 2, 18, 78, 6, 8, false, 0, true, true)
{
}

QString Tank::getUnitType() const
{
    return "Tank";
}
