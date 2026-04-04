#ifndef INFANTRY_H
#define INFANTRY_H

#include "unit.h"

class Infantry : public Unit
{
public:
    explicit Infantry(TeamSide side);
    QString getUnitType() const override;
};

#endif // INFANTRY_H
