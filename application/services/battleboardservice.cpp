#include "battleboardservice.h"

#include <QPushButton>
#include <QLayoutItem>
#include <QObject>
#include <QFrame>
#include <QPixmap>
#include <QIcon>
#include <QSize>

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
        return ":/icons/images/terrain/building .png";
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

    while (QLayoutItem* item = grid->takeAt(0))
    {
        if (item->widget())
            delete item->widget();

        delete item;
    }
}

QString buildOverlayStyle(bool isSelected,
                          bool isAvailableMove,
                          bool isBlockedMove,
                          bool isAttackable,
                          bool isHealable)
{
    QString borderStyle = "border: none;";
    QString backgroundStyle = "background-color: transparent;";

    if (isAvailableMove)
    {
        borderStyle = "border: 5px solid rgb(22, 163, 74);";
        backgroundStyle = "background-color: rgba(34, 197, 94, 120);";
    }
    else if (isAttackable)
    {
        borderStyle = "border: 5px solid rgb(147, 51, 234);";
        backgroundStyle = "background-color: rgba(168, 85, 247, 170);";
    }
    else if (isHealable)
    {
        borderStyle = "border: 5px solid rgb(14, 165, 233);";
        backgroundStyle = "background-color: rgba(56, 189, 248, 135);";
    }
    else if (isBlockedMove)
    {
        borderStyle = "border: 4px dashed rgb(107, 114, 128);";
        backgroundStyle = "background-color: rgba(75, 85, 99, 125);";
    }

    if (isSelected)
    {
        borderStyle = "border: 5px solid rgb(250, 204, 21);";
        backgroundStyle = "background-color: rgba(255, 213, 79, 115);";
    }

    return QString(
               "QFrame {"
               "%1"
               "%2"
               "margin: 0px;"
               "padding: 0px;"
               "}"
               ).arg(borderStyle, backgroundStyle);
}

void setButtonImage(QPushButton* button, const QString& imagePath, int tileSize)
{
    if (!button)
        return;

    QPixmap pixmap(imagePath);
    if (pixmap.isNull())
    {
        button->setText("");
        return;
    }

    button->setIcon(QIcon(pixmap));
    button->setIconSize(QSize(tileSize, tileSize));
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

    const bool hasSelection = gameState.hasSelectedPosition();

    for (int row = 0; row < boardHeight; ++row)
    {
        for (int col = 0; col < boardWidth; ++col)
        {
            auto* tileButton = new QPushButton(boardContainer);
            tileButton->setFixedSize(tileSize, tileSize);
            tileButton->setFocusPolicy(Qt::NoFocus);
            tileButton->setText("");
            tileButton->setFlat(true);

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

            setButtonImage(tileButton, imagePath, tileSize);

            tileButton->setStyleSheet(
                "QPushButton {"
                "border: none;"
                "padding: 0px;"
                "margin: 0px;"
                "background-color: transparent;"
                "}"
                );

            const bool isSelected =
                hasSelection &&
                gameState.getSelectedX() == col &&
                gameState.getSelectedY() == row;

            const bool isAvailableMove =
                hasSelection && gameState.isMovePositionAvailable(col, row);

            const bool isBlockedMove =
                hasSelection && gameState.isBlockedMovePosition(col, row);

            const bool isAttackable =
                hasSelection && gameState.isAttackablePosition(col, row);

            const bool isHealable =
                hasSelection && gameState.isHealablePosition(col, row);

            auto* overlay = new QFrame(tileButton);
            overlay->setGeometry(0, 0, tileSize, tileSize);
            overlay->setAttribute(Qt::WA_TransparentForMouseEvents, true);
            overlay->setStyleSheet(buildOverlayStyle(isSelected,
                                                     isAvailableMove,
                                                     isBlockedMove,
                                                     isAttackable,
                                                     isHealable));
            overlay->raise();

            QObject::connect(tileButton, &QPushButton::clicked, boardContainer, [onTileClicked, col, row]()
                             {
                                 if (onTileClicked)
                                     onTileClicked(col, row);
                             });

            grid->addWidget(tileButton, row, col);
        }
    }
}
