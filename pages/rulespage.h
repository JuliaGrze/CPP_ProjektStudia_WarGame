#ifndef RULESPAGE_H
#define RULESPAGE_H

#include <QWidget>

namespace Ui {
class RulesPage;
}

/**
 * @brief Klasa reprezentująca widok zasad gry.
 *
 * Wyświetla informacje dotyczące:
 * - mechaniki rozgrywki,
 * - sterowania,
 * - zasad walki i punktów akcji.
 */
class RulesPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor widoku zasad.
     *
     * @param parent Wskaźnik na widget nadrzędny.
     */
    explicit RulesPage(QWidget *parent = nullptr);

    /**
     * @brief Destruktor.
     */
    ~RulesPage();

signals:
    /**
     * @brief Sygnał powrotu do menu głównego.
     */
    void backClicked();

private:
    /**
     * @brief Ustawia treść zasad gry w interfejsie.
     *
     * Metoda wypełnia UI odpowiednim opisem zasad.
     */
    void setupRulesText();

private:
    Ui::RulesPage *ui; ///< Interfejs graficzny (Qt Designer).
};

#endif // RULESPAGE_H
