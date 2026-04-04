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
    ui->labelTurnNumber->setText(
        QString("<div style='text-align:center; font-size:18px; font-weight:800; color:#f9fafb;'>Tura %1</div>")
            .arg(m_gameState.getCurrentTurn())
        );
}

void BattlePage::refreshStatistics()
{
    int playerCount = m_gameState.getPlayerTeam().getAliveUnitsCount();
    int enemyCount = m_gameState.getEnemyTeam().getAliveUnitsCount();

    const bool isPlayerTurn = m_gameState.getCurrentSide() == TeamSide::Player;
    const QString currentSideText = isPlayerTurn ? "NIEBIESCY" : "CZERWONI";
    const QString currentSideColor = isPlayerTurn ? "#60a5fa" : "#f87171";

    ui->labelBattleLog->setText("Kliknij jednostkę na planszy, aby zobaczyć jej szczegóły.");

    ui->labelTeamsInfo->setText(
        QString(
            "<div style='text-align:center;'>"
            "<span style='color:#60a5fa; font-weight:700;'>Niebiescy: %1</span>"
            "<br/>"
            "<span style='color:#f87171; font-weight:700;'>Czerwoni: %2</span>"
            "<br/><br/>"
            "<span style='color:#d1d5db;'>Aktualny ruch:</span> "
            "<span style='color:%3; font-weight:800; font-size:16px;'>%4</span>"
            "</div>"
            )
            .arg(playerCount)
            .arg(enemyCount)
            .arg(currentSideColor)
            .arg(currentSideText)
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

            const bool isPlayerUnit = unit->getSide() == TeamSide::Player;
            const QString teamText = isPlayerUnit ? "DRUŻYNA: NIEBIESCY" : "DRUŻYNA: CZERWONI";
            const QString teamColor = isPlayerUnit ? "#60a5fa" : "#f87171";

            ui->labelUnitName->setText(
                QString(
                    "<div style='text-align:center;'>"
                    "<span style='font-size:12px; color:#9ca3af;'>WYBRANA JEDNOSTKA</span><br/>"
                    "<span style='font-size:18px; font-weight:800; color:white;'>%1</span><br/>"
                    "<span style='font-size:13px; font-weight:700; color:%2;'>%3</span>"
                    "</div>"
                    )
                    .arg(unit->getName())
                    .arg(teamColor)
                    .arg(teamText)
                );

            ui->labelUnitStats->setText(
                QString(
                    "<div style='line-height:1.7;'>"
                    "<span style='color:#fca5a5; font-weight:700;'>HP:</span> %1 / %2<br/>"
                    "<span style='color:#fdba74; font-weight:700;'>Atak:</span> %3<br/>"
                    "<span style='color:#86efac; font-weight:700;'>Zasięg:</span> %4<br/>"
                    "<span style='color:#c4b5fd; font-weight:700;'>Ruch:</span> %5"
                    "</div>"
                    )
                    .arg(unit->getHealth())
                    .arg(unit->getMaxHealth())
                    .arg(unit->getDamage())
                    .arg(unit->getRange())
                    .arg(unit->getMovementPoints())
                );

            ui->labelBattleLog->setText(
                QString("Wybrano jednostkę: %1 (%2).")
                    .arg(unit->getName())
                    .arg(isPlayerUnit ? "Niebiescy" : "Czerwoni")
                );

            return;
        }
    }

    ui->labelUnitName->setText(
        "<div style='text-align:center;'>"
        "<span style='font-size:12px; color:#9ca3af;'>PANEL JEDNOSTKI</span><br/>"
        "<span style='font-size:18px; font-weight:800; color:#e5e7eb;'>Brak wyboru</span>"
        "</div>"
        );

    ui->labelUnitStats->setText(
        "<div style='line-height:1.8; color:#9ca3af; text-align:center;'>"
        "Kliknij jednostkę na planszy,<br/>aby zobaczyć jej statystyki."
        "</div>"
        );
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
