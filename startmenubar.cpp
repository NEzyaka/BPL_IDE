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

#include "startmenubar.h"

StartMenuBar::StartMenuBar(QWidget *parent) : QMenuBar(parent)
{
    setParent(parent);
    createActions();
    createMenus();
    retranslateStrings();
    setupScheme();
}

void StartMenuBar::setupScheme()
{
    this->setStyleSheet("QMenuBar { background-color: rgb(245, 246, 248); color: rgb(39, 50, 56); }"
                        "QMenuBar::item { spacing: 5px; padding: 4px 7px; background: transparent; border-bottom-left-radius: 4px; border-bottom-right-radius: 4px; }"
                        "QMenuBar::item:selected { background: rgb(225, 226, 228); }"
                        "QMenuBar::item:pressed { background: rgb(225, 226, 228); }"
                        "QMenu { background-color: rgb(245, 246, 248); border: rgb(245, 246, 248); margin: 3px; }"
                        "QMenu::item { background-color: transparent; padding: 3px 5px 5px 25px; border-radius: 3px; }"
                        "QMenu::item:selected { background-color: rgb(225, 226, 228); color: rgb(39, 50, 56); }"
                        "QMenu::separator { background: rgb(133, 143, 153); height: 1 px; }");
}

void StartMenuBar::createActions()
{
    openAction = new QAction(this);
    newFileAction = new QAction(this);
    exitAction = new QAction(this);
    manualAction = new QAction(this);
    aboutAction = new QAction(this);
    aboutQtAction = new QAction(this);

    openAction->setIcon(QPixmap(":icons/open.png")); //setting up icons
    newFileAction->setIcon(QPixmap(":icons/new.png"));
    exitAction->setIcon(QPixmap(":icons/exit.png"));
    manualAction->setIcon(QPixmap(":icons/help.png"));
    aboutAction->setIcon(QPixmap(":icons/about.png"));
    aboutQtAction->setIcon(QPixmap(":icons/qt.png"));

    openAction->setShortcut(Qt::CTRL + Qt::Key_O); //setting up shortcuts
    newFileAction->setShortcut(Qt::CTRL + Qt::Key_N);
    exitAction->setShortcut(Qt::CTRL + Qt::Key_W);
    manualAction->setShortcut(Qt::Key_F1);

    connect(openAction, SIGNAL(triggered()), SIGNAL(open()));
    connect(newFileAction, SIGNAL(triggered()), SIGNAL(newFile()));
    connect(exitAction, SIGNAL(triggered()), SIGNAL(exit()));
    connect(aboutAction, SIGNAL(triggered()), SIGNAL(about()));
    connect(aboutQtAction, SIGNAL(triggered()), SIGNAL(aboutQt()));
    connect(manualAction, SIGNAL(triggered()), SIGNAL(manual()));
}


void StartMenuBar::createMenus()
{
    startFileMenu = new QMenu(this);
    startFileMenu->addAction(openAction);
    startFileMenu->addAction(newFileAction);
    startFileMenu->addSeparator();
    startFileMenu->addAction(exitAction);

    helpMenu = new QMenu(this);
    helpMenu->addAction(manualAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void StartMenuBar::retranslateStrings()
{
    this->clear();
    this->addMenu(startFileMenu);
    this->addMenu(helpMenu);

    openAction->setText(tr("Open"));
    newFileAction->setText(tr("New File..."));
    exitAction->setText(tr("Exit"));
    aboutAction->setText(tr("About"));
    aboutQtAction->setText(tr("About Qt"));
    manualAction->setText(tr("Manual"));

    startFileMenu->setTitle(tr("File"));
    helpMenu->setTitle(tr("Help"));
}
