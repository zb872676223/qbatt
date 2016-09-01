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
    qbattstats.cpp \
    qbattsysfsmethod.cpp \
    qbatticon_t.cpp \
    qbatttray_t.cpp

HEADERS  += qbattmain.h \
    qbattstats.h \
    qbattsysfsmethod.h \
    qbattpsu.h \
    qbatticon_t.h \
    qbatttray_t.h

FORMS    +=

RESOURCES += \
    icons.qrc
