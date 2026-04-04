#include "tank.h"

Tank::Tank(TeamSide side)
    : Unit("Czolg", side, 180, 40, 3, 2, 3)
{
}

QString Tank::getUnitType() const
{
    return "Tank";
}
