#include "aiturnservice.h"

#include "../engine/attackresolver.h"
#include "../engine/battleengine.h"
#include "../models/gamestate.h"
#include "../models/board.h"
#include "../models/tile.h"
#include "../models/unit.h"

#include <QList>
#include <QtGlobal>
#include <limits>

namespace
{
/**
 * @brief Reprezentuje pozycję jednostki na planszy.
 *
 * Struktura pomocnicza przechowuje współrzędne jednostki
 * oraz wskaźnik na obiekt tej jednostki.
 */
struct UnitPosition
{
    int x = -1;          ///< Współrzędna X jednostki.
    int y = -1;          ///< Współrzędna Y jednostki.
    Unit* unit = nullptr; ///< Wskaźnik na jednostkę znajdującą się na planszy.
};

/**
 * @brief Wyszukuje wszystkie żywe jednostki wybranej strony na planszy.
 *
 * @param gameState Aktualny stan gry.
 * @param side Strona, dla której mają zostać znalezione jednostki.
 * @return Lista pozycji jednostek należących do wskazanej strony.
 */
QList<UnitPosition> findUnits(const GameState& gameState, TeamSide side)
{
    QList<UnitPosition> positions;
    const Board& board = gameState.getBoard();

    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = 0; x < board.getWidth(); ++x)
        {
            const Tile* tile = board.getTile(x, y);
            if (!tile || !tile->isOccupied() || !tile->getUnit())
                continue;

            if (tile->getUnit()->getSide() == side && tile->getUnit()->isAlive())
                positions.append({ x, y, tile->getUnit() });
        }
    }

    return positions;
}

/**
 * @brief Oblicza odległość Manhattan pomiędzy dwoma polami.
 *
 * @param x1 Współrzędna X pierwszego pola.
 * @param y1 Współrzędna Y pierwszego pola.
 * @param x2 Współrzędna X drugiego pola.
 * @param y2 Współrzędna Y drugiego pola.
 * @return Odległość Manhattan.
 */
int manhattan(int x1, int y1, int x2, int y2)
{
    return qAbs(x1 - x2) + qAbs(y1 - y2);
}
}

/**
 * @brief Bezpiecznie zaznacza jednostkę AI.
 *
 * @param gameState Aktualny stan gry.
 * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
 * @param x Współrzędna X jednostki.
 * @param y Współrzędna Y jednostki.
 * @return true, jeśli zaznaczenie się powiodło.
 */
bool AITurnService::selectUnitSafely(GameState& gameState, BattleEngine& battleEngine, int x, int y) const
{
    if (gameState.isGameFinished())
        return false;

    battleEngine.handleTileClick(gameState, x, y);

    if (!gameState.hasSelectedPosition())
        return false;

    return gameState.getSelectedX() == x && gameState.getSelectedY() == y;
}

/**
 * @brief Próbuje znaleźć i wykonać możliwy atak.
 *
 * AI przeszukuje wszystkie swoje jednostki i wszystkie jednostki gracza,
 * a następnie wykonuje pierwszy poprawny atak możliwy do przeprowadzenia.
 *
 * @param gameState Aktualny stan gry.
 * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
 * @return true, jeśli wykonano atak.
 */
bool AITurnService::tryPerformAttack(GameState& gameState, BattleEngine& battleEngine) const
{
    AttackResolver resolver;

    const QList<UnitPosition> enemies = findUnits(gameState, TeamSide::Enemy);
    const QList<UnitPosition> players = findUnits(gameState, TeamSide::Player);

    for (const UnitPosition& enemyPos : enemies)
    {
        if (!enemyPos.unit || !enemyPos.unit->isAlive())
            continue;

        const Tile* enemyTile = gameState.getBoard().getTile(enemyPos.x, enemyPos.y);
        if (!enemyTile)
            continue;

        for (const UnitPosition& playerPos : players)
        {
            if (!playerPos.unit || !playerPos.unit->isAlive())
                continue;

            if (!resolver.canAttack(*enemyPos.unit,
                                    enemyPos.x,
                                    enemyPos.y,
                                    *playerPos.unit,
                                    playerPos.x,
                                    playerPos.y,
                                    enemyTile->getTerrain()))
            {
                continue;
            }

            if (!selectUnitSafely(gameState, battleEngine, enemyPos.x, enemyPos.y))
                continue;

            if (gameState.isGameFinished() || gameState.getCurrentSide() != TeamSide::Enemy)
                return false;

            if (!gameState.hasSelectedPosition())
                return false;

            if (gameState.getSelectedX() != enemyPos.x || gameState.getSelectedY() != enemyPos.y)
                continue;

            const Tile* targetTile = gameState.getBoard().getTile(playerPos.x, playerPos.y);
            if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                continue;

            if (!gameState.isAttackablePosition(playerPos.x, playerPos.y))
                continue;

            battleEngine.handleTileClick(gameState, playerPos.x, playerPos.y);
            return true;
        }
    }

    return false;
}

/**
 * @brief Oblicza ocenę pola dla ruchu AI.
 *
 * Pole jest tym lepsze, im:
 * - bliżej znajduje się najbliższy przeciwnik,
 * - większą osłonę zapewnia teren.
 *
 * @param gameState Aktualny stan gry.
 * @param moveX Współrzędna X analizowanego pola.
 * @param moveY Współrzędna Y analizowanego pola.
 * @return Wynik oceny pola.
 */
int AITurnService::scoreMovePosition(const GameState& gameState, int moveX, int moveY) const
{
    const QList<UnitPosition> players = findUnits(gameState, TeamSide::Player);

    int nearest = std::numeric_limits<int>::max();
    for (const UnitPosition& playerPos : players)
    {
        if (!playerPos.unit || !playerPos.unit->isAlive())
            continue;

        nearest = qMin(nearest, manhattan(moveX, moveY, playerPos.x, playerPos.y));
    }

    const Tile* tile = gameState.getBoard().getTile(moveX, moveY);
    const int coverBonus = tile ? tile->getCoverBonus() : 0;

    return nearest * 10 - coverBonus;
}

/**
 * @brief Próbuje znaleźć i wykonać najlepszy dostępny ruch.
 *
 * AI wybiera ruch, który minimalizuje wynik funkcji scoreMovePosition().
 *
 * @param gameState Aktualny stan gry.
 * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
 * @return true, jeśli wykonano ruch.
 */
bool AITurnService::tryPerformMove(GameState& gameState, BattleEngine& battleEngine) const
{
    const QList<UnitPosition> enemies = findUnits(gameState, TeamSide::Enemy);

    for (const UnitPosition& enemyPos : enemies)
    {
        if (!enemyPos.unit || !enemyPos.unit->isAlive())
            continue;

        if (!enemyPos.unit->canMoveNow())
            continue;

        if (!selectUnitSafely(gameState, battleEngine, enemyPos.x, enemyPos.y))
            continue;

        if (gameState.isGameFinished() || gameState.getCurrentSide() != TeamSide::Enemy)
            return false;

        const auto movePositions = gameState.getAvailableMovePositions();
        if (movePositions.isEmpty())
            continue;

        int bestIndex = -1;
        int bestScore = std::numeric_limits<int>::max();

        for (int i = 0; i < movePositions.size(); ++i)
        {
            const auto& move = movePositions[i];
            const int score = scoreMovePosition(gameState, move.first, move.second);

            if (score < bestScore)
            {
                bestScore = score;
                bestIndex = i;
            }
        }

        if (bestIndex >= 0)
        {
            battleEngine.handleTileClick(gameState,
                                         movePositions[bestIndex].first,
                                         movePositions[bestIndex].second);
            return true;
        }
    }

    return false;
}

/**
 * @brief Wykonuje jedną akcję AI w ramach aktualnej tury.
 *
 * Serwis najpierw próbuje wykonać atak. Jeśli nie znajdzie poprawnego ataku,
 * próbuje wykonać ruch. Jeśli żadna akcja nie jest możliwa, kończy turę.
 *
 * @param gameState Aktualny stan gry.
 * @param battleEngine Silnik odpowiedzialny za wykonywanie akcji.
 */
void AITurnService::performTurn(GameState& gameState, BattleEngine& battleEngine) const
{
    if (gameState.isGameFinished())
        return;

    if (gameState.getCurrentSide() != TeamSide::Enemy)
        return;

    if (!gameState.hasTurnActionPoints())
    {
        battleEngine.endTurn(gameState);
        return;
    }

    const QList<UnitPosition> enemies = findUnits(gameState, TeamSide::Enemy);
    const QList<UnitPosition> players = findUnits(gameState, TeamSide::Player);

    if (enemies.isEmpty() || players.isEmpty())
    {
        battleEngine.endTurn(gameState);
        return;
    }

    if (tryPerformAttack(gameState, battleEngine))
        return;

    if (tryPerformMove(gameState, battleEngine))
        return;

    battleEngine.endTurn(gameState);
}
