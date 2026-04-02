#ifndef BATTLEPAGE_H
#define BATTLEPAGE_H

#include <QWidget>
#include "../config/gameconfig.h"

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
    void drawBoard();
    void updateTurnInfo();
    void refreshStatistics();

signals:
    void backToMenuClicked();

private:
    Ui::BattlePage *ui;
    GameConfig m_config;
};

#endif // BATTLEPAGE_H
