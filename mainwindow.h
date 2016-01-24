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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QLabel>
#include <QVBoxLayout>

#include "codeeditor.h"
#include "highlighter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
private:
    QAction* openAction;
    QAction* saveAction;
    QAction* saveAsAction;
    QAction* exitAction;
    QAction* compileAction;
    QAction* aboutAction;
    QAction* aboutQtAction;
    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* fullscrAction;
    QAction* moveToolbarsAction;
    QAction* chooseCompilerPathAction;
    QAction* commentAction;
    QAction* helpAction;

    QAction* commentTBAction;
    QAction* compileTBAction;
    QAction* copyTBAction;
    QAction* cutTBAction;
    QAction* exitTBAction;
    QAction* openTBAction;
    QAction* pasteTBAction;
    QAction* redoTBAction;
    QAction* saveTBAction;
    QAction* saveAsTBAction;
    QAction* undoTBAction;

    void setupActions();
    void setupEditor();
    void setupFullMenu();
    void setupStartMenu();
    void setupStartFileMenu();
    void setupFileMenu();
    void setupEditMenu();
    void setupCompileMenu();
    void setupSettingsMenu();
    void setupHelpMenu();
    void setupToolbar();
    void configsRead();
    void setupLogo();
    bool fileSaved();
    QString currentTimeDate();
    void writeLog();
    void writeSessionLog(QString message);
    void clearSessionLog();
    void setupTheme();
    void setupFileBar();
    void setupEditBar();
    void setupInterprBar();
    QString readFromFile(QString filePath);
    void writeToFile(QString filePath, QString data);

    QToolBar* fileBar;
    QToolBar* editBar;
    QToolBar* interprBar;

    QMenu* startFileMenu;
    QMenu* fileMenu;
    QMenu* editMenu;
    QMenu* compileMenu;
    QMenu* settingsMenu;
    QMenu* helpMenu;
    QMenu* environmentMenu;
    QMenu* langMenu;

    CodeEditor* editor;
    Highlighter* highlighter;

    QString fileName;
    QString compilerPath;

    bool fileIsOpen;
    bool editorSetuped;

    QLabel* logoLabel;
    QVBoxLayout* logoLayout;
    QHBoxLayout* logoLayout1;

    void contextMenuEvent(QContextMenuEvent *event);

    QWidget *backgroundBox;

    QLabel* columnLine;
    QLabel* fileLabel;
    QStatusBar* statBar;
private slots:
    void about();
    void compile();
    void save();
    void saveAs();
    void open();
    void exit();
    void fullscr();
    void moveToolbars();
    void chooseCompilerPath();
    void comment();
    void textChanged();
    void help();
};

#endif // MAINWINDOW_H
