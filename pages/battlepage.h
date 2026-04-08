#ifndef BATTLEPAGE_H
#define BATTLEPAGE_H

#include <QWidget>

class GameController;
class QResizeEvent;
class QShowEvent;

namespace Ui {
class BattlePage;
}

/**
 * @brief Klasa reprezentująca widok bitwy w interfejsie użytkownika.
 *
 * Odpowiada za:
 * - wyświetlanie planszy,
 * - obsługę interakcji użytkownika,
 * - aktualizację informacji o turze i statystykach,
 * - prezentację podsumowania po zakończeniu gry.
 */
class BattlePage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor widoku bitwy.
     *
     * @param parent Wskaźnik na widget nadrzędny.
     */
    explicit BattlePage(QWidget *parent = nullptr);

    /**
     * @brief Destruktor.
     */
    ~BattlePage();

    /**
     * @brief Ustawia kontroler gry.
     *
     * @param controller Wskaźnik na GameController.
     */
    void setController(GameController* controller);

    /**
     * @brief Przerysowuje planszę.
     */
    void redrawBoard();

    /**
     * @brief Aktualizuje informacje o turze.
     */
    void updateTurnInfo();

    /**
     * @brief Odświeża statystyki gry.
     */
    void refreshStatistics();

    /**
     * @brief Aktualizuje informacje o wybranym polu planszy.
     */
    void updateTileInfo();

signals:
    /**
     * @brief Sygnał powrotu do menu głównego.
     */
    void backToMenuClicked();

    /**
     * @brief Sygnał rozpoczęcia nowej gry z tą samą konfiguracją.
     */
    void playAgainClicked();

    /**
     * @brief Sygnał przejścia do konfiguracji nowej gry.
     */
    void newConfigurationClicked();

protected:
    /**
     * @brief Obsługa zmiany rozmiaru okna.
     *
     * @param event Zdarzenie zmiany rozmiaru.
     */
    void resizeEvent(QResizeEvent *event) override;

    /**
     * @brief Obsługa wyświetlenia widoku.
     *
     * @param event Zdarzenie pokazania widoku.
     */
    void showEvent(QShowEvent *event) override;

private:
    /**
     * @brief Obsługa kliknięcia pola planszy.
     *
     * @param x Współrzędna X.
     * @param y Współrzędna Y.
     */
    void onTileClicked(int x, int y);

    /**
     * @brief Wyświetla okno podsumowania po zakończeniu gry.
     */
    void showPostGameSummaryDialog();

    /**
     * @brief Odświeża cały widok (plansza + UI).
     */
    void refreshView();

private:
    Ui::BattlePage *ui;                ///< Interfejs graficzny (Qt Designer).
    GameController* m_controller = nullptr; ///< Kontroler logiki gry.

    bool m_isDrawingBoard = false;     ///< Flaga zapobiegająca wielokrotnemu rysowaniu planszy.
    bool m_postGameDialogShown = false; ///< Flaga zapobiegająca wielokrotnemu wyświetleniu podsumowania.
};

#endif // BATTLEPAGE_H
