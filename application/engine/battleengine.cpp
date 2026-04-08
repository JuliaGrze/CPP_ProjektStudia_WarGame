#include "battleengine.h"

#include "../models/tile.h"
#include "../models/unit.h"
#include "../models/enums/terraintype.h"

#include <QQueue>
#include <QSet>
#include <QPair>
#include <QHash>
#include <limits>
#include <cmath>

namespace
{
/**
 * @brief Reprezentuje węzeł używany podczas wyznaczania kosztu ruchu.
 *
 * Struktura przechowuje współrzędne pola oraz łączny koszt
 * dotarcia do tego pola od pozycji początkowej.
 */
struct MoveNode
{
    int x;          ///< Współrzędna X pola.
    int y;          ///< Współrzędna Y pola.
    int spentCost;  ///< Łączny koszt dotarcia do pola.
};

/**
 * @brief Tworzy tekstowy klucz dla współrzędnych pola.
 *
 * Funkcja pomocnicza używana przy zapamiętywaniu odwiedzonych pól
 * i najniższych kosztów przejścia.
 *
 * @param x Współrzędna X pola.
 * @param y Współrzędna Y pola.
 * @return Klucz tekstowy w formacie „x_y”.
 */
QString makeKey(int x, int y)
{
    return QString("%1_%2").arg(x).arg(y);
}
}

/**
 * @brief Sprawdza, czy jednostka może zostać zaznaczona.
 *
 * Jednostka może zostać wybrana tylko wtedy, gdy:
 * - istnieje,
 * - żyje,
 * - należy do aktualnie aktywnej strony,
 * - drużyna posiada jeszcze punkty akcji.
 *
 * @param gameState Aktualny stan gry.
 * @param unit Jednostka sprawdzana pod kątem możliwości zaznaczenia.
 * @return true, jeśli jednostka może zostać zaznaczona, w przeciwnym razie false.
 */
bool BattleEngine::canSelectUnit(const GameState& gameState, const Unit* unit) const
{
    if (!unit || !unit->isAlive())
        return false;

    if (unit->getSide() != gameState.getCurrentSide())
        return false;

    if (gameState.getCurrentTurnActionPoints() <= 0)
        return false;

    return true;
}

/**
 * @brief Oblicza odległość pomiędzy dwoma polami planszy.
 *
 * Wykorzystuje metrykę Manhattan, czyli sumę bezwzględnych różnic
 * współrzędnych X i Y.
 *
 * @param x1 Współrzędna X pierwszego pola.
 * @param y1 Współrzędna Y pierwszego pola.
 * @param x2 Współrzędna X drugiego pola.
 * @param y2 Współrzędna Y drugiego pola.
 * @return Odległość pomiędzy wskazanymi polami.
 */
int BattleEngine::calculateDistance(int x1, int y1, int x2, int y2) const
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

/**
 * @brief Oblicza najniższy koszt ruchu pomiędzy dwoma polami.
 *
 * Metoda wyszukuje najtańszą ścieżkę przejścia z pola początkowego
 * do pola docelowego z uwzględnieniem:
 * - kosztu ruchu terenu,
 * - kosztu ruchu jednostki,
 * - pól nieprzechodnich,
 * - pól zajętych przez inne jednostki.
 *
 * Jeśli ścieżka nie istnieje, zwracana jest maksymalna wartość typu int.
 *
 * @param gameState Aktualny stan gry.
 * @param unit Jednostka, dla której obliczany jest koszt ruchu.
 * @param startX Współrzędna X pola początkowego.
 * @param startY Współrzędna Y pola początkowego.
 * @param targetX Współrzędna X pola docelowego.
 * @param targetY Współrzędna Y pola docelowego.
 * @return Najniższy koszt ruchu lub std::numeric_limits<int>::max(), jeśli brak ścieżki.
 */
int BattleEngine::calculateLowestMoveCost(const GameState& gameState,
                                          const Unit& unit,
                                          int startX,
                                          int startY,
                                          int targetX,
                                          int targetY) const
{
    if (startX == targetX && startY == targetY)
        return 0;

    QQueue<MoveNode> queue;
    QHash<QString, int> bestCost;

    queue.enqueue({ startX, startY, 0 });
    bestCost.insert(makeKey(startX, startY), 0);

    const int directions[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

    while (!queue.isEmpty())
    {
        const MoveNode current = queue.dequeue();

        for (const auto& direction : directions)
        {
            const int nextX = current.x + direction[0];
            const int nextY = current.y + direction[1];

            if (!gameState.getBoard().isInsideBoard(nextX, nextY))
                continue;

            const Tile* nextTile = gameState.getBoard().getTile(nextX, nextY);
            if (!nextTile || !nextTile->isWalkable())
                continue;

            if (nextTile->isOccupied() && !(nextX == targetX && nextY == targetY))
                continue;

            const int nextCost = current.spentCost + nextTile->getMovementCost() * unit.getMoveCostPerTile();
            const QString key = makeKey(nextX, nextY);

            if (bestCost.contains(key) && bestCost.value(key) <= nextCost)
                continue;

            bestCost.insert(key, nextCost);

            if (nextX == targetX && nextY == targetY)
                return nextCost;

            queue.enqueue({ nextX, nextY, nextCost });
        }
    }

    return std::numeric_limits<int>::max();
}

/**
 * @brief Wyznacza dostępne akcje dla wybranej jednostki.
 *
 * Metoda oblicza i zapisuje w stanie gry pola dostępne dla:
 * - ruchu,
 * - ataku,
 * - leczenia,
 * - pól zablokowanych.
 *
 * Wyniki te są później wykorzystywane do podświetlenia planszy
 * i interpretacji kliknięć użytkownika.
 *
 * @param gameState Aktualny stan gry.
 * @param startX Współrzędna X wybranej jednostki.
 * @param startY Współrzędna Y wybranej jednostki.
 */
void BattleEngine::calculateActionHighlights(GameState& gameState, int startX, int startY) const
{
    QVector<QPair<int, int>> availablePositions;
    QVector<QPair<int, int>> blockedPositions;
    QVector<QPair<int, int>> attackablePositions;
    QVector<QPair<int, int>> healablePositions;

    const Tile* startTile = gameState.getBoard().getTile(startX, startY);
    if (!startTile || !startTile->isOccupied() || !startTile->getUnit())
    {
        gameState.clearAllHighlights();
        return;
    }

    const Unit* unit = startTile->getUnit();
    const Board& board = gameState.getBoard();

    if (unit->canMoveNow())
    {
        const int maxAffordableCost = gameState.getCurrentTurnActionPoints();

        QQueue<MoveNode> queue;
        QSet<QString> availableSet;
        QSet<QString> blockedSet;
        QHash<QString, int> bestCost;

        queue.enqueue({ startX, startY, 0 });
        bestCost.insert(makeKey(startX, startY), 0);

        const int directions[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };

        while (!queue.isEmpty())
        {
            const MoveNode current = queue.dequeue();

            for (const auto& direction : directions)
            {
                const int nextX = current.x + direction[0];
                const int nextY = current.y + direction[1];

                if (!board.isInsideBoard(nextX, nextY))
                    continue;

                const Tile* nextTile = board.getTile(nextX, nextY);
                if (!nextTile)
                    continue;

                const QString key = makeKey(nextX, nextY);

                if (!nextTile->isWalkable())
                {
                    if (!blockedSet.contains(key))
                    {
                        blockedPositions.append({ nextX, nextY });
                        blockedSet.insert(key);
                    }
                    continue;
                }

                if (nextTile->isOccupied())
                {
                    if (!blockedSet.contains(key))
                    {
                        blockedPositions.append({ nextX, nextY });
                        blockedSet.insert(key);
                    }
                    continue;
                }

                const int nextCost = current.spentCost + nextTile->getMovementCost() * unit->getMoveCostPerTile();
                if (nextCost > maxAffordableCost)
                    continue;

                if (bestCost.contains(key) && bestCost.value(key) <= nextCost)
                    continue;

                bestCost.insert(key, nextCost);

                if (calculateDistance(startX, startY, nextX, nextY) <= unit->getMovementPoints())
                {
                    if (!availableSet.contains(key))
                    {
                        availablePositions.append({ nextX, nextY });
                        availableSet.insert(key);
                    }
                }

                queue.enqueue({ nextX, nextY, nextCost });
            }
        }
    }

    if (unit->canAttackNow() && unit->getAttackCost() <= gameState.getCurrentTurnActionPoints())
    {
        for (int y = 0; y < board.getHeight(); ++y)
        {
            for (int x = 0; x < board.getWidth(); ++x)
            {
                const Tile* targetTile = board.getTile(x, y);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive() || targetUnit->getSide() == unit->getSide())
                    continue;

                if (m_attackResolver.canAttack(*unit,
                                               startX,
                                               startY,
                                               *targetUnit,
                                               x,
                                               y,
                                               startTile->getTerrain()))
                {
                    attackablePositions.append({ x, y });
                }
            }
        }
    }

    if (unit->canHealNow() && unit->getAttackCost() <= gameState.getCurrentTurnActionPoints())
    {
        for (int y = 0; y < board.getHeight(); ++y)
        {
            for (int x = 0; x < board.getWidth(); ++x)
            {
                const Tile* targetTile = board.getTile(x, y);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive() || targetUnit->getSide() != unit->getSide())
                    continue;

                if (!targetUnit->isDamaged())
                    continue;

                const int distance = calculateDistance(startX, startY, x, y);
                if (distance >= unit->getMinRange() && distance <= unit->getRange())
                    healablePositions.append({ x, y });
            }
        }
    }

    gameState.setAvailableMovePositions(availablePositions);
    gameState.setBlockedMovePositions(blockedPositions);
    gameState.setAttackablePositions(attackablePositions);
    gameState.setHealablePositions(healablePositions);
}

/**
 * @brief Próbuje wykonać ruch aktualnie zaznaczoną jednostką.
 *
 * Metoda sprawdza poprawność ruchu, dostępność ścieżki,
 * koszt ruchu oraz liczbę dostępnych punktów akcji.
 * W przypadku powodzenia aktualizuje pozycję jednostki,
 * stan punktów akcji oraz komunikat dla użytkownika.
 *
 * @param gameState Aktualny stan gry.
 * @param targetX Współrzędna X pola docelowego.
 * @param targetY Współrzędna Y pola docelowego.
 * @return true, jeśli kliknięcie zostało obsłużone, w przeciwnym razie false.
 */
bool BattleEngine::tryMoveSelectedUnit(GameState& gameState, int targetX, int targetY)
{
    if (!gameState.hasSelectedPosition())
        return false;

    if (!gameState.isMovePositionAvailable(targetX, targetY))
    {
        gameState.setLastActionMessage("To pole nie jest dostępne do ruchu. Kliknij zielone pole.");
        return true;
    }

    Tile* sourceTile = gameState.getBoard().getTile(
        gameState.getSelectedX(),
        gameState.getSelectedY());

    Tile* targetTile = gameState.getBoard().getTile(targetX, targetY);

    if (!sourceTile || !targetTile || !sourceTile->isOccupied() || targetTile->isOccupied() || !targetTile->isWalkable())
        return false;

    Unit* unit = sourceTile->getUnit();
    if (!unit || !unit->canMoveNow())
    {
        gameState.setLastActionMessage("Ta jednostka wykonała już ruch w tej turze.");
        return true;
    }

    const int movementDistance = calculateDistance(sourceTile->getX(), sourceTile->getY(), targetX, targetY);
    if (movementDistance <= 0 || movementDistance > unit->getMovementPoints())
        return false;

    const int movementCost = calculateLowestMoveCost(
        gameState,
        *unit,
        sourceTile->getX(),
        sourceTile->getY(),
        targetX,
        targetY);

    if (movementCost == std::numeric_limits<int>::max())
    {
        gameState.setLastActionMessage("Nie znaleziono poprawnej ścieżki ruchu.");
        return true;
    }

    if (movementCost > gameState.getCurrentTurnActionPoints())
    {
        gameState.setLastActionMessage(
            QString("Za mało punktów akcji. Ruch %1 kosztuje %2 AP.")
                .arg(unit->getName())
                .arg(movementCost));
        return true;
    }

    sourceTile->removeUnit();
    targetTile->setUnit(unit);

    unit->markMoved();

    gameState.consumeTurnActionPoints(movementCost);
    gameState.setSelectedPosition(targetX, targetY);
    calculateActionHighlights(gameState, targetX, targetY);

    gameState.setLastActionMessage(
        QString("%1 przesunął się. Koszt ruchu: %2 AP. Pozostałe AP drużyny: %3/%4. Możesz dalej działać tą samą jednostką, jeśli starczy AP.")
            .arg(unit->getName())
            .arg(movementCost)
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints()));

    finishActionAndMaybeEndTurn(gameState);
    return true;
}

/**
 * @brief Usuwa z pola jednostkę, która została zniszczona.
 *
 * @param tile Pole planszy, z którego ma zostać usunięta jednostka.
 */
void BattleEngine::clearDefeatedUnitFromTile(Tile& tile) const
{
    Unit* unit = tile.getUnit();
    if (unit && !unit->isAlive())
        tile.removeUnit();
}

/**
 * @brief Próbuje wykonać leczenie aktualnie zaznaczoną jednostką.
 *
 * Metoda sprawdza poprawność celu, zasięg leczenia, dostępność punktów akcji
 * oraz możliwość wykonania tej akcji przez jednostkę.
 *
 * @param gameState Aktualny stan gry.
 * @param targetX Współrzędna X celu leczenia.
 * @param targetY Współrzędna Y celu leczenia.
 * @return true, jeśli kliknięcie zostało obsłużone, w przeciwnym razie false.
 */
bool BattleEngine::tryHealSelectedUnit(GameState& gameState, int targetX, int targetY)
{
    if (!gameState.hasSelectedPosition())
        return false;

    Tile* healerTile = gameState.getBoard().getTile(gameState.getSelectedX(), gameState.getSelectedY());
    Tile* targetTile = gameState.getBoard().getTile(targetX, targetY);

    if (!healerTile || !targetTile || !healerTile->isOccupied() || !targetTile->isOccupied())
        return false;

    Unit* healer = healerTile->getUnit();
    Unit* target = targetTile->getUnit();

    if (!healer || !target || !healer->canHealNow() || !healer->canHeal())
        return false;

    if (healer->getSide() != target->getSide())
        return false;

    if (!gameState.isHealablePosition(targetX, targetY))
    {
        if (!target->isDamaged())
            gameState.setLastActionMessage("Ta jednostka nie wymaga leczenia.");
        else
            gameState.setLastActionMessage("Nie możesz uleczyć tego celu. Kliknij niebiesko oznaczonego sojusznika.");
        return true;
    }

    const int actionCost = healer->getAttackCost();
    if (actionCost > gameState.getCurrentTurnActionPoints())
    {
        gameState.setLastActionMessage(
            QString("Za mało punktów akcji. Leczenie kosztuje %1 AP.")
                .arg(actionCost));
        return true;
    }

    const int before = target->getHealth();
    target->heal(healer->getHealAmount());
    const int healed = target->getHealth() - before;

    healer->markActed();
    gameState.consumeTurnActionPoints(actionCost);
    gameState.setSelectedPosition(healerTile->getX(), healerTile->getY());
    calculateActionHighlights(gameState, healerTile->getX(), healerTile->getY());

    gameState.setLastActionMessage(
        QString("%1 uleczył %2 o %3 HP. Stan celu: %4/%5 HP. Pozostałe AP drużyny: %6/%7. Możesz wykonać kolejną akcję, jeśli starczy AP.")
            .arg(healer->getName())
            .arg(target->getName())
            .arg(healed)
            .arg(target->getHealth())
            .arg(target->getMaxHealth())
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints()));

    finishActionAndMaybeEndTurn(gameState);
    return true;
}

/**
 * @brief Próbuje wykonać atak aktualnie zaznaczoną jednostką.
 *
 * Metoda sprawdza możliwość wykonania ataku, koszt punktów akcji,
 * dostępność celu oraz rozstrzyga wynik starcia przy użyciu AttackResolver.
 *
 * W przypadku sukcesu aktualizowane są statystyki walki, stan jednostek,
 * punkty akcji oraz komunikaty dla użytkownika.
 *
 * @param gameState Aktualny stan gry.
 * @param targetX Współrzędna X celu ataku.
 * @param targetY Współrzędna Y celu ataku.
 * @return true, jeśli kliknięcie zostało obsłużone, w przeciwnym razie false.
 */
bool BattleEngine::tryAttackSelectedUnit(GameState& gameState, int targetX, int targetY)
{
    if (!gameState.hasSelectedPosition())
        return false;

    Tile* attackerTile = gameState.getBoard().getTile(
        gameState.getSelectedX(),
        gameState.getSelectedY());
    Tile* defenderTile = gameState.getBoard().getTile(targetX, targetY);

    if (!attackerTile || !defenderTile || !attackerTile->isOccupied() || !defenderTile->isOccupied())
        return false;

    Unit* attacker = attackerTile->getUnit();
    Unit* defender = defenderTile->getUnit();

    if (!attacker || !defender)
        return false;

    if (!attacker->canAttackNow())
    {
        gameState.setLastActionMessage("Ta jednostka wykonała już atak w tej turze.");
        return true;
    }

    if (attacker->getSide() == defender->getSide())
        return false;

    const int attackCost = attacker->getAttackCost();
    if (attackCost > gameState.getCurrentTurnActionPoints())
    {
        gameState.setLastActionMessage(
            QString("Za mało punktów akcji. Atak jednostki %1 kosztuje %2 AP.")
                .arg(attacker->getName())
                .arg(attackCost));
        return true;
    }

    const int distance = calculateDistance(attackerTile->getX(), attackerTile->getY(), defenderTile->getX(), defenderTile->getY());
    if (!gameState.isAttackablePosition(targetX, targetY))
    {
        gameState.setLastActionMessage(
            QString("Cel jest poza zasięgiem lub niedostępny. %1 atakuje na dystansie %2-%3 pól, a cel jest %4 pól dalej. Kliknij fioletowo oznaczonego przeciwnika.")
                .arg(attacker->getName())
                .arg(attacker->getMinRange())
                .arg(attacker->getRange())
                .arg(distance));
        return true;
    }

    const AttackResult result = m_attackResolver.resolveAttack(
        *attacker,
        attackerTile->getX(),
        attackerTile->getY(),
        attackerTile->getTerrain(),
        *defender,
        defenderTile->getX(),
        defenderTile->getY(),
        defenderTile->getTerrain());

    if (!result.attackPerformed)
    {
        gameState.setLastActionMessage(result.message);
        return true;
    }

    gameState.recordShotFired(attacker->getSide());

    if (result.hit)
        gameState.recordSuccessfulHit(attacker->getSide(), result.damageDealt);

    if (result.targetDestroyed)
        gameState.recordUnitDestroyed(attacker->getSide());

    gameState.consumeTurnActionPoints(attackCost);
    clearDefeatedUnitFromTile(*defenderTile);
    gameState.setSelectedPosition(attackerTile->getX(), attackerTile->getY());
    calculateActionHighlights(gameState, attackerTile->getX(), attackerTile->getY());

    gameState.setLastActionMessage(
        QString("%1 Koszt ataku: %2 AP. Pozostałe AP drużyny: %3/%4. Jeśli starczy AP, możesz działać dalej tą jednostką.")
            .arg(result.message)
            .arg(attackCost)
            .arg(gameState.getCurrentTurnActionPoints())
            .arg(gameState.getMaxTurnActionPoints()));

    gameState.updateVictoryState();
    finishActionAndMaybeEndTurn(gameState);
    return true;
}

/**
 * @brief Sprawdza, czy wskazana jednostka ma dostępną jakąkolwiek akcję.
 *
 * Analizowane są możliwości:
 * - ataku,
 * - leczenia,
 * - ruchu.
 *
 * @param gameState Aktualny stan gry.
 * @param x Współrzędna X jednostki.
 * @param y Współrzędna Y jednostki.
 * @param unit Jednostka poddawana analizie.
 * @return true, jeśli jednostka może wykonać przynajmniej jedną akcję, w przeciwnym razie false.
 */
bool BattleEngine::unitHasAnyAvailableAction(const GameState& gameState, int x, int y, const Unit& unit) const
{
    if (!unit.isAlive() || unit.getSide() != gameState.getCurrentSide())
        return false;

    const int currentAp = gameState.getCurrentTurnActionPoints();
    if (currentAp <= 0)
        return false;

    const Tile* unitTile = gameState.getBoard().getTile(x, y);
    if (!unitTile)
        return false;

    const Board& board = gameState.getBoard();

    if (unit.canAttackNow() && unit.getAttackCost() <= currentAp)
    {
        for (int targetY = 0; targetY < board.getHeight(); ++targetY)
        {
            for (int targetX = 0; targetX < board.getWidth(); ++targetX)
            {
                const Tile* targetTile = board.getTile(targetX, targetY);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive())
                    continue;

                if (targetUnit->getSide() == unit.getSide())
                    continue;

                if (m_attackResolver.canAttack(unit,
                                               x,
                                               y,
                                               *targetUnit,
                                               targetX,
                                               targetY,
                                               unitTile->getTerrain()))
                {
                    return true;
                }
            }
        }
    }

    if (unit.canHealNow() && unit.getAttackCost() <= currentAp)
    {
        for (int targetY = 0; targetY < board.getHeight(); ++targetY)
        {
            for (int targetX = 0; targetX < board.getWidth(); ++targetX)
            {
                const Tile* targetTile = board.getTile(targetX, targetY);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive())
                    continue;

                if (targetUnit->getSide() != unit.getSide())
                    continue;

                if (!targetUnit->isDamaged())
                    continue;

                const int distance = calculateDistance(x, y, targetX, targetY);
                if (distance >= unit.getMinRange() && distance <= unit.getRange())
                    return true;
            }
        }
    }

    if (unit.canMoveNow())
    {
        for (int targetY = 0; targetY < board.getHeight(); ++targetY)
        {
            for (int targetX = 0; targetX < board.getWidth(); ++targetX)
            {
                const Tile* targetTile = board.getTile(targetX, targetY);
                if (!targetTile || targetTile->isOccupied() || !targetTile->isWalkable())
                    continue;

                const int distance = calculateDistance(x, y, targetX, targetY);
                if (distance <= 0 || distance > unit.getMovementPoints())
                    continue;

                const int moveCost = calculateLowestMoveCost(gameState, unit, x, y, targetX, targetY);
                if (moveCost != std::numeric_limits<int>::max() && moveCost <= currentAp)
                    return true;
            }
        }
    }

    return false;
}

/**
 * @brief Zwraca komunikat wyjaśniający brak dostępnych akcji jednostki.
 *
 * Metoda analizuje stan jednostki, dostępne punkty akcji oraz możliwość
 * wykonania ruchu, ataku i leczenia, a następnie przygotowuje komunikat
 * opisujący przyczynę braku sensownej akcji.
 *
 * @param gameState Aktualny stan gry.
 * @param x Współrzędna X jednostki.
 * @param y Współrzędna Y jednostki.
 * @param unit Jednostka, której dotyczy analiza.
 * @return Komunikat opisujący powód braku dostępnej akcji.
 */
QString BattleEngine::getUnitUnavailableReason(const GameState& gameState, int x, int y, const Unit& unit) const
{
    if (!unit.isAlive())
        return "Ta jednostka jest zniszczona.";

    if (unit.getSide() != gameState.getCurrentSide())
        return "To nie jest jednostka aktywnej drużyny.";

    const int currentAp = gameState.getCurrentTurnActionPoints();
    if (currentAp <= 0)
        return "Drużyna nie ma już punktów akcji.";

    const bool canStillMove = unit.canMoveNow();
    const bool canStillAttack = unit.canAttackNow();
    const bool canStillHeal = unit.canHealNow();

    const bool enoughApForAttack = unit.getAttackCost() <= currentAp;

    bool hasMoveTarget = false;
    bool hasAttackTarget = false;
    bool hasHealTarget = false;

    const Tile* unitTile = gameState.getBoard().getTile(x, y);
    if (!unitTile)
        return "Nie udało się odczytać pozycji jednostki.";

    const Board& board = gameState.getBoard();

    if (canStillAttack && enoughApForAttack)
    {
        for (int targetY = 0; targetY < board.getHeight(); ++targetY)
        {
            for (int targetX = 0; targetX < board.getWidth(); ++targetX)
            {
                const Tile* targetTile = board.getTile(targetX, targetY);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive() || targetUnit->getSide() == unit.getSide())
                    continue;

                if (m_attackResolver.canAttack(unit, x, y, *targetUnit, targetX, targetY, unitTile->getTerrain()))
                {
                    hasAttackTarget = true;
                    break;
                }
            }
            if (hasAttackTarget)
                break;
        }
    }

    if (canStillHeal && enoughApForAttack)
    {
        for (int targetY = 0; targetY < board.getHeight(); ++targetY)
        {
            for (int targetX = 0; targetX < board.getWidth(); ++targetX)
            {
                const Tile* targetTile = board.getTile(targetX, targetY);
                if (!targetTile || !targetTile->isOccupied() || !targetTile->getUnit())
                    continue;

                const Unit* targetUnit = targetTile->getUnit();
                if (!targetUnit || !targetUnit->isAlive() || targetUnit->getSide() != unit.getSide())
                    continue;

                if (!targetUnit->isDamaged())
                    continue;

                const int distance = calculateDistance(x, y, targetX, targetY);
                if (distance >= unit.getMinRange() && distance <= unit.getRange())
                {
                    hasHealTarget = true;
                    break;
                }
            }
            if (hasHealTarget)
                break;
        }
    }

    if (canStillMove)
    {
        for (int targetY = 0; targetY < board.getHeight(); ++targetY)
        {
            for (int targetX = 0; targetX < board.getWidth(); ++targetX)
            {
                const Tile* targetTile = board.getTile(targetX, targetY);
                if (!targetTile || targetTile->isOccupied() || !targetTile->isWalkable())
                    continue;

                const int distance = calculateDistance(x, y, targetX, targetY);
                if (distance <= 0 || distance > unit.getMovementPoints())
                    continue;

                const int moveCost = calculateLowestMoveCost(gameState, unit, x, y, targetX, targetY);
                if (moveCost != std::numeric_limits<int>::max() && moveCost <= currentAp)
                {
                    hasMoveTarget = true;
                    break;
                }
            }
            if (hasMoveTarget)
                break;
        }
    }

    if (!canStillMove && !canStillAttack && !canStillHeal)
        return "Ta jednostka wykonała już wszystkie swoje akcje w tej turze.";

    if (canStillMove && !hasMoveTarget && canStillAttack && !hasAttackTarget && (!canStillHeal || !hasHealTarget))
        return "Brak dostępnych pól ruchu i brak celów w zasięgu.";

    if (canStillMove && !hasMoveTarget && !canStillAttack && !canStillHeal)
        return "Ta jednostka nie może już atakować, a nie ma też dostępnych pól ruchu.";

    if (!canStillMove && canStillAttack && !hasAttackTarget && (!canStillHeal || !hasHealTarget))
        return "Ta jednostka wykonała już ruch, a w zasięgu nie ma żadnego celu.";

    if (canStillMove && !hasMoveTarget && canStillAttack && hasAttackTarget)
        return "Ta jednostka nie ma już dostępnego ruchu, ale może jeszcze zaatakować.";

    if (canStillMove && hasMoveTarget && canStillAttack && !hasAttackTarget && (!canStillHeal || !hasHealTarget))
        return "Ta jednostka nie ma celu w zasięgu, ale może się jeszcze poruszyć.";

    if (canStillAttack && !enoughApForAttack && !hasMoveTarget)
        return "Drużyna ma za mało punktów akcji na atak, a ta jednostka nie ma też dostępnego ruchu.";

    if (canStillAttack && !enoughApForAttack)
        return QString("Drużyna ma za mało punktów akcji na atak tej jednostki. Wymagane: %1 AP.")
            .arg(unit.getAttackCost());

    if (canStillHeal && !hasHealTarget && !hasMoveTarget && !hasAttackTarget)
        return "Brak sojusznika do leczenia, brak celu do ataku i brak dostępnego ruchu.";

    return "Ta jednostka nie ma obecnie żadnej sensownej akcji do wykonania.";
}

/**
 * @brief Sprawdza, czy aktywna drużyna ma jakąkolwiek dostępną akcję.
 *
 * Metoda przeszukuje wszystkie jednostki aktualnej strony i sprawdza,
 * czy przynajmniej jedna z nich może wykonać ruch, atak lub leczenie.
 *
 * @param gameState Aktualny stan gry.
 * @return true, jeśli istnieje dostępna akcja, w przeciwnym razie false.
 */
bool BattleEngine::hasAnyAvailableAction(const GameState& gameState) const
{
    const Board& board = gameState.getBoard();

    for (int y = 0; y < board.getHeight(); ++y)
    {
        for (int x = 0; x < board.getWidth(); ++x)
        {
            const Tile* tile = board.getTile(x, y);
            if (!tile || !tile->isOccupied() || !tile->getUnit())
                continue;

            const Unit* unit = tile->getUnit();
            if (!unit)
                continue;

            if (unitHasAnyAvailableAction(gameState, x, y, *unit))
                return true;
        }
    }

    return false;
}

/**
 * @brief Kończy bieżącą akcję i w razie potrzeby kończy turę.
 *
 * Metoda sprawdza:
 * - czy gra została już zakończona,
 * - czy drużyna ma jeszcze punkty akcji,
 * - czy istnieją jeszcze możliwe akcje do wykonania.
 *
 * Jeśli nie można wykonać kolejnych działań, tura kończy się automatycznie.
 *
 * @param gameState Aktualny stan gry.
 */
void BattleEngine::finishActionAndMaybeEndTurn(GameState& gameState)
{
    if (gameState.updateVictoryState())
        return;

    if (!gameState.hasTurnActionPoints())
    {
        endTurn(gameState);
        return;
    }

    if (!hasAnyAvailableAction(gameState))
    {
        gameState.setLastActionMessage(
            QString("Drużyna %1 nie ma już dostępnych akcji. Tura kończy się automatycznie.")
                .arg(gameState.getCurrentSide() == TeamSide::Player ? "Niebieskich" : "Czerwonych"));
        endTurn(gameState);
    }
}

/**
 * @brief Obsługuje kliknięcie pola planszy.
 *
 * Metoda interpretuje kliknięcie w zależności od aktualnego stanu gry:
 * - wybór jednostki,
 * - odznaczenie jednostki,
 * - ruch,
 * - atak,
 * - leczenie,
 * - zmiana wybranej jednostki.
 *
 * @param gameState Aktualny stan gry.
 * @param x Współrzędna X klikniętego pola.
 * @param y Współrzędna Y klikniętego pola.
 */
void BattleEngine::handleTileClick(GameState& gameState, int x, int y)
{
    if (gameState.isGameFinished())
        return;

    Tile* tile = gameState.getBoard().getTile(x, y);
    if (!tile)
        return;

    if (!gameState.hasSelectedPosition())
    {
        if (!tile->isOccupied())
        {
            gameState.setLastActionMessage("Najpierw wybierz swoją jednostkę.");
            return;
        }

        Unit* unit = tile->getUnit();
        if (!unit)
            return;

        if (unit->getSide() != gameState.getCurrentSide())
        {
            gameState.setLastActionMessage("To jest jednostka przeciwnika. Najpierw kliknij swoją jednostkę, potem fioletowy cel, aby zaatakować.");
            return;
        }

        if (!canSelectUnit(gameState, unit))
            return;

        if (!unitHasAnyAvailableAction(gameState, x, y, *unit))
        {
            gameState.setLastActionMessage(
                QString("%1: %2")
                    .arg(unit->getName())
                    .arg(getUnitUnavailableReason(gameState, x, y, *unit)));
            return;
        }

        gameState.setSelectedPosition(x, y);
        calculateActionHighlights(gameState, x, y);

        gameState.setLastActionMessage(
            QString("Wybrano jednostkę: %1. Zielone pola = ruch, fioletowe = cele ataku, niebieskie = leczenie. AP drużyny: %2/%3. Ruch: do %4 pól. Koszt ataku: %5 AP.")
                .arg(unit->getName())
                .arg(gameState.getCurrentTurnActionPoints())
                .arg(gameState.getMaxTurnActionPoints())
                .arg(unit->getMovementPoints())
                .arg(unit->getAttackCost()));
        return;
    }

    Tile* selectedTile = gameState.getBoard().getTile(
        gameState.getSelectedX(),
        gameState.getSelectedY());
    Unit* selectedUnit = selectedTile ? selectedTile->getUnit() : nullptr;

    if (!selectedUnit)
    {
        gameState.clearSelectedPosition();
        gameState.clearAllHighlights();
        return;
    }

    if (gameState.getSelectedX() == x && gameState.getSelectedY() == y)
    {
        gameState.clearSelectedPosition();
        gameState.clearAllHighlights();
        gameState.setLastActionMessage("Odznaczono jednostkę.");
        return;
    }

    if (!tile->isOccupied())
    {
        tryMoveSelectedUnit(gameState, x, y);
        return;
    }

    Unit* clickedUnit = tile->getUnit();
    if (!clickedUnit)
        return;

    if (clickedUnit->getSide() == gameState.getCurrentSide())
    {
        if (selectedUnit->canHeal() && gameState.isHealablePosition(x, y))
        {
            tryHealSelectedUnit(gameState, x, y);
            return;
        }

        if (canSelectUnit(gameState, clickedUnit))
        {
            if (!unitHasAnyAvailableAction(gameState, x, y, *clickedUnit))
            {
                gameState.setLastActionMessage(
                    QString("%1: %2")
                        .arg(clickedUnit->getName())
                        .arg(getUnitUnavailableReason(gameState, x, y, *clickedUnit)));
                return;
            }

            gameState.setSelectedPosition(x, y);
            calculateActionHighlights(gameState, x, y);

            gameState.setLastActionMessage(
                QString("Wybrano jednostkę: %1. Zielone pola = ruch, fioletowe = cele ataku, niebieskie = leczenie. AP drużyny: %2/%3. Ruch: do %4 pól. Koszt ataku: %5 AP.")
                    .arg(clickedUnit->getName())
                    .arg(gameState.getCurrentTurnActionPoints())
                    .arg(gameState.getMaxTurnActionPoints())
                    .arg(clickedUnit->getMovementPoints())
                    .arg(clickedUnit->getAttackCost()));
        }
        return;
    }

    tryAttackSelectedUnit(gameState, x, y);
}

/**
 * @brief Kończy aktualną turę.
 *
 * Jeśli gra nie została zakończona, metoda przełącza turę
 * na następną stronę konfliktu.
 *
 * @param gameState Aktualny stan gry.
 */
void BattleEngine::endTurn(GameState& gameState)
{
    if (!gameState.isGameFinished())
        gameState.nextTurn();
}
