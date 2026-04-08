#ifndef TEAMCOMPOSITION_H
#define TEAMCOMPOSITION_H

#include <QString>

/**
 * @brief Struktura przechowująca skład drużyny.
 *
 * Określa liczbę jednostek każdego typu w drużynie:
 * - piechota,
 * - czołgi,
 * - artyleria,
 * - medycy.
 */
struct TeamComposition
{
    int infantry = 0;   ///< Liczba jednostek piechoty.
    int tank = 0;       ///< Liczba czołgów.
    int artillery = 0;  ///< Liczba jednostek artylerii.
    int medic = 0;      ///< Liczba medyków.

    /**
     * @brief Zwraca łączną liczbę jednostek w drużynie.
     *
     * @return Suma wszystkich jednostek.
     */
    int totalUnits() const
    {
        return infantry + tank + artillery + medic;
    }

    /**
     * @brief Tworzy tekstową reprezentację składu drużyny.
     *
     * Przydatne do wyświetlania w UI.
     *
     * @return Sformatowany tekst opisujący skład drużyny.
     */
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
