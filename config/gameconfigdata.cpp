#include "gameconfigdata.h"

/**
 * @brief Zwraca listę dostępnych wariantów map.
 *
 * Lista wykorzystywana jest np. w interfejsie użytkownika
 * do wyboru typu mapy przed rozpoczęciem gry.
 *
 * @return Lista nazw wariantów map.
 */
QStringList GameConfigData::availableMapVariants()
{
    return {
        "Równiny",
        "Las i wzgórza",
        "Rzeka i most",
        "Zabudowania"
    };
}

/**
 * @brief Zwraca listę dostępnych rozmiarów planszy.
 *
 * Rozmiary zwracane są w formie tekstowej do wyświetlenia w UI.
 *
 * @return Lista rozmiarów planszy.
 */
QStringList GameConfigData::availableMapSizes()
{
    return {
        "10 x 10",
        "15 x 15",
        "20 x 20"
    };
}

/**
 * @brief Konwertuje tekstowy rozmiar planszy na wartość liczbową.
 *
 * Na podstawie tekstu wybranego w UI zwraca odpowiadający
 * mu rozmiar planszy jako liczbę całkowitą.
 *
 * @param text Tekst reprezentujący rozmiar (np. "10 x 10").
 * @return Rozmiar planszy jako liczba (10, 15 lub 20).
 */
int GameConfigData::mapSizeFromText(const QString& text)
{
    if (text == "10 x 10")
        return 10;
    if (text == "15 x 15")
        return 15;
    return 20;
}
