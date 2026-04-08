#include "aicontroller.h"

#include "../engine/attackresolver.h"
#include "../engine/battleengine.h"
#include "../models/gamestate.h"
#include "../models/board.h"
#include "../models/tile.h"
#include "../models/unit.h"
#include "../models/enums/teamside.h"

#include <QList>
#include <QtGlobal>
#include <limits>

namespace
{
/**
 * @brief Reprezentuje pozycję jednostki na planszy.
 *
 * Struktura pomocnicza przechowująca współrzędne jednostki
 * oraz wskaźnik na obiekt tej jednostki.
 */
struct UnitPosition
{
    int x = -1;        ///< Współrzędna X jednostki.
    int y = -1;        ///< Współrzędna Y jednostki.
    Unit* unit = nullptr; ///< Wskaźnik na jednostkę znajdującą się na planszy.
};

/**
 * @brief Wyszukuje wszystkie żywe jednostki wybranej strony na planszy.
 *
 * Funkcja przeszukuje całą planszę i zwraca listę pozycji jednostek
 * należących do wskazanej strony konfliktu.
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
 * Odległość Manhattan jest sumą bezwzględnych różnic współrzędnych X i Y.
 *
 * @param x1 Współrzędna X pierwszego pola.
 * @param y1 Współrzędna Y pierwszego pola.
 * @param x2 Współrzędna X drugiego pola.
 * @param y2 Współrzędna Y drugiego pola.
 * @return Odległość Manhattan pomiędzy dwoma polami.
 */
int manhattan(int x1, int y1, int x2, int y2)
{
    return qAbs(x1 - x2) + qAbs(y1 - y2);
}

/**
 * @brief Bezpiecznie zaznacza jednostkę na wskazanym polu.
 *
 * Funkcja symuluje kliknięcie pola planszy i sprawdza, czy po tej operacji
 * jednostka została rzeczywiście zaznaczona jako aktywna.
 *
 * @param gameState Aktualny stan gry.
 * @param battleEngine Silnik obsługujący akcje wykonywane na planszy.
 * @param x Współrzędna X pola.
 * @param y Współrzędna Y pola.
 * @return true, jeśli jednostka została poprawnie zaznaczona, w przeciwnym razie false.
 */
bool selectUnitSafely(GameState& gameState, BattleEngine& battleEngine, int x, int y)
{
    if (gameState.isGameFinished())
        return false;

    battleEngine.handleTileClick(gameState, x, y);

    if (!gameState.hasSelectedPosition())
        return false;

    return gameState.getSelectedX() == x && gameState.getSelectedY() == y;
}
}

/**
 * @brief Wykonuje turę gracza sterowanego automatycznie.
 *
 * Metoda realizuje logikę działania przeciwnika komputerowego.
 * Najpierw sprawdza możliwość wykonania ataku, a jeśli nie jest to możliwe,
 * próbuje wykonać ruch prowadzący do zbliżenia się do przeciwnika
 * z uwzględnieniem korzystniejszych pól planszy.
 *
 * Jeżeli żadna akcja nie może zostać wykonana, tura zostaje zakończona.
 *
 * @param gameState Aktualny stan gry.
 * @param battleEngine Silnik odpowiedzialny za obsługę akcji w trakcie walki.
 */
void AIController::performTurn(GameState& gameState, BattleEngine& battleEngine)
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

    AttackResolver resolver;

    const QList<UnitPosition> enemies = findUnits(gameState, TeamSide::Enemy);
    const QList<UnitPosition> players = findUnits(gameState, TeamSide::Player);

    if (enemies.isEmpty() || players.isEmpty())
    {
        battleEngine.endTurn(gameState);
        return;
    }

    // 1 akcja AI na jedno wywołanie:
    // najpierw próbujemy atak
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

            // Bezpieczne zaznaczenie atakującego
            if (!selectUnitSafely(gameState, battleEngine, enemyPos.x, enemyPos.y))
                continue;

            // Po zaznaczeniu mogło się już coś zmienić
            if (gameState.isGameFinished() || gameState.getCurrentSide() != TeamSide::Enemy)
                return;

            if (!gameState.hasSelectedPosition())
                return;

            // Dodatkowe zabezpieczenie: naprawdę mamy wybraną tę jednostkę
            if (gameState.getSelectedX() != enemyPos.x || gameState.getSelectedY() != enemyPos.y)
                continue;

            // Czy cel nadal istnieje i nadal jest oznaczony jako atakowalny
            const Tile* targetTile = gameState.getBoard().getTile(playerPos.x, playerPos.y);
            if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                continue;

            if (!gameState.isAttackablePosition(playerPos.x, playerPos.y))
                continue;

            battleEngine.handleTileClick(gameState, playerPos.x, playerPos.y);
            return;
        }
    }

    // jeśli nie ma ataku, próbujemy ruch
    for (const UnitPosition& enemyPos : enemies)
    {
        if (!enemyPos.unit || !enemyPos.unit->isAlive())
            continue;

        if (!enemyPos.unit->canMoveNow())
            continue;

        if (!selectUnitSafely(gameState, battleEngine, enemyPos.x, enemyPos.y))
            continue;

        if (gameState.isGameFinished() || gameState.getCurrentSide() != TeamSide::Enemy)
            return;

        const auto movePositions = gameState.getAvailableMovePositions();
        if (movePositions.isEmpty())
            continue;

        int bestIndex = -1;
        int bestScore = std::numeric_limits<int>::max();

        for (int i = 0; i < movePositions.size(); ++i)
        {
            const auto& move = movePositions[i];
            int nearest = std::numeric_limits<int>::max();

            for (const UnitPosition& playerPos : players)
            {
                if (!playerPos.unit || !playerPos.unit->isAlive())
                    continue;

                nearest = qMin(nearest, manhattan(move.first, move.second, playerPos.x, playerPos.y));
            }

            const Tile* tile = gameState.getBoard().getTile(move.first, move.second);
            const int coverBonus = tile ? tile->getCoverBonus() : 0;
            const int score = nearest * 10 - coverBonus;

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
            return;
        }
    }

    // jeśli nic nie da się zrobić, kończymy turę
    battleEngine.endTurn(gameState);
}
