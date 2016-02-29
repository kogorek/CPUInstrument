#-------------------------------------------------
#
# Project created by QtCreator 2016-02-05T22:46:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CPUInstrumentGUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cpu.cpp

HEADERS  += mainwindow.h \
    cpu.h

FORMS    += mainwindow.ui
