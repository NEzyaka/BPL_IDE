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

#include "toolbar.h"

ToolBar::ToolBar(QWidget *parent) : QToolBar(parent)
{
    setParent(parent);
    createActions();
    retranslateStrings();
    setMovable(true);
    setupScheme();
}

void ToolBar::setupScheme()
{
    this->setStyleSheet("QToolBar { background: rgb(245, 246, 248); border: none; }"
                        "QToolBar::item { border: none; }");
}

void ToolBar::createActions()
{
    openAction = new QAction(this);
    saveAction = new QAction(this);
    saveAsAction = new QAction(this);
    exitAction = new QAction(this);
    undoAction = new QAction(this);
    redoAction = new QAction(this);
    cutAction = new QAction(this);
    copyAction = new QAction(this);
    pasteAction = new QAction(this);
    commentAction = new QAction(this);
    interpretAction = new QAction(this);

    openAction->setIcon(QPixmap(":icons/open.png")); //setting up icons
    saveAction->setIcon(QPixmap(":icons/save.png"));
    saveAsAction->setIcon(QPixmap(":icons/saveAs.png"));
    exitAction->setIcon(QPixmap(":icons/exit.png"));
    undoAction->setIcon(QPixmap(":icons/undo.png"));
    redoAction->setIcon(QPixmap(":icons/redo.png"));
    cutAction->setIcon(QPixmap(":icons/cut.png"));
    copyAction->setIcon(QPixmap(":icons/copy.png"));
    pasteAction->setIcon(QPixmap(":icons/paste.png"));
    interpretAction->setIcon(QPixmap(":icons/compile.png"));
    commentAction->setIcon(QPixmap(":icons/comment.png"));

    connect(openAction, SIGNAL(triggered()), SIGNAL(open()));
    connect(saveAction, SIGNAL(triggered()), SIGNAL(save()));
    connect(saveAsAction, SIGNAL(triggered()), SIGNAL(saveAs()));
    connect(exitAction, SIGNAL(triggered()), SIGNAL(exit()));
    connect(undoAction, SIGNAL(triggered()), SIGNAL(undo()));
    connect(redoAction, SIGNAL(triggered()), SIGNAL(redo()));
    connect(cutAction, SIGNAL(triggered()), SIGNAL(cut()));
    connect(copyAction, SIGNAL(triggered()), SIGNAL(copy()));
    connect(pasteAction, SIGNAL(triggered()), SIGNAL(paste()));
    connect(commentAction, SIGNAL(triggered()), SIGNAL(comment()));
    connect(interpretAction, SIGNAL(triggered()), SIGNAL(interpret()));
}

void ToolBar::retranslateStrings()
{
    this->clear();
    this->addAction(openAction);
    this->addAction(saveAction);
    this->addAction(saveAsAction);
    this->addAction(exitAction);
    this->addSeparator();
    this->addAction(cutAction);
    this->addAction(copyAction);
    this->addAction(pasteAction);
    this->addAction(undoAction);
    this->addAction(redoAction);
    this->addSeparator();
    this->addAction(interpretAction);

    openAction->setText(tr("Open"));
    saveAction->setText(tr("Save"));
    saveAsAction->setText(tr("Save As..."));
    exitAction->setText(tr("Exit"));
    undoAction->setText(tr("Undo"));
    redoAction->setText(tr("Redo"));
    cutAction->setText(tr("Cut"));
    copyAction->setText(tr("Copy"));
    pasteAction->setText(tr("Paste"));
    interpretAction->setText(tr("Interpret"));
    commentAction->setText(tr("Comment a line"));
}
