#include "medic.h"

Medic::Medic(TeamSide side)
    : Unit("Medyk", side, 90, 10, 1, 3, 1)
{
}

QString Medic::getUnitType() const
{
    return "Medic";
}
