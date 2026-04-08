#include "artillery.h"

/**
 * @brief Konstruktor jednostki artylerii.
 *
 * Inicjalizuje parametry artylerii:
 * - umiarkowane HP,
 * - wysokie obrażenia,
 * - duży zasięg (min 2 – nie strzela z bliska),
 * - wysoka celność,
 * - brak możliwości ataku po ruchu.
 *
 * @param side Strona, do której należy jednostka.
 */
Artillery::Artillery(TeamSide side)
    : Unit("Artyleria", side, 80, 52, 2, 5, 2, 4, 2, 3, 76, 5, 4, false, 0, true, false)
{
}

/**
 * @brief Zwraca typ jednostki.
 *
 * @return Nazwa typu jednostki ("Artillery").
 */
QString Artillery::getUnitType() const
{
    return "Artillery";
}
