#ifndef TEAM_H
#define TEAM_H

#include <QString>
#include <QList>
#include <memory>
#include "unit.h"
#include "enums/teamside.h"

class Team
{
public:
    Team();
    Team(const QString& name, TeamSide side);

    QString getName() const;
    TeamSide getSide() const;

    void addUnit(std::shared_ptr<Unit> unit);
    QList<std::shared_ptr<Unit>> getUnits() const;

    int getUnitsCount() const;
    int getAliveUnitsCount() const;
    bool hasUnitsAlive() const;
    void clear();

private:
    QString m_name;
    TeamSide m_side = TeamSide::Player;
    QList<std::shared_ptr<Unit>> m_units;
};

#endif // TEAM_H
