#ifndef BATTLEENGINE_H
#define BATTLEENGINE_H

#include "../models/gamestate.h"

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
    void finishActionAndMaybeEndTurn(GameState& gameState);
    int calculateDistance(int x1, int y1, int x2, int y2) const;
};

#endif // BATTLEENGINE_H
