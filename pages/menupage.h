#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QWidget>

namespace Ui {
class MenuPage;
}

/**
 * @brief Klasa reprezentująca główne menu gry.
 *
 * Umożliwia użytkownikowi:
 * - rozpoczęcie gry,
 * - przejście do zasad,
 * - wyjście z aplikacji.
 */
class MenuPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor menu.
     *
     * @param parent Wskaźnik na widget nadrzędny.
     */
    explicit MenuPage(QWidget *parent = nullptr);

    /**
     * @brief Destruktor.
     */
    ~MenuPage();

signals:
    /**
     * @brief Sygnał rozpoczęcia gry.
     */
    void playClicked();

    /**
     * @brief Sygnał przejścia do zasad gry.
     */
    void rulesClicked();

    /**
     * @brief Sygnał zamknięcia aplikacji.
     */
    void exitClicked();

private:
    Ui::MenuPage *ui; ///< Interfejs graficzny (Qt Designer).
};

#endif // MENUPAGE_H
