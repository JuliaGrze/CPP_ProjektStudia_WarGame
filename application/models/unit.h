#ifndef UNIT_H
#define UNIT_H

#include <QString>
#include "enums/teamside.h"

class Unit
{
public:
    Unit(const QString& name,
         TeamSide side,
         int health,
         int damage,
         int range,
         int movementPoints);

    virtual ~Unit() = default;

    QString getName() const;
    TeamSide getSide() const;

    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getRange() const;
    int getMovementPoints() const;

    bool isAlive() const;
    void takeDamage(int amount);
    void heal(int amount);

    virtual QString getUnitType() const = 0;

protected:
    QString m_name;
    TeamSide m_side;
    int m_health;
    int m_maxHealth;
    int m_damage;
    int m_range;
    int m_movementPoints;
};

#endif // UNIT_H
