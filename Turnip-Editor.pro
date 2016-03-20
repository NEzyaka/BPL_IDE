######################################
#
# Copyright (C) 2015-2016 Nikita Mironov
# Contact: nekit2002mir@yandex.ru
#
# This file is part of Turnip Editor source code.
# Turnip Editor is open-source cross-platform IDE for Turnip programming language.
#
# Turnip Editor is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Turnip Editor is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Turnip Editor. If not, see <http://www.gnu.org/licenses/>.
#
######################################

QT           += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET       =  Turnip-Editor
TEMPLATE     =  app

SOURCES      += main.cpp\
                mainwindow.cpp \
                menubar.cpp \
                codeeditor.cpp \
                highlighter.cpp \
                toolbar.cpp \
                startmenubar.cpp \
                manualviewer.cpp \
                subtools.cpp \
                outputgetter.cpp \
                interpreter.cpp

HEADERS      += mainwindow.h \
                menubar.h \
                codeeditor.h \
                highlighter.h \
                toolbar.h \
                startmenubar.h \
                manualviewer.h \
                widget.h \
                subtools.h \
                outputgetter.h \
                interpreter.h

TRANSLATIONS += translations/russian.ts \
                translations/english.ts \
                translations/german.ts

RC_FILE       = resources.rc

RESOURCES    += icons.qrc \
                help.qrc \
                translations.qrc

DISTFILES    += resources.rc

#QMAKE_LFLAGS_RELEASE += -static -static-libgcc
