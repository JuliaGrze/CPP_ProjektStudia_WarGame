#include "gameconfigvalidationservice.h"

#include <cmath>

GameConfigValidationResult GameConfigValidationService::validate(const GameConfig& config)
{
    GameConfigValidationResult result;

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

    const int maxUnitsPerTeam = config.mapSize * 3;

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

    const int maxAllowedDifference = 3;

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

    result.isValid = true;
    return result;
}
