#ifndef MEDIC_H
#define MEDIC_H

#include "unit.h"

/**
 * @brief Klasa reprezentująca jednostkę medyka.
 *
 * Medyk to jednostka wsparcia, która:
 * - może leczyć sojuszników,
 * - nie jest nastawiona na zadawanie obrażeń,
 * - zwiększa przeżywalność drużyny.
 *
 * Kluczowa do utrzymania jednostek przy życiu podczas dłuższych walk.
 */
class Medic : public Unit
{
public:
    /**
     * @brief Konstruktor jednostki medyka.
     *
     * Inicjalizuje parametry jednostki w zależności od strony konfliktu.
     *
     * @param side Strona, do której należy jednostka.
     */
    explicit Medic(TeamSide side);

    /**
     * @brief Zwraca typ jednostki w formie tekstowej.
     *
     * @return Nazwa typu jednostki (np. "Medyk").
     */
    QString getUnitType() const override;
};

#endif // MEDIC_H
