#include "attackresolver.h"

#include "defaultattackstrategy.h"
#include "../models/unit.h"

#include <memory>
#include <cmath>

namespace
{
std::unique_ptr<IAttackStrategy> createDefaultStrategy()
{
    return std::make_unique<DefaultAttackStrategy>();
}
}

class AttackResolverPrivate
{
public:
    std::unique_ptr<IAttackStrategy> strategy = createDefaultStrategy();
};

AttackResolver::AttackResolver() = default;

int AttackResolver::calculateDistance(int x1, int y1, int x2, int y2) const
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

bool AttackResolver::canAttack(const Unit& attacker,
                               int attackerX,
                               int attackerY,
                               const Unit& defender,
                               int defenderX,
                               int defenderY) const
{
    if (!attacker.isAlive() || !defender.isAlive())
        return false;

    if (attacker.getSide() == defender.getSide())
        return false;

    const int distance = calculateDistance(attackerX, attackerY, defenderX, defenderY);
    return distance <= attacker.getRange();
}

AttackResult AttackResolver::resolveAttack(Unit& attacker,
                                           int attackerX,
                                           int attackerY,
                                           Unit& defender,
                                           int defenderX,
                                           int defenderY,
                                           TerrainType defenderTerrain) const
{
    AttackResult result;

    if (!canAttack(attacker, attackerX, attackerY, defender, defenderX, defenderY))
    {
        result.message = QString("%1 nie może zaatakować celu - przeciwnik jest poza zasięgiem.")
                             .arg(attacker.getName());
        return result;
    }

    DefaultAttackStrategy strategy;
    const int damage = strategy.calculateDamage(attacker, defender, defenderTerrain);

    defender.takeDamage(damage);

    result.attackPerformed = true;
    result.damageDealt = damage;
    result.targetDestroyed = !defender.isAlive();

    if (result.targetDestroyed)
    {
        result.message = QString("%1 zaatakował %2 i zniszczył jednostkę.")
                             .arg(attacker.getName())
                             .arg(defender.getName());
    }
    else
    {
        result.message = QString("%1 zaatakował %2 i zadał %3 obrażeń.")
                             .arg(attacker.getName())
                             .arg(defender.getName())
                             .arg(damage);
    }

    return result;
}
