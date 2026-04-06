#include "rulespage.h"
#include "ui_rulespage.h"

#include <QPushButton>

RulesPage::RulesPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RulesPage)
{
    ui->setupUi(this);

    setupRulesText();

    connect(ui->btnBack, &QPushButton::clicked,
            this, &RulesPage::backClicked);
}

RulesPage::~RulesPage()
{
    delete ui;
}

void RulesPage::setupRulesText()
{
    ui->textBrowser->setHtml(R"(
<h2>Cel gry</h2>
<p>
Celem gry jest zniszczenie wszystkich jednostek przeciwnika.
Rozgrywka odbywa się na planszy 2D w systemie turowym.
</p>

<h2>Przebieg gry</h2>
<p>
Gra składa się z tur. Każda drużyna wykonuje ruchy naprzemiennie.
W swojej turze gracz wybiera jednostkę i wykonuje akcje:
ruch, atak lub leczenie.
</p>

<h2>Plansza i teren</h2>
<ul>
<li>Równina – brak efektu</li>
<li>Las – zwiększa osłonę</li>
<li>Góry – trudniejszy ruch</li>
<li>Woda – nie można wejść</li>
<li>Budynki – osłona</li>
</ul>

<h2>Jednostki</h2>
<ul>
<li><b>Piechota</b> – uniwersalna</li>
<li><b>Czołg</b> – silny i wytrzymały</li>
<li><b>Artyleria</b> – daleki zasięg, słaba z bliska</li>
<li><b>Medyk</b> – leczy sojuszników</li>
</ul>

<h2>Statystyki</h2>
<ul>
<li>HP – życie jednostki</li>
<li>Obrażenia – siła ataku</li>
<li>Zasięg – odległość ataku</li>
<li>Pancerz – redukcja obrażeń</li>
<li>Celność – szansa trafienia</li>
<li>Unik – szansa uniknięcia ataku</li>
</ul>

<h2>Walka</h2>
<p>
Atak jest losowy – zależy od celności, uniku, dystansu i terenu.
Nie każdy strzał trafia.
</p>

<h2>Punkty akcji</h2>
<p>
Drużyna posiada wspólną pulę punktów akcji.
Każda akcja (ruch, atak, leczenie) zużywa punkty.
Po ich wyczerpaniu kończy się tura.
</p>

<h2>Sztuczna inteligencja</h2>
<p>
Przeciwnik sterowany komputerowo wykonuje ruchy i ataki
na podstawie sytuacji na planszy.
</p>

<h2>Warunek zwycięstwa</h2>
<p>
Wygrywa drużyna, która jako pierwsza zniszczy wszystkie jednostki przeciwnika.
</p>

<h2>Statystyki po grze</h2>
<ul>
<li>liczba tur</li>
<li>straty</li>
<li>obrażenia</li>
<li>skuteczność</li>
</ul>
)");
}
