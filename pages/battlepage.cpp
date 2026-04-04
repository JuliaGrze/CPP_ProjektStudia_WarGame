#include "battlepage.h"
#include "ui_battlepage.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLayoutItem>
#include <QIcon>
#include <QString>
#include <QSize>
#include <QTimer>

#include "../application/models/board.h"
#include "../application/models/tile.h"
#include "../application/models/unit.h"
#include "../application/models/team.h"
#include "../application/models/enums/terraintype.h"
#include "../application/models/enums/teamside.h"

namespace
{
QString getTerrainPath(TerrainType terrain)
{
    switch (terrain)
    {
    case TerrainType::Plain:
        return ":/icons/images/terrain/grass.png";
    case TerrainType::Forest:
        return ":/icons/images/terrain/forest.png";
    case TerrainType::Mountain:
        return ":/icons/images/terrain/mountain.png";
    case TerrainType::Water:
        return ":/icons/images/terrain/ocean.png";
    case TerrainType::Building:
        return ":/icons/images/terrain/mountain.png"; // tymczasowo
    }

    return ":/icons/images/terrain/grass.png";
}

QString getUnitIconPath(const Unit* unit)
{
    if (!unit)
        return "";

    const bool isPlayer = unit->getSide() == TeamSide::Player;
    const QString type = unit->getUnitType();

    if (type == "Infantry")
        return isPlayer ? ":/icons/images/units/soldier_blue.png"
                        : ":/icons/images/units/soldier_red.png";

    if (type == "Tank")
        return isPlayer ? ":/icons/images/units/tank_blue.png"
                        : ":/icons/images/units/tank_red.png";

    if (type == "Medic")
        return isPlayer ? ":/icons/images/units/medic_blue.png"
                        : ":/icons/images/units/medic_red.png";

    if (type == "Artillery")
        return isPlayer ? ":/icons/images/units/artillery_blue.png"
                        : ":/icons/images/units/artillery_red.png";

    return "";
}
}

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
                           drawBoard();
                       });
}

void BattlePage::setGameState(const GameState& gameState)
{
    m_gameState = gameState;
    updateTurnInfo();
    refreshStatistics();

    QTimer::singleShot(0, this, [this]()
                       {
                           drawBoard();
                       });
}

void BattlePage::drawBoard()
{
    if (m_isDrawingBoard)
        return;

    auto *grid = ui->gridLayout_2;
    if (!grid)
        return;

    const Board& board = m_gameState.getBoard();

    int boardWidth = board.getWidth();
    int boardHeight = board.getHeight();

    if (boardWidth <= 0 || boardHeight <= 0)
    {
        boardWidth = m_config.mapSize;
        boardHeight = m_config.mapSize;
    }

    if ((boardWidth != 10 && boardWidth != 15 && boardWidth != 20) ||
        (boardHeight != 10 && boardHeight != 15 && boardHeight != 20))
    {
        boardWidth = 10;
        boardHeight = 10;
    }

    int availableWidth = ui->boardContainer->width();
    int availableHeight = ui->boardContainer->height();

    if (availableWidth < 50 || availableHeight < 50)
        return;

    int boardPixelSize = qMin(availableWidth, availableHeight);
    int tileSize = boardPixelSize / qMax(boardWidth, boardHeight);

    if (tileSize < 12)
        return;

    m_isDrawingBoard = true;

    while (QLayoutItem *item = grid->takeAt(0))
    {
        if (item->widget())
            delete item->widget();
        delete item;
    }

    grid->setSpacing(0);

    int totalBoardWidth = tileSize * boardWidth;
    int totalBoardHeight = tileSize * boardHeight;

    int horizontalMargin = (availableWidth - totalBoardWidth) / 2;
    int verticalMargin = (availableHeight - totalBoardHeight) / 2;

    if (horizontalMargin < 0)
        horizontalMargin = 0;

    if (verticalMargin < 0)
        verticalMargin = 0;

    grid->setContentsMargins(horizontalMargin, verticalMargin,
                             horizontalMargin, verticalMargin);

    for (int row = 0; row < boardHeight; ++row)
    {
        for (int col = 0; col < boardWidth; ++col)
        {
            auto *tileButton = new QPushButton(ui->boardContainer);
            tileButton->setFixedSize(tileSize, tileSize);
            tileButton->setFocusPolicy(Qt::NoFocus);
            tileButton->setText("");

            QString terrainPath = ":/icons/images/terrain/grass.png";
            QString unitPath;

            const Tile* tile = m_gameState.getBoard().getTile(col, row);
            if (tile)
            {
                terrainPath = getTerrainPath(tile->getTerrain());

                if (tile->isOccupied())
                    unitPath = getUnitIconPath(tile->getUnit());
            }

            QString imagePath = unitPath.isEmpty() ? terrainPath : unitPath;

            QString style = QString(
                                "QPushButton {"
                                "border-image: url(%1) 0 0 0 0 stretch stretch;"
                                "border: 1px solid #dfe6ee;"
                                "padding: 0px;"
                                "margin: 0px;"
                                "}")
                                .arg(imagePath);

            tileButton->setStyleSheet(style);
            grid->addWidget(tileButton, row, col);
        }
    }

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
    drawBoard();
}

void BattlePage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    QTimer::singleShot(0, this, [this]()
                       {
                           drawBoard();
                       });
}
