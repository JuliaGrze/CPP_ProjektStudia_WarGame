#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "team.h"
#include "enums/teamside.h"

/**
 * @brief Reprezentuje gracza biorącego udział w rozgrywce.
 *
 * Klasa przechowuje podstawowe informacje o graczu, takie jak:
 * - nazwa,
 * - strona konfliktu,
 * - sposób sterowania,
 * - przypisana drużyna.
 *
 * Może reprezentować zarówno gracza sterowanego przez użytkownika,
 * jak i gracza sterowanego automatycznie.
 */
class Player
{
public:
    /**
     * @brief Domyślny konstruktor gracza.
     */
    Player();

    /**
     * @brief Tworzy gracza o podanych parametrach.
     *
     * @param name Nazwa gracza.
     * @param side Strona konfliktu przypisana do gracza.
     * @param isAi Informacja, czy gracz jest sterowany automatycznie.
     */
    Player(const QString& name, TeamSide side, bool isAi);

    /**
     * @brief Zwraca nazwę gracza.
     *
     * @return Nazwa gracza.
     */
    QString getName() const;

    /**
     * @brief Ustawia nazwę gracza.
     *
     * @param name Nowa nazwa gracza.
     */
    void setName(const QString& name);

    /**
     * @brief Zwraca stronę konfliktu przypisaną do gracza.
     *
     * @return Strona konfliktu gracza.
     */
    TeamSide getSide() const;

    /**
     * @brief Informuje, czy gracz jest sterowany automatycznie.
     *
     * @return true, jeśli gracz jest sterowany automatycznie, w przeciwnym razie false.
     */
    bool isAi() const;

    /**
     * @brief Zwraca drużynę przypisaną do gracza.
     *
     * @return Referencja do drużyny gracza.
     */
    Team& getTeam();

    /**
     * @brief Zwraca stałą referencję do drużyny przypisanej do gracza.
     *
     * @return Stała referencja do drużyny gracza.
     */
    const Team& getTeam() const;

private:
    QString m_name;                  ///< Nazwa gracza.
    TeamSide m_side = TeamSide::Player; ///< Strona konfliktu przypisana do gracza.
    bool m_isAi = false;             ///< Informacja, czy gracz jest sterowany automatycznie.
    Team m_team;                     ///< Drużyna należąca do gracza.
};

#endif // PLAYER_H
