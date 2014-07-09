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
        mainwindow.cpp \
    stickyline.cpp \
    stickynote.cpp \
    stickynotesignalhandler.cpp \
    stickylinesignalhandler.cpp

HEADERS  += mainwindow.h \
    staffGamesConstants.h \
    stickyline.h \
    stickynote.h \
    stickynotesignalhandler.h \
    stickylinesignalhandler.h

FORMS    += mainwindow.ui

CONFIG += mobility
CONFIG += c++11
MOBILITY = 

RESOURCES += \
    res/res.qrc

