#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pages/menupage.h"
#include "pages/configpage.h"
#include "pages/battlepage.h"
#include "pages/rulespage.h"
#include "application/controllers/gamecontroller.h"

/**
 * @brief Konstruktor głównego okna aplikacji.
 *
 * Inicjalizuje interfejs użytkownika oraz tworzy wszystkie główne
 * strony aplikacji. Dodaje je do QStackedWidget i ustawia menu jako
 * widok początkowy.
 *
 * Konfiguruje również połączenia sygnałów i slotów odpowiedzialnych
 * za nawigację pomiędzy ekranami oraz uruchamianie rozgrywki.
 *
 * @param parent Wskaźnik na obiekt nadrzędny Qt.
 */
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

    /// Przejście do konfiguracji gry
    connect(menuPage, &MenuPage::playClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(configPage);
            });

    /// Przejście do zasad gry
    connect(menuPage, &MenuPage::rulesClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(rulesPage);
            });

    /// Zamknięcie aplikacji
    connect(menuPage, &MenuPage::exitClicked, this, &MainWindow::close);

    /// Powrót z zasad do menu
    connect(rulesPage, &RulesPage::backClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(menuPage);
            });

    /// Powrót z konfiguracji do menu
    connect(configPage, &ConfigPage::backClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(menuPage);
            });

    /// Rozpoczęcie gry na podstawie konfiguracji
    connect(configPage, &ConfigPage::startClicked, this, [this](const GameConfig& config)
            {
                gameController->startGame(config);
                battlePage->setController(gameController);
                ui->stackedWidget->setCurrentWidget(battlePage);
            });

    /// Powrót z bitwy do menu
    connect(battlePage, &BattlePage::backToMenuClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(menuPage);
            });

    /// Przejście do ponownej konfiguracji
    connect(battlePage, &BattlePage::newConfigurationClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(configPage);
            });

    /// Ponowne uruchomienie gry z tą samą konfiguracją
    connect(battlePage, &BattlePage::playAgainClicked, this, [this]()
            {
                const GameConfig config = gameController->getGameConfig();
                gameController->startGame(config);
                battlePage->setController(gameController);
                ui->stackedWidget->setCurrentWidget(battlePage);
            });
}

/**
 * @brief Destruktor głównego okna aplikacji.
 *
 * Zwalnia zasoby, w tym kontroler gry oraz interfejs użytkownika.
 */
MainWindow::~MainWindow()
{
    delete gameController;
    delete ui;
}
