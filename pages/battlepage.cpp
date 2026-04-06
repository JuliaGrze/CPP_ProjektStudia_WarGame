#include "battlepage.h"
#include "ui_battlepage.h"

#include <QPushButton>
#include <QTimer>
#include <QDialog>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QLabel>

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
    m_controller = controller;
    m_postGameDialogShown = false;

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

    if (gameFinished)
    {
        const bool playerWon = winnerSide == TeamSide::Player;
        const QString winnerText = playerWon ? "ZWYCIĘSTWO: NIEBIESCY" : "ZWYCIĘSTWO: CZERWONI";
        const QString winnerColor = playerWon ? "#60a5fa" : "#f87171";

        ui->labelTurnNumber->setText(
            QString(
                "<div style='text-align:center;'>"
                "<div style='font-size:18px; font-weight:800; color:#f9fafb;'>Koniec gry</div>"
                "<div style='font-size:14px; font-weight:800; color:%1; margin-top:4px;'>%2</div>"
                "<div style='font-size:12px; color:#d1d5db; margin-top:4px;'>Liczba tur: %3</div>"
                "</div>"
                )
                .arg(winnerColor)
                .arg(winnerText)
                .arg(currentTurn)
            );
        return;
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

    const bool isPlayerTurn = gameState.getCurrentSide() == TeamSide::Player;
    const QString currentSideText = isPlayerTurn ? "NIEBIESCY" : "CZERWONI";
    const QString currentSideColor = isPlayerTurn ? "#60a5fa" : "#f87171";

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
            "<br/><br/>"
            "<span style='color:#60a5fa; font-weight:700;'>Niebiescy trafienia:</span> %7 / %8 (%9%%)"
            "<br/>"
            "<span style='color:#f87171; font-weight:700;'>Czerwoni trafienia:</span> %10 / %11 (%12%%)"
            "</div>"
            )
            .arg(playerCount)
            .arg(enemyCount)
            .arg(currentSideColor)
            .arg(currentSideText)
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints())
            .arg(playerStats.hits)
            .arg(playerStats.shotsFired)
            .arg(gameState.getAccuracyPercent(TeamSide::Player))
            .arg(enemyStats.hits)
            .arg(enemyStats.shotsFired)
            .arg(gameState.getAccuracyPercent(TeamSide::Enemy))
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
                    "<span style='color:#cbd5e1;'>AP drużyny:</span> %17 / %18<br/>"
                    "<span style='color:#fbbf24;'>Tura:</span> %19"
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
                    .arg(gameState.getCurrentTurn())
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
            "Tura: <b>%3</b><br/>"
            "Niebiescy trafienia: <b>%4 / %5</b><br/>"
            "Czerwoni trafienia: <b>%6 / %7</b><br/>"
            "Kliknij swoją jednostkę, a potem puste pole, niebieskiego sojusznika do leczenia albo fioletowego przeciwnika."
            "</div>"
            )
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints())
            .arg(gameState.getCurrentTurn())
            .arg(playerStats.hits)
            .arg(playerStats.shotsFired)
            .arg(enemyStats.hits)
            .arg(enemyStats.shotsFired)
        );
}
void BattlePage::showPostGameSummaryDialog()
{
    if (!m_controller)
        return;

    const GameState& gameState = m_controller->getGameState();

    const TeamBattleStats playerStats = gameState.getStatsForSide(TeamSide::Player);
    const TeamBattleStats enemyStats = gameState.getStatsForSide(TeamSide::Enemy);

    const bool playerWon = gameState.getWinnerSide() == TeamSide::Player;
    const QString winnerColor = playerWon ? "#60a5fa" : "#f87171";
    const QString winnerText = playerWon ? "Wygrywają Niebiescy" : "Wygrywają Czerwoni";

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
        QString(
            "<div style='font-family:Segoe UI, Arial, sans-serif; width:100%%;'>"

            "<div style='text-align:center; margin-bottom:20px;'>"
            "<div style='font-size:30px; font-weight:900; color:#f9fafb;'>PODSUMOWANIE BITWY</div>"
            "<div style='margin-top:10px; font-size:20px; font-weight:800; color:%1;'>%2</div>"
            "<div style='margin-top:8px; font-size:16px; color:#fbbf24; font-weight:800;'>Liczba tur: %3</div>"
            "</div>"

            "<div style='text-align:center;'>"
            "<table style='margin-left:auto; margin-right:auto; border-collapse:separate; border-spacing:22px 0;'>"
            "<tr>"

            "<td valign='top' "
            "style='min-width:300px; max-width:300px; background-color:#111827; border:2px solid #2563eb; border-radius:16px; padding:18px 22px;'>"
            "<div style='text-align:center; font-size:24px; font-weight:900; color:#60a5fa; margin-bottom:14px;'>NIEBIESCY</div>"
            "<div style='font-size:18px; line-height:2.0; color:#e5e7eb;'>"
            "<b style='color:#93c5fd;'>Straty:</b> %4<br/>"
            "<b style='color:#93c5fd;'>Zadane obrażenia:</b> %5<br/>"
            "<b style='color:#93c5fd;'>Trafienia:</b> %6 / %7<br/>"
            "<b style='color:#93c5fd;'>Skuteczność:</b> %8%%<br/>"
            "<b style='color:#93c5fd;'>Zniszczone jednostki:</b> %9"
            "</div>"
            "</td>"

            "<td valign='top' "
            "style='min-width:300px; max-width:300px; background-color:#111827; border:2px solid #dc2626; border-radius:16px; padding:18px 22px;'>"
            "<div style='text-align:center; font-size:24px; font-weight:900; color:#f87171; margin-bottom:14px;'>CZERWONI</div>"
            "<div style='font-size:18px; line-height:2.0; color:#e5e7eb;'>"
            "<b style='color:#fca5a5;'>Straty:</b> %10<br/>"
            "<b style='color:#fca5a5;'>Zadane obrażenia:</b> %11<br/>"
            "<b style='color:#fca5a5;'>Trafienia:</b> %12 / %13<br/>"
            "<b style='color:#fca5a5;'>Skuteczność:</b> %14%%<br/>"
            "<b style='color:#fca5a5;'>Zniszczone jednostki:</b> %15"
            "</div>"
            "</td>"

            "</tr>"
            "</table>"
            "</div>"

            "<div style='margin-top:24px; text-align:center; color:#9ca3af; font-size:14px;'>"
            "Wybierz, co chcesz zrobić dalej."
            "</div>"

            "</div>"
            )
            .arg(winnerColor)
            .arg(winnerText)
            .arg(gameState.getCurrentTurn())
            .arg(gameState.getLossesForSide(TeamSide::Player))
            .arg(playerStats.damageDealt)
            .arg(playerStats.hits)
            .arg(playerStats.shotsFired)
            .arg(gameState.getAccuracyPercent(TeamSide::Player))
            .arg(playerStats.unitsDestroyed)
            .arg(gameState.getLossesForSide(TeamSide::Enemy))
            .arg(enemyStats.damageDealt)
            .arg(enemyStats.hits)
            .arg(enemyStats.shotsFired)
            .arg(gameState.getAccuracyPercent(TeamSide::Enemy))
            .arg(enemyStats.unitsDestroyed)
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

    updateTurnInfo();
    refreshStatistics();
    updateTileInfo();
    redrawBoard();
}

void BattlePage::updateTileInfo()
{
    if (!m_controller)
    {
        ui->labelTileInfo->setText(buildDefaultTileInfoText());
        return;
    }

    const GameState& gameState = m_controller->getGameState();

    if (!gameState.hasSelectedPosition())
    {
        ui->labelTileInfo->setText(buildDefaultTileInfoText());
        return;
    }

    const Tile* tile = gameState.getBoard().getTile(gameState.getSelectedX(), gameState.getSelectedY());
    ui->labelTileInfo->setText(buildTileInfoText(tile));
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
