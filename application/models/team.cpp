#include "team.h"

Team::Team() = default;

Team::Team(const QString& name, TeamSide side)
    : m_name(name), m_side(side)
{
}

QString Team::getName() const
{
    return m_name;
}

TeamSide Team::getSide() const
{
    return m_side;
}

void Team::addUnit(std::shared_ptr<Unit> unit)
{
    if (unit)
        m_units.append(unit);
}

QList<std::shared_ptr<Unit>> Team::getUnits() const
{
    return m_units;
}

int Team::getUnitsCount() const
{
    return m_units.size();
}

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

bool Team::hasUnitsAlive() const
{
    return getAliveUnitsCount() > 0;
}

void Team::clear()
{
    m_units.clear();
}
