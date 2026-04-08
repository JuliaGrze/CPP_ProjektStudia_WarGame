#include "gameconfigvalidationservice.h"

#include <cmath>

/**
 * @brief Waliduje konfigurację gry.
 *
 * Sprawdza poprawność ustawień przed rozpoczęciem rozgrywki, w tym:
 * - poprawność ogólną konfiguracji,
 * - liczbę jednostek w drużynach względem rozmiaru mapy,
 * - różnicę liczby jednostek między drużynami.
 *
 * @param config Konfiguracja gry do sprawdzenia.
 * @return Wynik walidacji wraz z komunikatem błędu (jeśli wystąpi).
 */
GameConfigValidationResult GameConfigValidationService::validate(const GameConfig& config)
{
    GameConfigValidationResult result;

    /**
     * @brief Sprawdzenie podstawowej poprawności konfiguracji.
     */
    if (!config.isValid())
    {
        result.isValid = false;
        result.message =
            "Konfiguracja gry jest niepoprawna.\n\n"
            "Sprawdź, czy obie drużyny mają jednostki "
            "oraz czy mapa i rozmiar planszy są poprawnie ustawione.";
        return result;
    }

    const int playerUnits = config.playerTeam.totalUnits();
    const int enemyUnits = config.enemyTeam.totalUnits();

    /**
     * @brief Maksymalna liczba jednostek zależna od rozmiaru mapy.
     */
    const int maxUnitsPerTeam = config.mapSize * 3;

    /**
     * @brief Sprawdzenie limitu jednostek na drużynę.
     */
    if (playerUnits > maxUnitsPerTeam || enemyUnits > maxUnitsPerTeam)
    {
        result.isValid = false;
        result.message = QString(
                             "Na wybranej planszy jedna drużyna ma zbyt dużo jednostek.\n\n"
                             "Maksymalna liczba jednostek na drużynę dla tej planszy: %1\n"
                             "Twoja drużyna: %2\n"
                             "Drużyna przeciwna: %3")
                             .arg(maxUnitsPerTeam)
                             .arg(playerUnits)
                             .arg(enemyUnits);
        return result;
    }

    /**
     * @brief Maksymalna dopuszczalna różnica jednostek między drużynami.
     */
    const int maxAllowedDifference = 3;

    /**
     * @brief Sprawdzenie balansu drużyn.
     */
    if (std::abs(playerUnits - enemyUnits) > maxAllowedDifference)
    {
        result.isValid = false;
        result.message = QString(
                             "Różnica w liczbie jednostek między drużynami jest zbyt duża.\n\n"
                             "Maksymalna dozwolona różnica: %1\n"
                             "Twoja drużyna: %2\n"
                             "Drużyna przeciwna: %3")
                             .arg(maxAllowedDifference)
                             .arg(playerUnits)
                             .arg(enemyUnits);
        return result;
    }

    /**
     * @brief Konfiguracja poprawna.
     */
    result.isValid = true;
    return result;
}
