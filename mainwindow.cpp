#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pages/menupage.h"
#include "pages/configpage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *menu = new MenuPage(this);
    auto *config = new ConfigPage(this);

    ui->stackedWidget->addWidget(menu);    // index 0
    ui->stackedWidget->addWidget(config);  // index 1

    ui->stackedWidget->setCurrentIndex(0);

    connect(menu, &MenuPage::playClicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
    });

    connect(config, &ConfigPage::backClicked, this, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
    });

    connect(menu, &MenuPage::exitClicked, this, [this]() {
        close();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

