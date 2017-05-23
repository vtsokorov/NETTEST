#-------------------------------------------------
#
# Project created by QtCreator 2016-05-18T22:28:04
#
#-------------------------------------------------

QT       += core gui network

QMAKE_CXXFLAGS += -std=c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    global.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    aboutdialog.ui

RESOURCES += \
    img.qrc
