#include "menupage.h"
#include "ui_menupage.h"

/**
 * @brief Konstruktor strony menu głównego.
 *
 * Inicjalizuje interfejs użytkownika oraz podłącza sygnały
 * przycisków do odpowiednich akcji:
 * - rozpoczęcie gry,
 * - wyświetlenie zasad,
 * - wyjście z aplikacji.
 *
 * @param parent Wskaźnik na widget nadrzędny.
 */
MenuPage::MenuPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MenuPage)
{
    ui->setupUi(this);

    connect(ui->btnPlay, &QPushButton::clicked, this, &MenuPage::playClicked);
    connect(ui->btnRules, &QPushButton::clicked, this, &MenuPage::rulesClicked);
    connect(ui->btnExit, &QPushButton::clicked, this, &MenuPage::exitClicked);
}

/**
 * @brief Destruktor strony menu.
 */
MenuPage::~MenuPage()
{
    delete ui;
}
