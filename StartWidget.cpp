#include "StartWidget.h"
#include <QPushButton>
#include <QLabel>
#include <QBoxLayout>
#include "DlgGameUserSettings.h"
#include "GameUserSettings.h"
#include <QDebug>

StartWidget::StartWidget(QWidget *parent)
    : QWidget{parent}
    , _settings(std::make_unique<GameUserSettings>())
{
    _settings->readSettings();

    QPixmap pixPictures;
    pixPictures.load(QStringLiteral(":/images/ScreenImage.jpg"));

    QLabel *labelImage = new QLabel(this);
    labelImage->setScaledContents(true);
    labelImage->setPixmap(pixPictures);

    QHBoxLayout *layoutStartGameHoriz = new QHBoxLayout;
    QPushButton *btnStartGame = new QPushButton(this);
    btnStartGame->setMinimumWidth(200);
    btnStartGame->setStyleSheet(QStringLiteral("color: yellow; background: green;"));
    btnStartGame->setText(QStringLiteral("Start Game"));
    layoutStartGameHoriz->addStretch();
    layoutStartGameHoriz->addWidget(btnStartGame);
    layoutStartGameHoriz->addStretch();

    QHBoxLayout *layoutSettingsHoriz = new QHBoxLayout;
    QPushButton *btnSettings = new QPushButton(this);
    btnSettings->setMinimumWidth(200);
    btnSettings->setStyleSheet(QStringLiteral("color: yellow; background: green;"));
    btnSettings->setText(QStringLiteral("Settings..."));
    layoutSettingsHoriz->addStretch();
    layoutSettingsHoriz->addWidget(btnSettings);
    layoutSettingsHoriz->addStretch();

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(labelImage);
    layout->addSpacing(8);
    layout->addLayout(layoutStartGameHoriz);
    layout->addLayout(layoutSettingsHoriz);
    setLayout(layout);

    connect(btnStartGame, SIGNAL(clicked(bool)), this, SLOT(onStartGame()));
    connect(btnSettings, SIGNAL(clicked(bool)), this, SLOT(onSettingsDialog()));

    dlgSettings = new DlgGameUserSettings(_settings.get(), this);
    dlgSettings->setCols(_settings->settings[0]);
    dlgSettings->setRows(_settings->settings[1]);
    dlgSettings->setCountFoods(_settings->settings[2]);
    dlgSettings->setDelay(_settings->settings[3]);
}

StartWidget::~StartWidget()
{
    _settings->settings[0] = dlgSettings->getCols();
    _settings->settings[1] = dlgSettings->getRows();
    _settings->settings[2] = dlgSettings->getCountFoods();
    _settings->settings[3] = dlgSettings->getDelay();
    _settings->writeSettings();
}

void StartWidget::onStartGame()
{
    emit signalStartGame();

}

void StartWidget::onSettingsDialog()
{
    if (dlgSettings->exec() == QDialog::Accepted)
    {
        emit signalSetGameSettings(dlgSettings->getCols(), dlgSettings->getRows(),
                                   dlgSettings->getCountFoods(), dlgSettings->getDelay());
    }
}

DlgGameUserSettings *StartWidget::getDialogSettings() const
{
    return dlgSettings;
}
