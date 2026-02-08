#include "MainWidget.h"
#include "ui_MainWidget.h"
#include <QStackedWidget>
#include <QBoxLayout>
#include <QDebug>
#include "GameWidget.h"
#include "StartWidget.h"
#include "DlgGameUserSettings.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_MainWidget)
{
    ui->setupUi(this);
    resize(800, 600);

    startWidget = new StartWidget();
    connect(startWidget, SIGNAL(signalStartGame()), this, SLOT(onWidgetStartGame()));
    connect(startWidget, SIGNAL(signalSetGameSettings(int, int, int, int)), this, SLOT(onSetGameSettings(int, int, int, int)));

    const auto dlgSettings = startWidget->getDialogSettings();
    gameWidget = new GameWidget(dlgSettings->getCols() + 1, dlgSettings->getRows() + 1, dlgSettings->getCountFoods());
    gameWidget->game->delay = dlgSettings->getDelay();
    connect(gameWidget, SIGNAL(signalStopGame()), this, SLOT(onWidgetStopGame()));

    stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(startWidget);
    stackedWidget->addWidget(gameWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(stackedWidget);
    setLayout(layout);

    setPalette(QPalette(Qt::darkGray));
}

MainWidget::~MainWidget()
{
}

void MainWidget::onWidgetStopGame()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWidget::onWidgetStartGame()
{
    stackedWidget->setCurrentIndex(1);
    gameWidget->startGame();
}

void MainWidget::onSetGameSettings(int cols, int rows, int countFoods, int delay)
{
    gameWidget->setGameSettings(cols, rows, countFoods, delay);
}
