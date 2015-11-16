/****************************************************************************
**
** Copyright (C) 2015 Nikita Mironov
** Contact: nekit2002mir@yandex.ru
**
** This file is part of BPL IDE source code.
**
** BPL IDE is open-source cross-platform IDE for BPL.
** This program is licensed under the terms of the GNU General Public License version 3.
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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
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

    QString fileName = "";
    QString compilerPath = "";

    bool fileIsOpen = false;
    bool editorSetuped = false;

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
