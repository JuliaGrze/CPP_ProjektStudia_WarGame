#ifndef TEAMCONFIGDIALOG_H
#define TEAMCONFIGDIALOG_H

#include <QDialog>
#include "teamcomposition.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class TeamConfigDialog;
}
QT_END_NAMESPACE

/**
 * @brief Okno dialogowe do konfiguracji składu drużyny.
 *
 * Umożliwia użytkownikowi ustawienie liczby jednostek:
 * - piechoty,
 * - czołgów,
 * - artylerii,
 * - medyków.
 *
 * Dialog pozwala również na podgląd łącznej liczby jednostek.
 */
class TeamConfigDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor dialogu konfiguracji drużyny.
     *
     * @param title Tytuł okna (np. "Twoja drużyna").
     * @param initialComposition Początkowy skład drużyny.
     * @param parent Wskaźnik na widget nadrzędny.
     */
    explicit TeamConfigDialog(const QString& title,
                              const TeamComposition& initialComposition,
                              QWidget *parent = nullptr);

    /**
     * @brief Destruktor.
     */
    ~TeamConfigDialog();

    /**
     * @brief Zwraca aktualny skład drużyny ustawiony w dialogu.
     *
     * @return Skład drużyny.
     */
    TeamComposition composition() const;

private:
    /**
     * @brief Wczytuje dane początkowe do interfejsu.
     */
    void loadData();

    /**
     * @brief Aktualizuje etykietę z łączną liczbą jednostek.
     */
    void updateTotalLabel();

private:
    Ui::TeamConfigDialog *ui; ///< Interfejs graficzny (Qt Designer).

    QString m_title; ///< Tytuł dialogu.
    TeamComposition m_composition; ///< Aktualny skład drużyny.
};

#endif // TEAMCONFIGDIALOG_H
