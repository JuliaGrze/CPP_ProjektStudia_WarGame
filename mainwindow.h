#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MenuPage;
class ConfigPage;
class BattlePage;
class RulesPage;
class GameController;

/**
 * @brief Główne okno aplikacji WarGame.
 *
 * Klasa odpowiada za zarządzanie głównym interfejsem użytkownika
 * oraz przełączanie pomiędzy poszczególnymi ekranami aplikacji:
 * - menu głównym,
 * - konfiguracją rozgrywki,
 * - widokiem bitwy,
 * - ekranem zasad.
 *
 * Dodatkowo przechowuje instancję kontrolera gry odpowiedzialnego
 * za logikę rozgrywki.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor głównego okna.
     *
     * Tworzy i inicjalizuje wszystkie komponenty interfejsu.
     *
     * @param parent Wskaźnik na obiekt nadrzędny Qt.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destruktor głównego okna.
     *
     * Zwalnia zaalokowane zasoby.
     */
    ~MainWindow();

private:
    Ui::MainWindow *ui;          ///< Interfejs użytkownika wygenerowany przez Qt Designer.
    MenuPage *menuPage;          ///< Strona menu głównego.
    ConfigPage *configPage;      ///< Strona konfiguracji gry.
    BattlePage *battlePage;      ///< Strona rozgrywki (bitwy).
    RulesPage *rulesPage;        ///< Strona z zasadami gry.
    GameController *gameController; ///< Kontroler zarządzający logiką gry.
};

#endif // MAINWINDOW_H
