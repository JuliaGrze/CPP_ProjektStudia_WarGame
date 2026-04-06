#ifndef BATTLEPAGE_H
#define BATTLEPAGE_H

#include <QWidget>

class GameController;
class QResizeEvent;
class QShowEvent;

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
    void redrawBoard();
    void updateTurnInfo();
    void refreshStatistics();
    void updateTileInfo();

signals:
    void backToMenuClicked();
    void playAgainClicked();
    void newConfigurationClicked();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void onTileClicked(int x, int y);
    void showPostGameSummaryDialog();

private:
    Ui::BattlePage *ui;
    GameController* m_controller = nullptr;
    bool m_isDrawingBoard = false;
    bool m_postGameDialogShown = false;
};

#endif // BATTLEPAGE_H
