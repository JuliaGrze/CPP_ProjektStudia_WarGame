#ifndef UNIT_H
#define UNIT_H

#include <QString>
#include "enums/teamside.h"
#include "enums/terraintype.h"

/**
 * @brief Abstrakcyjna klasa bazowa reprezentująca jednostkę w grze.
 *
 * Klasa przechowuje wszystkie podstawowe statystyki jednostki oraz
 * logikę związaną z jej działaniami w turze, takimi jak ruch, atak
 * i leczenie.
 *
 * Dodatkowo klasa udostępnia zestaw metod wirtualnych opisujących
 * zachowanie jednostki w zależności od sytuacji na planszy.
 * Dzięki temu klasy pochodne mogą dynamicznie zmieniać sposób:
 * - poruszania się po terenie,
 * - atakowania przeciwnika,
 * - bronienia się,
 * - leczenia sojuszników.
 *
 * Jest klasą bazową dla konkretnych typów jednostek
 * (np. Infantry, Tank, Artillery, Medic).
 */
class Unit
{
public:
    /**
     * @brief Konstruktor jednostki.
     *
     * Inicjalizuje wszystkie parametry bojowe oraz właściwości jednostki.
     *
     * @param name Nazwa jednostki.
     * @param side Strona konfliktu.
     * @param health Początkowe zdrowie.
     * @param damage Siła ataku.
     * @param minRange Minimalny zasięg ataku.
     * @param maxRange Maksymalny zasięg ataku.
     * @param movementPoints Maksymalna liczba pól ruchu.
     * @param attackCost Koszt ataku w punktach akcji.
     * @param moveCostPerTile Bazowy koszt ruchu za jedno pole.
     * @param armor Wartość pancerza.
     * @param accuracy Celność jednostki.
     * @param evasion Unik jednostki.
     * @param armorPiercing Przebicie pancerza.
     * @param canHeal Czy jednostka może leczyć.
     * @param healAmount Ilość leczenia.
     * @param canMoveAfterAttack Czy może się ruszyć po ataku.
     * @param canAttackAfterMove Czy może zaatakować po ruchu.
     */
    Unit(const QString& name,
         TeamSide side,
         int health,
         int damage,
         int minRange,
         int maxRange,
         int movementPoints,
         int attackCost,
         int moveCostPerTile,
         int armor,
         int accuracy,
         int evasion,
         int armorPiercing = 0,
         bool canHeal = false,
         int healAmount = 0,
         bool canMoveAfterAttack = true,
         bool canAttackAfterMove = true);

    /**
     * @brief Wirtualny destruktor klasy bazowej.
     */
    virtual ~Unit() = default;

    /** @name Podstawowe informacje */
    ///@{
    QString getName() const;
    TeamSide getSide() const;
    ///@}

    /** @name Statystyki */
    ///@{
    int getHealth() const;
    int getMaxHealth() const;
    int getDamage() const;
    int getMinRange() const;
    int getRange() const;
    int getMovementPoints() const;
    int getAttackCost() const;
    int getMoveCostPerTile() const;
    int getArmor() const;
    int getAccuracy() const;
    int getEvasion() const;
    int getArmorPiercing() const;
    int getHealAmount() const;
    ///@}

    /** @name Stan jednostki */
    ///@{
    bool canHeal() const;
    bool isAlive() const;
    bool isDamaged() const;
    ///@}

    /**
     * @brief Zadaje obrażenia jednostce.
     *
     * @param amount Ilość obrażeń.
     */
    void takeDamage(int amount);

    /**
     * @brief Leczy jednostkę.
     *
     * @param amount Ilość punktów zdrowia do przywrócenia.
     */
    void heal(int amount);

    /** @name Zarządzanie turą */
    ///@{
    void markMoved();
    void markActed();
    bool hasMovedThisTurn() const;
    bool hasActedThisTurn() const;

    bool canMoveNow() const;
    bool canAttackNow() const;
    bool canHealNow() const;

    void resetTurnResources();
    ///@}

    /** @name Polimorficzne zachowanie jednostki */
    ///@{

    /**
     * @brief Sprawdza, czy jednostka może wejść na pole danego typu.
     *
     * Domyślna implementacja blokuje wejście na wodę.
     * Klasy pochodne mogą nadpisać tę metodę, jeśli poruszają się
     * inaczej po mapie.
     *
     * @param terrain Typ terenu.
     * @return true, jeśli wejście na pole jest dozwolone.
     */
    virtual bool canEnterTerrain(TerrainType terrain) const;

    /**
     * @brief Zwraca koszt wejścia jednostki na pole danego typu.
     *
     * Domyślna implementacja zwraca koszt zależny od rodzaju terenu.
     * Klasy pochodne mogą nadpisać tę metodę, aby np. piechota
     * lepiej poruszała się po lesie, a czołg gorzej po górach.
     *
     * @param terrain Typ terenu.
     * @return Koszt ruchu.
     */
    virtual int getTerrainMoveCost(TerrainType terrain) const;

    /**
     * @brief Zwraca premię do zasięgu ataku wynikającą z terenu.
     *
     * Metoda pozwala jednostce dynamicznie reagować na typ pola,
     * na którym stoi. Domyślnie góry zwiększają zasięg.
     *
     * @param terrain Typ terenu zajmowanego przez atakującego.
     * @return Premia do maksymalnego zasięgu ataku.
     */
    virtual int getAttackRangeBonus(TerrainType terrain) const;

    /**
     * @brief Zwraca dodatkowy modyfikator trafienia przeciw konkretnemu celowi.
     *
     * Metoda może zostać nadpisana przez klasy pochodne, jeśli dana
     * jednostka lepiej lub gorzej radzi sobie przeciw określonym typom
     * przeciwników.
     *
     * @param defender Jednostka będąca celem ataku.
     * @return Modyfikator trafienia.
     */
    virtual int getAttackAccuracyModifierAgainst(const Unit& defender) const;

    /**
     * @brief Zwraca dodatkowy modyfikator obrażeń przeciw konkretnemu celowi.
     *
     * Metoda pozwala wprowadzić różnice między jednostkami, np. większą
     * skuteczność czołgu przeciw lekkim celom albo artylerii przeciw
     * celom opancerzonym.
     *
     * @param defender Jednostka będąca celem ataku.
     * @return Modyfikator obrażeń.
     */
    virtual int getDamageModifierAgainst(const Unit& defender) const;

    /**
     * @brief Zwraca premię obronną wynikającą z typu terenu.
     *
     * Domyślna implementacja uwzględnia osłonę zapewnianą przez las,
     * góry oraz budynki.
     *
     * @param terrain Typ terenu zajmowanego przez jednostkę.
     * @return Premia obronna wpływająca na trafienie przeciwnika.
     */
    virtual int getDefenseBonusOnTerrain(TerrainType terrain) const;

    /**
     * @brief Zwraca redukcję obrażeń wynikającą z typu terenu.
     *
     * Domyślna implementacja zmniejsza obrażenia otrzymywane na polach
     * zapewniających osłonę.
     *
     * @param terrain Typ terenu zajmowanego przez jednostkę.
     * @return Redukcja obrażeń.
     */
    virtual int getDamageReductionOnTerrain(TerrainType terrain) const;

    /**
     * @brief Sprawdza, czy jednostka może leczyć wskazany cel.
     *
     * Domyślna implementacja pozwala leczyć tylko wtedy, gdy:
     * - jednostka posiada zdolność leczenia,
     * - cel jest sojusznikiem,
     * - cel żyje,
     * - cel jest ranny.
     *
     * Klasy pochodne mogą nadpisywać tę metodę, jeśli ich zasady leczenia
     * są bardziej specyficzne.
     *
     * @param target Jednostka docelowa.
     * @return true, jeśli leczenie jest możliwe.
     */
    virtual bool canHealTarget(const Unit& target) const;

    ///@}

    /**
     * @brief Zwraca typ jednostki w formie tekstowej.
     *
     * Metoda czysto wirtualna – musi być zaimplementowana
     * w klasach pochodnych.
     *
     * @return Nazwa typu jednostki.
     */
    virtual QString getUnitType() const = 0;

protected:
    QString m_name;              ///< Nazwa jednostki.
    TeamSide m_side;             ///< Strona konfliktu.

    int m_health;                ///< Aktualne zdrowie.
    int m_maxHealth;             ///< Maksymalne zdrowie.
    int m_damage;                ///< Siła ataku.
    int m_minRange;              ///< Minimalny zasięg.
    int m_maxRange;              ///< Maksymalny zasięg.

    int m_movementPoints;        ///< Maksymalny zasięg ruchu.
    int m_attackCost;            ///< Koszt ataku w AP.
    int m_moveCostPerTile;       ///< Bazowy koszt ruchu na pole.

    int m_armor;                 ///< Pancerz.
    int m_accuracy;              ///< Celność.
    int m_evasion;               ///< Unik.
    int m_armorPiercing;         ///< Przebicie pancerza.

    bool m_canHeal;              ///< Czy jednostka może leczyć.
    int m_healAmount;            ///< Ilość leczenia.

    bool m_canMoveAfterAttack;   ///< Czy może ruszyć się po ataku.
    bool m_canAttackAfterMove;   ///< Czy może zaatakować po ruchu.

    bool m_hasMovedThisTurn = false; ///< Czy wykonała ruch w turze.
    bool m_hasActedThisTurn = false; ///< Czy wykonała akcję w turze.
};

#endif // UNIT_H
