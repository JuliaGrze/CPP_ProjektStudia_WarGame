#include "infantry.h"

Infantry::Infantry(TeamSide side)
    : Unit("Piechota", side, 100, 24, 1, 2, 3, 2, 1, 6, 72, 12, 0, false, 0, true, true)
{
}

QString Infantry::getUnitType() const
{
    return "Infantry";
}
