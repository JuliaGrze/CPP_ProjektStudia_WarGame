#include "battlepage.h"
#include "ui_battlepage.h"

#include <QPushButton>
#include <QTimer>

#include "../application/controllers/gamecontroller.h"
#include "../application/models/gamestate.h"
#include "../application/models/team.h"
#include "../application/models/tile.h"
#include "../application/models/unit.h"
#include "../application/models/enums/teamside.h"
#include "../application/services/battleboardservice.h"
#include "../config/gameconfig.h"
#include "../application/models/player.h"

BattlePage::BattlePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BattlePage)
{
    ui->setupUi(this);

    connect(ui->btnBackToMenu, &QPushButton::clicked,
            this, &BattlePage::backToMenuClicked);

    connect(ui->btnEndTurn, &QPushButton::clicked, this, [this]()
            {
                if (!m_controller)
                    return;

                m_controller->endTurn();
                updateTurnInfo();
                refreshStatistics();
                redrawBoard();
            });

    updateTurnInfo();
    refreshStatistics();
}

BattlePage::~BattlePage()
{
    delete ui;
}

void BattlePage::setController(GameController* controller)
{
    m_controller = controller;

    updateTurnInfo();
    refreshStatistics();

    QTimer::singleShot(0, this, [this]()
                       {
                           redrawBoard();
                       });
}

void BattlePage::redrawBoard()
{
    if (m_isDrawingBoard || !m_controller)
        return;

    m_isDrawingBoard = true;

    const GameState& gameState = m_controller->getGameState();
    const GameConfig& config = m_controller->getGameConfig();

    BattleBoardService::drawBoard(
        ui->gridLayout_2,
        ui->boardContainer,
        gameState,
        config,
        [this](int x, int y)
        {
            onTileClicked(x, y);
        }
        );

    m_isDrawingBoard = false;
}

void BattlePage::updateTurnInfo()
{
    int currentTurn = 1;

    if (m_controller)
        currentTurn = m_controller->getGameState().getCurrentTurn();

    ui->labelTurnNumber->setText(
        QString("<div style='text-align:center; font-size:18px; font-weight:800; color:#f9fafb;'>Tura %1</div>")
            .arg(currentTurn)
        );
}

void BattlePage::refreshStatistics()
{
    if (!m_controller)
    {
        ui->labelBattleLog->setText("Brak aktywnej gry.");
        ui->labelTeamsInfo->setText(
            "<div style='text-align:center; color:#9ca3af;'>Brak danych rozgrywki</div>"
            );
        ui->labelUnitName->setText(
            "<div style='text-align:center;'>"
            "<span style='font-size:12px; color:#9ca3af;'>PANEL JEDNOSTKI</span><br/>"
            "<span style='font-size:18px; font-weight:800; color:#e5e7eb;'>Brak wyboru</span>"
            "</div>"
            );
        ui->labelUnitStats->setText(
            "<div style='line-height:1.8; color:#9ca3af; text-align:center;'>"
            "Uruchom grę, aby zobaczyć statystyki."
            "</div>"
            );
        return;
    }

    const GameState& gameState = m_controller->getGameState();

    int playerCount = gameState.getPlayerTeam().getAliveUnitsCount();
    int enemyCount = gameState.getEnemyTeam().getAliveUnitsCount();

    const bool isPlayerTurn = gameState.getCurrentSide() == TeamSide::Player;
    const QString currentSideText = isPlayerTurn ? "NIEBIESCY" : "CZERWONI";
    const QString currentSideColor = isPlayerTurn ? "#60a5fa" : "#f87171";

    ui->labelBattleLog->setText(gameState.getLastActionMessage());

    ui->labelTeamsInfo->setText(
        QString(
            "<div style='text-align:center;'>"
            "<span style='color:#60a5fa; font-weight:700;'>Niebiescy: %1</span>"
            "<br/>"
            "<span style='color:#f87171; font-weight:700;'>Czerwoni: %2</span>"
            "<br/><br/>"
            "<span style='color:#d1d5db;'>Aktualny ruch:</span> "
            "<span style='color:%3; font-weight:800; font-size:16px;'>%4</span>"
            "<br/>"
            "<span style='color:#fbbf24; font-weight:700;'>AP drużyny:</span> %5 / %6"
            "</div>"
            )
            .arg(playerCount)
            .arg(enemyCount)
            .arg(currentSideColor)
            .arg(currentSideText)
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints())
        );

    if (gameState.hasSelectedPosition())
    {
        const Tile* tile = gameState.getBoard().getTile(
            gameState.getSelectedX(),
            gameState.getSelectedY()
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
                    "<span style='color:#c4b5fd; font-weight:700;'>AP drużyny:</span> %5 / %6<br/>"
                    "<span style='color:#93c5fd; font-weight:700;'>MP jednostki:</span> %7 / %8"
                    "</div>"
                    )
                    .arg(unit->getHealth())
                    .arg(unit->getMaxHealth())
                    .arg(unit->getDamage())
                    .arg(unit->getRange())
                    .arg(gameState.getCurrentTurnActionPoints())
                    .arg(gameState.getMaxTurnActionPoints())
                    .arg(unit->getCurrentMovementPoints())
                    .arg(unit->getMovementPoints())
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
        QString(
            "<div style='line-height:1.8; color:#9ca3af; text-align:center;'>"
            "AP drużyny: <b>%1 / %2</b><br/>"
            "Kliknij jednostkę na planszy,<br/>aby zobaczyć jej statystyki."
            "</div>"
            )
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints())
        );
}

void BattlePage::onTileClicked(int x, int y)
{
    if (!m_controller)
        return;

    m_controller->handleTileClick(x, y);

    updateTurnInfo();
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
