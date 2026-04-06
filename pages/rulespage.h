#ifndef RULESPAGE_H
#define RULESPAGE_H

#include <QWidget>

namespace Ui {
class RulesPage;
}

class RulesPage : public QWidget
{
    Q_OBJECT

public:
    explicit RulesPage(QWidget *parent = nullptr);
    ~RulesPage();

signals:
    void backClicked();

private:
    Ui::RulesPage *ui;
    void setupRulesText();
};

#endif // RULESPAGE_H
