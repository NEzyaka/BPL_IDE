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
#include <QApplication>
#include <QDesktopWidget>
#include <QTranslator>
#include <QDir>
#include <QLibraryInfo>
#include <QDebug>
#include <QLabel>
#include <QBoxLayout>

#include "menubar.h"
#include "startmenubar.h"
#include "toolbar.h"
#include "manualviewer.h"
#include "codeeditor.h"
#include "highlighter.h"
#include "subtools.h"
#include "outputgetter.h"
#include "interpreter.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void changeEvent(QEvent *event);
private:
    QTranslator* qtTranslator;
    QTranslator* appTranslator;
    void setLanguage(QString language);
    QString currentLang;

    MenuBar* menuBar;
    StartMenuBar* startMenuBar;
    ToolBar* toolBar;
    QLabel* columnLine;
    QStatusBar* statBar;
    OutputGetter* getter;
    Interpreter* interpreter;

    void createMenu();
    void createStartMenu();
    void createToolBar();
    void retranslateStrings();

    void setupEditor();
    void configsRead();
    QString getInterpreterPath();
    void setupLogo();
    void setupTheme();

    CodeEditor* editor;
    Highlighter* highlighter;

    QString fileName;
    QString interpreterPath;
    bool fileIsOpen = false;
    bool editorSetuped = false;

    QLabel* logoLabel;
    QVBoxLayout* logoLayout;
    QHBoxLayout* logoLayout1;
    QWidget *backgroundBox;

    SubTools* tools;
    QString beginSessionTime;

private slots:
    void open();
    void newFile();
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
    void setLanguage(QAction* language);
    void about();
    void aboutQt();
    void manual();
    void textChanged();
};

#endif // MAINWINDOW_H
