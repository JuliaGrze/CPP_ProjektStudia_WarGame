#include "unit.h"
#include <algorithm>

Unit::Unit(const QString& name,
           TeamSide side,
           int health,
           int damage,
           int range,
           int movementPoints)
    : m_name(name),
    m_side(side),
    m_health(health),
    m_maxHealth(health),
    m_damage(damage),
    m_range(range),
    m_movementPoints(movementPoints)
{
}

QString Unit::getName() const
{
    return m_name;
}

TeamSide Unit::getSide() const
{
    return m_side;
}

int Unit::getHealth() const
{
    return m_health;
}

int Unit::getMaxHealth() const
{
    return m_maxHealth;
}

int Unit::getDamage() const
{
    return m_damage;
}

int Unit::getRange() const
{
    return m_range;
}

int Unit::getMovementPoints() const
{
    return m_movementPoints;
}

bool Unit::isAlive() const
{
    return m_health > 0;
}

void Unit::takeDamage(int amount)
{
    if (amount < 0)
        return;

    m_health = std::max(0, m_health - amount);
}

void Unit::heal(int amount)
{
    if (amount < 0)
        return;

    m_health = std::min(m_maxHealth, m_health + amount);
}
