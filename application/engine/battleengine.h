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
    QVector<QPair<int, int>> calculateAvailableMovePositions(const GameState& gameState, int x, int y) const;
    bool tryMoveSelectedUnit(GameState& gameState, int targetX, int targetY);
    int calculateDistance(int x1, int y1, int x2, int y2) const;
};

#endif // BATTLEENGINE_H
