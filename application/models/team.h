#ifndef TEAM_H
#define TEAM_H

#include <QString>
#include <QList>
#include <memory>
#include "unit.h"
#include "enums/teamside.h"

/**
 * @brief Reprezentuje drużynę jednostek w grze.
 *
 * Klasa przechowuje:
 * - nazwę drużyny,
 * - stronę konfliktu,
 * - listę jednostek należących do drużyny.
 *
 * Umożliwia zarządzanie jednostkami oraz sprawdzanie ich stanu.
 */
class Team
{
public:
    /**
     * @brief Domyślny konstruktor drużyny.
     */
    Team();

    /**
     * @brief Tworzy drużynę o podanej nazwie i stronie konfliktu.
     *
     * @param name Nazwa drużyny.
     * @param side Strona konfliktu przypisana do drużyny.
     */
    Team(const QString& name, TeamSide side);

    /**
     * @brief Zwraca nazwę drużyny.
     *
     * @return Nazwa drużyny.
     */
    QString getName() const;

    /**
     * @brief Zwraca stronę konfliktu drużyny.
     *
     * @return Strona konfliktu.
     */
    TeamSide getSide() const;

    /**
     * @brief Dodaje jednostkę do drużyny.
     *
     * @param unit Wskaźnik do dodawanej jednostki.
     */
    void addUnit(std::shared_ptr<Unit> unit);

    /**
     * @brief Zwraca listę jednostek drużyny.
     *
     * @return Lista jednostek.
     */
    QList<std::shared_ptr<Unit>> getUnits() const;

    /**
     * @brief Zwraca całkowitą liczbę jednostek w drużynie.
     *
     * @return Liczba jednostek.
     */
    int getUnitsCount() const;

    /**
     * @brief Zwraca liczbę żywych jednostek.
     *
     * @return Liczba jednostek, które nie zostały zniszczone.
     */
    int getAliveUnitsCount() const;

    /**
     * @brief Sprawdza, czy drużyna ma jeszcze żywe jednostki.
     *
     * @return true, jeśli istnieje przynajmniej jedna żywa jednostka.
     */
    bool hasUnitsAlive() const;

    /**
     * @brief Usuwa wszystkie jednostki z drużyny.
     */
    void clear();

private:
    QString m_name;                      ///< Nazwa drużyny.
    TeamSide m_side = TeamSide::Player; ///< Strona konfliktu drużyny.
    QList<std::shared_ptr<Unit>> m_units; ///< Lista jednostek drużyny.
};

#endif // TEAM_H
