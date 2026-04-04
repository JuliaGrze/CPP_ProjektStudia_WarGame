#ifndef ATTACKRESOLVER_H
#define ATTACKRESOLVER_H

#include <QString>

#include "../models/enums/terraintype.h"

class Unit;

struct AttackResult
{
    bool attackPerformed = false;
    bool targetDestroyed = false;
    int damageDealt = 0;
    QString message;
};

class AttackResolver
{
public:
    explicit AttackResolver();

    bool canAttack(const Unit& attacker,
                   int attackerX,
                   int attackerY,
                   const Unit& defender,
                   int defenderX,
                   int defenderY) const;

    AttackResult resolveAttack(Unit& attacker,
                               int attackerX,
                               int attackerY,
                               Unit& defender,
                               int defenderX,
                               int defenderY,
                               TerrainType defenderTerrain) const;

private:
    int calculateDistance(int x1, int y1, int x2, int y2) const;
};

#endif // ATTACKRESOLVER_H
