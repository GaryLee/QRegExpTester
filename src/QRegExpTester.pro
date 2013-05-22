#-------------------------------------------------
#
# Project created by QtCreator 2013-05-21T10:13:45
#
#-------------------------------------------------
# This file is subject to the terms and conditions defined in
# file 'LICENSE.txt', which is part of this source code package.

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QRegExpTester
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    regextesterwindow.cpp \
    regexpcontroller.cpp

HEADERS  += mainwindow.h \
    regextesterwindow.h \
    regexpcontroller.h

FORMS    += mainwindow.ui \
    regextesterwindow.ui

RESOURCES += \
    qregexptester.qrc

OTHER_FILES += \
    LICENSE.txt
