/****************************************************************************
**
** Copyright (C) 2015-2016 Nikita Mironov
** Contact: nekit2002mir@yandex.ru
**
** This file is part of Turnip Editor source code.
** Turnip Editor is open-source cross-platform IDE for Turnip programming language.
**
** Turnip Editor is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Turnip Editor is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with Turnip Editor. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#ifndef STARTMENUBAR_H
#define STARTMENUBAR_H

#include <QAction>
#include <QMenuBar>

class StartMenuBar : public QMenuBar
{
    Q_OBJECT
public:
    explicit StartMenuBar(QWidget *parent = 0);
    void retranslateStrings();

signals:
    void open();
    void exit();
    void about();
    void aboutQt();
    void manual();

private:
    void createActions();
    void createMenus();
    void setupScheme();

    QAction* openAction;
    QAction* exitAction;
    QAction* aboutAction;
    QAction* aboutQtAction;
    QAction* manualAction;

    QMenu* startFileMenu;
    QMenu* helpMenu;
};

#endif // STARTMENUBAR_H
