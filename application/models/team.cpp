#include "team.h"

/**
 * @brief Domyślny konstruktor drużyny.
 */
Team::Team() = default;

/**
 * @brief Konstruktor drużyny z parametrami.
 *
 * @param name Nazwa drużyny.
 * @param side Strona konfliktu.
 */
Team::Team(const QString& name, TeamSide side)
    : m_name(name), m_side(side)
{
}

/**
 * @brief Zwraca nazwę drużyny.
 *
 * @return Nazwa drużyny.
 */
QString Team::getName() const
{
    return m_name;
}

/**
 * @brief Zwraca stronę konfliktu drużyny.
 *
 * @return Strona konfliktu.
 */
TeamSide Team::getSide() const
{
    return m_side;
}

/**
 * @brief Dodaje jednostkę do drużyny.
 *
 * @param unit Wskaźnik do jednostki.
 */
void Team::addUnit(std::shared_ptr<Unit> unit)
{
    if (unit)
        m_units.append(unit);
}

/**
 * @brief Zwraca listę jednostek drużyny.
 *
 * @return Lista jednostek.
 */
QList<std::shared_ptr<Unit>> Team::getUnits() const
{
    return m_units;
}

/**
 * @brief Zwraca całkowitą liczbę jednostek w drużynie.
 *
 * @return Liczba wszystkich jednostek.
 */
int Team::getUnitsCount() const
{
    return m_units.size();
}

/**
 * @brief Zwraca liczbę żywych jednostek.
 *
 * @return Liczba jednostek, które nie zostały zniszczone.
 */
int Team::getAliveUnitsCount() const
{
    int count = 0;

    for (const auto& unit : m_units)
    {
        if (unit && unit->isAlive())
            ++count;
    }

    return count;
}

/**
 * @brief Sprawdza, czy drużyna posiada żywe jednostki.
 *
 * @return true, jeśli istnieje przynajmniej jedna żywa jednostka.
 */
bool Team::hasUnitsAlive() const
{
    return getAliveUnitsCount() > 0;
}

/**
 * @brief Usuwa wszystkie jednostki z drużyny.
 */
void Team::clear()
{
    m_units.clear();
}
