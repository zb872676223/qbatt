#-------------------------------------------------
#
# Project created by QtCreator 2016-01-02T13:20:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qbatt
TEMPLATE = app


SOURCES += main.cpp\
        qbattmain.cpp \
    qbattmodel.cpp \
    qbattstats.cpp

HEADERS  += qbattmain.h \
    qbattmodel.h \
    qbattstats.h

FORMS    += qbattmain.ui
