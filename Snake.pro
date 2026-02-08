QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

DEFINES += is_stacked_widget

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    DlgGameUserSettings.cpp \
    Foods.cpp \
    Game.cpp \
    GameUserSettings.cpp \
    GameWidget.cpp \
    MainWidget.cpp \
    Point.cpp \
    Snake.cpp \
    Sprite.cpp \
    StartWidget.cpp \
    main.cpp

HEADERS += \
    DlgGameUserSettings.h \
    Foods.h \
    Game.h \
    GameUserSettings.h \
    GameWidget.h \
    MainWidget.h \
    Point.h \
    Snake.h \
    Sprite.h \
    StartWidget.h

FORMS += \
    DlgGameUserSettings.ui \
    MainWidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES +=
