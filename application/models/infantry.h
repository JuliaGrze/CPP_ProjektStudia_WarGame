#ifndef INFANTRY_H
#define INFANTRY_H

#include "unit.h"

/**
 * @brief Klasa reprezentująca jednostkę piechoty.
 *
 * Piechota to podstawowa jednostka bojowa, charakteryzująca się:
 * - zbalansowanymi statystykami,
 * - umiarkowanym zasięgiem i mobilnością,
 * - możliwością działania w różnych warunkach terenowych.
 *
 * Jest uniwersalną jednostką, nadającą się zarówno do ataku,
 * jak i obrony.
 */
class Infantry : public Unit
{
public:
    /**
     * @brief Konstruktor jednostki piechoty.
     *
     * Inicjalizuje parametry jednostki w zależności od strony konfliktu.
     *
     * @param side Strona, do której należy jednostka.
     */
    explicit Infantry(TeamSide side);

    /**
     * @brief Zwraca typ jednostki w formie tekstowej.
     *
     * @return Nazwa typu jednostki (np. "Piechota").
     */
    QString getUnitType() const override;
};

#endif // INFANTRY_H
