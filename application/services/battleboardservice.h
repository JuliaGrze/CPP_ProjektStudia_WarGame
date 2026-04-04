#ifndef BATTLEBOARDSERVICE_H
#define BATTLEBOARDSERVICE_H

#include <QGridLayout>
#include <QWidget>
#include "../../config/gameconfig.h"
#include "../models/gamestate.h"

class BattleBoardService
{
public:
    static void drawBoard(QGridLayout* grid,
                          QWidget* boardContainer,
                          const GameState& gameState,
                          const GameConfig& config);
};

#endif // BATTLEBOARDSERVICE_H
