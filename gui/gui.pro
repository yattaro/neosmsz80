! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


FORMS += mainwindow.ui
HEADERS += mainwindow.h
SOURCES += mainwindow.cpp

RESOURCES += \
    resources.qrc
