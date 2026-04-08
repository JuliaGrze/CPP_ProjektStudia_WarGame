#ifndef IATTACKSTRATEGY_H
#define IATTACKSTRATEGY_H

#include "../models/enums/terraintype.h"

class Unit;

/**
 * @brief Interfejs strategii obliczania obrażeń.
 *
 * Klasa abstrakcyjna definiująca sposób wyznaczania obrażeń
 * zadawanych podczas ataku. Umożliwia stosowanie różnych
 * algorytmów obliczeń poprzez implementacje strategii.
 *
 * Zastosowanie wzorca Strategy pozwala na łatwe rozszerzanie
 * systemu walki bez modyfikowania istniejącego kodu.
 */
class IAttackStrategy
{
public:
    /**
     * @brief Wirtualny destruktor interfejsu.
     */
    virtual ~IAttackStrategy() = default;

    /**
     * @brief Oblicza obrażenia zadawane obrońcy.
     *
     * Metoda powinna zostać zaimplementowana przez konkretną strategię
     * i uwzględniać parametry jednostek oraz wpływ terenu.
     *
     * @param attacker Jednostka wykonująca atak.
     * @param defender Jednostka będąca celem ataku.
     * @param defenderTerrain Typ terenu zajmowanego przez obrońcę.
     * @return Wartość obliczonych obrażeń.
     */
    virtual int calculateDamage(const Unit& attacker,
                                const Unit& defender,
                                TerrainType defenderTerrain) const = 0;
};

#endif // IATTACKSTRATEGY_H
