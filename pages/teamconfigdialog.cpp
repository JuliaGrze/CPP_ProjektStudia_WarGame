#include "teamconfigdialog.h"
#include "ui_teamconfigdialog.h"

TeamConfigDialog::TeamConfigDialog(const QString& title,
                                   const TeamComposition& initialComposition,
                                   QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TeamConfigDialog)
    , m_title(title)
    , m_composition(initialComposition)
{
    ui->setupUi(this);

    setWindowTitle(title);
    ui->titleLabel->setText(title);

    loadData();
    updateTotalLabel();

    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    connect(ui->saveButton, &QPushButton::clicked, this, &QDialog::accept);

    connect(ui->infantrySpinBox, &QSpinBox::valueChanged, this, [this](int) { updateTotalLabel(); });
    connect(ui->tankSpinBox, &QSpinBox::valueChanged, this, [this](int) { updateTotalLabel(); });
    connect(ui->artillerySpinBox, &QSpinBox::valueChanged, this, [this](int) { updateTotalLabel(); });
    connect(ui->medicSpinBox, &QSpinBox::valueChanged, this, [this](int) { updateTotalLabel(); });
}

TeamConfigDialog::~TeamConfigDialog()
{
    delete ui;
}

TeamComposition TeamConfigDialog::composition() const
{
    TeamComposition result;
    result.infantry = ui->infantrySpinBox->value();
    result.tank = ui->tankSpinBox->value();
    result.artillery = ui->artillerySpinBox->value();
    result.medic = ui->medicSpinBox->value();
    return result;
}

void TeamConfigDialog::loadData()
{
    ui->infantrySpinBox->setValue(m_composition.infantry);
    ui->tankSpinBox->setValue(m_composition.tank);
    ui->artillerySpinBox->setValue(m_composition.artillery);
    ui->medicSpinBox->setValue(m_composition.medic);
}

void TeamConfigDialog::updateTotalLabel()
{
    const int total =
        ui->infantrySpinBox->value() +
        ui->tankSpinBox->value() +
        ui->artillerySpinBox->value() +
        ui->medicSpinBox->value();

    ui->totalLabel->setText(QString("Łącznie jednostek: %1").arg(total));
}
