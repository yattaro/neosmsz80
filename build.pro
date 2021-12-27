TEMPLATE = app

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += main.cpp

LIBS += -lemu -lgui -Llibs

TARGET = neosmsz80-qt
