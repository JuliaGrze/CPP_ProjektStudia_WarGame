#ifndef BATTLEPAGE_H
#define BATTLEPAGE_H

#include <QWidget>
#include <QResizeEvent>
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

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::BattlePage *ui;
    GameConfig m_config;
    bool m_isDrawingBoard = false;
};

#endif // BATTLEPAGE_H
