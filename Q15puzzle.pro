#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T00:35:25
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Q15puzzle
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    qnumber.cpp

HEADERS  += mainwindow.hpp \
    puzzle.hpp \
    qnumber.hpp

FORMS    += mainwindow.ui
QMAKE_CXXFLAGS += -std=c++1z
