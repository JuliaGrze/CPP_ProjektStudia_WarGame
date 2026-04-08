#include "defaultattackstrategy.h"
#include "../models/unit.h"

#include <algorithm>

/**
 * @brief Oblicza wartość obrażeń zadawanych obrońcy.
 *
 * Metoda wyznacza bazowe obrażenia na podstawie siły ataku jednostki
 * atakującej, a następnie zmniejsza je w zależności od typu terenu,
 * na którym znajduje się obrońca.
 *
 * Teren może zapewniać dodatkową ochronę, zmniejszając skuteczność ataku.
 * Końcowa wartość obrażeń nie może być mniejsza niż 5.
 *
 * @param attacker Jednostka wykonująca atak.
 * @param defender Jednostka będąca celem ataku.
 * @param defenderTerrain Typ terenu zajmowanego przez obrońcę.
 * @return Końcowa wartość obrażeń zadawanych obrońcy.
 */
int DefaultAttackStrategy::calculateDamage(const Unit& attacker,
                                           const Unit& defender,
                                           TerrainType defenderTerrain) const
{
    Q_UNUSED(defender);

    int damage = attacker.getDamage();

    switch (defenderTerrain)
    {
    case TerrainType::Forest:
        damage -= 8;
        break;
    case TerrainType::Mountain:
        damage -= 12;
        break;
    case TerrainType::Building:
        damage -= 15;
        break;
    case TerrainType::Water:
    case TerrainType::Plain:
    default:
        break;
    }

    return std::max(5, damage);
}
