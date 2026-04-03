#include "battlepage.h"
#include "ui_battlepage.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLayoutItem>
#include <QIcon>
#include <QString>
#include <QSize>
#include <QTimer>

BattlePage::BattlePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BattlePage)
{
    ui->setupUi(this);

    connect(ui->btnBackToMenu, &QPushButton::clicked,
            this, &BattlePage::backToMenuClicked);

    updateTurnInfo();
    refreshStatistics();

    QTimer::singleShot(0, this, [this]()
                       {
                           drawBoard();
                       });
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
}

void BattlePage::drawBoard()
{
    if (m_isDrawingBoard)
        return;

    auto *grid = ui->gridLayout_2;
    if (!grid)
        return;

    int boardSize = m_config.mapSize;

    // Obsługujemy tylko 10x10, 15x15 i 20x20
    if (boardSize != 10 && boardSize != 15 && boardSize != 20)
        boardSize = 10;

    int availableWidth = ui->boardContainer->width();
    int availableHeight = ui->boardContainer->height();

    // jeśli kontener jeszcze nie jest gotowy, nie rysujemy
    if (availableWidth < 50 || availableHeight < 50)
        return;

    int boardPixelSize = qMin(availableWidth, availableHeight);
    int tileSize = boardPixelSize / boardSize;

    // zabezpieczenie przed zbyt małymi polami
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

    int totalBoardSize = tileSize * boardSize;
    int horizontalMargin = (availableWidth - totalBoardSize) / 2;
    int verticalMargin = (availableHeight - totalBoardSize) / 2;

    if (horizontalMargin < 0)
        horizontalMargin = 0;

    if (verticalMargin < 0)
        verticalMargin = 0;

    grid->setContentsMargins(horizontalMargin, verticalMargin,
                             horizontalMargin, verticalMargin);

    for (int row = 0; row < boardSize; ++row)
    {
        for (int col = 0; col < boardSize; ++col)
        {
            auto *tile = new QPushButton(ui->boardContainer);
            tile->setFixedSize(tileSize, tileSize);
            tile->setFocusPolicy(Qt::NoFocus);
            tile->setText("");
            tile->setIcon(QIcon());

            QString terrainPath = ":/icons/images/terrain/grass.png";
            QString unitPath;

            // ===== TEREN =====
            if ((row == 1 && col == 3) ||
                (row == boardSize / 2 && col == boardSize / 2) ||
                (row == boardSize - 4 && col == boardSize / 2 + 1))
            {
                terrainPath = ":/icons/images/terrain/forest.png";
            }

            if (row == 2 && col == 2)
            {
                terrainPath = ":/icons/images/terrain/mountain.png";
            }

            if ((row == 0 && col == boardSize / 2) ||
                (row == boardSize - 5 && col == 1))
            {
                terrainPath = ":/icons/images/terrain/ocean.png";
            }

            // ===== JEDNOSTKI NIEBIESKIE =====
            if (row == 0 && col == 1)
                unitPath = ":/icons/images/units/soldier_blue.png";

            if (row == 1 && col == 2)
                unitPath = ":/icons/images/units/tank_blue.png";

            if (row == 3 && col == 0)
                unitPath = ":/icons/images/units/medic_blue.png";

            // ===== JEDNOSTKI CZERWONE =====
            if (row == 1 && col == boardSize - 2)
                unitPath = ":/icons/images/units/soldier_red.png";

            if (row == 3 && col == boardSize - 2)
                unitPath = ":/icons/images/units/tank_red.png";

            if (row == boardSize - 2 && col == 3)
                unitPath = ":/icons/images/units/medic_red.png";

            QString style =
                "QPushButton {"
                "background-color: #1f2937;"
                "border: 1px solid #dfe6ee;"
                "padding: 0px;"
                "margin: 0px;"
                "}";

            if (unitPath.isEmpty())
            {
                style = QString(
                            "QPushButton {"
                            "border-image: url(%1) 0 0 0 0 stretch stretch;"
                            "border: 1px solid #dfe6ee;"
                            "padding: 0px;"
                            "margin: 0px;"
                            "}"
                            ).arg(terrainPath);
            }
            else
            {
                tile->setIcon(QIcon(unitPath));
                tile->setIconSize(QSize(tileSize - 10, tileSize - 10));
            }

            tile->setStyleSheet(style);
            grid->addWidget(tile, row, col);
        }
    }

    m_isDrawingBoard = false;
}

void BattlePage::updateTurnInfo()
{
    ui->labelTurnNumber->setText("Tura 1");
}

void BattlePage::refreshStatistics()
{
    ui->labelUnitName->setText("Żołnierz");
    ui->labelUnitStats->setText("HP: 100\nAtak: 20\nZasięg: 2\nRuch: 3");
    ui->labelBattleLog->setText("Rozpoczęcie bitwy.\nNiebiescy wykonują pierwszy ruch.");
    ui->labelTeamsInfo->setText("Niebiescy: 3 | Czerwoni: 3");
}

void BattlePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    drawBoard();
}
