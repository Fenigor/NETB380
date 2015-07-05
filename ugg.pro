#-------------------------------------------------
#
# Project created by QtCreator 2015-05-10T16:23:02
#
#-------------------------------------------------

QT       += core gui multimedia
QT       += core sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ugg
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    secdialog.cpp \
    musicplayerdialog.cpp

HEADERS  += mainwindow.h \
    secdialog.h \
    musicplayerdialog.h

FORMS    += mainwindow.ui \
    secdialog.ui \
    musicplayerdialog.ui
