#include "battlepage.h"
#include "ui_battlepage.h"

#include <QPushButton>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QLabel>

#include "../application/controllers/gamecontroller.h"
#include "../application/models/gamestate.h"
#include "../application/models/team.h"
#include "../application/models/tile.h"
#include "../application/models/unit.h"
#include "../application/models/enums/teamside.h"
#include "../application/services/battleboardservice.h"
#include "../application/helpers/battleuiformatter.h"
#include "../config/gameconfig.h"

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
            });

    connect(ui->btnShowBattleLog, &QPushButton::clicked, this, [this]()
            {
                if (!m_controller)
                    return;

                const QStringList log = m_controller->getGameState().getBattleLog();

                QDialog dialog(this);
                dialog.setWindowTitle("Log bitwy");
                dialog.resize(700, 500);

                QVBoxLayout* layout = new QVBoxLayout(&dialog);

                QPlainTextEdit* textEdit = new QPlainTextEdit(&dialog);
                textEdit->setReadOnly(true);
                textEdit->setPlainText(log.join("\n"));

                QPushButton* btnClose = new QPushButton("Zamknij", &dialog);
                connect(btnClose, &QPushButton::clicked, &dialog, &QDialog::accept);

                layout->addWidget(textEdit);
                layout->addWidget(btnClose);

                dialog.exec();
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
    if (m_controller)
    {
        disconnect(m_controller, nullptr, this, nullptr);
    }

    m_controller = controller;
    m_postGameDialogShown = false;

    if (m_controller)
    {
        connect(m_controller, &GameController::stateChanged,
                this, &BattlePage::refreshView);
    }

    QTimer::singleShot(0, this, [this]()
                       {
                           refreshView();
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
    bool gameFinished = false;
    TeamSide winnerSide = TeamSide::Player;

    if (m_controller)
    {
        const GameState& gameState = m_controller->getGameState();
        currentTurn = gameState.getCurrentTurn();
        currentSide = gameState.getCurrentSide();
        gameFinished = gameState.isGameFinished();
        winnerSide = gameState.getWinnerSide();
    }

    ui->labelTurnNumber->setText(
        BattleUiFormatter::buildTurnInfoHtml(
            currentTurn,
            currentSide,
            gameFinished,
            winnerSide
            )
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
        ui->labelUnitName->setText(BattleUiFormatter::buildNoUnitSelectedNameHtml());
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

    const TeamBattleStats playerStats = gameState.getStatsForSide(TeamSide::Player);
    const TeamBattleStats enemyStats = gameState.getStatsForSide(TeamSide::Enemy);

    ui->labelBattleLog->setText(gameState.getLastActionMessage());

    if (gameState.isGameFinished() && !m_postGameDialogShown)
    {
        m_postGameDialogShown = true;
        QTimer::singleShot(0, this, [this]()
                           {
                               showPostGameSummaryDialog();
                           });
    }

    ui->labelTeamsInfo->setText(
        BattleUiFormatter::buildTeamsInfoHtml(
            playerCount,
            enemyCount,
            gameState.getCurrentSide(),
            gameState.getCurrentTurnActionPoints(),
            gameState.getMaxTurnActionPoints(),
            playerStats,
            enemyStats,
            gameState.getAccuracyPercent(TeamSide::Player),
            gameState.getAccuracyPercent(TeamSide::Enemy)
            )
        );

    if (gameState.hasSelectedPosition() && !gameState.isGameFinished())
    {
        const Tile* tile = gameState.getBoard().getTile(
            gameState.getSelectedX(),
            gameState.getSelectedY()
            );

        if (tile && tile->isOccupied() && tile->getUnit())
        {
            const Unit* unit = tile->getUnit();

            ui->labelUnitName->setText(
                BattleUiFormatter::buildSelectedUnitNameHtml(unit)
                );

            const int moveCount = gameState.getAvailableMovePositions().size();
            const int attackCount = gameState.getAttackablePositions().size();
            const int healCount = gameState.getHealablePositions().size();

            ui->labelUnitStats->setText(
                BattleUiFormatter::buildSelectedUnitStatsHtml(
                    unit,
                    moveCount,
                    attackCount,
                    healCount,
                    gameState.getCurrentTurnActionPoints(),
                    gameState.getMaxTurnActionPoints(),
                    gameState.getCurrentTurn()
                    )
                );

            return;
        }
    }

    ui->labelUnitName->setText(
        BattleUiFormatter::buildNoUnitSelectedNameHtml()
        );

    ui->labelUnitStats->setText(
        BattleUiFormatter::buildNoUnitSelectedStatsHtml(
            gameState.getCurrentTurnActionPoints(),
            gameState.getMaxTurnActionPoints(),
            gameState.getCurrentTurn(),
            playerStats,
            enemyStats
            )
        );
}

void BattlePage::showPostGameSummaryDialog()
{
    if (!m_controller)
        return;

    const GameState& gameState = m_controller->getGameState();

    QDialog dialog(this);
    dialog.setWindowTitle("Podsumowanie bitwy");
    dialog.resize(920, 640);
    dialog.setModal(true);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(24, 24, 24, 20);
    layout->setSpacing(18);

    QLabel* summaryLabel = new QLabel(&dialog);
    summaryLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    summaryLabel->setWordWrap(true);
    summaryLabel->setText(
        BattleUiFormatter::buildPostGameSummaryHtml(gameState)
        );

    QPushButton* btnMenu = new QPushButton("Powrót do menu", &dialog);
    QPushButton* btnPlayAgain = new QPushButton("Zagraj ponownie", &dialog);
    QPushButton* btnNewConfig = new QPushButton("Nowa konfiguracja", &dialog);

    btnMenu->setMinimumHeight(46);
    btnPlayAgain->setMinimumHeight(46);
    btnNewConfig->setMinimumHeight(46);

    QString selectedAction = "menu";

    connect(btnMenu, &QPushButton::clicked, &dialog, [&dialog, &selectedAction]()
            {
                selectedAction = "menu";
                dialog.accept();
            });

    connect(btnPlayAgain, &QPushButton::clicked, &dialog, [&dialog, &selectedAction]()
            {
                selectedAction = "again";
                dialog.accept();
            });

    connect(btnNewConfig, &QPushButton::clicked, &dialog, [&dialog, &selectedAction]()
            {
                selectedAction = "config";
                dialog.accept();
            });

    QHBoxLayout* buttonsLayout = new QHBoxLayout();
    buttonsLayout->setSpacing(12);
    buttonsLayout->addWidget(btnMenu);
    buttonsLayout->addWidget(btnPlayAgain);
    buttonsLayout->addWidget(btnNewConfig);

    layout->addWidget(summaryLabel, 1);
    layout->addLayout(buttonsLayout);

    dialog.setStyleSheet(
        "QDialog { background-color: #1f2937; }"
        "QLabel { color: white; background: transparent; }"
        "QPushButton {"
        "   background-color: #374151;"
        "   color: white;"
        "   border: 1px solid #4b5563;"
        "   padding: 8px 16px;"
        "   border-radius: 10px;"
        "   font-weight: 700;"
        "   font-size: 15px;"
        "}"
        "QPushButton:hover { background-color: #4b5563; }"
        "QPushButton:pressed { background-color: #111827; }"
        );

    dialog.exec();

    if (selectedAction == "again")
    {
        emit playAgainClicked();
    }
    else if (selectedAction == "config")
    {
        emit newConfigurationClicked();
    }
    else
    {
        emit backToMenuClicked();
    }
}

void BattlePage::onTileClicked(int x, int y)
{
    if (!m_controller)
        return;

    m_controller->handleTileClick(x, y);
}

void BattlePage::updateTileInfo()
{
    if (!m_controller)
    {
        ui->labelTileInfo->setText(BattleUiFormatter::buildDefaultTileInfoText());
        return;
    }

    const GameState& gameState = m_controller->getGameState();

    if (!gameState.hasSelectedPosition())
    {
        ui->labelTileInfo->setText(BattleUiFormatter::buildDefaultTileInfoText());
        return;
    }

    const Tile* tile = gameState.getBoard().getTile(
        gameState.getSelectedX(),
        gameState.getSelectedY()
        );

    ui->labelTileInfo->setText(BattleUiFormatter::buildTileInfoText(tile));
}

void BattlePage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QTimer::singleShot(0, this, [this]() { redrawBoard(); });
}

void BattlePage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    QTimer::singleShot(0, this, [this]() { redrawBoard(); });
}

void BattlePage::refreshView()
{
    updateTurnInfo();
    refreshStatistics();
    updateTileInfo();
    redrawBoard();
}
