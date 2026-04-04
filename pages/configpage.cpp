#include "configpage.h"
#include "ui_configpage.h"
#include "teamconfigdialog.h"
#include "../config/gameconfigdata.h"

#include <QListView>
#include <QMessageBox>
#include <QPushButton>

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

ConfigPage::~ConfigPage()
{
    delete ui;
}

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

void ConfigPage::onPlayerTeamConfigClicked()
{
    TeamConfigDialog dialog("Konfiguracja twojej drużyny", m_playerTeam, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_playerTeam = dialog.composition();
        updateTeamSummaries();
    }
}

void ConfigPage::onEnemyTeamConfigClicked()
{
    TeamConfigDialog dialog("Konfiguracja drużyny przeciwnej", m_enemyTeam, this);

    if (dialog.exec() == QDialog::Accepted)
    {
        m_enemyTeam = dialog.composition();
        updateTeamSummaries();
    }
}

void ConfigPage::updateTeamSummaries()
{
    ui->labelPlayerTeamSummary->setText(m_playerTeam.toDisplayString());
    ui->labelEnemyTeamSummary->setText(m_enemyTeam.toDisplayString());
}

GameConfig ConfigPage::saveConfiguration() const
{
    GameConfig config;
    config.mapVariant = ui->cmbMap->currentText();
    config.mapSize = GameConfigData::mapSizeFromText(ui->cmbMapSize->currentText());
    config.playerTeam = m_playerTeam;
    config.enemyTeam = m_enemyTeam;
    return config;
}

GameConfig ConfigPage::getConfiguration() const
{
    return saveConfiguration();
}

bool ConfigPage::validateConfiguration() const
{
    const GameConfig config = saveConfiguration();
    return config.isValid();
}

void ConfigPage::onStartClicked()
{
    const GameConfig config = saveConfiguration();

    if (!config.isValid())
    {
        QMessageBox::warning(
            this,
            "Błąd konfiguracji",
            "Konfiguracja gry jest niepoprawna.\n\n"
            "Sprawdź, czy obie drużyny mają jednostki "
            "oraz czy mapa i rozmiar planszy są poprawnie ustawione."
            );
        return;
    }

    const int playerUnits = config.playerTeam.totalUnits();
    const int enemyUnits = config.enemyTeam.totalUnits();

    const int maxUnitsPerTeam = config.mapSize * 3;

    if (playerUnits > maxUnitsPerTeam || enemyUnits > maxUnitsPerTeam)
    {
        QMessageBox::warning(
            this,
            "Za dużo jednostek",
            QString("Na wybranej planszy jedna drużyna ma zbyt dużo jednostek.\n\n"
                    "Maksymalna liczba jednostek na drużynę dla tej planszy: %1\n"
                    "Twoja drużyna: %2\n"
                    "Drużyna przeciwna: %3")
                .arg(maxUnitsPerTeam)
                .arg(playerUnits)
                .arg(enemyUnits)
            );
        return;
    }

    const int maxAllowedDifference = 3;

    if (std::abs(playerUnits - enemyUnits) > maxAllowedDifference)
    {
        QMessageBox::warning(
            this,
            "Nierówne drużyny",
            QString("Różnica w liczbie jednostek między drużynami jest zbyt duża.\n\n"
                    "Maksymalna dozwolona różnica: %1\n"
                    "Twoja drużyna: %2\n"
                    "Drużyna przeciwna: %3")
                .arg(maxAllowedDifference)
                .arg(playerUnits)
                .arg(enemyUnits)
            );
        return;
    }

    emit startClicked(config);
}
