#include "unit.h"
#include <algorithm>

Unit::Unit(const QString& name,
           TeamSide side,
           int health,
           int damage,
           int minRange,
           int maxRange,
           int movementPoints,
           int attackCost,
           int moveCostPerTile,
           int armor,
           int accuracy,
           int evasion,
           int armorPiercing,
           bool canHeal,
           int healAmount,
           bool canMoveAfterAttack,
           bool canAttackAfterMove)
    : m_name(name),
    m_side(side),
    m_health(health),
    m_maxHealth(health),
    m_damage(damage),
    m_minRange(minRange),
    m_maxRange(maxRange),
    m_movementPoints(movementPoints),
    m_attackCost(attackCost),
    m_moveCostPerTile(moveCostPerTile),
    m_armor(armor),
    m_accuracy(accuracy),
    m_evasion(evasion),
    m_armorPiercing(armorPiercing),
    m_canHeal(canHeal),
    m_healAmount(healAmount),
    m_canMoveAfterAttack(canMoveAfterAttack),
    m_canAttackAfterMove(canAttackAfterMove)
{
}

QString Unit::getName() const { return m_name; }
TeamSide Unit::getSide() const { return m_side; }
int Unit::getHealth() const { return m_health; }
int Unit::getMaxHealth() const { return m_maxHealth; }
int Unit::getDamage() const { return m_damage; }
int Unit::getMinRange() const { return m_minRange; }
int Unit::getRange() const { return m_maxRange; }
int Unit::getMovementPoints() const { return m_movementPoints; }
int Unit::getAttackCost() const { return m_attackCost; }
int Unit::getMoveCostPerTile() const { return m_moveCostPerTile; }
int Unit::getArmor() const { return m_armor; }
int Unit::getAccuracy() const { return m_accuracy; }
int Unit::getEvasion() const { return m_evasion; }
int Unit::getArmorPiercing() const { return m_armorPiercing; }
int Unit::getHealAmount() const { return m_healAmount; }
bool Unit::canHeal() const { return m_canHeal; }

bool Unit::isAlive() const { return m_health > 0; }
bool Unit::isDamaged() const { return m_health < m_maxHealth; }

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

void Unit::markMoved()
{
    m_hasMovedThisTurn = true;
}

void Unit::markActed()
{
    m_hasActedThisTurn = true;
}

bool Unit::hasMovedThisTurn() const { return m_hasMovedThisTurn; }
bool Unit::hasActedThisTurn() const { return m_hasActedThisTurn; }

bool Unit::canMoveNow() const
{
    return isAlive();
}

bool Unit::canAttackNow() const
{
    return isAlive();
}

bool Unit::canHealNow() const
{
    return m_canHeal && canAttackNow();
}

void Unit::resetTurnResources()
{
    m_hasMovedThisTurn = false;
    m_hasActedThisTurn = false;
}
