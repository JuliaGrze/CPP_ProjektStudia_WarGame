#ifndef TANK_H
#define TANK_H

#include "unit.h"

/**
 * @brief Klasa reprezentująca jednostkę czołgu.
 *
 * Czołg to ciężka jednostka bojowa, która:
 * - posiada wysoką wartość pancerza,
 * - zadaje duże obrażenia,
 * - jest mniej mobilna niż lżejsze jednostki.
 *
 * Sprawdza się w bezpośrednich starciach i przełamywaniu linii przeciwnika.
 */
class Tank : public Unit
{
public:
    /**
     * @brief Konstruktor jednostki czołgu.
     *
     * Inicjalizuje parametry jednostki w zależności od strony konfliktu.
     *
     * @param side Strona, do której należy jednostka.
     */
    explicit Tank(TeamSide side);

    /**
     * @brief Zwraca typ jednostki w formie tekstowej.
     *
     * @return Nazwa typu jednostki (np. "Czołg").
     */
    QString getUnitType() const override;
};

#endif // TANK_H
