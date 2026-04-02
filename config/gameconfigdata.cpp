#include "gameconfigdata.h"

QStringList GameConfigData::availableMapVariants()
{
    return {
        "Równiny",
        "Las i wzgórza",
        "Rzeka i most",
        "Zabudowania"
    };
}

QStringList GameConfigData::availableMapSizes()
{
    return {
        "10 x 10",
        "15 x 15",
        "20 x 20"
    };
}

int GameConfigData::mapSizeFromText(const QString& text)
{
    if (text == "10 x 10")
        return 10;
    if (text == "15 x 15")
        return 15;
    return 20;
}
