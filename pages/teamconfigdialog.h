#ifndef TEAMCONFIGDIALOG_H
#define TEAMCONFIGDIALOG_H

#include <QDialog>
#include "teamcomposition.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class TeamConfigDialog;
}
QT_END_NAMESPACE

class TeamConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TeamConfigDialog(const QString& title,
                              const TeamComposition& initialComposition,
                              QWidget *parent = nullptr);
    ~TeamConfigDialog();

    TeamComposition composition() const;

private:
    void loadData();
    void updateTotalLabel();

    Ui::TeamConfigDialog *ui;
    QString m_title;
    TeamComposition m_composition;
};

#endif // TEAMCONFIGDIALOG_H
