#ifndef BATTLEPAGE_H
#define BATTLEPAGE_H

#include <QWidget>
#include <QResizeEvent>
#include <QShowEvent>

class GameController;

namespace Ui {
class BattlePage;
}

class BattlePage : public QWidget
{
    Q_OBJECT

public:
    explicit BattlePage(QWidget *parent = nullptr);
    ~BattlePage();

    void setController(GameController* controller);

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
    void updateTileInfo();

    Ui::BattlePage *ui;
    GameController* m_controller = nullptr;
    bool m_isDrawingBoard = false;
};

#endif // BATTLEPAGE_H
