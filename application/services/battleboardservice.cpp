#include "battleboardservice.h"

#include <QPushButton>
#include <QLayoutItem>
#include <QObject>

#include "../models/board.h"
#include "../models/tile.h"
#include "../models/unit.h"
#include "../models/enums/terraintype.h"
#include "../models/enums/teamside.h"

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

void clearGrid(QGridLayout* grid)
{
    if (!grid)
        return;

    while (QLayoutItem *item = grid->takeAt(0))
    {
        if (item->widget())
            delete item->widget();

        delete item;
    }
}
}

void BattleBoardService::drawBoard(QGridLayout* grid,
                                   QWidget* boardContainer,
                                   const GameState& gameState,
                                   const GameConfig& config,
                                   const std::function<void(int, int)>& onTileClicked)
{
    if (!grid || !boardContainer)
        return;

    const Board& board = gameState.getBoard();

    int boardWidth = board.getWidth();
    int boardHeight = board.getHeight();

    if (boardWidth <= 0 || boardHeight <= 0)
    {
        boardWidth = config.mapSize;
        boardHeight = config.mapSize;
    }

    if ((boardWidth != 10 && boardWidth != 15 && boardWidth != 20) ||
        (boardHeight != 10 && boardHeight != 15 && boardHeight != 20))
    {
        boardWidth = 10;
        boardHeight = 10;
    }

    const int availableWidth = boardContainer->width();
    const int availableHeight = boardContainer->height();

    if (availableWidth < 50 || availableHeight < 50)
        return;

    const int boardPixelSize = qMin(availableWidth, availableHeight);
    const int tileSize = boardPixelSize / qMax(boardWidth, boardHeight);

    if (tileSize < 12)
        return;

    clearGrid(grid);

    grid->setSpacing(0);

    const int totalBoardWidth = tileSize * boardWidth;
    const int totalBoardHeight = tileSize * boardHeight;

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
            auto *tileButton = new QPushButton(boardContainer);
            tileButton->setFixedSize(tileSize, tileSize);
            tileButton->setFocusPolicy(Qt::NoFocus);
            tileButton->setText("");

            QString terrainPath = ":/icons/images/terrain/grass.png";
            QString unitPath;

            const Tile* tile = gameState.getBoard().getTile(col, row);
            if (tile)
            {
                terrainPath = getTerrainPath(tile->getTerrain());

                if (tile->isOccupied())
                    unitPath = getUnitIconPath(tile->getUnit());
            }

            const QString imagePath = unitPath.isEmpty() ? terrainPath : unitPath;

            const bool isSelected =
                gameState.hasSelectedPosition() &&
                gameState.getSelectedX() == col &&
                gameState.getSelectedY() == row;

            const bool isAvailableMove = gameState.isMovePositionAvailable(col, row);

            QString borderStyle = "border: 1px solid #dfe6ee;";

            if (isAvailableMove)
                borderStyle = "border: 3px solid #4ade80;";

            if (isSelected)
                borderStyle = "border: 3px solid #ffd54f;";

            const QString style = QString(
                                      "QPushButton {"
                                      "border-image: url(%1) 0 0 0 0 stretch stretch;"
                                      "%2"
                                      "padding: 0px;"
                                      "margin: 0px;"
                                      "}"
                                      ).arg(imagePath, borderStyle);

            tileButton->setStyleSheet(style);

            QObject::connect(tileButton, &QPushButton::clicked, boardContainer, [onTileClicked, col, row]()
                             {
                                 if (onTileClicked)
                                     onTileClicked(col, row);
                             });

            grid->addWidget(tileButton, row, col);
        }
    }
}
