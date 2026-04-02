#include "menupage.h"
#include "ui_menupage.h"

MenuPage::MenuPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuPage)
{
    ui->setupUi(this);

    connect(ui->btnPlay, &QPushButton::clicked, this, &MenuPage::playClicked);
    connect(ui->btnRules, &QPushButton::clicked, this, &MenuPage::rulesClicked);
    connect(ui->btnExit, &QPushButton::clicked, this, &MenuPage::exitClicked);
}

MenuPage::~MenuPage()
{
    delete ui;
}
