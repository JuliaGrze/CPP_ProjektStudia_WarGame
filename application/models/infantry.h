#ifndef INFANTRY_H
#define INFANTRY_H

#include "unit.h"

/**
 * @brief Klasa reprezentująca jednostkę piechoty.
 *
 * Piechota jest lekką i uniwersalną jednostką bojową.
 * Dobrze radzi sobie w terenie zapewniającym osłonę,
 * takim jak las czy budynki, i porusza się w nim sprawniej
 * niż jednostki ciężkie.
 *
 * Klasa nadpisuje wybrane metody bazowe klasy Unit,
 * dzięki czemu jej zachowanie jest dynamicznie wybierane
 * w czasie działania programu.
 */
class Infantry : public Unit
{
public:
    /**
     * @brief Tworzy jednostkę piechoty dla wskazanej strony konfliktu.
     *
     * @param side Strona konfliktu.
     */
    explicit Infantry(TeamSide side);

    /**
     * @brief Zwraca tekstową nazwę typu jednostki.
     *
     * @return Napis "Infantry".
     */
    QString getUnitType() const override;

    /**
     * @brief Zwraca koszt ruchu piechoty po wskazanym terenie.
     *
     * Piechota porusza się sprawnie po lesie i budynkach,
     * dlatego koszt ruchu w takim terenie jest dla niej niższy
     * niż dla jednostek ciężkich.
     *
     * @param terrain Typ terenu.
     * @return Koszt ruchu.
     */
    int getTerrainMoveCost(TerrainType terrain) const override;

    /**
     * @brief Zwraca premię obronną piechoty wynikającą z terenu.
     *
     * Piechota bardzo dobrze korzysta z osłony terenu,
     * szczególnie w lesie i budynkach.
     *
     * @param terrain Typ terenu zajmowanego przez jednostkę.
     * @return Premia obronna.
     */
    int getDefenseBonusOnTerrain(TerrainType terrain) const override;

    /**
     * @brief Zwraca redukcję obrażeń wynikającą z terenu dla piechoty.
     *
     * Piechota potrafi dobrze wykorzystać teren jako osłonę,
     * dlatego zmniejsza otrzymywane obrażenia bardziej niż czołg.
     *
     * @param terrain Typ terenu zajmowanego przez jednostkę.
     * @return Redukcja obrażeń.
     */
    int getDamageReductionOnTerrain(TerrainType terrain) const override;

    /**
     * @brief Zwraca modyfikator trafienia przeciw wskazanemu obrońcy.
     *
     * Piechota jest mniej skuteczna przeciw silnie opancerzonym celom,
     * takim jak czołg.
     *
     * @param defender Jednostka będąca celem ataku.
     * @return Modyfikator trafienia.
     */
    int getAttackAccuracyModifierAgainst(const Unit& defender) const override;

    /**
     * @brief Zwraca modyfikator obrażeń przeciw wskazanemu obrońcy.
     *
     * Piechota zadaje mniejsze obrażenia ciężkim, opancerzonym celom.
     *
     * @param defender Jednostka będąca celem ataku.
     * @return Modyfikator obrażeń.
     */
    int getDamageModifierAgainst(const Unit& defender) const override;
};

#endif // INFANTRY_H
