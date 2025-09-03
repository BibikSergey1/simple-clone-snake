#include "MainWidget.h"

#include <QApplication>
#ifndef is_stacked_widget
#include "GameWidget.h"
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
#ifdef is_stacked_widget
    MainWidget w;
    w.show();
#else
    GameWidget wgt(10 + 1, 10 + 1, 5);
    wgt.resize(800, 600);
    wgt.game->delay = 450;
    wgt.startGame();
    wgt.show();
#endif
    return a.exec();
}
