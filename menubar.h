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

#ifndef MENUBAR_H
#define MENUBAR_H

#include <QMenuBar>
#include <QActionGroup>

#include "widget.h"

class MenuBar : public QMenuBar, public Widget
{
    Q_OBJECT
public:
    explicit MenuBar(QWidget *parent = 0);
    void retranslateStrings();
    bool getFullscrState() { return fullscrAction->isChecked(); }
    void setFullscrState(bool state) { fullscrAction->setChecked(state); }
    void setLanguage(QString lang);
    void setupScheme();

signals:
    void open();
    void save();
    void saveAs();
    void exit();
    void undo();
    void redo();
    void copy();
    void paste();
    void cut();
    void comment();
    void interpret();
    void fullscr();
    void chooseInterpreter();
    void groupLanguage(QAction*);
    void about();
    void aboutQt();
    void manual();

private:
    void createActions();
    void createMenus();


    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;
    
    QAction* undoAction;
    QAction* redoAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* cutAction;
    QAction* commentAction;
    
    QAction* interpretAction;
    
    QAction* fullscrAction;
    QAction* chooseInterpreterAction;
    QActionGroup* languageActionGroup;
    QAction* rusAction;
    QAction* engAction;
    QAction* gerAction;
    
    QAction* aboutAction;
    QAction* aboutQtAction;
    QAction* manualAction;
    
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* compileMenu;
    QMenu* settingsMenu;
    QMenu* environmentMenu;
    QMenu* langMenu;
    QMenu* helpMenu;

};

#endif // MENUBAR_H
