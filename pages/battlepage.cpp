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

namespace
{
QString terrainToDisplayName(TerrainType terrain)
{
    switch (terrain)
    {
    case TerrainType::Plain:    return "Równina";
    case TerrainType::Forest:   return "Las";
    case TerrainType::Mountain: return "Wzgórze";
    case TerrainType::Water:    return "Woda";
    case TerrainType::Building: return "Budynek";
    }

    return "Nieznany";
}

QString buildDefaultTileInfoText()
{
    return "Brak wybranego pola.\n"
           "Kliknij swoją jednostkę,\n"
           "aby zobaczyć informacje o terenie.";
}

QString buildTileInfoText(const Tile* tile)
{
    if (!tile)
        return buildDefaultTileInfoText();

    const QString terrainName = terrainToDisplayName(tile->getTerrain());
    const QString walkableText = tile->isWalkable() ? "tak" : "nie";

    return QString("Teren: %1\n"
                   "Koszt ruchu: %2\n"
                   "Osłona: +%3\n"
                   "Bonus zasięgu: +%4\n"
                   "Przechodnie: %5")
        .arg(terrainName)
        .arg(tile->getMovementCost())
        .arg(tile->getCoverBonus())
        .arg(tile->getRangeBonus())
        .arg(walkableText);
}
}

BattlePage::BattlePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BattlePage)
{
    ui->setupUi(this);

    ui->labelLegend->setText(
        "<div style='line-height:1.6;'>"
        "<span style='color:#fbbf24; font-weight:700;'>Żółte</span> - wybrana jednostka<br/>"
        "<span style='color:#22c55e; font-weight:700;'>Zielone</span> - ruch<br/>"
        "<span style='color:#a855f7; font-weight:700;'>Fioletowe</span> - atak<br/>"
        "<span style='color:#38bdf8; font-weight:700;'>Niebieskie</span> - leczenie<br/>"
        "<span style='color:#9ca3af; font-weight:700;'>Szare</span> - blokada"
        "</div>"
        );

    ui->labelTileInfo->setText(buildDefaultTileInfoText());

    connect(ui->btnBackToMenu, &QPushButton::clicked,
            this, &BattlePage::backToMenuClicked);

    connect(ui->btnEndTurn, &QPushButton::clicked, this, [this]()
            {
                if (!m_controller)
                    return;

                m_controller->endTurn();
                updateTurnInfo();
                refreshStatistics();
                updateTileInfo();
                redrawBoard();
            });

    updateTurnInfo();
    refreshStatistics();
    updateTileInfo();
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
    updateTileInfo();

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
    TeamSide currentSide = TeamSide::Player;

    if (m_controller)
    {
        const GameState& gameState = m_controller->getGameState();
        currentTurn = gameState.getCurrentTurn();
        currentSide = gameState.getCurrentSide();
    }

    const bool isPlayerTurn = currentSide == TeamSide::Player;
    const QString sideText = isPlayerTurn ? "NIEBIESCY" : "CZERWONI";
    const QString sideColor = isPlayerTurn ? "#60a5fa" : "#f87171";

    ui->labelTurnNumber->setText(
        QString(
            "<div style='text-align:center;'>"
            "<div style='font-size:18px; font-weight:800; color:#f9fafb;'>Tura %1</div>"
            "<div style='font-size:14px; font-weight:700; color:%2; margin-top:4px;'>Ruch: %3</div>"
            "</div>"
            )
            .arg(currentTurn)
            .arg(sideColor)
            .arg(sideText)
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

    const int playerCount = gameState.getPlayerTeam().getAliveUnitsCount();
    const int enemyCount = gameState.getEnemyTeam().getAliveUnitsCount();

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

            const int moveCount = gameState.getAvailableMovePositions().size();
            const int attackCount = gameState.getAttackablePositions().size();
            const int healCount = gameState.getHealablePositions().size();

            ui->labelUnitStats->setText(
                QString(
                    "<div style='line-height:1.7;'>"
                    "<span style='color:#fca5a5; font-weight:700;'>HP:</span> %1 / %2<br/>"
                    "<span style='color:#fdba74; font-weight:700;'>Atak:</span> %3<br/>"
                    "<span style='color:#86efac; font-weight:700;'>Zasięg:</span> %4 - %5<br/>"
                    "<span style='color:#93c5fd; font-weight:700;'>Ruch:</span> do %6 pól<br/>"
                    "<span style='color:#fbbf24; font-weight:700;'>Koszt ataku:</span> %7 AP<br/>"
                    "<span style='color:#c4b5fd; font-weight:700;'>Pancerz:</span> %8<br/>"
                    "<span style='color:#f9a8d4; font-weight:700;'>Celność:</span> %9<br/>"
                    "<span style='color:#a7f3d0; font-weight:700;'>Unik:</span> %10<br/>"
                    "<span style='color:#38bdf8; font-weight:700;'>Leczenie:</span> %11<br/>"
                    "<span style='color:#d1d5db;'>Ruch wykonany wcześniej:</span> %12<br/>"
                    "<span style='color:#d1d5db;'>Atak / leczenie wcześniej:</span> %13<br/>"
                    "<span style='color:#22c55e; font-weight:700;'>Zielone pola:</span> %14<br/>"
                    "<span style='color:#a855f7; font-weight:700;'>Fioletowe cele ataku:</span> %15<br/>"
                    "<span style='color:#38bdf8; font-weight:700;'>Niebieskie cele leczenia:</span> %16<br/>"
                    "<span style='color:#cbd5e1;'>AP drużyny:</span> %17 / %18<br/><br/>"
                    "<span style='color:#9ca3af;'>Jak grać:</span> kliknij jednostkę, potem zielone pole, fioletowego przeciwnika albo niebieskiego sojusznika. Po ruchu możesz dalej atakować lub leczyć, jeśli masz AP."
                    "</div>"
                    )
                    .arg(unit->getHealth())
                    .arg(unit->getMaxHealth())
                    .arg(unit->getDamage())
                    .arg(unit->getMinRange())
                    .arg(unit->getRange())
                    .arg(unit->getMovementPoints())
                    .arg(unit->getAttackCost())
                    .arg(unit->getArmor())
                    .arg(unit->getAccuracy())
                    .arg(unit->getEvasion())
                    .arg(unit->canHeal() ? QString::number(unit->getHealAmount()) : "-")
                    .arg(unit->hasMovedThisTurn() ? "tak" : "nie")
                    .arg(unit->hasActedThisTurn() ? "tak" : "nie")
                    .arg(moveCount)
                    .arg(attackCount)
                    .arg(healCount)
                    .arg(gameState.getCurrentTurnActionPoints())
                    .arg(gameState.getMaxTurnActionPoints())
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
            "Ruch kosztuje tyle AP, ile wynika z terenu i ścieżki.<br/>"
            "Kliknij swoją jednostkę, a potem puste pole, niebieskiego sojusznika do leczenia albo fioletowego przeciwnika."
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
    updateTileInfo();
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
    redrawBoard();
}

void BattlePage::updateTileInfo()
{
    if (!m_controller)
    {
        ui->labelTileInfo->setText(buildDefaultTileInfoText());
        return;
    }

    const GameState& state = m_controller->getGameState();

    if (state.hasSelectedPosition())
    {
        const Tile* tile = state.getBoard().getTile(
            state.getSelectedX(),
            state.getSelectedY());

        ui->labelTileInfo->setText(buildTileInfoText(tile));
        return;
    }

    ui->labelTileInfo->setText(buildDefaultTileInfoText());
}
