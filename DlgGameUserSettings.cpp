#include "DlgGameUserSettings.h"
#include "ui_DlgGameUserSettings.h"
#include <QPushButton>
#include <QSpinBox>
#include "GameUserSettings.h"

DlgGameUserSettings::DlgGameUserSettings(GameUserSettings *settings, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DlgGameUserSettings)
    , _settings(settings)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral("Settings"));

    connect(ui->pushButtonApplay, &QPushButton::clicked, this, &DlgGameUserSettings::applay);
    connect(ui->pushButtonCancal, &QPushButton::clicked, this, &DlgGameUserSettings::cancel);

    connect(ui->spinBoxCols, SIGNAL(valueChanged(int)), this, SLOT(setRows(int)));
    connect(ui->spinBoxRows, SIGNAL(valueChanged(int)), this, SLOT(setCols(int)));
}

DlgGameUserSettings::~DlgGameUserSettings()
{
    delete ui;
}

int DlgGameUserSettings::getRows() const
{
    return ui->spinBoxRows->value();
}

int DlgGameUserSettings::getCols() const
{
    return ui->spinBoxCols->value();
}

int DlgGameUserSettings::getCountFoods() const
{
    return ui->spinBoxCountFoods->value();
}

int DlgGameUserSettings::getDelay() const
{
    return ui->spinBoxDelay->value();
}

void DlgGameUserSettings::setCols(int value)
{
    ui->spinBoxCols->setValue(value);
}

void DlgGameUserSettings::setRows(int value)
{
    ui->spinBoxRows->setValue(value);
}

void DlgGameUserSettings::setCountFoods(int value)
{
    ui->spinBoxCountFoods->setValue(value);
}

void DlgGameUserSettings::setDelay(int value)
{
    ui->spinBoxDelay->setValue(value);
}

void DlgGameUserSettings::applay()
{
    accept();
}

void DlgGameUserSettings::cancel()
{
    reject();
}
