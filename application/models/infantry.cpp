#include "infantry.h"
#include "tank.h"

/**
 * @brief Konstruktor jednostki piechoty.
 *
 * Ustawia statystyki piechoty jako lekkiej i uniwersalnej jednostki,
 * dobrze radzącej sobie w terenie zapewniającym osłonę.
 *
 * @param side Strona konfliktu.
 */
Infantry::Infantry(TeamSide side)
    : Unit("Piechota",
           side,
           100,  // health
           24,   // damage
           1,    // minRange
           2,    // maxRange
           3,    // movementPoints
           2,    // attackCost
           1,    // moveCostPerTile
           6,    // armor
           72,   // accuracy
           12,   // evasion
           0,    // armorPiercing
           false,// canHeal
           0,    // healAmount
           true, // canMoveAfterAttack
           true) // canAttackAfterMove
{
}

/**
 * @brief Zwraca nazwę typu jednostki.
 *
 * @return Napis "Infantry".
 */
QString Infantry::getUnitType() const
{
    return "Infantry";
}

/**
 * @brief Zwraca koszt ruchu piechoty po wskazanym terenie.
 *
 * Piechota dobrze radzi sobie w lesie i budynkach,
 * a w górach porusza się lepiej niż jednostki ciężkie.
 *
 * @param terrain Typ terenu.
 * @return Koszt ruchu.
 */
int Infantry::getTerrainMoveCost(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Plain:
        return 1;
    case TerrainType::Forest:
        return 1;
    case TerrainType::Mountain:
        return 2;
    case TerrainType::Building:
        return 1;
    case TerrainType::Water:
    default:
        return 9999;
    }
}

/**
 * @brief Zwraca premię obronną piechoty wynikającą z terenu.
 *
 * Piechota szczególnie dobrze korzysta z osłony w lesie i budynkach.
 *
 * @param terrain Typ terenu.
 * @return Premia obronna.
 */
int Infantry::getDefenseBonusOnTerrain(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Forest:
        return 14;
    case TerrainType::Mountain:
        return 10;
    case TerrainType::Building:
        return 16;
    case TerrainType::Plain:
    case TerrainType::Water:
    default:
        return 0;
    }
}

/**
 * @brief Zwraca redukcję obrażeń wynikającą z terenu dla piechoty.
 *
 * Piechota potrafi lepiej wykorzystać osłonę terenu niż jednostki ciężkie.
 *
 * @param terrain Typ terenu.
 * @return Redukcja obrażeń.
 */
int Infantry::getDamageReductionOnTerrain(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Forest:
        return 3;
    case TerrainType::Mountain:
        return 4;
    case TerrainType::Building:
        return 5;
    case TerrainType::Plain:
    case TerrainType::Water:
    default:
        return 0;
    }
}

/**
 * @brief Zwraca modyfikator trafienia piechoty przeciw obrońcy.
 *
 * Piechota ma utrudnione trafienie ciężko opancerzonego czołgu.
 *
 * @param defender Jednostka będąca celem ataku.
 * @return Modyfikator trafienia.
 */
int Infantry::getAttackAccuracyModifierAgainst(const Unit& defender) const
{
    if (dynamic_cast<const Tank*>(&defender))
        return -4;

    return 0;
}

/**
 * @brief Zwraca modyfikator obrażeń piechoty przeciw obrońcy.
 *
 * Piechota zadaje mniejsze obrażenia czołgom.
 *
 * @param defender Jednostka będąca celem ataku.
 * @return Modyfikator obrażeń.
 */
int Infantry::getDamageModifierAgainst(const Unit& defender) const
{
    if (dynamic_cast<const Tank*>(&defender))
        return -4;

    return 0;
}
