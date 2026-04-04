#ifndef IUNIT_H
#define IUNIT_H

#include <QString>
#include "../enums/teamside.h"

class IUnit
{
public:
    virtual ~IUnit() = default;

    virtual QString getName() const = 0;
    virtual TeamSide getSide() const = 0;

    virtual int getHealth() const = 0;
    virtual int getMaxHealth() const = 0;
    virtual int getDamage() const = 0;
    virtual int getRange() const = 0;

    virtual int getMovementPoints() const = 0;
    virtual int getCurrentMovementPoints() const = 0;

    virtual bool isAlive() const = 0;
    virtual void takeDamage(int amount) = 0;
    virtual void heal(int amount) = 0;

    virtual void consumeMovementPoints(int amount) = 0;
    virtual void resetTurnResources() = 0;

    virtual QString getUnitType() const = 0;
};

#endif // IUNIT_H
