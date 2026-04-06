#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pages/menupage.h"
#include "pages/configpage.h"
#include "pages/battlepage.h"
#include "pages/rulespage.h"
#include "application/controllers/gamecontroller.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , menuPage(new MenuPage(this))
    , configPage(new ConfigPage(this))
    , battlePage(new BattlePage(this))
    , rulesPage(new RulesPage(this))
    , gameController(new GameController())
{
    ui->setupUi(this);

    setMinimumSize(1100, 900);

    ui->stackedWidget->addWidget(menuPage);
    ui->stackedWidget->addWidget(configPage);
    ui->stackedWidget->addWidget(battlePage);
    ui->stackedWidget->addWidget(rulesPage);

    ui->stackedWidget->setCurrentWidget(menuPage);

    connect(menuPage, &MenuPage::playClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(configPage);
            });

    connect(menuPage, &MenuPage::exitClicked, this, &MainWindow::close);

    connect(configPage, &ConfigPage::backClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(menuPage);
            });

    connect(configPage, &ConfigPage::startClicked, this, [this](const GameConfig& config)
            {
                gameController->startGame(config);
                battlePage->setController(gameController);
                ui->stackedWidget->setCurrentWidget(battlePage);
            });

    connect(battlePage, &BattlePage::backToMenuClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(menuPage);
            });

    connect(menuPage, &MenuPage::rulesClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(rulesPage);
            });

    connect(rulesPage, &RulesPage::backClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(menuPage);
            });
}

MainWindow::~MainWindow()
{
    delete gameController;
    delete ui;
}
