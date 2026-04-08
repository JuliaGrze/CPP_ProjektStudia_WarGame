#include "player.h"

/**
 * @brief Domyślny konstruktor gracza.
 *
 * Tworzy gracza:
 * - o nazwie "Gracz",
 * - po stronie gracza (Player),
 * - sterowanego przez człowieka,
 * - z domyślną drużyną.
 */
Player::Player()
    : m_name("Gracz"),
    m_side(TeamSide::Player),
    m_isAi(false),
    m_team("Drużyna gracza", TeamSide::Player)
{
}

/**
 * @brief Konstruktor gracza z parametrami.
 *
 * @param name Nazwa gracza.
 * @param side Strona konfliktu.
 * @param isAi Określa, czy gracz jest sterowany automatycznie.
 */
Player::Player(const QString& name, TeamSide side, bool isAi)
    : m_name(name),
    m_side(side),
    m_isAi(isAi),
    m_team(name + " - drużyna", side)
{
}

/**
 * @brief Zwraca nazwę gracza.
 *
 * @return Nazwa gracza.
 */
QString Player::getName() const
{
    return m_name;
}

/**
 * @brief Ustawia nazwę gracza.
 *
 * @param name Nowa nazwa.
 */
void Player::setName(const QString& name)
{
    m_name = name;
}

/**
 * @brief Zwraca stronę konfliktu gracza.
 *
 * @return Strona konfliktu.
 */
TeamSide Player::getSide() const
{
    return m_side;
}

/**
 * @brief Sprawdza, czy gracz jest sterowany automatycznie.
 *
 * @return true, jeśli gracz jest sterowany przez AI.
 */
bool Player::isAi() const
{
    return m_isAi;
}

/**
 * @brief Zwraca drużynę gracza.
 *
 * @return Referencja do drużyny.
 */
Team& Player::getTeam()
{
    return m_team;
}

/**
 * @brief Zwraca stałą drużynę gracza.
 *
 * @return Stała referencja do drużyny.
 */
const Team& Player::getTeam() const
{
    return m_team;
}
