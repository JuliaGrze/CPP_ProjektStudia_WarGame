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
         int minRange,
         int maxRange,
         int movementPoints,
         int attackCost,
         int moveCostPerTile,
         int armor,
         int accuracy,
         int evasion,
         int armorPiercing = 0,
         bool canHeal = false,
         int healAmount = 0,
         bool canMoveAfterAttack = true,
         bool canAttackAfterMove = true);

    virtual ~Unit() = default;

    QString getName() const;
    TeamSide getSide() const;

    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getMinRange() const;
    int getRange() const;
    int getMovementPoints() const;
    int getAttackCost() const;
    int getMoveCostPerTile() const;
    int getArmor() const;
    int getAccuracy() const;
    int getEvasion() const;
    int getArmorPiercing() const;
    int getHealAmount() const;

    bool canHeal() const;
    bool isAlive() const;
    bool isDamaged() const;

    void takeDamage(int amount);
    void heal(int amount);

    void markMoved();
    void markActed();
    bool hasMovedThisTurn() const;
    bool hasActedThisTurn() const;

    bool canMoveNow() const;
    bool canAttackNow() const;
    bool canHealNow() const;

    void resetTurnResources();

    virtual QString getUnitType() const = 0;

protected:
    QString m_name;
    TeamSide m_side;
    int m_health;
    int m_maxHealth;
    int m_damage;
    int m_minRange;
    int m_maxRange;

    int m_movementPoints;
    int m_attackCost;
    int m_moveCostPerTile;

    int m_armor;
    int m_accuracy;
    int m_evasion;
    int m_armorPiercing;

    bool m_canHeal;
    int m_healAmount;

    bool m_canMoveAfterAttack;
    bool m_canAttackAfterMove;

    bool m_hasMovedThisTurn = false;
    bool m_hasActedThisTurn = false;
};

#endif // UNIT_H
