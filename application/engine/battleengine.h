#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include "../models/gamestate.h"
#include "attackresolver.h"

class Unit;
class Tile;

class BattleEngine
{
public:
    BattleEngine() = default;

    void handleTileClick(GameState& gameState, int x, int y);
    void endTurn(GameState& gameState);

private:
    bool canSelectUnit(const GameState& gameState, const Unit* unit) const;
    void calculateMoveHighlights(GameState& gameState, int x, int y) const;
    bool tryMoveSelectedUnit(GameState& gameState, int targetX, int targetY);
    bool tryAttackSelectedUnit(GameState& gameState, int targetX, int targetY);
    bool tryHealSelectedUnit(GameState& gameState, int targetX, int targetY);
    void clearDefeatedUnitFromTile(Tile& tile) const;
    void finishActionAndMaybeEndTurn(GameState& gameState);

    bool hasAnyAvailableAction(const GameState& gameState) const;
    bool unitHasAnyAvailableAction(const GameState& gameState, int x, int y, const Unit& unit) const;

    int calculateDistance(int x1, int y1, int x2, int y2) const;
    int calculateLowestMoveCost(const GameState& gameState,
                                const Unit& unit,
                                int startX,
                                int startY,
                                int targetX,
                                int targetY) const;

private:
    AttackResolver m_attackResolver;
};

#endif // BATTLEENGINE_H
