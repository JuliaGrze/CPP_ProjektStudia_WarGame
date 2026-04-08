#include "tank.h"

/**
 * @brief Konstruktor jednostki czołgu.
 *
 * Inicjalizuje parametry czołgu:
 * - bardzo wysokie zdrowie,
 * - wysokie obrażenia,
 * - średni zasięg,
 * - wysoki pancerz,
 * - umiarkowaną mobilność,
 * - możliwość ruchu i ataku w tej samej turze.
 *
 * @param side Strona, do której należy jednostka.
 */
Tank::Tank(TeamSide side)
    : Unit("Czolg", side, 180, 42, 1, 3, 2, 3, 2, 18, 78, 6, 8, false, 0, true, true)
{
}

/**
 * @brief Zwraca typ jednostki.
 *
 * @return Nazwa typu jednostki ("Tank").
 */
QString Tank::getUnitType() const
{
    return "Tank";
}
