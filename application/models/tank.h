#ifndef TANK_H
#define TANK_H

#include "unit.h"

class Tank : public Unit
{
public:
    explicit Tank(TeamSide side);
    QString getUnitType() const override;
};

#endif // TANK_H
