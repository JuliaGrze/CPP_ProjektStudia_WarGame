#include "medic.h"

/**
 * @brief Konstruktor jednostki medyka.
 *
 * Ustawia podstawowe statystyki medyka jako jednostki lekkiej,
 * słabszej ofensywnie, ale posiadającej zdolność leczenia sojuszników.
 *
 * @param side Strona konfliktu.
 */
Medic::Medic(TeamSide side)
    : Unit("Medyk",
           side,
           90,   // health
           10,   // damage
           1,    // minRange
           1,    // maxRange
           3,    // movementPoints
           2,    // attackCost
           1,    // moveCostPerTile
           4,    // armor
           68,   // accuracy
           10,   // evasion
           0,    // armorPiercing
           true, // canHeal
           28,   // healAmount
           true, // canMoveAfterAttack
           true) // canAttackAfterMove
{
}

/**
 * @brief Zwraca nazwę typu jednostki.
 *
 * @return Napis "Medic".
 */
QString Medic::getUnitType() const
{
    return "Medic";
}

/**
 * @brief Zwraca koszt ruchu medyka po wskazanym terenie.
 *
 * Medyk porusza się sprawniej niż ciężkie jednostki,
 * dlatego koszt wejścia w las i budynki pozostaje niski.
 *
 * @param terrain Typ terenu.
 * @return Koszt ruchu.
 */
int Medic::getTerrainMoveCost(TerrainType terrain) const
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
 * @brief Zwraca premię obronną medyka wynikającą z terenu.
 *
 * Medyk korzysta z osłony otoczenia lepiej niż ciężkie jednostki,
 * szczególnie w lesie oraz w budynkach.
 *
 * @param terrain Typ terenu.
 * @return Premia obronna.
 */
int Medic::getDefenseBonusOnTerrain(TerrainType terrain) const
{
    switch (terrain)
    {
    case TerrainType::Forest:
        return 12;
    case TerrainType::Mountain:
        return 8;
    case TerrainType::Building:
        return 14;
    case TerrainType::Plain:
    case TerrainType::Water:
    default:
        return 0;
    }
}

/**
 * @brief Sprawdza, czy medyk może leczyć wskazaną jednostkę.
 *
 * Medyk może leczyć wyłącznie sojuszników, którzy żyją
 * i mają niepełne zdrowie.
 *
 * @param target Jednostka docelowa.
 * @return true, jeśli leczenie jest możliwe.
 */
bool Medic::canHealTarget(const Unit& target) const
{
    if (!canHeal())
        return false;

    if (target.getSide() != getSide())
        return false;

    if (!target.isAlive())
        return false;

    if (!target.isDamaged())
        return false;

    return true;
}
