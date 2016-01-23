#-------------------------------------------------
#
# Project created by QtCreator 2015-08-21T17:42:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Turnip-Editor
TEMPLATE = app

SOURCES +=  main.cpp\
            mainwindow.cpp \
            codeeditor.cpp \
            highlighter.cpp

HEADERS  += mainwindow.h \
            codeeditor.h \
            highlighter.h

RC_FILE =   resources.rc

DISTFILES += resources.rc
