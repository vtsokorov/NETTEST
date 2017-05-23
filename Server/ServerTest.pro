#-------------------------------------------------
#
# Project created by QtCreator 2016-05-20T13:10:25
#
#-------------------------------------------------

QT       += core gui network

QMAKE_CXXFLAGS += -std=c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ServerTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    servertest.cpp \
    threadtest.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    servertest.h \
    threadtest.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui

RESOURCES += \
    img.qrc
