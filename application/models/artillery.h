#ifndef ARTILLERY_H
#define ARTILLERY_H

#include "unit.h"

class Artillery : public Unit
{
public:
    explicit Artillery(TeamSide side);
    QString getUnitType() const override;
};

#endif // ARTILLERY_H
