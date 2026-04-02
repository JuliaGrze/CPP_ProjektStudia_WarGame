#ifndef GAMECONFIGDATA_H
#define GAMECONFIGDATA_H

#include <QStringList>

class GameConfigData
{
public:
    static QStringList availableMapVariants();
    static QStringList availableMapSizes();
    static int mapSizeFromText(const QString& text);
};

#endif // GAMECONFIGDATA_H
