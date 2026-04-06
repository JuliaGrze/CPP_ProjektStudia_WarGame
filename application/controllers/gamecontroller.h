#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>

#include "../models/gamestate.h"
#include "../../config/gameconfig.h"
#include "../engine/battleengine.h"
#include "../controllers/aicontroller.h"

class GameController : public QObject
{
    Q_OBJECT

public:
    explicit GameController(QObject* parent = nullptr);

    void startGame(const GameConfig& config);
    void handleTileClick(int x, int y);
    void endTurn();

    const GameState& getGameState() const;
    const GameConfig& getGameConfig() const;

signals:
    void stateChanged();

private:
    void processAiTurnIfNeeded();

private:
    GameConfig m_config;
    GameState m_state;
    BattleEngine m_engine;
    AIController m_aiController;
};

#endif // GAMECONTROLLER_H
