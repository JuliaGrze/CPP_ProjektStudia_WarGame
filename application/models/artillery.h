#ifndef ARTILLERY_H
#define ARTILLERY_H

#include "unit.h"

/**
 * @brief Klasa reprezentująca jednostkę artylerii.
 *
 * Artyleria to jednostka dystansowa, która:
 * - posiada duży zasięg ataku,
 * - zadaje wysokie obrażenia,
 * - jest mniej skuteczna na krótkim dystansie.
 *
 * Dziedziczy po klasie Unit i implementuje jej zachowanie
 * specyficzne dla tego typu jednostki.
 */
class Artillery : public Unit
{
public:
    /**
     * @brief Konstruktor jednostki artylerii.
     *
     * Inicjalizuje parametry jednostki w zależności od strony konfliktu.
     *
     * @param side Strona, do której należy jednostka.
     */
    explicit Artillery(TeamSide side);

    /**
     * @brief Zwraca typ jednostki w formie tekstowej.
     *
     * @return Nazwa typu jednostki (np. "Artyleria").
     */
    QString getUnitType() const override;
};

#endif // ARTILLERY_H
