#ifndef UNIT_H
#define UNIT_H

#include <QString>
#include "enums/teamside.h"

/**
 * @brief Abstrakcyjna klasa bazowa reprezentująca jednostkę w grze.
 *
 * Klasa przechowuje wszystkie podstawowe statystyki jednostki oraz
 * logikę związaną z jej działaniami w turze (ruch, atak, leczenie).
 *
 * Jest klasą bazową dla konkretnych typów jednostek (np. Infantry, Tank, Medic).
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
     * @param moveCostPerTile Koszt ruchu za jedno pole.
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
     * @brief Wirtualny destruktor.
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
    int m_moveCostPerTile;       ///< Koszt ruchu na pole.

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
