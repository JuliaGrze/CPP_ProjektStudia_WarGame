#include "player.h"

Player::Player()
    : m_name("Gracz"),
    m_side(TeamSide::Player),
    m_isAi(false),
    m_team("Drużyna gracza", TeamSide::Player)
{
}

Player::Player(const QString& name, TeamSide side, bool isAi)
    : m_name(name),
    m_side(side),
    m_isAi(isAi),
    m_team(name + " - drużyna", side)
{
}

QString Player::getName() const
{
    return m_name;
}

void Player::setName(const QString& name)
{
    m_name = name;
}

TeamSide Player::getSide() const
{
    return m_side;
}

bool Player::isAi() const
{
    return m_isAi;
}

Team& Player::getTeam()
{
    return m_team;
}

const Team& Player::getTeam() const
{
    return m_team;
}
