#include "tank.h"

Tank::Tank(TeamSide side)
    : Unit("Czołg", side, 180, 40, 3, 2)
{
}

QString Tank::getUnitType() const
{
    return "Tank";
}
