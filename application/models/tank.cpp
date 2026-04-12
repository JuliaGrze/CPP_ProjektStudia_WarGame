#include "tank.h"
#include "infantry.h"
#include "medic.h"

/**
 * @brief Konstruktor jednostki czołgu.
 *
 * Ustawia statystyki czołgu jako ciężkiej jednostki bojowej:
 * wysokie zdrowie, duże obrażenia i pancerz, ale słabsza mobilność
 * w trudnym terenie.
 *
 * @param side Strona konfliktu.
 */
Tank::Tank(TeamSide side)
    : Unit("Czolg",
           side,
           180,  // health
           42,   // damage
           1,    // minRange
           3,    // maxRange
           2,    // movementPoints
           3,    // attackCost
           2,    // moveCostPerTile
           18,   // armor
           78,   // accuracy
           6,    // evasion
           8,    // armorPiercing
           false,// canHeal
           0,    // healAmount
           true, // canMoveAfterAttack
           true) // canAttackAfterMove
{
}

/**
 * @brief Zwraca nazwę typu jednostki.
 *
 * @return Napis "Tank".
 */
QString Tank::getUnitType() const
{
    return "Tank";
}

/**
 * @brief Zwraca koszt ruchu czołgu po wskazanym terenie.
 *
 * Czołg dobrze radzi sobie na otwartym terenie, ale gorzej
 * porusza się po lesie i górach.
 *
 * @param terrain Typ terenu.
 * @return Koszt ruchu.
 */
int Tank::getTerrainMoveCost(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Plain:
        return 1;
    case TerrainType::Forest:
        return 3;
    case TerrainType::Mountain:
        return 4;
    case TerrainType::Building:
        return 2;
    case TerrainType::Water:
    default:
        return 9999;
    }
}

/**
 * @brief Zwraca premię do trafienia przeciw konkretnemu obrońcy.
 *
 * Czołg jest szczególnie skuteczny przeciw lekkim jednostkom,
 * takim jak piechota czy medyk.
 *
 * @param defender Jednostka będąca celem ataku.
 * @return Modyfikator trafienia.
 */
int Tank::getAttackAccuracyModifierAgainst(const Unit& defender) const
{
    if (dynamic_cast<const Infantry*>(&defender))
        return 4;

    if (dynamic_cast<const Medic*>(&defender))
        return 6;

    return 0;
}

/**
 * @brief Zwraca dodatkowy modyfikator obrażeń przeciw obrońcy.
 *
 * Czołg zadaje większe obrażenia lekkim celom.
 *
 * @param defender Jednostka będąca celem ataku.
 * @return Modyfikator obrażeń.
 */
int Tank::getDamageModifierAgainst(const Unit& defender) const
{
    if (dynamic_cast<const Infantry*>(&defender))
        return 6;

    if (dynamic_cast<const Medic*>(&defender))
        return 8;

    return 0;
}

/**
 * @brief Zwraca premię obronną czołgu wynikającą z terenu.
 *
 * Czołg korzysta z osłony terenu słabiej niż jednostki lekkie.
 *
 * @param terrain Typ terenu.
 * @return Premia obronna.
 */
int Tank::getDefenseBonusOnTerrain(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Forest:
        return 6;
    case TerrainType::Mountain:
        return 4;
    case TerrainType::Building:
        return 8;
    case TerrainType::Plain:
    case TerrainType::Water:
    default:
        return 0;
    }
}

/**
 * @brief Zwraca redukcję obrażeń wynikającą z terenu dla czołgu.
 *
 * Czołg korzysta z osłony terenowej, ale mniej niż piechota.
 *
 * @param terrain Typ terenu.
 * @return Redukcja obrażeń.
 */
int Tank::getDamageReductionOnTerrain(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Forest:
        return 1;
    case TerrainType::Mountain:
        return 2;
    case TerrainType::Building:
        return 3;
    case TerrainType::Plain:
    case TerrainType::Water:
    default:
        return 0;
    }
}
