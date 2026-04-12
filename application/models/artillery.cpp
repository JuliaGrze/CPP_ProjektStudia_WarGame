#include "artillery.h"
#include "tank.h"

/**
 * @brief Konstruktor jednostki artylerii.
 *
 * Ustawia statystyki artylerii jako jednostki dystansowej:
 * duże obrażenia, większy zasięg oraz słabszą mobilność
 * i obronę bezpośrednią.
 *
 * @param side Strona konfliktu.
 */
Artillery::Artillery(TeamSide side)
    : Unit("Artyleria",
           side,
           80,   // health
           52,   // damage
           2,    // minRange
           5,    // maxRange
           2,    // movementPoints
           4,    // attackCost
           2,    // moveCostPerTile
           3,    // armor
           76,   // accuracy
           5,    // evasion
           4,    // armorPiercing
           false,// canHeal
           0,    // healAmount
           true, // canMoveAfterAttack
           false)// canAttackAfterMove
{
}

/**
 * @brief Zwraca nazwę typu jednostki.
 *
 * @return Napis "Artillery".
 */
QString Artillery::getUnitType() const
{
    return "Artillery";
}

/**
 * @brief Zwraca koszt ruchu artylerii po wskazanym terenie.
 *
 * Artyleria gorzej porusza się po lesie i górach niż piechota,
 * ponieważ jest jednostką cięższą i mniej zwrotną.
 *
 * @param terrain Typ terenu.
 * @return Koszt ruchu.
 */
int Artillery::getTerrainMoveCost(TerrainType terrain) const
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
 * @brief Zwraca premię do zasięgu ataku wynikającą z terenu.
 *
 * Artyleria szczególnie dobrze wykorzystuje pozycje wysoko położone.
 *
 * @param terrain Typ terenu.
 * @return Premia do zasięgu.
 */
int Artillery::getAttackRangeBonus(TerrainType terrain) const
{
    if (terrain == TerrainType::Mountain)
        return 2;

    return 0;
}

/**
 * @brief Zwraca dodatkowy modyfikator trafienia przeciw obrońcy.
 *
 * Artyleria jest skuteczna przeciw dużym i ciężkim celom.
 *
 * @param defender Jednostka będąca celem ataku.
 * @return Modyfikator trafienia.
 */
int Artillery::getAttackAccuracyModifierAgainst(const Unit& defender) const
{
    if (dynamic_cast<const Tank*>(&defender))
        return 3;

    return 1;
}

/**
 * @brief Zwraca dodatkowy modyfikator obrażeń przeciw obrońcy.
 *
 * Artyleria zadaje większe obrażenia ciężkim jednostkom.
 *
 * @param defender Jednostka będąca celem ataku.
 * @return Modyfikator obrażeń.
 */
int Artillery::getDamageModifierAgainst(const Unit& defender) const
{
    if (dynamic_cast<const Tank*>(&defender))
        return 5;

    return 2;
}

/**
 * @brief Zwraca premię obronną artylerii wynikającą z terenu.
 *
 * Artyleria korzysta z osłony umiarkowanie.
 *
 * @param terrain Typ terenu.
 * @return Premia obronna.
 */
int Artillery::getDefenseBonusOnTerrain(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Forest:
        return 8;
    case TerrainType::Mountain:
        return 6;
    case TerrainType::Building:
        return 10;
    case TerrainType::Plain:
    case TerrainType::Water:
    default:
        return 0;
    }
}

/**
 * @brief Zwraca redukcję obrażeń wynikającą z terenu dla artylerii.
 *
 * @param terrain Typ terenu.
 * @return Redukcja obrażeń.
 */
int Artillery::getDamageReductionOnTerrain(TerrainType terrain) const
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
