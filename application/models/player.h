#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "team.h"
#include "enums/teamside.h"

class Player
{
public:
    Player();
    Player(const QString& name, TeamSide side, bool isAi);

    QString getName() const;
    void setName(const QString& name);

    TeamSide getSide() const;
    bool isAi() const;

    Team& getTeam();
    const Team& getTeam() const;

private:
    QString m_name;
    TeamSide m_side = TeamSide::Player;
    bool m_isAi = false;
    Team m_team;
};

#endif // PLAYER_H
