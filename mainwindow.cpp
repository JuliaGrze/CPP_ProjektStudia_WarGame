#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pages/menupage.h"
#include "pages/configpage.h"
#include "pages/battlepage.h"

#include "application/models/board.h"
#include "application/models/tile.h"
#include "application/models/enums/terraintype.h"
#include "application/helpers/unitplacementhelper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , menuPage(new MenuPage(this))
    , configPage(new ConfigPage(this))
    , battlePage(new BattlePage(this))
{
    ui->setupUi(this);

    setMinimumSize(1100, 900);

    ui->stackedWidget->addWidget(menuPage);
    ui->stackedWidget->addWidget(configPage);
    ui->stackedWidget->addWidget(battlePage);

    ui->stackedWidget->setCurrentWidget(menuPage);

    connect(menuPage, &MenuPage::playClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(configPage);
            });

    connect(menuPage, &MenuPage::exitClicked, this, &MainWindow::close);

    connect(configPage, &ConfigPage::backClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(menuPage);
            });

    connect(configPage, &ConfigPage::startClicked, this, [this](const GameConfig& config)
            {
                GameState state = createTestGameState(config);
                battlePage->setGameState(state);

                ui->stackedWidget->setCurrentWidget(battlePage);
            });

    connect(battlePage, &BattlePage::backToMenuClicked, this, [this]()
            {
                ui->stackedWidget->setCurrentWidget(menuPage);
            });
}

MainWindow::~MainWindow()
{
    delete ui;
}

GameState MainWindow::createTestGameState(const GameConfig& config)
{
    GameState state;

    int boardSize = config.mapSize;
    if (boardSize != 10 && boardSize != 15 && boardSize != 20)
        boardSize = 10;

    Board board(boardSize, boardSize);

    // ===== TEREN =====
    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = 0; x < board.getWidth(); ++x)
        {
            Tile* tile = board.getTile(x, y);
            if (tile)
                tile->setTerrain(TerrainType::Plain);
        }
    }

    if (config.mapVariant == "Las i wzgórza")
    {
        for (int i = 1; i < boardSize - 1; i += 3)
        {
            Tile* forestTile = board.getTile(boardSize / 3, i);
            Tile* mountainTile = board.getTile((boardSize / 3) * 2, i);

            if (forestTile)
                forestTile->setTerrain(TerrainType::Forest);

            if (mountainTile)
                mountainTile->setTerrain(TerrainType::Mountain);
        }
    }
    else if (config.mapVariant == "Rzeka i most")
    {
        int riverColumn = boardSize / 2;

        for (int y = 0; y < boardSize; ++y)
        {
            Tile* waterTile = board.getTile(riverColumn, y);
            if (waterTile)
                waterTile->setTerrain(TerrainType::Water);
        }

        Tile* bridgeTile1 = board.getTile(riverColumn, boardSize / 2);
        Tile* bridgeTile2 = board.getTile(riverColumn, boardSize / 2 - 1);

        if (bridgeTile1)
            bridgeTile1->setTerrain(TerrainType::Plain);

        if (bridgeTile2)
            bridgeTile2->setTerrain(TerrainType::Plain);
    }
    else if (config.mapVariant == "Zabudowania")
    {
        for (int y = 2; y < boardSize - 2; y += 4)
        {
            for (int x = boardSize / 2 - 1; x <= boardSize / 2 + 1; ++x)
            {
                Tile* buildingTile = board.getTile(x, y);
                if (buildingTile)
                    buildingTile->setTerrain(TerrainType::Building);
            }
        }
    }

    // ===== ROZMIESZCZENIE JEDNOSTEK =====
    UnitPlacementHelper::placeTeams(
        board,
        state.getPlayerTeam(),
        state.getEnemyTeam(),
        config.playerTeam,
        config.enemyTeam
        );

    state.setBoard(board);

    return state;
}
