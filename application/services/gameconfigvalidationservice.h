#ifndef GAMECONFIGVALIDATIONSERVICE_H
#define GAMECONFIGVALIDATIONSERVICE_H

#include "../../config/gameconfig.h"
#include <QString>

/**
 * @brief Wynik walidacji konfiguracji gry.
 *
 * Struktura przechowuje informację:
 * - czy konfiguracja jest poprawna,
 * - komunikat błędu lub informacyjny.
 */
struct GameConfigValidationResult
{
    bool isValid = false; ///< Informacja, czy konfiguracja jest poprawna.
    QString message;      ///< Komunikat walidacyjny (błąd lub informacja).
};

/**
 * @brief Serwis odpowiedzialny za walidację konfiguracji gry.
 *
 * Klasa sprawdza poprawność parametrów ustawionych przed rozpoczęciem rozgrywki,
 * takich jak liczba jednostek, mapa czy inne ustawienia.
 */
class GameConfigValidationService
{
public:
    /**
     * @brief Waliduje konfigurację gry.
     *
     * @param config Konfiguracja gry do sprawdzenia.
     * @return Wynik walidacji wraz z komunikatem.
     */
    static GameConfigValidationResult validate(const GameConfig& config);
};

#endif // GAMECONFIGVALIDATIONSERVICE_H
