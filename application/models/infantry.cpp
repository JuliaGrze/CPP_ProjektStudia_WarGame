#include "infantry.h"

Infantry::Infantry(TeamSide side)
    : Unit("Piechota", side, 100, 20, 2, 3)
{
}

QString Infantry::getUnitType() const
{
    return "Infantry";
}
