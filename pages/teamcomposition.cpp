#include "teamcomposition.h"
#include "ui_teamcomposition.h"

TeamComposition::TeamComposition(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TeamComposition)
{
    ui->setupUi(this);
}

TeamComposition::~TeamComposition()
{
    delete ui;
}
