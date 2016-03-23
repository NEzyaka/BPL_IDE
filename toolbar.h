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

#ifndef TOOLBAR_H
#define TOOLBAR_H


#include <QToolBar>
#include <QAction>

#include "widget.h"

class ToolBar : public QToolBar, public Widget
{
    Q_OBJECT
public:
    ToolBar(QWidget *parent = 0);
    void retranslateStrings();
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

public slots:
    void setUndoEnabled(bool state);
    void setRedoEnabled(bool state);
    void setCopyEnabled(bool state);
    void setCutEnabled(bool state);

private:
    void createActions();

    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* commentAction;
    QAction* interpretAction;
};

#endif // TOOLBAR_H
