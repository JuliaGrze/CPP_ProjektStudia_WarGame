#include "battlepage.h"
#include "ui_battlepage.h"

#include <QPushButton>
#include <QTimer>

#include "../application/models/team.h"
#include "../application/services/battleboardservice.h"

BattlePage::BattlePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BattlePage)
{
    ui->setupUi(this);

    connect(ui->btnBackToMenu, &QPushButton::clicked,
            this, &BattlePage::backToMenuClicked);

    updateTurnInfo();
    refreshStatistics();
}

BattlePage::~BattlePage()
{
    delete ui;
}

void BattlePage::setConfiguration(const GameConfig& config)
{
    m_config = config;
    updateTurnInfo();
    refreshStatistics();

    QTimer::singleShot(0, this, [this]()
                       {
                           redrawBoard();
                       });
}

void BattlePage::setGameState(const GameState& gameState)
{
    m_gameState = gameState;
    updateTurnInfo();
    refreshStatistics();

    QTimer::singleShot(0, this, [this]()
                       {
                           redrawBoard();
                       });
}

void BattlePage::redrawBoard()
{
    if (m_isDrawingBoard)
        return;

    m_isDrawingBoard = true;

    BattleBoardService::drawBoard(
        ui->gridLayout_2,
        ui->boardContainer,
        m_gameState,
        m_config
        );

    m_isDrawingBoard = false;
}

void BattlePage::updateTurnInfo()
{
    ui->labelTurnNumber->setText(QString("Tura %1").arg(m_gameState.getCurrentTurn()));
}

void BattlePage::refreshStatistics()
{
    ui->labelBattleLog->setText("Rozpoczęcie bitwy.\nOczekiwanie na pierwszą akcję.");

    int playerCount = m_gameState.getPlayerTeam().getAliveUnitsCount();
    int enemyCount = m_gameState.getEnemyTeam().getAliveUnitsCount();

    ui->labelTeamsInfo->setText(
        QString("Niebiescy: %1 | Czerwoni: %2").arg(playerCount).arg(enemyCount)
        );

    const Team& playerTeam = m_gameState.getPlayerTeam();
    auto units = playerTeam.getUnits();

    if (!units.isEmpty() && units.first())
    {
        const auto& unit = units.first();

        ui->labelUnitName->setText(unit->getName());
        ui->labelUnitStats->setText(
            QString("HP: %1\nAtak: %2\nZasięg: %3\nRuch: %4")
                .arg(unit->getHealth())
                .arg(unit->getDamage())
                .arg(unit->getRange())
                .arg(unit->getMovementPoints())
            );
    }
    else
    {
        ui->labelUnitName->setText("Brak jednostki");
        ui->labelUnitStats->setText("HP: -\nAtak: -\nZasięg: -\nRuch: -");
    }
}

void BattlePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    redrawBoard();
}

void BattlePage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    QTimer::singleShot(0, this, [this]()
                       {
                           redrawBoard();
                       });
}
