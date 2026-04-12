#ifndef MEDIC_H
#define MEDIC_H

#include "unit.h"

/**
 * @brief Klasa reprezentująca jednostkę medyka.
 *
 * Medyk jest jednostką wsparcia, której główną rolą jest leczenie
 * rannych sojuszników. Dziedziczy po klasie Unit i nadpisuje
 * zachowania związane z leczeniem oraz poruszaniem się po terenie.
 *
 * Dzięki temu logika gry może operować na wskaźniku do klasy bazowej
 * Unit, a konkretne zachowanie medyka zostanie wybrane dynamicznie
 * w czasie działania programu.
 */
class Medic : public Unit
{
public:
    /**
     * @brief Tworzy jednostkę medyka dla wskazanej strony konfliktu.
     *
     * @param side Strona konfliktu, do której należy jednostka.
     */
    explicit Medic(TeamSide side);

    /**
     * @brief Zwraca tekstową nazwę typu jednostki.
     *
     * @return Napis "Medic".
     */
    QString getUnitType() const override;

    /**
     * @brief Zwraca koszt ruchu medyka po wskazanym terenie.
     *
     * Medyk porusza się podobnie do lekkiej piechoty,
     * dlatego lepiej radzi sobie w lesie i budynkach niż cięższe jednostki.
     *
     * @param terrain Typ terenu.
     * @return Koszt ruchu po danym polu.
     */
    int getTerrainMoveCost(TerrainType terrain) const override;

    /**
     * @brief Zwraca premię obronną medyka wynikającą z terenu.
     *
     * Medyk korzysta z osłony terenu, szczególnie w lesie i budynkach,
     * ponieważ jest jednostką lekką i wsparciową.
     *
     * @param terrain Typ terenu zajmowanego przez medyka.
     * @return Premia obronna.
     */
    int getDefenseBonusOnTerrain(TerrainType terrain) const override;

    /**
     * @brief Sprawdza, czy medyk może leczyć wskazaną jednostkę.
     *
     * Leczenie jest możliwe tylko wtedy, gdy:
     * - cel należy do tej samej strony,
     * - cel żyje,
     * - cel jest ranny.
     *
     * @param target Jednostka docelowa.
     * @return true, jeśli leczenie jest możliwe.
     */
    bool canHealTarget(const Unit& target) const override;
};

#endif // MEDIC_H
