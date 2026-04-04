#include "battlepage.h"
#include "ui_battlepage.h"

#include <QPushButton>
#include <QTimer>

#include "../application/services/battleinteractionservice.h"
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
        m_config,
        [this](int x, int y)
        {
            onTileClicked(x, y);
        }
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

    QString sideText = (m_gameState.getCurrentSide() == TeamSide::Player)
                           ? "Niebiescy"
                           : "Czerwoni";

    ui->labelTeamsInfo->setText(
        QString("Niebiescy: %1 | Czerwoni: %2 | Ruch: %3")
            .arg(playerCount)
            .arg(enemyCount)
            .arg(sideText)
        );

    if (m_gameState.hasSelectedPosition())
    {
        const Tile* tile = m_gameState.getBoard().getTile(
            m_gameState.getSelectedX(),
            m_gameState.getSelectedY()
            );

        if (tile && tile->isOccupied() && tile->getUnit())
        {
            const Unit* unit = tile->getUnit();

            ui->labelUnitName->setText(unit->getName());
            ui->labelUnitStats->setText(
                QString("HP: %1\nAtak: %2\nZasięg: %3\nRuch: %4")
                    .arg(unit->getHealth())
                    .arg(unit->getDamage())
                    .arg(unit->getRange())
                    .arg(unit->getMovementPoints())
                );
            return;
        }
    }

    ui->labelUnitName->setText("Brak jednostki");
    ui->labelUnitStats->setText("HP: -\nAtak: -\nZasięg: -\nRuch: -");
}
void BattlePage::onTileClicked(int x, int y)
{
    BattleInteractionService::handleTileClick(m_gameState, x, y);
    refreshStatistics();
    redrawBoard();
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
