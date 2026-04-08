#ifndef TEAMSIDE_H
#define TEAMSIDE_H

/**
 * @brief Określa stronę konfliktu w grze.
 *
 * Wyliczenie rozróżnia dwie drużyny biorące udział w rozgrywce:
 * gracza oraz przeciwnika.
 */
enum class TeamSide
{
    Player, /**< Drużyna gracza (np. Niebiescy). */
    Enemy   /**< Drużyna przeciwnika (np. Czerwoni). */
};

#endif // TEAMSIDE_H
