#-------------------------------------------------
#
# Project created by QtCreator 2016-01-02T13:20:59
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qbatt
TEMPLATE = app


SOURCES += main.cpp\
        qbattmain.cpp \
    qbattstats.cpp \
    qbattdbusmethod.cpp \
    qbattsysfsmethod.cpp \
    qbatticon_t.cpp \
    qbatttray_t.cpp \
    qbattsingle.cpp

HEADERS  += qbattmain.h \
    qbattstats.h \
    qbattdbusmethod.h \
    qbattsysfsmethod.h \
    qbattpsu.h \
    qbatticon_t.h \
    qbatttray_t.h \
    qbattsingle.h

FORMS    +=

RESOURCES += \
    icons.qrc
