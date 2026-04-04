#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QWidget>
#include "../config/teamComposition.h"
#include "../config/gameconfig.h"

namespace Ui {
class ConfigPage;
}

class ConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit ConfigPage(QWidget *parent = nullptr);
    ~ConfigPage();

    GameConfig saveConfiguration() const;
    GameConfig getConfiguration() const;
    bool validateConfiguration() const;

signals:
    void backClicked();
    void startClicked(const GameConfig& config);

private slots:
    void onPlayerTeamConfigClicked();
    void onEnemyTeamConfigClicked();
    void onStartClicked();

private:
    void loadConfigurationOptions();
    void updateTeamSummaries();

private:
    Ui::ConfigPage *ui;
    TeamComposition m_playerTeam;
    TeamComposition m_enemyTeam;
};

#endif // CONFIGPAGE_H
