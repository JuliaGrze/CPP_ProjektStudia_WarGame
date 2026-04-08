#include "medic.h"

/**
 * @brief Konstruktor jednostki medyka.
 *
 * Inicjalizuje parametry medyka:
 * - średnie zdrowie,
 * - bardzo niskie obrażenia (nie jest jednostką ofensywną),
 * - brak zasięgu (działa tylko z bliska),
 * - możliwość leczenia sojuszników,
 * - dobra mobilność,
 * - możliwość ruchu i działania w tej samej turze.
 *
 * @param side Strona, do której należy jednostka.
 */
Medic::Medic(TeamSide side)
    : Unit("Medyk", side, 90, 10, 1, 1, 3, 2, 1, 4, 68, 10, 0, true, 28, true, true)
{
}

/**
 * @brief Zwraca typ jednostki.
 *
 * @return Nazwa typu jednostki ("Medic").
 */
QString Medic::getUnitType() const
{
    return "Medic";
}
