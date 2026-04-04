#ifndef MEDIC_H
#define MEDIC_H

#include "unit.h"

class Medic : public Unit
{
public:
    explicit Medic(TeamSide side);
    QString getUnitType() const override;
};

#endif // MEDIC_H
