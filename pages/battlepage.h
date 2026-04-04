#ifndef BATTLEPAGE_H
#define BATTLEPAGE_H

#include <QWidget>
#include <QResizeEvent>
#include <QShowEvent>
#include "../config/gameconfig.h"
#include "../application/models/gamestate.h"

namespace Ui {
class BattlePage;
}

class BattlePage : public QWidget
{
    Q_OBJECT

public:
    explicit BattlePage(QWidget *parent = nullptr);
    ~BattlePage();

    void setConfiguration(const GameConfig& config);
    void setGameState(const GameState& gameState);

    void updateTurnInfo();
    void refreshStatistics();

signals:
    void backToMenuClicked();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void redrawBoard();
    void onTileClicked(int x, int y);

    Ui::BattlePage *ui;
    GameConfig m_config;
    GameState m_gameState;
    bool m_isDrawingBoard = false;
};

#endif // BATTLEPAGE_H
