#-------------------------------------------------
#
# Project created by QtCreator 2018-03-25T19:33:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += serialport

CONFIG += c++11

TARGET = p20Client
TEMPLATE = app


SOURCES += main.cpp \
    commgrid.cpp \
    canvas.cpp \
    screen.cpp \
    colourchooser.cpp \
    testsquare.cpp

HEADERS  += \
    commgrid.h \
    canvas.h \
    screen.h \
    colourchooser.h \
    testsquare.h

FORMS    +=
