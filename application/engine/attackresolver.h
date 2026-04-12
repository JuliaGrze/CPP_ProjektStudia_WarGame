#ifndef ATTACKRESOLVER_H
#define ATTACKRESOLVER_H

#include <QString>
#include "../models/enums/terraintype.h"

class Unit;

/**
 * @brief Zawiera rezultat wykonania ataku.
 *
 * Struktura przechowuje komplet informacji o przebiegu ataku,
 * w tym dane o trafieniu, zadanych obrażeniach, wpływie pancerza
 * i terenu oraz końcowym stanie zdrowia celu.
 */
struct AttackResult
{
    bool attackPerformed = false;   ///< Informuje, czy atak został wykonany.
    bool targetDestroyed = false;   ///< Informuje, czy cel został zniszczony.
    bool hit = false;               ///< Informuje, czy atak trafił cel.

    int distance = 0;               ///< Odległość pomiędzy atakującym a celem.
    int hitChance = 0;              ///< Obliczona szansa trafienia w procentach.
    int roll = 0;                   ///< Wylosowana wartość użyta do sprawdzenia trafienia.

    int baseDamage = 0;             ///< Bazowa wartość obrażeń po uwzględnieniu modyfikatora jednostki.
    int randomDamageBonus = 0;      ///< Dodatkowa losowa premia do obrażeń.
    int defenderArmorUsed = 0;      ///< Wartość pancerza wykorzystana przy obronie.
    int terrainReduction = 0;       ///< Redukcja obrażeń wynikająca z terenu.
    int damageDealt = 0;            ///< Rzeczywiście zadane obrażenia.

    int targetHealthAfter = 0;      ///< Liczba punktów zdrowia celu po ataku.
    int targetMaxHealth = 0;        ///< Maksymalna liczba punktów zdrowia celu.

    QString message;                ///< Komunikat opisujący wynik ataku.
};

/**
 * @brief Odpowiada za sprawdzanie i rozstrzyganie ataków.
 *
 * Klasa zawiera logikę związaną z:
 * - sprawdzaniem, czy atak jest możliwy,
 * - obliczaniem dystansu,
 * - wyznaczaniem szansy trafienia,
 * - obliczaniem końcowych obrażeń.
 *
 * Klasa korzysta z dynamicznego polimorfizmu klasy Unit,
 * dzięki czemu różne typy jednostek mogą wpływać na:
 * - zasięg ataku,
 * - celność przeciw konkretnym celom,
 * - modyfikator obrażeń,
 * - premię obronną i redukcję obrażeń wynikającą z terenu.
 */
class AttackResolver
{
public:
    /**
     * @brief Tworzy obiekt odpowiedzialny za rozstrzyganie ataków.
     */
    explicit AttackResolver();

    /**
     * @brief Sprawdza, czy atak może zostać wykonany.
     *
     * Metoda analizuje parametry atakującego i obrońcy,
     * ich pozycje na planszy oraz teren zajmowany przez atakującego.
     *
     * Maksymalny zasięg ataku może zostać dynamicznie zmodyfikowany
     * przez konkretną klasę jednostki atakującej.
     *
     * @param attacker Jednostka wykonująca atak.
     * @param attackerX Współrzędna X atakującego.
     * @param attackerY Współrzędna Y atakującego.
     * @param defender Jednostka będąca celem ataku.
     * @param defenderX Współrzędna X obrońcy.
     * @param defenderY Współrzędna Y obrońcy.
     * @param attackerTerrain Typ terenu zajmowanego przez atakującego.
     * @return true, jeśli atak może zostać wykonany, w przeciwnym razie false.
     */
    bool canAttack(const Unit& attacker,
                   int attackerX,
                   int attackerY,
                   const Unit& defender,
                   int defenderX,
                   int defenderY,
                   TerrainType attackerTerrain = TerrainType::Plain) const;

    /**
     * @brief Rozstrzyga atak i zwraca jego szczegółowy wynik.
     *
     * Metoda oblicza dystans, szansę trafienia, wynik losowania,
     * końcowe obrażenia oraz aktualizuje stan obrońcy.
     *
     * W obliczeniach wykorzystywane są metody wirtualne klasy Unit,
     * dzięki czemu zachowanie zależy od rzeczywistego typu jednostki.
     *
     * @param attacker Jednostka wykonująca atak.
     * @param attackerX Współrzędna X atakującego.
     * @param attackerY Współrzędna Y atakującego.
     * @param attackerTerrain Typ terenu zajmowanego przez atakującego.
     * @param defender Jednostka będąca celem ataku.
     * @param defenderX Współrzędna X obrońcy.
     * @param defenderY Współrzędna Y obrońcy.
     * @param defenderTerrain Typ terenu zajmowanego przez obrońcę.
     * @return Struktura zawierająca szczegółowy wynik ataku.
     */
    AttackResult resolveAttack(Unit& attacker,
                               int attackerX,
                               int attackerY,
                               TerrainType attackerTerrain,
                               Unit& defender,
                               int defenderX,
                               int defenderY,
                               TerrainType defenderTerrain) const;

private:
    /**
     * @brief Oblicza odległość pomiędzy dwoma polami planszy.
     *
     * @param x1 Współrzędna X pierwszego pola.
     * @param y1 Współrzędna Y pierwszego pola.
     * @param x2 Współrzędna X drugiego pola.
     * @param y2 Współrzędna Y drugiego pola.
     * @return Odległość pomiędzy wskazanymi polami.
     */
    int calculateDistance(int x1, int y1, int x2, int y2) const;

    /**
     * @brief Oblicza szansę trafienia celu.
     *
     * Uwzględnia parametry jednostek, odległość, wpływ terenu
     * oraz polimorficzne modyfikatory zależne od typu atakującego
     * i obrońcy.
     *
     * @param attacker Jednostka atakująca.
     * @param defender Jednostka broniąca się.
     * @param distance Odległość pomiędzy jednostkami.
     * @param attackerTerrain Typ terenu zajmowanego przez atakującego.
     * @param defenderTerrain Typ terenu zajmowanego przez obrońcę.
     * @return Szansa trafienia wyrażona w procentach.
     */
    int calculateHitChance(const Unit& attacker,
                           const Unit& defender,
                           int distance,
                           TerrainType attackerTerrain,
                           TerrainType defenderTerrain) const;
};

#endif // ATTACKRESOLVER_H
