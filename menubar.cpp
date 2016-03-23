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

#include "menubar.h"

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    setParent(parent);
    createActions();
    createMenus();
    retranslateStrings();
}

void MenuBar::setupScheme()
{
    this->setStyleSheet("QMenuBar { background-color: rgb(245, 246, 248); color: rgb(39, 50, 56); }"
                        "QMenuBar::item { spacing: 5px; padding: 4px 7px; background: transparent; border-bottom-left-radius: 4px; border-bottom-right-radius: 4px; }"
                        "QMenuBar::item:selected { background: rgb(225, 226, 228); }"
                        "QMenuBar::item:pressed { background: rgb(225, 226, 228); }"
                        "QMenu { background-color: rgb(245, 246, 248); border: rgb(245, 246, 248); margin: 3px; }"
                        "QMenu::item { background-color: transparent; padding: 3px 5px 5px 25px; border-radius: 3px; }"
                        "QMenu::item:selected { background-color: rgb(225, 226, 228); color: rgb(39, 50, 56); }"
                        "QMenu::separator { background: rgb(133, 143, 153); height: 1 px; }"
                        "QMenu::item:checked { background: rgb(200, 230, 201); }");
}

void MenuBar::setLanguage(QString lang)
{
    if(lang == "Russian")
    {
        rusAction->setChecked(true);
        engAction->setChecked(false);
        gerAction->setChecked(false);
    }
    else if(lang == "English")
    {
        rusAction->setChecked(false);
        engAction->setChecked(true);
        gerAction->setChecked(false);
    }
    else if(lang == "German")
    {
        rusAction->setChecked(false);
        engAction->setChecked(false);
        gerAction->setChecked(true);
    }
}

void MenuBar::createActions()
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
    interpretAction = new QAction(this);
    fullscrAction = new QAction(this);
    chooseInterpreterAction = new QAction(this);
    languageActionGroup = new QActionGroup(this);
    rusAction = new QAction(languageActionGroup);
    engAction = new QAction(languageActionGroup);
    gerAction = new QAction(languageActionGroup);
    commentAction = new QAction(this);
    manualAction = new QAction(this);
    aboutAction = new QAction(this);
    aboutQtAction = new QAction(this);

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
    fullscrAction->setIcon(QPixmap(":icons/fullscreen.png"));
    chooseInterpreterAction->setIcon(QPixmap(":icons/compiler.png"));
    engAction->setIcon(QPixmap(":icons/en.png"));
    rusAction->setIcon(QPixmap(":icons/ru.png"));
    gerAction->setIcon(QPixmap(":icons/de.png"));
    commentAction->setIcon(QPixmap(":icons/comment.png"));
    manualAction->setIcon(QPixmap(":icons/help.png"));
    aboutAction->setIcon(QPixmap(":icons/about.png"));
    aboutQtAction->setIcon(QPixmap(":icons/qt.png"));

    openAction->setShortcut(Qt::CTRL + Qt::Key_O); //setting up shortcuts
    saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
    exitAction->setShortcut(Qt::CTRL + Qt::Key_W);
    undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
    redoAction->setShortcut(Qt::CTRL + Qt::Key_Y);
    cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
    copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);
    interpretAction->setShortcut(Qt::CTRL + Qt::Key_R);
    fullscrAction->setShortcut(Qt::Key_F11);
    chooseInterpreterAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_I);
    engAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_E);
    rusAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_R);
    gerAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_G);
    commentAction->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    manualAction->setShortcut(Qt::Key_F1);

    fullscrAction->setCheckable(true); //setting up checkability
    rusAction->setCheckable(true);
    engAction->setCheckable(true);
    gerAction->setCheckable(true);

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
    connect(fullscrAction, SIGNAL(triggered()), SIGNAL(fullscr()));
    connect(chooseInterpreterAction, SIGNAL(triggered()), SIGNAL(chooseInterpreter()));
    connect(languageActionGroup, SIGNAL(triggered(QAction*)), SIGNAL(groupLanguage(QAction*)));
    connect(aboutAction, SIGNAL(triggered()), SIGNAL(about()));
    connect(aboutQtAction, SIGNAL(triggered()), SIGNAL(aboutQt()));
    connect(manualAction, SIGNAL(triggered()), SIGNAL(manual()));
}


void MenuBar::createMenus()
{
    fileMenu = new QMenu(this);
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu = new QMenu(this);
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(commentAction);

    compileMenu = new QMenu(this);
    compileMenu->addAction(interpretAction);

    settingsMenu = new QMenu(this);
    environmentMenu = new QMenu(this);
    environmentMenu->setIcon(QPixmap(":icons/view.png"));
    langMenu = new QMenu(this);
    langMenu->setIcon(QPixmap(":icons/earth.png"));
    langMenu->addActions(languageActionGroup->actions());
    environmentMenu->addMenu(langMenu);
    environmentMenu->addAction(fullscrAction);
    settingsMenu->addMenu(environmentMenu);
    settingsMenu->addAction(chooseInterpreterAction);

    helpMenu = new QMenu(this);
    helpMenu->addAction(manualAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MenuBar::retranslateStrings()
{
    this->clear();
    this->addMenu(fileMenu);
    this->addMenu(editMenu);
    this->addMenu(compileMenu);
    this->addMenu(settingsMenu);
    this->addMenu(helpMenu);

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
    fullscrAction->setText(tr("Fullscreen mode"));
    chooseInterpreterAction->setText(tr("Choose interpreter"));
    rusAction->setText(tr("Russian"));
    engAction->setText(tr("English"));
    gerAction->setText(tr("German"));
    commentAction->setText(tr("Comment a line"));
    aboutAction->setText(tr("About"));
    aboutQtAction->setText(tr("About Qt"));
    manualAction->setText(tr("Manual"));

    fileMenu->setTitle(tr("File"));
    editMenu->setTitle(tr("Edit"));
    compileMenu->setTitle(tr("Interpretation"));
    settingsMenu->setTitle(tr("Settings"));
    environmentMenu->setTitle(tr("Environment"));
    langMenu->setTitle(tr("Language"));
    helpMenu->setTitle(tr("Help"));
}

void MenuBar::setUndoEnabled(bool state)
{
    undoAction->setEnabled(state);
}

void MenuBar::setRedoEnabled(bool state)
{
    redoAction->setEnabled(state);
}

void MenuBar::setCopyEnabled(bool state)
{
    copyAction->setEnabled(state);
}

void MenuBar::setCutEnabled(bool state)
{
    cutAction->setEnabled(state);
}
