#ifndef BATTLEBOARDSERVICE_H
#define BATTLEBOARDSERVICE_H

#include <QGridLayout>
#include <QWidget>
#include <functional>

#include "../../config/gameconfig.h"
#include "../models/gamestate.h"

class BattleBoardService
{
public:
    static void drawBoard(QGridLayout* grid,
                          QWidget* boardContainer,
                          const GameState& gameState,
                          const GameConfig& config,
                          const std::function<void(int, int)>& onTileClicked);
};

#endif // BATTLEBOARDSERVICE_H
