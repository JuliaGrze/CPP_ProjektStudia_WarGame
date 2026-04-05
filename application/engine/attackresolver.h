#ifndef ATTACKRESOLVER_H
#define ATTACKRESOLVER_H

#include <QString>
#include "../models/enums/terraintype.h"

class Unit;

struct AttackResult
{
    bool attackPerformed = false;
    bool targetDestroyed = false;
    bool hit = false;
    int hitChance = 0;
    int roll = 0;
    int damageDealt = 0;
    int targetHealthAfter = 0;
    int targetMaxHealth = 0;
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
                   int defenderY,
                   TerrainType attackerTerrain = TerrainType::Plain) const;

    AttackResult resolveAttack(Unit& attacker,
                               int attackerX,
                               int attackerY,
                               TerrainType attackerTerrain,
                               Unit& defender,
                               int defenderX,
                               int defenderY,
                               TerrainType defenderTerrain) const;

private:
    int calculateDistance(int x1, int y1, int x2, int y2) const;
    int calculateHitChance(const Unit& attacker,
                           const Unit& defender,
                           int distance,
                           TerrainType attackerTerrain,
                           TerrainType defenderTerrain) const;
    int calculateDamage(const Unit& attacker,
                        const Unit& defender,
                        TerrainType defenderTerrain) const;
};

#endif // ATTACKRESOLVER_H
