#ifndef MAPTYPE_H
#define MAPTYPE_H

/**
 * @brief Typ mapy używanej w grze.
 *
 * Wyliczenie określa dostępne warianty plansz,
 * które różnią się układem terenu i charakterystyką rozgrywki.
 */
enum class MapType
{
    Plains,        /**< Mapa równinna – otwarty teren bez przeszkód. */
    ForestHills,   /**< Mapa leśno-wzgórzowa – zapewnia osłonę i utrudnia ruch. */
    RiverBridge,   /**< Mapa z rzeką i mostem – ograniczone przejścia. */
    Urban          /**< Mapa miejska – dużo budynków i osłon. */
};

#endif // MAPTYPE_H
