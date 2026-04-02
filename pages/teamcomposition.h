#ifndef TEAMCOMPOSITION_H
#define TEAMCOMPOSITION_H

#include <QString>

struct TeamComposition
{
    int infantry = 0;
    int tank = 0;
    int artillery = 0;
    int medic = 0;

    int totalUnits() const
    {
        return infantry + tank + artillery + medic;
    }

    QString toDisplayString() const
    {
        return QString("Piechota: %1\nCzołg: %2\nArtyleria: %3\nMedyk: %4\nŁącznie: %5")
            .arg(infantry)
            .arg(tank)
            .arg(artillery)
            .arg(medic)
            .arg(totalUnits());
    }
};

#endif // TEAMCOMPOSITION_H
