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

#include "mainwindow.h"

//including libraries
#include <QMessageBox>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QTextStream>
#include <QToolBar>
#include <cstdlib>
#include <QLabel>
#include <QStatusBar>
#include <QFileInfo>
#include <QTime>
#include <QDate>
#include <QApplication>
#include <QDirIterator>
#include <QDialog>
#include <QTabWidget>
#include <QTextCursor>
#include <QTextBrowser>

MainWindow::MainWindow() //constructor
{
    clearSessionLog();
    writeSessionLog("Session started");

    configsRead();
    setupLogo();
    setupStartMenu();

    this->setWindowTitle("Turnip Editor 16.01");
    this->setWindowIcon(QPixmap("logo.png"));
    this->setMinimumSize(480, 270);
    this->resize(800, 600);

    startFileMenu->setStyleSheet("QMenu { background-color: rgb(245, 246, 248); border: rgb(245, 246, 248); margin: 3px; }"
                                 "QMenu::item { background-color: transparent; padding: 3px 5px 5px 25px; border-radius: 2px; }"
                                 "QMenu::item:selected { background-color: rgb(225, 226, 228); color: rgb(39, 50, 56); }"
                                 "QMenu::separator { background: rgb(133, 143, 153); height: 1 px; }");

    helpMenu->setStyleSheet(startFileMenu->styleSheet());

    this->menuBar()->setStyleSheet("QMenuBar { background-color: rgb(245, 246, 248); color: rgb(39, 50, 56); }"
                                   "QMenuBar::item { spacing: 5px; padding: 3px 5px; background: transparent; border-radius: 2px; }"
                                   "QMenuBar::item:selected { background: rgb(225, 226, 228); }"
                                   "QMenuBar::item:pressed { background: rgb(225, 226, 228); }");

    backgroundBox->setStyleSheet("background: rgb(245, 246, 248)");
}

MainWindow::~MainWindow() //destructor
{
    writeSessionLog("Session finished");
    writeLog();
}


//setup-functions

void MainWindow::setupLogo() //setting up logo
{
    logoLabel = new QLabel(this);
    logoLabel->setPixmap(QPixmap("icons/background.png"));

    logoLayout = new QVBoxLayout();
    logoLayout->addStretch();
    logoLayout->addWidget(logoLabel);
    logoLayout->addStretch();

    logoLayout1 = new QHBoxLayout();
    logoLayout1->addStretch();
    logoLayout1->addLayout(logoLayout);
    logoLayout1->addStretch();

    backgroundBox = new QWidget();
    backgroundBox->setLayout(logoLayout1);

    setCentralWidget(backgroundBox);

    writeSessionLog("Logo was successfully setuped");
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event) //setting up context menu
{
    QMenu* contextMenu = new QMenu(this);
    setupActions();

    contextMenu->setStyleSheet("QMenu { background-color: rgb(245, 246, 248); border: rgb(245, 246, 248); margin: 3px; }"
                               "QMenu::item { background-color: transparent; padding: 3px 5px 5px 25px; border-radius: 2px; }"
                               "QMenu::item:selected { background-color: rgb(225, 226, 228); color: rgb(39, 50, 56); }"
                               "QMenu::separator { background: rgb(133, 143, 153); height: 1 px; }");

    contextMenu->addAction(openAction);
    contextMenu->addSeparator();
    contextMenu->addAction(exitAction);

    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    contextMenu->exec(event->globalPos());

    delete contextMenu;

    writeSessionLog("Context menu shown");
}

void MainWindow::setupToolbar() //setting up toolbars
{
    this->setupFileBar();
    this->setupEditBar();
    this->setupInterprBar();

    writeSessionLog("Toolbars was successfully setuped");
}

void MainWindow::setupFileBar()
{
    fileBar = new QToolBar(this);

    fileBar->addAction(openAction);
    fileBar->addAction(saveAction);
    fileBar->addAction(saveAsAction);
    fileBar->addAction(exitAction);

    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    this->addToolBar(Qt::TopToolBarArea, fileBar);
    fileBar->setMovable(false);
}

void MainWindow::setupEditBar()
{
    editBar = new QToolBar(this);

    editBar->addAction(cutAction);
    editBar->addAction(copyAction);
    editBar->addAction(pasteAction);
    editBar->addAction(undoAction);
    editBar->addAction(redoAction);
    editBar->addAction(commentAction);

    connect(cutAction, SIGNAL(triggered()), editor, SLOT(cut()));
    connect(copyAction, SIGNAL(triggered()), editor, SLOT(copy()));
    connect(pasteAction, SIGNAL(triggered()), editor, SLOT(paste()));
    connect(undoAction, SIGNAL(triggered()), editor, SLOT(undo()));
    connect(redoAction, SIGNAL(triggered()), editor, SLOT(redo()));
    connect(commentAction, SIGNAL(triggered()), editor, SLOT(comment()));

    this->addToolBar(Qt::TopToolBarArea, editBar);
    editBar->setMovable(false);
}

void MainWindow::setupInterprBar()
{
    interprBar = new QToolBar(this);

    interprBar->addAction(compileAction);

    connect(compileAction, SIGNAL(triggered()), this, SLOT(compile()));

    this->addToolBar(Qt::TopToolBarArea, interprBar);
    interprBar->setMovable(false);
}

void MainWindow::setupStartMenu() //setting up start menubar
{
    setupActions();

    setupStartFileMenu();
    setupHelpMenu();

    writeSessionLog("Start-menubar was successfully setuped");
}

void MainWindow::setupFullMenu() //setting up full menubar
{
    menuBar()->clear();

    setupActions();

    delete startFileMenu;
    delete helpMenu;

    setupFileMenu();
    setupEditMenu();
    setupCompileMenu();
    setupSettingsMenu();
    setupHelpMenu();

    writeSessionLog("Menubar was successfully setuped");
}

void MainWindow::setupActions() //setting up actions
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
    compileAction = new QAction(this);
    aboutAction = new QAction(this);
    aboutQtAction = new QAction(this);
    fullscrAction = new QAction(this);
    moveToolbarsAction = new QAction(this);
    chooseCompilerPathAction = new QAction(this);
    commentAction = new QAction(this);
    helpAction = new QAction(this);

    openAction->setText("Открыть"); //setting up text
    saveAction->setText("Сохранить");
    saveAsAction->setText("Сохранить как...");
    exitAction->setText("Выйти");
    undoAction->setText("Отменить");
    redoAction->setText("Повторить");
    cutAction->setText("Вырезать");
    copyAction->setText("Копировать");
    pasteAction->setText("Вставить");
    compileAction->setText("Интерпретировать");
    aboutAction->setText("О программе");
    aboutQtAction->setText("О библиотеке Qt");
    fullscrAction->setText("Полноэкранный режим");
    moveToolbarsAction->setText("Двигать панели инструментов");
    chooseCompilerPathAction->setText("Выбрать интерпретатор");
    commentAction->setText("Закомментирвоать строку");
    helpAction->setText("Справка");

    openAction->setIcon(QPixmap("icons/open.png")); //setting up icons
    saveAction->setIcon(QPixmap("icons/save.png"));
    saveAsAction->setIcon(QPixmap("icons/saveAs.png"));
    exitAction->setIcon(QPixmap("icons/exit.png"));
    undoAction->setIcon(QPixmap("icons/undo.png"));
    redoAction->setIcon(QPixmap("icons/redo.png"));
    cutAction->setIcon(QPixmap("icons/cut.png"));
    copyAction->setIcon(QPixmap("icons/copy.png"));
    pasteAction->setIcon(QPixmap("icons/paste.png"));
    compileAction->setIcon(QPixmap("icons/compile.png"));
    aboutAction->setIcon(QPixmap("icons/about.png"));
    aboutQtAction->setIcon(QPixmap("icons/qt.png"));
    fullscrAction->setIcon(QPixmap("icons/fullscreen.png"));
    moveToolbarsAction->setIcon(QPixmap("icons/move.png"));
    chooseCompilerPathAction->setIcon(QPixmap("icons/compiler.png"));
    commentAction->setIcon(QPixmap("icons/comment.png"));
    helpAction->setIcon(QPixmap("icons/help.png"));

    fullscrAction->setCheckable(true); //setting up checkability
    moveToolbarsAction->setCheckable(true);

    writeSessionLog("Actions were successfully setuped");
}

void MainWindow::setupEditor() //setting up editor
{
    delete logoLabel; //clearing RAM after logo
    delete logoLayout;
    delete logoLayout1;
    delete backgroundBox;

    editor = new CodeEditor;
    editorSetuped = true;

    editor->setLocale(QLocale::Russian);

    //settings
    QFont* font = new QFont;

    font->setFamily("Consolas");
    font->setFixedPitch(true);
    font->setPointSize(11);
    editor->setFont(*font);

    highlighter = new Highlighter(editor->document());

    setupToolbar();
    setupFullMenu();

    openAction->setIcon(QPixmap("icons/open.png")); //setting up icons
    saveAction->setIcon(QPixmap("icons/save.png"));
    saveAsAction->setIcon(QPixmap("icons/saveAs.png"));

    this->setCentralWidget(editor);

    setupTheme();

    statBar = new QStatusBar(this);
    columnLine = new QLabel(this);
    statBar->addWidget(columnLine);
    this->setStatusBar(statBar);

    connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(textChanged())); //work with code


    writeSessionLog("Editor was successfully setuped");

}

void MainWindow::setupStartFileMenu() //setting up start file-menu
{
    startFileMenu = new QMenu(this);

    startFileMenu->setTitle("&Файл");

    startFileMenu->addAction(openAction);
    startFileMenu->addSeparator();
    startFileMenu->addAction(exitAction);

    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    openAction->setShortcut(Qt::CTRL + Qt::Key_O);
    exitAction->setShortcut(Qt::CTRL + Qt::Key_Q);

    menuBar()->addMenu(startFileMenu);

    writeSessionLog("Start file-menu was successfully setuped");
}

void MainWindow::setupFileMenu() //setting up file-menu
{
    fileMenu = new QMenu(this);

    fileMenu->setTitle("&Файл");

    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exit()));

    openAction->setShortcut(Qt::CTRL + Qt::Key_O);
    saveAction->setShortcut(Qt::CTRL + Qt::Key_S);
    exitAction->setShortcut(Qt::CTRL + Qt::Key_Q);

    menuBar()->addMenu(fileMenu);

    writeSessionLog("File-menu was successfully setuped");
}

void MainWindow::setupEditMenu() //setting up edit-menu
{
    editMenu = new QMenu(this);

    editMenu->setTitle("&Правка");

    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(commentAction);

    connect(cutAction, SIGNAL(triggered()), editor, SLOT(cut()));
    connect(copyAction, SIGNAL(triggered()), editor, SLOT(copy()));
    connect(pasteAction, SIGNAL(triggered()), editor, SLOT(paste()));
    connect(undoAction, SIGNAL(triggered()), editor, SLOT(undo()));
    connect(redoAction, SIGNAL(triggered()), editor, SLOT(redo()));
    connect(commentAction, SIGNAL(triggered()), this, SLOT(comment()));

    undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
    redoAction->setShortcut(Qt::CTRL + Qt::Key_Y);
    cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
    copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);

    menuBar()->addMenu(editMenu);

    writeSessionLog("Edit-menu was successfully setuped");
}

void MainWindow::setupCompileMenu() //setting up interpreting-menu
{
    compileMenu = new QMenu(this);

    compileMenu->setTitle("&Сборка");

    compileMenu->addAction(compileAction);

    connect(compileAction, SIGNAL(triggered()), this, SLOT(compile()));

    compileAction->setShortcut(Qt::CTRL + Qt::Key_R);

    menuBar()->addMenu(compileMenu);

    writeSessionLog("Compile-menu was successfully setuped");
}

void MainWindow::setupSettingsMenu() //setting up start settings-menu
{
    settingsMenu = new QMenu(this);
    environmentMenu = new QMenu(this);

    settingsMenu->setTitle("&Настройки");
    environmentMenu->setTitle("Среда");

    environmentMenu->setIcon(QPixmap("icons/view.png"));
    environmentMenu->addAction(fullscrAction);
    environmentMenu->addAction(moveToolbarsAction);

    fullscrAction->setChecked(false);
    fullscrAction->setShortcut(Qt::Key_F11);

    moveToolbarsAction->setChecked(false);

    connect(fullscrAction, SIGNAL(triggered()), this, SLOT(fullscr()));
    connect(moveToolbarsAction, SIGNAL(triggered()), this, SLOT(moveToolbars()));
    connect(chooseCompilerPathAction, SIGNAL(triggered()), this, SLOT(chooseCompilerPath()));

    settingsMenu->addMenu(environmentMenu);
    settingsMenu->addAction(chooseCompilerPathAction);
    menuBar()->addMenu(settingsMenu);

    writeSessionLog("Settings-menu was successfully setuped");
}

void MainWindow::setupHelpMenu() //setting up help-menu
{
    helpMenu = new QMenu(this);

    helpMenu->setTitle("&Справка");

    helpAction->setShortcut(Qt::Key_F1);

    helpMenu->addAction(helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);

    connect(helpAction, SIGNAL(triggered()), this, SLOT(help()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    menuBar()->addMenu(helpMenu);

    writeSessionLog("Help-menu was successfully setuped");
}


//methods

void MainWindow::setupTheme() //theme
{
    editor->backColor = QColor(43, 48, 59);
    editor->fontColor = QColor(84, 96, 107);

    editor->setStyleSheet("CodeEditor { background: rgb(43, 48, 59); color: rgb(192, 197, 206); border: none; selection-background-color: rgb(245, 246, 248); }"
                          "QScrollBar:vertical { background: transparent; border-top-right-radius: 4px; border-bottom-right-radius: 4px; width: 12px; margin: 0px; }"
                          "QScrollBar::handle:vertical { background-color: rgb(61, 70, 79); border-radius: 3px; min-height: 20px; margin: 0px 2px 0px 2px; }"
                          "QScrollBar::add-line:vertical { background: none; height: 0px; subcontrol-position: right; subcontrol-origin: margin; }"
                          "QScrollBar::sub-line:vertical { background: none; height: 0px; subcontrol-position: left; subcontrol-origin: margin; }"
                          "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");

    qApp->setStyleSheet("QStatusBar { background: rgb(43, 48, 59); }"
                        "QStatusBar::item { border: none; padding: 3px 3px 3px 3px; }"

                        "QToolBar { background: rgb(245, 246, 248); border: none; }"
                        "QToolBar::item { border: none; }"

                        "QMenu { background-color: rgb(245, 246, 248); border: rgb(245, 246, 248); margin: 3px; }"
                        "QMenu::item { background-color: transparent; padding: 3px 5px 5px 25px; border-radius: 2px; }"
                        "QMenu::item:selected { background-color: rgb(225, 226, 228); color: rgb(39, 50, 56); }"
                        "QMenu::separator { background: rgb(133, 143, 153); height: 1 px; }"
                        "QMenu::icon:checked { background: rgb(200, 230, 201); }"

                        "QMenuBar { background-color: rgb(245, 246, 248); color: rgb(39, 50, 56); }"
                        "QMenuBar::item { spacing: 5px; padding: 3px 5px; background: transparent; border-radius: 2px; }"
                        "QMenuBar::item:selected { background: rgb(225, 226, 228); }"
                        "QMenuBar::item:pressed { background: rgb(225, 226, 228); }");

    writeSessionLog("Fitted theme");
}

bool MainWindow::fileSaved() //checking saving of file
{
    bool* isSaved = new bool(false);

    if(readFromFile(fileName) == editor->toPlainText())
    {
        *isSaved = true;
        writeSessionLog("Successful checking of saving " + fileName);
    }

    return isSaved;
    delete isSaved;
}

QString MainWindow::currentTimeDate() //getting current date and time for writing logs
{
    return QDate::currentDate().toString("dd.MM.yyyy") + " " + QTime::currentTime().toString("hh:mm:ss");
}

void MainWindow::writeLog() //appending global log
{
    QFile* file = new QFile("log.log");

    if (!file->open(QIODevice::Append))
    {
        QMessageBox::critical(this, "Turnip Editor", "Ошибка записи в лог-файл!");
        return;
    }
    else
    {
        QTextStream* stream = new QTextStream(&*file);
        *stream << "\n" << "Session witch ended at " << currentTimeDate() << "\n" << readFromFile("sessionLog.log");
        stream->flush();
        file->close();
        delete file;
        delete stream;
    }
}

void MainWindow::writeSessionLog(QString message) //writing session-log
{
    QFile* file = new QFile("sessionLog.log");
    if (!file->open(QIODevice::Append))
    {
        QMessageBox::critical(this, "Turnip Editor", "Ошибка записи в лог-файл сессии!");
        return;
    }
    else
    {
        QTextStream* stream = new QTextStream(&*file);
        *stream << currentTimeDate() << " - " << message << "\n";
        stream->flush();
        file->close();
        delete file;
        delete stream;
    }
}

void MainWindow::clearSessionLog() //cleaning session-log
{
    QFile* file = new QFile("sessionLog.log");
    if (!file->open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "Turnip Editor", "Ошибка записи в лог-файл сессии!");
        return;
    }
    else
    {
        QTextStream* stream = new QTextStream(&*file);
        *stream << "";
        stream->flush();
        file->close();
        delete file;
        delete stream;
    }
}

void MainWindow::configsRead() //configs reading
{
    //reading interpreter path config
    QFile* interpreterPathFromFile = new QFile("configs/compilerConfig.config");
    if (!interpreterPathFromFile->open(QFile::ReadOnly | QFile::Text))
        writeToFile("configs/compilerConfig.config", "");
    else
    {
        QString* compilerpath = new QString (readFromFile("configs/compilerConfig.config"));
        QFileInfo* interpr = new QFileInfo(*compilerpath);

        if(*compilerpath == "" || !interpr->isExecutable())
        {
            delete interpr;
            QFileInfo* autoInterpreter = new QFileInfo("bpl_interpreter.exe");
            if(autoInterpreter->isExecutable())
            {
                QMessageBox* autoDetecox = new QMessageBox;
                autoDetecox->setText("<b>Turnip Editor автоматически обнаружил интерпретатор для Turnip<br>в " + QString(autoInterpreter->absolutePath()) + "</b>");
                autoDetecox->setInformativeText("Вы хотите использовать этот интерпретатор?");
                autoDetecox->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                autoDetecox->setDefaultButton(QMessageBox::Yes);
                int* ret = new int(autoDetecox->exec());
                delete autoDetecox;
                switch(*ret)
                {
                case QMessageBox::Yes:
                    compilerPath = autoInterpreter->absoluteFilePath();
                    break;
                case QMessageBox::Cancel:
                    chooseCompilerPath();
                    break;
                }
                delete ret;
            }
            else
            {
                QString* searchpath = new QString("С:\\");
                QDirIterator* iterator = new QDirIterator(*searchpath, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
                while(iterator->hasNext())
                {
                    iterator->next();
                    if(iterator->fileInfo().absoluteFilePath().contains("bpl_interpreter", Qt::CaseInsensitive))
                    {
                        QMessageBox* autoDetecox = new QMessageBox;
                        autoDetecox->setText("<b>Turnip Editor автоматически обнаружил интерпретатор для Turnip<br>в " + QString(iterator->fileInfo().absolutePath() + "</b>"));
                        autoDetecox->setInformativeText("Вы хотите использовать этот интерпретатор?");
                        autoDetecox->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                        autoDetecox->setDefaultButton(QMessageBox::Yes);
                        int* ret = new int(autoDetecox->exec());
                        delete autoDetecox;
                        switch(*ret)
                        {
                        case QMessageBox::Yes:
                            compilerPath = iterator->fileInfo().absoluteFilePath();
                            break;
                        case QMessageBox::Cancel:
                            chooseCompilerPath();
                            break;
                        }
                        delete ret;
                        break;
                    }
                }
                delete searchpath;
                delete iterator;
                delete autoInterpreter;
            }
        }
        else compilerPath = *compilerpath;
        delete compilerpath;

        writeToFile("configs/compilerConfig.config", compilerPath);
    }
    interpreterPathFromFile->close();
    delete interpreterPathFromFile;

    //reading completer model
    QFile* completerModelFromFile = new QFile("configs/completerModel.config");
    if (!completerModelFromFile->open(QFile::ReadOnly | QFile::Text))
        writeToFile("configs/completerModel.config", "");
    completerModelFromFile->close();
    delete completerModelFromFile;

    writeSessionLog("Configuration files were successfully readed");
}

QString MainWindow::readFromFile(QString filePath)
{
    QString data = "";

    QFile* file = new QFile(filePath);
    if(file->open(QFile::ReadOnly | QFile::Text))
        data = file->readAll();
    file->close();
    delete file;

    return data;
}

void MainWindow::writeToFile(QString filePath, QString data)
{
    QFile* file = new QFile(filePath);
    if(file->open(QIODevice::WriteOnly))
    {
        QTextStream* stream = new QTextStream(&*file);
        *stream << data;
        stream->flush();
        file->close();
        delete file;
        delete stream;
    }
}


//slots

void MainWindow::open() //open
{
    fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Исходный код Turnip (*.trnp *.txt *.ext)");

    if(fileName != "")
    {
        if(editorSetuped == false)
            setupEditor();

        editor->setPlainText(readFromFile(fileName));
        fileIsOpen = true;

        this->setWindowTitle(fileName + " - Turnip Editor 16.01");

        writeSessionLog(fileName + " was successfully opened");
    }
}

void MainWindow::save() //save
{
    if(fileName != "" && fileIsOpen == true)
    {
        writeToFile(fileName, editor->toPlainText());

        if(fileSaved())
            writeSessionLog(fileName + " was successfully saved");
        else
        {
            QMessageBox::critical(this, "Turnip Editor", "Файл не может быть сохранён!");
            writeSessionLog("Error of saving " + fileName);
        }
    }
}

void MainWindow::saveAs() //save as
{
    fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Исходный код Turnip (*.trnp *.txt *.ext");

    if(fileName != "" && fileIsOpen == true)
    {
        writeToFile(fileName, editor->toPlainText());

        if(fileSaved())
            writeSessionLog(fileName + " was successfully saved");
        else
        {
            QMessageBox::critical(this, "Turnip Editor", "Файл не может быть сохранён!");
            writeSessionLog("Error of saving " + fileName);
        }
    }
}

void MainWindow::compile() //interpreting
{
    save();

    if(fileSaved() == true)
    {
        if(!compilerPath.isEmpty())
        {
            QString* bufPath = new QString(compilerPath);
            QString* arg = new QString(bufPath->append(" " + fileName));
            delete bufPath;
            system(arg->toStdString().c_str());
            delete arg;
            writeSessionLog(fileName + " was successfully interpreted");
        }
        else
        {
            QMessageBox::critical(this, "Turnip Editor", "Интерпретатор не найден или повреждён. Пожалуйста, выберите интерпретатор.");
            writeSessionLog("Error of interpreting " + fileName);
            chooseCompilerPath();
            compile();
        }
    }
    else QMessageBox::critical(this, "Turnip Editor", "Ошибка сохранения файла!");
}

void MainWindow::about() //about
{
    QMessageBox::about(this, "О программе",
                       "<h1>Turnip Editor 16.01</h1>"
                       "<h3>Простая среда разработки для простого языка программирования</h3>"
                       "<p>Добро пожаловать в Turnip Editor! Для работы с Turnip не нужно никакого опыта в программировании, да-да, вы не ослышались, Turnip Editor прекрасно подходит для новичков. Подробнее о Turnip и функциях Turnip Editor смотрите в спарвке (F1).</p>"

                       "<p>Turnip Editor - свободная программа: вы можете перераспространять ее и/или "
                       "изменять ее на условиях Стандартной общественной лицензии GNU в том виде, "
                       "в каком она была опубликована Фондом свободного программного обеспечения; "
                       "либо версии 3 лицензии, либо (по вашему выбору) любой более поздней "
                       "версии. "
                       "Turnip Editor распространяется в надежде, что она будет полезной, "
                       "но БЕЗО ВСЯКИХ ГАРАНТИЙ; даже без неявной гарантии ТОВАРНОГО ВИДА "
                       "или ПРИГОДНОСТИ ДЛЯ ОПРЕДЕЛЕННЫХ ЦЕЛЕЙ. Подробнее см. в Стандартной "
                       "общественной лицензии GNU. "
                       "Вы должны были получить копию Стандартной общественной лицензии GNU "
                       "вместе с этой программой. Если это не так, см. <a href = 'http://www.gnu.org/licenses/'>http://www.gnu.org/licenses/</a>.</p>"
                       "<p>Больше информации на  <a href = 'https://github.com/NEzyaka/Turnip-Editor'>https://github.com/NEzyaka/Turnip-Editor</a></p>"
                       "<p><b>Нашли баг, или просто хотите задать вопрос? Пишите <a href='mailto:nekit2002mir@yandex.ru?subject=Message to the Turnip Editor developer' title='Message to the Turnip Editor developer'>сюда</a>.</b></p>"
                       "<p>Copyright (С) 2015-2016 Никита Миронов.</p>");

    writeSessionLog("About shown");
}

void MainWindow::exit() //quit
{
    QMessageBox* quit = new QMessageBox(this);
    quit->setText("<b>Вы хотите выйти?</b>");
    quit->setInformativeText("Вы уверены?");
    quit->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    quit->setDefaultButton(QMessageBox::Yes);
    int *q = new int(quit->exec());
    delete quit;
    switch(*q)
    {
    case QMessageBox::Yes:
        if(fileIsOpen == true && fileSaved() == false)
        {
            QMessageBox* save = new QMessageBox(this);
            save->setText("Файл " + fileName + " был изменён");
            save->setInformativeText("Вы хотите сохранить его?");
            save->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            save->setDefaultButton(QMessageBox::Yes);
            int *s = new int (save->exec());
            delete save;
            switch(*s)
            {
            case QMessageBox::Yes:
                saveAs();
                break;
            case QMessageBox::No:
                break;
            }
            delete s;
        }
        qApp->quit();
        break;
    case QMessageBox::No:
        return;
    }
    delete q;
}

void MainWindow::fullscr() //fullscreen mode
{
    if(fullscrAction->isChecked())
    {
        fullscrAction->setChecked(true);
        this->showFullScreen();
        writeSessionLog("Full-screen mode switched on");
    }
    else if(!fullscrAction->isChecked())
    {
        fullscrAction->setChecked(false);
        this->showNormal();
        writeSessionLog("Full-screen mode switched off");
    }
}

void MainWindow::moveToolbars() //toolbars moving
{
    if(moveToolbarsAction->isChecked())
    {
        moveToolbarsAction->setChecked(true);

        fileBar->setMovable(true);
        editBar->setMovable(true);
        interprBar->setMovable(true);

        writeSessionLog("Toolbars-move mode switched on");
    }
    else if(!moveToolbarsAction->isChecked())
    {
        moveToolbarsAction->setChecked(false);

        fileBar->setMovable(false);
        editBar->setMovable(false);
        interprBar->setMovable(false);

        writeSessionLog("Toolbars-move mode switched off");
    }
}

void MainWindow::chooseCompilerPath() //choose interpreter
{
    if(editorSetuped == false)
        while(compilerPath == "")
            compilerPath = QFileDialog::getOpenFileName(this, "Выбор интерпретатора", "", "Интерпретатор Turnip (*.exe)");
    else compilerPath = QFileDialog::getOpenFileName(this, "Выбор интерпретатора", "", "Интерпретатор Turnip (*.exe)");

    writeToFile("configs/compilerConfig.config", compilerPath);
    writeSessionLog("Choosed Turnip Runner in " + compilerPath);
}

void MainWindow::comment() //line commenting
{
    QTextCursor* tc = new QTextCursor(editor->textCursor());
    tc->select(QTextCursor::WordUnderCursor);
    tc->insertText("//" + tc->selectedText());
    delete tc;

    writeSessionLog("Commented 1 line");
}

void MainWindow::textChanged() //working with code
{
    this->save();

    //anti-cyrillic
    QTextCursor* tc = new QTextCursor(editor->textCursor());
    tc->select(QTextCursor::WordUnderCursor);

    if(tc->selectedText() == "ЗКШТЕ" || tc->selectedText() == "PRINЕ" || tc->selectedText() == "PRIТЕ" || tc->selectedText() == "PRШТЕ" || tc->selectedText() == "PКШТЕ")
        tc->insertText("PRINT");
    else if(tc->selectedText() == "ЗКШ" || tc->selectedText() == "PRШ" || tc->selectedText() == "PКШ")
        tc->insertText("PRI");
    else if(tc->selectedText() == "PRINTМФК" || tc->selectedText() == "PRINTVФК" || tc->selectedText() == "PRINTVAК")
        tc->insertText("PRINTVAR");
    else if(tc->selectedText() == "PRINTТГЬ" || tc->selectedText() == "PRINTNГЬ" || tc->selectedText() == "PRINTNUЬ")
        tc->insertText("PRINTNUM");
    else if(tc->selectedText() == "ВГЬЗ" || tc->selectedText() == "DUMЗ" || tc->selectedText() == "DUЬЗ" || tc->selectedText() == "DГЬЗ")
        tc->insertText("DUMP");
    else if(tc->selectedText() == "ВГЬ" || tc->selectedText() == "DГЬ" || tc->selectedText() == "DUЬ")
        tc->insertText("DUM");
    else if(tc->selectedText() == "DUMPМФК" || tc->selectedText() == "DUMPVФК" || tc->selectedText() == "DUMPVAК")
        tc->insertText("DUMPVAR");
    else if(tc->selectedText() == "ТУЧЕДШТУ" || tc->selectedText() == "NУЧЕДШТУ" || tc->selectedText() == "NEЧЕДШТУ" || tc->selectedText() == "NEXЕДШТУ" || tc->selectedText() == "NEXTДШТУ" || tc->selectedText() == "NEXTLШТУ" || tc->selectedText() == "NEXTLIТУ" || tc->selectedText() == "NEXTLINУ")
        tc->insertText("NEXTLINE");
    else if(tc->selectedText() == "ТУЧ" || tc->selectedText() == "NУЧ" || tc->selectedText() == "NEЧ")
        tc->insertText("NEX");
    else if(tc->selectedText() == "ФДУКЕ" || tc->selectedText() == "AДУКЕ" || tc->selectedText() == "ALУКЕ" || tc->selectedText() == "ALEКЕ" || tc->selectedText() == "ALERЕ")
        tc->insertText("ALERT");
    else if(tc->selectedText() == "ФДУ" || tc->selectedText() == "AДУ" || tc->selectedText() == "ALУ")
        tc->insertText("ALE");
    else if(tc->selectedText() == "ШТЕУПУК" || tc->selectedText() == "IТЕУПУК" || tc->selectedText() == "INЕУПУК" || tc->selectedText() == "INTУПУК" || tc->selectedText() == "INTEПУК" || tc->selectedText() == "INTEGУК" || tc->selectedText() == "INTEGEК")
        tc->insertText("INTEGER");
    else if(tc->selectedText() == "ШТЕ" || tc->selectedText() == "IТЕ" || tc->selectedText() == "INЕ")
        tc->insertText("INT");
    else if(tc->selectedText() == "ИЩЩД" || tc->selectedText() == "BЩЩД" || tc->selectedText() == "BOЩД" || tc->selectedText() == "BOOД")
        tc->insertText("BOOL");
    else if(tc->selectedText() == "ИЩЩ" || tc->selectedText() == "BЩЩ" || tc->selectedText() == "BOЩ")
        tc->insertText("BOO");
    else if(tc->selectedText() == "ДШТУ" || tc->selectedText() == "LШТУ" || tc->selectedText() == "LIТУ" || tc->selectedText() == "LINУ")
        tc->insertText("LINE");
    else if(tc->selectedText() == "ДШТ" || tc->selectedText() == "LШТ" || tc->selectedText() == "LIТ")
        tc->insertText("LIN");
    else if(tc->selectedText() == "СДУФКЫСКУУТ" || tc->selectedText() == "CДУФКЫСКУУТ" || tc->selectedText() == "CLУФКЫСКУУТ" || tc->selectedText() == "CLEУФКЫСКУУТ" || tc->selectedText() == "CLEAКЫСКУУТ" || tc->selectedText() == "CLEARЫСКУУТ" || tc->selectedText() == "CLEARSСКУУТ" || tc->selectedText() == "CLEARSСRУУТ" || tc->selectedText() == "CLEARSСКEУТ" || tc->selectedText() == "CLEARSСREEТ")
        tc->insertText("CLEARSCREEN");
    else if(tc->selectedText() == "СДУ" || tc->selectedText() == "CДУ" || tc->selectedText() == "CLУ")
        tc->insertText("CLE");
    else if(tc->selectedText() == "ЕКГУ" || tc->selectedText() == "TКГУ" || tc->selectedText() == "TRГУ" || tc->selectedText() == "TRUУ")
        tc->insertText("TRUE");
    else if(tc->selectedText() == "ЕКГ" || tc->selectedText() == "TКГ" || tc->selectedText() == "TRГ")
        tc->insertText("TRU");
    else if(tc->selectedText() == "АФДЫУ" || tc->selectedText() == "FФДЫУ" || tc->selectedText() == "FAДЫУ" || tc->selectedText() == "FALЫУ" || tc->selectedText() == "FALSУ")
        tc->insertText("FALSE");
    else if(tc->selectedText() == "АФД" || tc->selectedText() == "FФД" || tc->selectedText() == "FAД")
        tc->insertText("FAL");
    else if(tc->selectedText() == "СЩЬЬФТВ" || tc->selectedText() == "CЩЬЬФТВ" || tc->selectedText() == "COЬЬФТВ" || tc->selectedText() == "COMЬФТВ" || tc->selectedText() == "COMMФТВ" || tc->selectedText() == "COMMAТВ" || tc->selectedText() == "COMMANВ")
        tc->insertText("COMMAND");
    else if(tc->selectedText() == "СЩЬ" || tc->selectedText() == "CЩЬ" || tc->selectedText() == "COЬ")
        tc->insertText("COM");
    else if(tc->selectedText() == "АЩТЕСЩДЩК" || tc->selectedText() == "FЩТЕСЩДЩК" || tc->selectedText() == "FOТЕСЩДЩК" || tc->selectedText() == "FONЕСЩДЩК" || tc->selectedText() == "FONTСЩДЩК" || tc->selectedText() == "FONTCЩДЩК" || tc->selectedText() == "FONTCOДЩК"|| tc->selectedText() == "FONTCOLЩК" || tc->selectedText() == "FONTCOLOК")
        tc->insertText("FONTCOLOR");
    else if(tc->selectedText() == "АЩТ" || tc->selectedText() == "FЩТ" || tc->selectedText() == "FOТ")
        tc->insertText("FON");
    else if(tc->selectedText() == "ША" || tc->selectedText() == "IА")
        tc->insertText("IF");
    else if(tc->selectedText() == "УДЫУ" || tc->selectedText() == "EДЫУ" || tc->selectedText() == "ELЫУ" || tc->selectedText() == "ELSУ")
        tc->insertText("ELSE");
    else if(tc->selectedText() == "УТВША" || tc->selectedText() == "EТВША" || tc->selectedText() == "ENВША" || tc->selectedText() == "ENDША" || tc->selectedText() == "ENDIА")
        tc->insertText("ENDIF");

    delete tc;

    QTextCursor* lineAndColumn = new QTextCursor(editor->textCursor());
    columnLine->setText("<font color = #4f5b66 size = 4>Строка " + QString::number(lineAndColumn->blockNumber()+1) + ", Столбец " + QString::number(lineAndColumn->columnNumber()+1) + "</font>");
    delete lineAndColumn;
}

void MainWindow::help() //help
{
    QDialog* help = new QDialog(this);

    help->setWindowTitle("Помощь по Turnip Editor");

    QTabWidget* parts = new QTabWidget(help);

    QWidget* startWidget = new QWidget(help);
    QWidget* operatorsWidget = new QWidget(help);
    QWidget* varsWidget = new QWidget(help);
    QWidget* ifWidget = new QWidget(help);
    QWidget* examplesWidget = new QWidget(help);

    QVBoxLayout* helpLayout = new QVBoxLayout(help);
    QVBoxLayout* startLayout = new QVBoxLayout(startWidget);
    QVBoxLayout* operatorsLayout = new QVBoxLayout(operatorsWidget);
    QVBoxLayout* varsLayout = new QVBoxLayout(varsWidget);
    QVBoxLayout* ifLayout = new QVBoxLayout(ifWidget);
    QVBoxLayout* examplesLayout = new QVBoxLayout(examplesWidget);

    QTextBrowser* startLabel = new QTextBrowser(startWidget);
    QTextBrowser* operatorsLabel = new QTextBrowser(operatorsWidget);
    QTextBrowser* varsLabel = new QTextBrowser(varsWidget);
    QTextBrowser* ifLabel = new QTextBrowser(ifWidget);
    QTextBrowser* examplesLabel = new QTextBrowser(examplesWidget);

    startLayout->addWidget(startLabel);
    startLabel->setHtml(readFromFile("configs/help/start.htm"));
    startWidget->setLayout(startLayout);

    operatorsLayout->addWidget(operatorsLabel);
    operatorsLabel->setText(readFromFile("configs/help/operators.htm"));
    operatorsWidget->setLayout(operatorsLayout);

    varsLayout->addWidget(varsLabel);
    varsLabel->setText(readFromFile("configs/help/vars.htm"));
    varsWidget->setLayout(varsLayout);

    ifLayout->addWidget(ifLabel);
    ifLabel->setText(readFromFile("configs/help/if.htm"));
    ifWidget->setLayout(ifLayout);

    examplesLayout->addWidget(examplesLabel);
    examplesLabel->setText(readFromFile("configs/help/examples.htm"));
    examplesWidget->setLayout(examplesLayout);

    parts->addTab(startWidget, "&Начало");
    parts->addTab(operatorsWidget, "О&ператоры");
    parts->addTab(varsWidget, "Типы &данных");
    parts->addTab(ifWidget, "&Условия");
    parts->addTab(examplesWidget, "&Примеры");

    helpLayout->addWidget(parts);
    help->setLayout(helpLayout);

    help->setStyleSheet("QTabWidget::tab-bar { alignment: center; }"
                        "QTextBrowser { border: none; }"

                        "QScrollBar:vertical { background: transparent; border-top-right-radius: 4px; border-bottom-right-radius: 4px; width: 12px; margin: 0px; }"
                        "QScrollBar::handle:vertical { background-color: rgb(225, 226, 228); border-radius: 3px; min-height: 20px; margin: 0px 2px 0px 2px; }"
                        "QScrollBar::add-line:vertical { background: none; height: 0px; subcontrol-position: right; subcontrol-origin: margin; }"
                        "QScrollBar::sub-line:vertical { background: none; height: 0px; subcontrol-position: left; subcontrol-origin: margin; }"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");

    help->resize(750, 500);
    help->setMinimumSize(750, 500);
    help->show();

    if(help->exec() == 0)
    {
        delete startLabel;
        delete operatorsLabel;
        delete varsLabel;
        delete ifLabel;

        delete startLayout;
        delete operatorsLayout;
        delete varsLayout;
        delete ifLayout;

        delete startWidget;
        delete operatorsWidget;
        delete varsWidget;
        delete ifWidget;

        delete parts;
        delete helpLayout;
        delete help;
    }

    writeSessionLog("Shown help");
}
