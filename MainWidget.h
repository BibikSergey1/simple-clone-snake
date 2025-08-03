#pragma once

#include <QWidget>

class Ui_MainWidget;
class QStackedWidget;
class GameWidget;
class StartWidget;

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    std::unique_ptr<Ui_MainWidget> ui;
    QStackedWidget *stackedWidget = nullptr;
    GameWidget *gameWidget = nullptr;
    StartWidget *startWidget = nullptr;    

public slots:
    void onWidgetStopGame();
    void onWidgetStartGame();
    void onSetGameSettings(int cols, int rows, int countFoods, int delay);
};
