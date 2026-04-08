#ifndef DEFAULTATTACKSTRATEGY_H
#define DEFAULTATTACKSTRATEGY_H

#include "iattackstrategy.h"

/**
 * @brief Domyślna strategia obliczania obrażeń.
 *
 * Klasa implementuje interfejs IAttackStrategy i odpowiada
 * za standardowy sposób wyznaczania obrażeń zadawanych podczas ataku.
 * Obliczenia uwzględniają parametry jednostki atakującej,
 * jednostki broniącej się oraz wpływ terenu obrońcy.
 */
class DefaultAttackStrategy : public IAttackStrategy
{
public:
    /**
     * @brief Oblicza wartość obrażeń zadawanych obrońcy.
     *
     * Metoda wyznacza końcową liczbę obrażeń na podstawie cech
     * atakującego, obrońcy oraz typu terenu zajmowanego przez obrońcę.
     *
     * @param attacker Jednostka wykonująca atak.
     * @param defender Jednostka będąca celem ataku.
     * @param defenderTerrain Typ terenu zajmowanego przez obrońcę.
     * @return Wartość obliczonych obrażeń.
     */
    int calculateDamage(const Unit& attacker,
                        const Unit& defender,
                        TerrainType defenderTerrain) const override;
};

#endif // DEFAULTATTACKSTRATEGY_H
