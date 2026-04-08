#include "configpage.h"
#include "ui_configpage.h"
#include "teamconfigdialog.h"
#include "../config/gameconfigdata.h"
#include "../application/services/gameconfigvalidationservice.h"

#include <QListView>
#include <QMessageBox>
#include <QPushButton>

/**
 * @brief Konstruktor widoku konfiguracji gry.
 *
 * Inicjalizuje interfejs użytkownika, podłącza obsługę przycisków
 * oraz ładuje dostępne opcje konfiguracji.
 *
 * @param parent Wskaźnik na widget nadrzędny.
 */
ConfigPage::ConfigPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ConfigPage)
{
    ui->setupUi(this);

    connect(ui->btnBackToMenu, &QPushButton::clicked, this, &ConfigPage::backClicked);
    connect(ui->btnPlayerTeamConfig, &QPushButton::clicked, this, &ConfigPage::onPlayerTeamConfigClicked);
    connect(ui->btnEnemyTeamConfig, &QPushButton::clicked, this, &ConfigPage::onEnemyTeamConfigClicked);
    connect(ui->btnStart, &QPushButton::clicked, this, &ConfigPage::onStartClicked);

    loadConfigurationOptions();
    updateTeamSummaries();
}

/**
 * @brief Destruktor widoku konfiguracji.
 */
ConfigPage::~ConfigPage()
{
    delete ui;
}

/**
 * @brief Ładuje dostępne opcje konfiguracji do interfejsu.
 *
 * Ustawia listy wariantów map i rozmiarów planszy
 * oraz wybiera domyślne wartości początkowe.
 */
void ConfigPage::loadConfigurationOptions()
{
    ui->cmbMap->setView(new QListView(this));
    ui->cmbMapSize->setView(new QListView(this));

    ui->cmbMap->clear();
    ui->cmbMap->addItems(GameConfigData::availableMapVariants());

    ui->cmbMapSize->clear();
    ui->cmbMapSize->addItems(GameConfigData::availableMapSizes());

    ui->cmbMap->setCurrentIndex(0);
    ui->cmbMapSize->setCurrentText("20 x 20");
}

/**
 * @brief Obsługuje kliknięcie konfiguracji drużyny gracza.
 *
 * Otwiera okno dialogowe konfiguracji i po zatwierdzeniu
 * aktualizuje skład drużyny gracza.
 */
void ConfigPage::onPlayerTeamConfigClicked()
{
    TeamConfigDialog dialog("Konfiguracja twojej drużyny", m_playerTeam, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_playerTeam = dialog.composition();
        updateTeamSummaries();
    }
}

/**
 * @brief Obsługuje kliknięcie konfiguracji drużyny przeciwnika.
 *
 * Otwiera okno dialogowe konfiguracji i po zatwierdzeniu
 * aktualizuje skład drużyny przeciwnika.
 */
void ConfigPage::onEnemyTeamConfigClicked()
{
    TeamConfigDialog dialog("Konfiguracja drużyny przeciwnej", m_enemyTeam, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_enemyTeam = dialog.composition();
        updateTeamSummaries();
    }
}

/**
 * @brief Aktualizuje tekstowe podsumowanie obu drużyn w interfejsie.
 */
void ConfigPage::updateTeamSummaries()
{
    ui->labelPlayerTeamSummary->setText(m_playerTeam.toDisplayString());
    ui->labelEnemyTeamSummary->setText(m_enemyTeam.toDisplayString());
}

/**
 * @brief Odczytuje konfigurację gry z interfejsu użytkownika.
 *
 * Tworzy obiekt GameConfig na podstawie aktualnie wybranych
 * ustawień mapy, rozmiaru planszy i składów drużyn.
 *
 * @return Zapisana konfiguracja gry.
 */
GameConfig ConfigPage::saveConfiguration() const
{
    GameConfig config;
    config.mapVariant = ui->cmbMap->currentText();
    config.mapSize = GameConfigData::mapSizeFromText(ui->cmbMapSize->currentText());
    config.playerTeam = m_playerTeam;
    config.enemyTeam = m_enemyTeam;
    return config;
}

/**
 * @brief Zwraca aktualną konfigurację gry.
 *
 * @return Aktualna konfiguracja.
 */
GameConfig ConfigPage::getConfiguration() const
{
    return saveConfiguration();
}

/**
 * @brief Sprawdza podstawową poprawność konfiguracji gry.
 *
 * @return true, jeśli konfiguracja jest poprawna.
 */
bool ConfigPage::validateConfiguration() const
{
    const GameConfig config = saveConfiguration();
    return config.isValid();
}

/**
 * @brief Obsługuje kliknięcie przycisku rozpoczęcia gry.
 *
 * Metoda waliduje konfigurację. Jeśli konfiguracja jest niepoprawna,
 * wyświetlane jest okno ostrzeżenia. W przeciwnym razie emitowany
 * jest sygnał rozpoczęcia gry.
 */
void ConfigPage::onStartClicked()
{
    const GameConfig config = saveConfiguration();
    const GameConfigValidationResult validationResult =
        GameConfigValidationService::validate(config);

    if (!validationResult.isValid)
    {
        QMessageBox::warning(
            this,
            "Błąd konfiguracji",
            validationResult.message
            );
        return;
    }

    emit startClicked(config);
}
