#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "config/gameconfig.h"
#include "application/models/gamestate.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MenuPage;
class ConfigPage;
class BattlePage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    GameState createTestGameState(const GameConfig& config);

private:
    Ui::MainWindow *ui;
    MenuPage *menuPage;
    ConfigPage *configPage;
    BattlePage *battlePage;
};

#endif // MAINWINDOW_H
