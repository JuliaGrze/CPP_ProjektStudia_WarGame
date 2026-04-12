#ifndef TANK_H
#define TANK_H

#include "unit.h"

/**
 * @brief Klasa reprezentująca jednostkę czołgu.
 *
 * Czołg jest jednostką ciężką, dobrze opancerzoną i silną ofensywnie,
 * ale gorzej radzi sobie w trudnym terenie, takim jak las czy góry.
 *
 * Klasa nadpisuje wybrane metody bazowe klasy Unit, dzięki czemu
 * jej zachowanie jest dynamicznie wybierane w czasie działania programu.
 * Pozwala to uzyskać sensowny polimorfizm związany z ruchem,
 * obroną i walką przeciw różnym typom przeciwników.
 */
class Tank : public Unit
{
public:
    /**
     * @brief Tworzy jednostkę czołgu dla wskazanej strony konfliktu.
     *
     * @param side Strona konfliktu.
     */
    explicit Tank(TeamSide side);

    /**
     * @brief Zwraca tekstową nazwę typu jednostki.
     *
     * @return Napis "Tank".
     */
    QString getUnitType() const override;

    /**
     * @brief Zwraca koszt ruchu czołgu po wskazanym terenie.
     *
     * Czołg jako jednostka ciężka gorzej porusza się po trudnym terenie,
     * szczególnie po lesie i górach.
     *
     * @param terrain Typ terenu.
     * @return Koszt ruchu po danym polu.
     */
    int getTerrainMoveCost(TerrainType terrain) const override;

    /**
     * @brief Zwraca premię do trafienia przeciw konkretnemu przeciwnikowi.
     *
     * Czołg może być skuteczniejszy przeciw lekkim jednostkom,
     * które trudniej wytrzymują ostrzał ciężkiego uzbrojenia.
     *
     * @param defender Jednostka będąca celem ataku.
     * @return Modyfikator trafienia.
     */
    int getAttackAccuracyModifierAgainst(const Unit& defender) const override;

    /**
     * @brief Zwraca dodatkowy modyfikator obrażeń przeciw przeciwnikowi.
     *
     * Czołg może zadawać większe obrażenia lekkim celom.
     *
     * @param defender Jednostka będąca celem ataku.
     * @return Modyfikator obrażeń.
     */
    int getDamageModifierAgainst(const Unit& defender) const override;

    /**
     * @brief Zwraca premię obronną czołgu wynikającą z terenu.
     *
     * Czołg korzysta z osłony terenu słabiej niż lekkie jednostki,
     * ponieważ jego rozmiar i charakterystyka ograniczają skuteczność ukrycia.
     *
     * @param terrain Typ terenu zajmowanego przez czołg.
     * @return Premia obronna.
     */
    int getDefenseBonusOnTerrain(TerrainType terrain) const override;

    /**
     * @brief Zwraca redukcję obrażeń wynikającą z terenu dla czołgu.
     *
     * Czołg nadal korzysta z osłony terenu, ale inaczej niż lekka piechota.
     *
     * @param terrain Typ terenu zajmowanego przez czołg.
     * @return Redukcja obrażeń.
     */
    int getDamageReductionOnTerrain(TerrainType terrain) const override;
};

#endif // TANK_H
