#include "battlepage.h"
#include "ui_battlepage.h"

#include <QGridLayout>
#include <QPushButton>
#include <QLayoutItem>
#include <QString>

BattlePage::BattlePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::BattlePage)
{
    ui->setupUi(this);

    connect(ui->btnBackToMenu, &QPushButton::clicked,
            this, &BattlePage::backToMenuClicked);

    drawBoard();
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
    drawBoard();
    updateTurnInfo();
    refreshStatistics();
}

void BattlePage::drawBoard()
{
    auto *grid = ui->gridLayout_2;
    if (!grid)
        return;

    while (QLayoutItem *item = grid->takeAt(0))
    {
        if (item->widget())
            delete item->widget();
        delete item;
    }

    int boardSize = 8;

    if (m_config.mapSize > 0)
        boardSize = m_config.mapSize;

    if (boardSize < 6)
        boardSize = 6;

    if (boardSize > 12)
        boardSize = 12;

    grid->setSpacing(2);
    grid->setContentsMargins(8, 8, 8, 8);

    int tileSize = 42;
    if (boardSize >= 10)
        tileSize = 34;
    if (boardSize >= 12)
        tileSize = 28;

    for (int row = 0; row < boardSize; ++row)
    {
        for (int col = 0; col < boardSize; ++col)
        {
            auto *tile = new QPushButton(this);
            tile->setFixedSize(tileSize, tileSize);
            tile->setFocusPolicy(Qt::NoFocus);

            QString style =
                "QPushButton {"
                "background-color: #0f172a;"
                "border: 1px solid #e5e7eb;"
                "color: white;"
                "font-weight: bold;"
                "padding: 0px;"
                "}";

            QString text;

            // przykładowe jednostki niebieskie
            if ((row == 0 && col == 1) ||
                (row == 1 && col == 2) ||
                (row == 3 && col == 0))
            {
                text = "B";
                style =
                    "QPushButton {"
                    "background-color: #1d4ed8;"
                    "border: 1px solid #e5e7eb;"
                    "color: white;"
                    "font-weight: bold;"
                    "padding: 0px;"
                    "}";
            }

            // przykładowe jednostki czerwone
            if ((row == 1 && col == boardSize - 2) ||
                (row == 3 && col == boardSize - 2) ||
                (row == boardSize - 2 && col == 3))
            {
                text = "C";
                style =
                    "QPushButton {"
                    "background-color: #b91c1c;"
                    "border: 1px solid #e5e7eb;"
                    "color: white;"
                    "font-weight: bold;"
                    "padding: 0px;"
                    "}";
            }

            // przykładowe przeszkody / teren
            if ((row == 1 && col == 3) ||
                (row == 4 && col == 5))
            {
                text = "";
                style =
                    "QPushButton {"
                    "background-color: #4d7c0f;"
                    "border: 1px solid #e5e7eb;"
                    "color: white;"
                    "font-weight: bold;"
                    "padding: 0px;"
                    "}";
            }

            tile->setText(text);
            tile->setStyleSheet(style);

            grid->addWidget(tile, row, col);
        }
    }
}

void BattlePage::updateTurnInfo()
{
    ui->labelTurnNumber->setText("Tura 1");
}

void BattlePage::refreshStatistics()
{
    ui->labelUnitName->setText("Piechota");
    ui->labelUnitStats->setText("HP: 100\nAtak: 20\nZasięg: 2\nRuch: 3");
    ui->labelBattleLog->setText("Rozpoczęcie bitwy.\nNiebiescy wykonują pierwszy ruch.");
    ui->labelTeamsInfo->setText("Niebiescy: 3 | Czerwoni: 3");
}
