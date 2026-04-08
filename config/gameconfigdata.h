#ifndef GAMECONFIGDATA_H
#define GAMECONFIGDATA_H

#include <QStringList>

/**
 * @brief Klasa dostarczająca dane konfiguracyjne gry do interfejsu użytkownika.
 *
 * Zawiera listy dostępnych opcji, takich jak:
 * - warianty map,
 * - rozmiary planszy,
 * oraz metody pomocnicze do konwersji danych wejściowych.
 */
class GameConfigData
{
public:
    /**
     * @brief Zwraca listę dostępnych wariantów map.
     *
     * @return Lista nazw wariantów map.
     */
    static QStringList availableMapVariants();

    /**
     * @brief Zwraca listę dostępnych rozmiarów planszy.
     *
     * @return Lista rozmiarów planszy w formie tekstowej.
     */
    static QStringList availableMapSizes();

    /**
     * @brief Konwertuje tekstowy rozmiar planszy na wartość liczbową.
     *
     * @param text Tekst reprezentujący rozmiar planszy (np. "10x10").
     * @return Rozmiar planszy jako liczba całkowita.
     */
    static int mapSizeFromText(const QString& text);
};

#endif // GAMECONFIGDATA_H
