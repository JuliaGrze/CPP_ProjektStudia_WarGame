#include "infantry.h"

/**
 * @brief Konstruktor jednostki piechoty.
 *
 * Inicjalizuje parametry piechoty:
 * - zbalansowane zdrowie,
 * - umiarkowane obrażenia,
 * - krótki zasięg ataku,
 * - dobrą mobilność,
 * - możliwość ruchu i ataku w tej samej turze.
 *
 * @param side Strona, do której należy jednostka.
 */
Infantry::Infantry(TeamSide side)
    : Unit("Piechota", side, 100, 24, 1, 2, 3, 2, 1, 6, 72, 12, 0, false, 0, true, true)
{
}

/**
 * @brief Zwraca typ jednostki.
 *
 * @return Nazwa typu jednostki ("Infantry").
 */
QString Infantry::getUnitType() const
{
    return "Infantry";
}
