#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "pages/menupage.h"
#include "pages/configpage.h"
#include "pages/battlepage.h"

#include "application/models/board.h"
#include "application/models/tile.h"
#include "application/models/infantry.h"
#include "application/models/tank.h"
#include "application/models/artillery.h"
#include "application/models/medic.h"
#include "application/models/enums/terraintype.h"
#include "application/models/enums/teamside.h"

#include <memory>

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
            board.getTile(boardSize / 3, i)->setTerrain(TerrainType::Forest);
            board.getTile((boardSize / 3) * 2, i)->setTerrain(TerrainType::Mountain);
        }
    }
    else if (config.mapVariant == "Rzeka i most")
    {
        int riverColumn = boardSize / 2;

        for (int y = 0; y < boardSize; ++y)
            board.getTile(riverColumn, y)->setTerrain(TerrainType::Water);

        board.getTile(riverColumn, boardSize / 2)->setTerrain(TerrainType::Plain);
        board.getTile(riverColumn, boardSize / 2 - 1)->setTerrain(TerrainType::Plain);
    }
    else if (config.mapVariant == "Zabudowania")
    {
        for (int y = 2; y < boardSize - 2; y += 4)
        {
            for (int x = boardSize / 2 - 1; x <= boardSize / 2 + 1; ++x)
                board.getTile(x, y)->setTerrain(TerrainType::Building);
        }
    }

    // ===== POZYCJE STARTOWE =====
    QVector<QPair<int, int>> playerPositions;
    QVector<QPair<int, int>> enemyPositions;

    for (int y = 1; y < boardSize - 1; ++y)
    {
        playerPositions.append({1, y});
        playerPositions.append({2, y});

        enemyPositions.append({boardSize - 2, y});
        enemyPositions.append({boardSize - 3, y});
    }

    int playerIndex = 0;
    int enemyIndex = 0;

    // ===== FUNKCJA TWORZENIA JEDNOSTEK =====
    auto createAndPlaceUnits = [&](const TeamComposition& comp,
                                   Team& team,
                                   TeamSide side,
                                   const QVector<QPair<int,int>>& positions,
                                   int& index)
    {
        auto addUnit = [&](std::shared_ptr<Unit> unit)
        {
            team.addUnit(unit);

            while (index < positions.size())
            {
                int x = positions[index].first;
                int y = positions[index].second;
                ++index;

                Tile* tile = board.getTile(x, y);
                if (tile && !tile->isOccupied() && tile->isWalkable())
                {
                    tile->setUnit(unit.get());
                    break;
                }
            }
        };

        for (int i = 0; i < comp.infantry; ++i)
            addUnit(std::make_shared<Infantry>(side));

        for (int i = 0; i < comp.tank; ++i)
            addUnit(std::make_shared<Tank>(side));

        for (int i = 0; i < comp.artillery; ++i)
            addUnit(std::make_shared<Artillery>(side));

        for (int i = 0; i < comp.medic; ++i)
            addUnit(std::make_shared<Medic>(side));
    };

    // ===== TWORZENIE DRUŻYN =====
    createAndPlaceUnits(config.playerTeam,
                        state.getPlayerTeam(),
                        TeamSide::Player,
                        playerPositions,
                        playerIndex);

    createAndPlaceUnits(config.enemyTeam,
                        state.getEnemyTeam(),
                        TeamSide::Enemy,
                        enemyPositions,
                        enemyIndex);

    state.setBoard(board);

    return state;
}
