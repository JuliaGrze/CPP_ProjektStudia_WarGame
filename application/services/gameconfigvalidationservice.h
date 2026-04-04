#ifndef GAMECONFIGVALIDATIONSERVICE_H
#define GAMECONFIGVALIDATIONSERVICE_H

#include "../../config/gameconfig.h"
#include <QString>

struct GameConfigValidationResult
{
    bool isValid = false;
    QString message;
};

class GameConfigValidationService
{
public:
    static GameConfigValidationResult validate(const GameConfig& config);
};

#endif // GAMECONFIGVALIDATIONSERVICE_H
