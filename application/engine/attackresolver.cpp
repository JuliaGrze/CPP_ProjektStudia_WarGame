#include "attackresolver.h"
#include "../models/unit.h"

#include <QRandomGenerator>
#include <algorithm>
#include <cmath>

/**
 * @brief Tworzy obiekt odpowiedzialny za rozstrzyganie ataków.
 */
AttackResolver::AttackResolver() = default;

/**
 * @brief Oblicza odległość pomiędzy dwoma polami planszy.
 *
 * Wykorzystuje metrykę Manhattan, czyli sumę bezwzględnych różnic
 * współrzędnych X i Y.
 *
 * @param x1 Współrzędna X pierwszego pola.
 * @param y1 Współrzędna Y pierwszego pola.
 * @param x2 Współrzędna X drugiego pola.
 * @param y2 Współrzędna Y drugiego pola.
 * @return Odległość pomiędzy wskazanymi polami.
 */
int AttackResolver::calculateDistance(int x1, int y1, int x2, int y2) const
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

/**
 * @brief Sprawdza, czy atak może zostać wykonany.
 *
 * Metoda weryfikuje, czy:
 * - obie jednostki żyją,
 * - atakujący może wykonać atak w bieżącej turze,
 * - jednostki należą do przeciwnych stron,
 * - cel znajduje się w dopuszczalnym zasięgu ataku.
 *
 * Maksymalny zasięg jest wyznaczany dynamicznie na podstawie
 * rzeczywistego typu jednostki atakującej i typu terenu.
 *
 * @param attacker Jednostka wykonująca atak.
 * @param attackerX Współrzędna X atakującego.
 * @param attackerY Współrzędna Y atakującego.
 * @param defender Jednostka będąca celem ataku.
 * @param defenderX Współrzędna X obrońcy.
 * @param defenderY Współrzędna Y obrońcy.
 * @param attackerTerrain Typ terenu zajmowanego przez atakującego.
 * @return true, jeśli atak może zostać wykonany, w przeciwnym razie false.
 */
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
    const int maxRange = attacker.getRange() + attacker.getAttackRangeBonus(attackerTerrain);

    return distance >= attacker.getMinRange() && distance <= maxRange;
}

/**
 * @brief Oblicza procentową szansę trafienia celu.
 *
 * Szansa trafienia zależy od:
 * - bazowej wartości trafienia,
 * - celności atakującego,
 * - uniku obrońcy,
 * - premii obronnej wynikającej z terenu,
 * - kary za dystans,
 * - premii terenowej atakującego,
 * - polimorficznego modyfikatora trafienia przeciw danemu celowi.
 *
 * Wynik końcowy jest ograniczany do przedziału od 15% do 95%.
 *
 * @param attacker Jednostka atakująca.
 * @param defender Jednostka broniąca się.
 * @param distance Odległość pomiędzy jednostkami.
 * @param attackerTerrain Typ terenu zajmowanego przez atakującego.
 * @param defenderTerrain Typ terenu zajmowanego przez obrońcę.
 * @return Szansa trafienia wyrażona w procentach.
 */
int AttackResolver::calculateHitChance(const Unit& attacker,
                                       const Unit& defender,
                                       int distance,
                                       TerrainType attackerTerrain,
                                       TerrainType defenderTerrain) const
{
    const int baseChance = 50;
    const int attackerAccuracy = attacker.getAccuracy();
    const int defenderDefense = defender.getEvasion();
    const int terrainDefense = defender.getDefenseBonusOnTerrain(defenderTerrain);
    const int attackerVsTargetModifier = attacker.getAttackAccuracyModifierAgainst(defender);

    int distancePenalty = distance * 5;
    if (distance <= 1)
        distancePenalty = 0;

    int attackerTerrainBonus = 0;
    if (attackerTerrain == TerrainType::Mountain)
        attackerTerrainBonus = 5;

    const int rawHitChance =
        baseChance
        + attackerAccuracy
        - defenderDefense
        - terrainDefense
        - distancePenalty
        + attackerTerrainBonus
        + attackerVsTargetModifier;

    return std::clamp(rawHitChance, 15, 95);
}

/**
 * @brief Rozstrzyga przebieg ataku i zwraca jego wynik.
 *
 * Metoda:
 * - sprawdza możliwość wykonania ataku,
 * - oblicza dystans i szansę trafienia,
 * - wykonuje losowanie trafienia,
 * - oblicza obrażenia z uwzględnieniem pancerza i terenu,
 * - aktualizuje stan zdrowia obrońcy,
 * - przygotowuje komunikat tekstowy opisujący wynik akcji.
 *
 * W obliczeniach wykorzystywane są metody wirtualne klasy Unit,
 * dzięki czemu zachowanie zależy od rzeczywistego typu jednostki.
 *
 * @param attacker Jednostka wykonująca atak.
 * @param attackerX Współrzędna X atakującego.
 * @param attackerY Współrzędna Y atakującego.
 * @param attackerTerrain Typ terenu zajmowanego przez atakującego.
 * @param defender Jednostka będąca celem ataku.
 * @param defenderX Współrzędna X obrońcy.
 * @param defenderY Współrzędna Y obrońcy.
 * @param defenderTerrain Typ terenu zajmowanego przez obrońcę.
 * @return Struktura zawierająca szczegółowy wynik ataku.
 */
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

    result.baseDamage = attacker.getDamage() + attacker.getDamageModifierAgainst(defender);
    result.randomDamageBonus = QRandomGenerator::global()->bounded(-5, 6);

    const int armorAfterPiercing =
        std::max(0, defender.getArmor() - attacker.getArmorPiercing());

    result.defenderArmorUsed = armorAfterPiercing;
    result.terrainReduction = defender.getDamageReductionOnTerrain(defenderTerrain);

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
