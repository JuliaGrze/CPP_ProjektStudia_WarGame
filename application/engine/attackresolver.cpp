#include "attackresolver.h"
#include "../models/unit.h"

#include <QRandomGenerator>
#include <algorithm>
#include <cmath>

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
                               int defenderY,
                               TerrainType attackerTerrain) const
{
    if (!attacker.isAlive() || !defender.isAlive())
        return false;

    if (!attacker.canAttackNow())
        return false;

    if (attacker.getSide() == defender.getSide())
        return false;

    const int distance = calculateDistance(attackerX, attackerY, defenderX, defenderY);
    const int maxRange = attacker.getRange() + (attackerTerrain == TerrainType::Mountain ? 1 : 0);

    return distance >= attacker.getMinRange() && distance <= maxRange;
}

int AttackResolver::calculateHitChance(const Unit& attacker,
                                       const Unit& defender,
                                       int distance,
                                       TerrainType attackerTerrain,
                                       TerrainType defenderTerrain) const
{
    int distancePenalty = 0;
    if (distance > 1)
        distancePenalty = (distance - 1) * 7;

    int rangeBonus = 0;
    if (attackerTerrain == TerrainType::Mountain)
        rangeBonus = 5;

    int coverBonus = 0;
    switch (defenderTerrain)
    {
    case TerrainType::Forest: coverBonus = 15; break;
    case TerrainType::Mountain: coverBonus = 10; break;
    case TerrainType::Building: coverBonus = 20; break;
    case TerrainType::Plain:
    case TerrainType::Water:
    default: break;
    }

    const int raw = attacker.getAccuracy() - defender.getEvasion() - distancePenalty - coverBonus + rangeBonus;
    return std::clamp(raw, 25, 95);
}

int AttackResolver::calculateDamage(const Unit& attacker,
                                    const Unit& defender,
                                    TerrainType defenderTerrain) const
{
    const int randomPart = QRandomGenerator::global()->bounded(-5, 6);

    int terrainReduction = 0;
    switch (defenderTerrain)
    {
    case TerrainType::Forest: terrainReduction = 2; break;
    case TerrainType::Mountain: terrainReduction = 3; break;
    case TerrainType::Building: terrainReduction = 4; break;
    case TerrainType::Plain:
    case TerrainType::Water:
    default: break;
    }

    const int armorAfterPiercing = std::max(0, defender.getArmor() - attacker.getArmorPiercing());
    const int rawDamage = attacker.getDamage() + randomPart;

    return std::max(1, rawDamage - armorAfterPiercing - terrainReduction);
}

AttackResult AttackResolver::resolveAttack(Unit& attacker,
                                           int attackerX,
                                           int attackerY,
                                           TerrainType attackerTerrain,
                                           Unit& defender,
                                           int defenderX,
                                           int defenderY,
                                           TerrainType defenderTerrain) const
{
    AttackResult result;

    if (!canAttack(attacker, attackerX, attackerY, defender, defenderX, defenderY, attackerTerrain))
    {
        result.message = QString("%1 nie może wykonać ataku.").arg(attacker.getName());
        return result;
    }

    const int distance = calculateDistance(attackerX, attackerY, defenderX, defenderY);
    result.hitChance = calculateHitChance(attacker, defender, distance, attackerTerrain, defenderTerrain);
    result.roll = QRandomGenerator::global()->bounded(1, 101);
    result.attackPerformed = true;
    result.targetMaxHealth = defender.getMaxHealth();

    if (result.roll > result.hitChance)
    {
        result.hit = false;
        result.targetHealthAfter = defender.getHealth();
        result.message = QString("%1 zaatakował %2, ale spudłował. Rzut: %3, potrzebne: %4 lub mniej.")
                             .arg(attacker.getName())
                             .arg(defender.getName())
                             .arg(result.roll)
                             .arg(result.hitChance);
        return result;
    }

    result.hit = true;
    result.damageDealt = calculateDamage(attacker, defender, defenderTerrain);
    defender.takeDamage(result.damageDealt);
    result.targetDestroyed = !defender.isAlive();
    result.targetHealthAfter = defender.getHealth();

    if (result.targetDestroyed)
    {
        result.message = QString("%1 trafił %2 za %3 obrażeń i zniszczył jednostkę. Rzut: %4/%5.")
                             .arg(attacker.getName())
                             .arg(defender.getName())
                             .arg(result.damageDealt)
                             .arg(result.roll)
                             .arg(result.hitChance);
    }
    else
    {
        result.message = QString("%1 trafił %2 za %3 obrażeń. Pozostało %4/%5 HP. Rzut: %6/%7.")
                             .arg(attacker.getName())
                             .arg(defender.getName())
                             .arg(result.damageDealt)
                             .arg(result.targetHealthAfter)
                             .arg(result.targetMaxHealth)
                             .arg(result.roll)
                             .arg(result.hitChance);
    }

    return result;
}
