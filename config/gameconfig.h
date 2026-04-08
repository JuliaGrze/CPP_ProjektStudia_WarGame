#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <QString>
#include "teamComposition.h"

/**
 * @brief Struktura przechowująca konfigurację gry.
 *
 * Zawiera wszystkie ustawienia wymagane do rozpoczęcia rozgrywki,
 * takie jak:
 * - wariant mapy,
 * - rozmiar planszy,
 * - skład drużyn gracza i przeciwnika.
 */
struct GameConfig
{
    QString mapVariant; ///< Nazwa wariantu mapy (np. "Las i wzgórza", "Rzeka i most").
    int mapSize = 20;   ///< Rozmiar planszy (np. 10, 15, 20).

    TeamComposition playerTeam; ///< Skład drużyny gracza.
    TeamComposition enemyTeam;  ///< Skład drużyny przeciwnika.

    /**
     * @brief Sprawdza poprawność konfiguracji.
     *
     * Walidacja obejmuje:
     * - ustawienie wariantu mapy,
     * - dodatni rozmiar planszy,
     * - obecność jednostek w obu drużynach.
     *
     * @return true, jeśli konfiguracja jest poprawna.
     */
    bool isValid() const
    {
        return !mapVariant.isEmpty()
        && mapSize > 0
            && playerTeam.totalUnits() > 0
            && enemyTeam.totalUnits() > 0;
    }
};

#endif // GAMECONFIG_H
