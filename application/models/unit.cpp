#include "unit.h"
#include <algorithm>

/**
 * @brief Konstruktor jednostki.
 *
 * Inicjalizuje wszystkie podstawowe parametry bojowe, ruchowe
 * oraz właściwości specjalne jednostki.
 *
 * @param name Nazwa jednostki.
 * @param side Strona konfliktu.
 * @param health Początkowa i maksymalna liczba punktów zdrowia.
 * @param damage Siła ataku jednostki.
 * @param minRange Minimalny zasięg ataku.
 * @param maxRange Maksymalny zasięg ataku.
 * @param movementPoints Maksymalna liczba pól ruchu.
 * @param attackCost Koszt ataku w punktach akcji.
 * @param moveCostPerTile Koszt ruchu za jedno pole.
 * @param armor Wartość pancerza jednostki.
 * @param accuracy Celność jednostki.
 * @param evasion Unik jednostki.
 * @param armorPiercing Wartość przebicia pancerza.
 * @param canHeal Informacja, czy jednostka może leczyć.
 * @param healAmount Wartość leczenia.
 * @param canMoveAfterAttack Informacja, czy jednostka może ruszyć się po ataku.
 * @param canAttackAfterMove Informacja, czy jednostka może zaatakować po ruchu.
 */
Unit::Unit(const QString& name,
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
           int armorPiercing,
           bool canHeal,
           int healAmount,
           bool canMoveAfterAttack,
           bool canAttackAfterMove)
    : m_name(name),
    m_side(side),
    m_health(health),
    m_maxHealth(health),
    m_damage(damage),
    m_minRange(minRange),
    m_maxRange(maxRange),
    m_movementPoints(movementPoints),
    m_attackCost(attackCost),
    m_moveCostPerTile(moveCostPerTile),
    m_armor(armor),
    m_accuracy(accuracy),
    m_evasion(evasion),
    m_armorPiercing(armorPiercing),
    m_canHeal(canHeal),
    m_healAmount(healAmount),
    m_canMoveAfterAttack(canMoveAfterAttack),
    m_canAttackAfterMove(canAttackAfterMove)
{
}

/**
 * @brief Zwraca nazwę jednostki.
 *
 * @return Nazwa jednostki.
 */
QString Unit::getName() const
{
    return m_name;
}

/**
 * @brief Zwraca stronę konfliktu jednostki.
 *
 * @return Strona konfliktu.
 */
TeamSide Unit::getSide() const
{
    return m_side;
}

/**
 * @brief Zwraca aktualną liczbę punktów zdrowia.
 *
 * @return Aktualne zdrowie jednostki.
 */
int Unit::getHealth() const
{
    return m_health;
}

/**
 * @brief Zwraca maksymalną liczbę punktów zdrowia.
 *
 * @return Maksymalne zdrowie jednostki.
 */
int Unit::getMaxHealth() const
{
    return m_maxHealth;
}

/**
 * @brief Zwraca siłę ataku jednostki.
 *
 * @return Wartość obrażeń bazowych.
 */
int Unit::getDamage() const
{
    return m_damage;
}

/**
 * @brief Zwraca minimalny zasięg ataku.
 *
 * @return Minimalny zasięg.
 */
int Unit::getMinRange() const
{
    return m_minRange;
}

/**
 * @brief Zwraca maksymalny zasięg ataku.
 *
 * @return Maksymalny zasięg.
 */
int Unit::getRange() const
{
    return m_maxRange;
}

/**
 * @brief Zwraca liczbę punktów ruchu jednostki.
 *
 * @return Maksymalny dystans ruchu.
 */
int Unit::getMovementPoints() const
{
    return m_movementPoints;
}

/**
 * @brief Zwraca koszt ataku w punktach akcji.
 *
 * @return Koszt ataku.
 */
int Unit::getAttackCost() const
{
    return m_attackCost;
}

/**
 * @brief Zwraca koszt ruchu za jedno pole.
 *
 * @return Koszt ruchu na pole.
 */
int Unit::getMoveCostPerTile() const
{
    return m_moveCostPerTile;
}

/**
 * @brief Zwraca wartość pancerza jednostki.
 *
 * @return Wartość pancerza.
 */
int Unit::getArmor() const
{
    return m_armor;
}

/**
 * @brief Zwraca celność jednostki.
 *
 * @return Wartość celności.
 */
int Unit::getAccuracy() const
{
    return m_accuracy;
}

/**
 * @brief Zwraca unik jednostki.
 *
 * @return Wartość uniku.
 */
int Unit::getEvasion() const
{
    return m_evasion;
}

/**
 * @brief Zwraca wartość przebicia pancerza.
 *
 * @return Wartość przebicia pancerza.
 */
int Unit::getArmorPiercing() const
{
    return m_armorPiercing;
}

/**
 * @brief Zwraca wartość leczenia jednostki.
 *
 * @return Liczba punktów zdrowia przywracanych przez leczenie.
 */
int Unit::getHealAmount() const
{
    return m_healAmount;
}

/**
 * @brief Sprawdza, czy jednostka może leczyć.
 *
 * @return true, jeśli jednostka posiada zdolność leczenia.
 */
bool Unit::canHeal() const
{
    return m_canHeal;
}

/**
 * @brief Sprawdza, czy jednostka żyje.
 *
 * @return true, jeśli zdrowie jednostki jest większe od zera.
 */
bool Unit::isAlive() const
{
    return m_health > 0;
}

/**
 * @brief Sprawdza, czy jednostka jest ranna.
 *
 * @return true, jeśli aktualne zdrowie jest mniejsze od maksymalnego.
 */
bool Unit::isDamaged() const
{
    return m_health < m_maxHealth;
}

/**
 * @brief Zadaje obrażenia jednostce.
 *
 * Zdrowie jednostki nie może spaść poniżej zera.
 *
 * @param amount Liczba zadawanych obrażeń.
 */
void Unit::takeDamage(int amount)
{
    if (amount < 0)
        return;

    m_health = std::max(0, m_health - amount);
}

/**
 * @brief Leczy jednostkę.
 *
 * Zdrowie jednostki nie może przekroczyć wartości maksymalnej.
 *
 * @param amount Liczba punktów zdrowia do przywrócenia.
 */
void Unit::heal(int amount)
{
    if (amount < 0)
        return;

    m_health = std::min(m_maxHealth, m_health + amount);
}

/**
 * @brief Oznacza, że jednostka wykonała ruch w tej turze.
 */
void Unit::markMoved()
{
    m_hasMovedThisTurn = true;
}

/**
 * @brief Oznacza, że jednostka wykonała akcję w tej turze.
 */
void Unit::markActed()
{
    m_hasActedThisTurn = true;
}

/**
 * @brief Sprawdza, czy jednostka wykonała już ruch w tej turze.
 *
 * @return true, jeśli ruch został już wykonany.
 */
bool Unit::hasMovedThisTurn() const
{
    return m_hasMovedThisTurn;
}

/**
 * @brief Sprawdza, czy jednostka wykonała już akcję w tej turze.
 *
 * @return true, jeśli akcja została już wykonana.
 */
bool Unit::hasActedThisTurn() const
{
    return m_hasActedThisTurn;
}

/**
 * @brief Sprawdza, czy jednostka może się jeszcze poruszyć.
 *
 * @return true, jeśli jednostka żyje i nie wykonała jeszcze ruchu.
 */
bool Unit::canMoveNow() const
{
    if (!isAlive())
        return false;

    if (m_hasMovedThisTurn)
        return false;

    return true;
}

/**
 * @brief Sprawdza, czy jednostka może jeszcze wykonać atak.
 *
 * @return true, jeśli jednostka żyje i nie wykonała jeszcze akcji.
 */
bool Unit::canAttackNow() const
{
    if (!isAlive())
        return false;

    if (m_hasActedThisTurn)
        return false;

    return true;
}

/**
 * @brief Sprawdza, czy jednostka może wykonać leczenie.
 *
 * @return true, jeśli jednostka może leczyć i nie wykonała jeszcze akcji.
 */
bool Unit::canHealNow() const
{
    if (!m_canHeal)
        return false;

    return canAttackNow();
}

/**
 * @brief Resetuje zasoby tury jednostki.
 *
 * Czyści informacje o wykonanym ruchu i akcji,
 * przygotowując jednostkę do nowej tury.
 */
void Unit::resetTurnResources()
{
    m_hasMovedThisTurn = false;
    m_hasActedThisTurn = false;
}
