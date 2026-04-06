#ifndef BATTLEUIFORMATTER_H
#define BATTLEUIFORMATTER_H

#include <QString>

#include "../models/enums/teamside.h"
#include "../models/enums/terraintype.h"
#include "../models/gamestate.h"

class Tile;
class Unit;
class GameState;

class BattleUiFormatter
{
public:
    static QString terrainToDisplayName(TerrainType terrain);

    static QString buildDefaultTileInfoText();
    static QString buildTileInfoText(const Tile* tile);

    static QString buildTurnInfoHtml(
        int currentTurn,
        TeamSide currentSide,
        bool gameFinished,
        TeamSide winnerSide
        );

    static QString buildTeamsInfoHtml(
        int playerCount,
        int enemyCount,
        TeamSide currentSide,
        int currentAp,
        int maxAp,
        const TeamBattleStats& playerStats,
        const TeamBattleStats& enemyStats,
        int playerAccuracy,
        int enemyAccuracy
        );

    static QString buildNoUnitSelectedNameHtml();
    static QString buildNoUnitSelectedStatsHtml(
        int currentAp,
        int maxAp,
        int currentTurn,
        const TeamBattleStats& playerStats,
        const TeamBattleStats& enemyStats
        );

    static QString buildSelectedUnitNameHtml(const Unit* unit);
    static QString buildSelectedUnitStatsHtml(
        const Unit* unit,
        int moveCount,
        int attackCount,
        int healCount,
        int currentAp,
        int maxAp,
        int currentTurn
        );

    static QString buildPostGameSummaryHtml(const GameState& gameState);
};

#endif // BATTLEUIFORMATTER_H
