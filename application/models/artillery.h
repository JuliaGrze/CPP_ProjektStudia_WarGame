#ifndef ARTILLERY_H
#define ARTILLERY_H

#include "unit.h"

/**
 * @brief Klasa reprezentująca jednostkę artylerii.
 *
 * Artyleria jest jednostką dystansową, która specjalizuje się
 * w ostrzeliwaniu celów z większej odległości. Lepiej wykorzystuje
 * korzystne pozycje terenowe do zwiększenia zasięgu ataku,
 * ale gorzej radzi sobie z poruszaniem po trudnym terenie.
 *
 * Klasa nadpisuje metody bazowe klasy Unit, aby jej zachowanie
 * było dynamicznie wybierane podczas działania gry.
 */
class Artillery : public Unit
{
public:
    /**
     * @brief Tworzy jednostkę artylerii dla wskazanej strony konfliktu.
     *
     * @param side Strona konfliktu.
     */
    explicit Artillery(TeamSide side);

    /**
     * @brief Zwraca tekstową nazwę typu jednostki.
     *
     * @return Napis "Artillery".
     */
    QString getUnitType() const override;

    /**
     * @brief Zwraca koszt ruchu artylerii po wskazanym terenie.
     *
     * Artyleria jako ciężka jednostka dystansowa gorzej porusza się
     * po lesie i górach niż piechota.
     *
     * @param terrain Typ terenu.
     * @return Koszt ruchu.
     */
    int getTerrainMoveCost(TerrainType terrain) const override;

    /**
     * @brief Zwraca premię do zasięgu ataku wynikającą z terenu.
     *
     * Artyleria szczególnie dobrze wykorzystuje pozycje wysoko położone,
     * dlatego na górze może uzyskać większą premię do zasięgu
     * niż inne jednostki.
     *
     * @param terrain Typ terenu zajmowanego przez artylerię.
     * @return Premia do maksymalnego zasięgu ataku.
     */
    int getAttackRangeBonus(TerrainType terrain) const override;

    /**
     * @brief Zwraca dodatkowy modyfikator trafienia przeciw obrońcy.
     *
     * Artyleria może być bardziej skuteczna przeciw cięższym celom,
     * które trudniej ukryć przed ostrzałem pośrednim.
     *
     * @param defender Jednostka będąca celem ataku.
     * @return Modyfikator trafienia.
     */
    int getAttackAccuracyModifierAgainst(const Unit& defender) const override;

    /**
     * @brief Zwraca dodatkowy modyfikator obrażeń przeciw obrońcy.
     *
     * Artyleria może zadawać większe obrażenia ciężkim celom,
     * takim jak czołgi.
     *
     * @param defender Jednostka będąca celem ataku.
     * @return Modyfikator obrażeń.
     */
    int getDamageModifierAgainst(const Unit& defender) const override;

    /**
     * @brief Zwraca premię obronną artylerii wynikającą z terenu.
     *
     * Artyleria korzysta z osłony terenu umiarkowanie.
     *
     * @param terrain Typ terenu zajmowanego przez jednostkę.
     * @return Premia obronna.
     */
    int getDefenseBonusOnTerrain(TerrainType terrain) const override;

    /**
     * @brief Zwraca redukcję obrażeń wynikającą z terenu dla artylerii.
     *
     * @param terrain Typ terenu zajmowanego przez jednostkę.
     * @return Redukcja obrażeń.
     */
    int getDamageReductionOnTerrain(TerrainType terrain) const override;
};

#endif // ARTILLERY_H
