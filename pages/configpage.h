#ifndef CONFIGPAGE_H
#define CONFIGPAGE_H

#include <QWidget>
#include "../config/teamComposition.h"
#include "../config/gameconfig.h"

namespace Ui {
class ConfigPage;
}

/**
 * @brief Klasa reprezentująca widok konfiguracji gry.
 *
 * Umożliwia użytkownikowi:
 * - wybór wariantu mapy i rozmiaru planszy,
 * - konfigurację składu obu drużyn,
 * - rozpoczęcie gry lub powrót do menu.
 */
class ConfigPage : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Konstruktor widoku konfiguracji.
     *
     * @param parent Wskaźnik na widget nadrzędny.
     */
    explicit ConfigPage(QWidget *parent = nullptr);

    /**
     * @brief Destruktor.
     */
    ~ConfigPage();

    /**
     * @brief Zapisuje konfigurację na podstawie danych z UI.
     *
     * @return Obiekt konfiguracji gry.
     */
    GameConfig saveConfiguration() const;

    /**
     * @brief Zwraca aktualną konfigurację.
     *
     * @return Obiekt konfiguracji gry.
     */
    GameConfig getConfiguration() const;

    /**
     * @brief Sprawdza poprawność konfiguracji.
     *
     * @return true, jeśli konfiguracja jest poprawna.
     */
    bool validateConfiguration() const;

signals:
    /**
     * @brief Sygnał powrotu do menu.
     */
    void backClicked();

    /**
     * @brief Sygnał rozpoczęcia gry.
     *
     * @param config Wybrana konfiguracja gry.
     */
    void startClicked(const GameConfig& config);

private slots:
    /**
     * @brief Obsługa kliknięcia konfiguracji drużyny gracza.
     */
    void onPlayerTeamConfigClicked();

    /**
     * @brief Obsługa kliknięcia konfiguracji drużyny przeciwnika.
     */
    void onEnemyTeamConfigClicked();

    /**
     * @brief Obsługa kliknięcia przycisku startu gry.
     */
    void onStartClicked();

private:
    /**
     * @brief Ładuje dostępne opcje konfiguracji do UI.
     */
    void loadConfigurationOptions();

    /**
     * @brief Aktualizuje podsumowania drużyn w UI.
     */
    void updateTeamSummaries();

private:
    Ui::ConfigPage *ui; ///< Interfejs graficzny (Qt Designer).

    TeamComposition m_playerTeam; ///< Skład drużyny gracza.
    TeamComposition m_enemyTeam;  ///< Skład drużyny przeciwnika.
};

#endif // CONFIGPAGE_H
