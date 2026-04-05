#include "medic.h"

Medic::Medic(TeamSide side)
    : Unit("Medyk", side, 90, 10, 1, 1, 3, 2, 1, 4, 68, 10, 0, true, 28, true, true)
{
}

QString Medic::getUnitType() const
{
    return "Medic";
}
