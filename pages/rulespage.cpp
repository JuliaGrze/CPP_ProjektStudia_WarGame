#include "rulespage.h"
#include "ui_rulespage.h"

#include <QPushButton>

/**
 * @brief Konstruktor strony zasad gry.
 *
 * Inicjalizuje interfejs użytkownika, ustawia treść zasad
 * oraz podłącza przycisk powrotu do menu.
 *
 * @param parent Wskaźnik na widget nadrzędny.
 */
RulesPage::RulesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RulesPage)
{
    ui->setupUi(this);

    setupRulesText();

    connect(ui->btnBack, &QPushButton::clicked,
            this, &RulesPage::backClicked);
}

/**
 * @brief Destruktor strony zasad.
 */
RulesPage::~RulesPage()
{
    delete ui;
}

/**
 * @brief Ustawia treść zasad gry w komponencie tekstowym.
 *
 * Metoda generuje i przypisuje sformatowany HTML zawierający:
 * - cel gry,
 * - przebieg rozgrywki,
 * - opis terenu,
 * - typy jednostek,
 * - statystyki,
 * - zasady walki,
 * - punkty akcji,
 * - działanie AI,
 * - warunek zwycięstwa,
 * - statystyki po zakończeniu gry.
 */
void RulesPage::setupRulesText()
{
    ui->textBrowser->setHtml(R"(
        <h2 style="color:#2f80ed;">Cel gry</h2>
        <p>
        Celem gry jest zniszczenie wszystkich jednostek przeciwnika.
        Rozgrywka odbywa się na planszy 2D w systemie turowym.
        </p>

        <h2 style="color:#2f80ed;">Przebieg gry</h2>
        <p>
        Gra składa się z tur. Każda drużyna wykonuje ruchy naprzemiennie.
        W swojej turze gracz wybiera jednostkę i wykonuje akcje:
        <b>ruch</b>, <b>atak</b> lub <b>leczenie</b>.
        </p>

        <h2 style="color:#2f80ed;">Plansza i teren</h2>
        <ul>
        <li><b>Równina</b> – brak efektu</li>
        <li><b>Las</b> – zwiększa osłonę</li>
        <li><b>Góry</b> – trudniejszy ruch</li>
        <li><b>Woda</b> – nie można wejść</li>
        <li><b>Budynki</b> – osłona</li>
        </ul>

        <h2 style="color:#2f80ed;">Jednostki</h2>
        <ul>
        <li><b>Piechota</b> – uniwersalna</li>
        <li><b>Czołg</b> – silny i wytrzymały</li>
        <li><b>Artyleria</b> – daleki zasięg, słaba z bliska</li>
        <li><b>Medyk</b> – leczy sojuszników</li>
        </ul>

        <h2 style="color:#2f80ed;">Statystyki</h2>
        <ul>
        <li><b>HP</b> – życie jednostki</li>
        <li><b>Obrażenia</b> – siła ataku</li>
        <li><b>Zasięg</b> – odległość ataku</li>
        <li><b>Pancerz</b> – redukcja obrażeń</li>
        <li><b>Celność</b> – szansa trafienia</li>
        <li><b>Unik</b> – szansa uniknięcia ataku</li>
        </ul>

        <h2 style="color:#2f80ed;">Walka</h2>
        <p>
        Atak zawiera element losowości – zależy od celności, uniku, dystansu i terenu.
        Nie każdy strzał trafia.
        </p>

        <h2 style="color:#2f80ed;">Punkty akcji</h2>
        <p>
        Drużyna posiada wspólną pulę punktów akcji.
        Każda akcja zużywa punkty – po ich wyczerpaniu kończy się tura.
        </p>

        <h2 style="color:#2f80ed;">Sztuczna inteligencja</h2>
        <p>
        Przeciwnik sterowany komputerowo podejmuje decyzje na podstawie sytuacji na planszy.
        </p>

        <h2 style="color:#2f80ed;">Warunek zwycięstwa</h2>
        <p>
        Wygrywa drużyna, która jako pierwsza zniszczy wszystkie jednostki przeciwnika.
        </p>

        <h2 style="color:#2f80ed;">Statystyki po grze</h2>
        <ul>
        <li>liczba tur</li>
        <li>straty</li>
        <li>obrażenia</li>
        <li>skuteczność</li>
        </ul>
        )");
}
