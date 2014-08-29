#-------------------------------------------------
#
# Project created by QtCreator 2014-07-03T20:18:17
#
#-------------------------------------------------

QT       += core gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = staffGames
TEMPLATE = app


SOURCES += main.cpp\
    ledgerline.cpp \
    linemanager.cpp \
    note.cpp \
    staffline.cpp \
    staffscene.cpp \
    game_notefinding.cpp \
    mainwindow.cpp \
    title.cpp

HEADERS  += \
    staffGamesConstants.h \
    ledgerline.h \
    linemanager.h \
    note.h \
    staffline.h \
    staffscene.h \
    game_notefinding.h \
    mainwindow.h \
    title.h

FORMS    += \
    game_notefinding.ui \
    mainwindow.ui \
    title.ui

CONFIG += mobility
CONFIG += c++11

MOBILITY = 

RESOURCES += \
    res/res.qrc

OTHER_FILES += \
    Urls.txt \
    res/Minim.jpeg

