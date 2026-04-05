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

int AttackResolver::calculateTerrainDefenseBonus(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Forest:
        return 10;
    case TerrainType::Mountain:
        return 8;
    case TerrainType::Building:
        return 12;
    case TerrainType::Plain:
    case TerrainType::Water:
    default:
        return 0;
    }
}

int AttackResolver::calculateTerrainDamageReduction(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Forest:
        return 2;
    case TerrainType::Mountain:
        return 3;
    case TerrainType::Building:
        return 4;
    case TerrainType::Plain:
    case TerrainType::Water:
    default:
        return 0;
    }
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

    int maxRange = attacker.getRange();
    if (attackerTerrain == TerrainType::Mountain)
        maxRange += 1;

    return distance >= attacker.getMinRange() && distance <= maxRange;
}

int AttackResolver::calculateHitChance(const Unit& attacker,
                                       const Unit& defender,
                                       int distance,
                                       TerrainType attackerTerrain,
                                       TerrainType defenderTerrain) const
{
    const int baseChance = 50;
    const int attackerAccuracy = attacker.getAccuracy();
    const int defenderDefense = defender.getEvasion();
    const int terrainDefense = calculateTerrainDefenseBonus(defenderTerrain);

    int distancePenalty = distance * 5;
    if (distance <= 1)
        distancePenalty = 0;

    int attackerTerrainBonus = 0;
    if (attackerTerrain == TerrainType::Mountain)
        attackerTerrainBonus = 5;

    // Zgodnie z założeniem:
    // hitChance = base + accuracy - defense - distance
    const int rawHitChance =
        baseChance
        + attackerAccuracy
        - defenderDefense
        - terrainDefense
        - distancePenalty
        + attackerTerrainBonus;

    return std::clamp(rawHitChance, 15, 95);
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
        result.message = QString("%1 nie może teraz zaatakować tego celu.")
                             .arg(attacker.getName());
        return result;
    }

    result.attackPerformed = true;
    result.distance = calculateDistance(attackerX, attackerY, defenderX, defenderY);
    result.hitChance = calculateHitChance(attacker, defender, result.distance, attackerTerrain, defenderTerrain);
    result.roll = QRandomGenerator::global()->bounded(1, 101);
    result.targetMaxHealth = defender.getMaxHealth();

    attacker.markActed();

    // Trafienie, gdy rzut <= szansa
    if (result.roll > result.hitChance)
    {
        result.hit = false;
        result.targetHealthAfter = defender.getHealth();

        result.message = QString(
                             "%1 zaatakował %2, ale spudłował. "
                             "Szansa trafienia: %3%%, rzut: %4. "
                             "Dystans: %5.")
                             .arg(attacker.getName())
                             .arg(defender.getName())
                             .arg(result.hitChance)
                             .arg(result.roll)
                             .arg(result.distance);

        return result;
    }

    result.hit = true;

    result.baseDamage = attacker.getDamage();
    result.randomDamageBonus = QRandomGenerator::global()->bounded(-5, 6);

    const int armorAfterPiercing =
        std::max(0, defender.getArmor() - attacker.getArmorPiercing());

    result.defenderArmorUsed = armorAfterPiercing;
    result.terrainReduction = calculateTerrainDamageReduction(defenderTerrain);

    const int rawDamage =
        result.baseDamage
        + result.randomDamageBonus
        - result.defenderArmorUsed
        - result.terrainReduction;

    result.damageDealt = std::max(1, rawDamage);

    defender.takeDamage(result.damageDealt);

    result.targetDestroyed = !defender.isAlive();
    result.targetHealthAfter = defender.getHealth();

    if (result.targetDestroyed)
    {
        result.message = QString(
                             "%1 trafił %2 i zniszczył jednostkę. "
                             "Obrażenia: %3 (bazowe %4, losowość %+5, pancerz -%6, teren -%7). "
                             "Szansa trafienia: %8%%, rzut: %9.")
                             .arg(attacker.getName())
                             .arg(defender.getName())
                             .arg(result.damageDealt)
                             .arg(result.baseDamage)
                             .arg(result.randomDamageBonus)
                             .arg(result.defenderArmorUsed)
                             .arg(result.terrainReduction)
                             .arg(result.hitChance)
                             .arg(result.roll);
    }
    else
    {
        result.message = QString(
                             "%1 trafił %2 za %3 obrażeń. "
                             "HP celu: %4/%5. "
                             "Rozpiska: bazowe %6, losowość %+7, pancerz -%8, teren -%9. "
                             "Szansa trafienia: %10%%, rzut: %11.")
                             .arg(attacker.getName())
                             .arg(defender.getName())
                             .arg(result.damageDealt)
                             .arg(result.targetHealthAfter)
                             .arg(result.targetMaxHealth)
                             .arg(result.baseDamage)
                             .arg(result.randomDamageBonus)
                             .arg(result.defenderArmorUsed)
                             .arg(result.terrainReduction)
                             .arg(result.hitChance)
                             .arg(result.roll);
    }

    return result;
}
