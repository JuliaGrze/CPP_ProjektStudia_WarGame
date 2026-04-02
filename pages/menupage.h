#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QWidget>

namespace Ui {
class MenuPage;
}

class MenuPage : public QWidget
{
    Q_OBJECT

public:
    explicit MenuPage(QWidget *parent = nullptr);
    ~MenuPage();

signals:
    void playClicked();
    void rulesClicked();
    void exitClicked();

private:
    Ui::MenuPage *ui;
};

#endif // MENUPAGE_H
