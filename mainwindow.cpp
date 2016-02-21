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

#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QStatusBar>
#include <QFileInfo>
#include <QTime>
#include <QDate>
#include <QApplication>
#include <QDirIterator>
#include <QDialog>
#include <QTabWidget>
#include <QTextCursor>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    tools = new SubTools;

    tools->clearSessionLog();
    tools->writeSessionLog("Session started");

    qtTranslator = new QTranslator;
    appTranslator = new QTranslator;

    qApp->installTranslator(qtTranslator);
    qApp->installTranslator(appTranslator);

    configsRead();
    setupLogo();
    createStartMenu();

    this->setWindowTitle("Turnip Editor 16.03 Preview");
    this->setWindowIcon(QIcon(":icons/logo.png"));
    this->setMinimumSize(480, 270);
    this->resize(800, 600);

    beginSessionTime = tools->currentTimeDate();
}

MainWindow::~MainWindow()
{
    tools->writeSessionLog("Session finished");
    tools->writeLog(beginSessionTime, tools->currentTimeDate());
}

void MainWindow::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::LanguageChange) {
        retranslateStrings();
    }
    else QWidget::changeEvent(event);
}

void MainWindow::createMenu()
{
    menuBar = new MenuBar(this);
    this->setMenuBar(menuBar);

    Widget* widget = menuBar;
    widget->setupScheme();

    connect(menuBar, SIGNAL(open()), SLOT(open()));
    connect(menuBar, SIGNAL(save()), SLOT(save()));
    connect(menuBar, SIGNAL(saveAs()), SLOT(saveAs()));
    connect(menuBar, SIGNAL(exit()), SLOT(exit()));
    connect(menuBar, SIGNAL(undo()), SLOT(undo()));
    connect(menuBar, SIGNAL(redo()), SLOT(redo()));
    connect(menuBar, SIGNAL(cut()), SLOT(cut()));
    connect(menuBar, SIGNAL(copy()), SLOT(copy()));
    connect(menuBar, SIGNAL(paste()), SLOT(paste()));
    connect(menuBar, SIGNAL(comment()), SLOT(comment()));
    connect(menuBar, SIGNAL(interpret()), SLOT(interpret()));
    connect(menuBar, SIGNAL(fullscr()), SLOT(fullscr()));
    connect(menuBar, SIGNAL(chooseInterpreter()), SLOT(chooseInterpreter()));
    connect(menuBar, SIGNAL(groupLanguage(QAction*)), SLOT(setLanguage(QAction*)));
    connect(menuBar, SIGNAL(about()), SLOT(about()));
    connect(menuBar, SIGNAL(aboutQt()), SLOT(aboutQt()));
    connect(menuBar, SIGNAL(manual()), SLOT(manual()));

    menuBar->setLanguage(currentLang);
}

void MainWindow::createStartMenu()
{
    startMenuBar = new StartMenuBar(this);
    this->setMenuBar(startMenuBar);

    connect(startMenuBar, SIGNAL(open()), SLOT(open()));
    connect(startMenuBar, SIGNAL(newFile()), SLOT(newFile()));
    connect(startMenuBar, SIGNAL(exit()), SLOT(exit()));
    connect(startMenuBar, SIGNAL(about()), SLOT(about()));
    connect(startMenuBar, SIGNAL(aboutQt()), SLOT(aboutQt()));
    connect(startMenuBar, SIGNAL(manual()), SLOT(manual()));
}

void MainWindow::createToolBar()
{
    toolBar = new ToolBar(this);
    this->addToolBar(toolBar);

    connect(toolBar, SIGNAL(open()), SLOT(open()));
    connect(toolBar, SIGNAL(save()), SLOT(save()));
    connect(toolBar, SIGNAL(saveAs()), SLOT(saveAs()));
    connect(toolBar, SIGNAL(exit()), SLOT(exit()));
    connect(toolBar, SIGNAL(undo()), SLOT(undo()));
    connect(toolBar, SIGNAL(redo()), SLOT(redo()));
    connect(toolBar, SIGNAL(cut()), SLOT(cut()));
    connect(toolBar, SIGNAL(copy()), SLOT(copy()));
    connect(toolBar, SIGNAL(paste()), SLOT(paste()));
    connect(toolBar, SIGNAL(interpret()), SLOT(interpret()));
}

void MainWindow::retranslateStrings()
{
    if(editorSetuped)
    {
        Widget* widget;

        widget = menuBar;
        menuBar->clear();
        this->createMenu();
        widget->retranslateStrings();

        widget = editor;
        widget->retranslateStrings();

        widget = toolBar;
        widget->retranslateStrings();
    }
}

void MainWindow::setupLogo() //setting up logo
{
    logoLabel = new QLabel(this);
    logoLabel->setPixmap(QPixmap(":icons/background.png"));

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

    backgroundBox->setStyleSheet("background: rgb(245, 246, 248)");

    setCentralWidget(backgroundBox);

    tools->writeSessionLog("Logo was successfully setuped");
}

void MainWindow::setupEditor() //setting up editor
{
    createMenu();
    createToolBar();

    editor = new CodeEditor(this);
    editorSetuped = true;
    highlighter = new Highlighter(editor->document());

    this->setCentralWidget(editor);

    delete logoLabel; //clearing RAM after logo
    delete logoLayout;
    delete logoLayout1;
    delete backgroundBox;

    statBar = new QStatusBar(this);
    columnLine = new QLabel(this);
    statBar->addWidget(columnLine);
    this->setStatusBar(statBar);

    setupTheme();

    connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(textChanged()));

    tools->writeSessionLog("Editor was successfully setuped");

}

void MainWindow::setupTheme() //theme
{
    Widget* widget;

    widget = menuBar;
    widget->setupScheme();

    widget = editor;
    widget->setupScheme();

    widget = toolBar;
    widget->setupScheme();

    statBar->setStyleSheet("QStatusBar { background: rgb(43, 48, 59); }"
                           "QStatusBar::item { border: none; padding: 3px 3px 3px 3px; }"
                           "QLabel { font: 12pt; font-family: Consolas; color: rgb(84, 96, 107); }");

    tools->writeSessionLog("Fitted theme");
}

void MainWindow::configsRead() //configs reading
{
    setLanguage(tools->getLanguage());
    //tools->retranslateStrings();

    if(!tools->configsExists())
    {
        QMessageBox::warning(this, "Turnip Editor", tr("Error of reding configuration file, program may be crashed!"));
        tools->writeSessionLog("Error of reading configs!");
    }
    else
        tools->writeSessionLog("Configuration files were successfully readed");

    interpreterPath = tools->getInterpreterPath();
}

void MainWindow::setLanguage(QString lang)
{
    qtTranslator->load("qt_" + QLocale::system().name(),
                       QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(qtTranslator);

    if(lang == "Russian" || lang == "Rus")
    {
        appTranslator->load(":translations/russian.qm");
        qApp->installTranslator(appTranslator);
        currentLang = "Russian";
    }
    else if(lang == "English" || lang == "Eng")
    {
        appTranslator->load(":translations/english.qm");
        qApp->installTranslator(appTranslator);
        currentLang = "English";
    }
    else if((lang == "German" || lang == "Ger")  || (lang == "Deutsch"  || lang == "Deu"))
    {
        appTranslator->load(":translations/german.qm");
        qApp->installTranslator(appTranslator);
        currentLang = "German";
    }
}

//slots

void MainWindow::open()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Open"), "", tr("Turnip source code (*.txt *.trnp *.ext)"));

    if(fileName != "")
    {
        if(!editorSetuped)
            setupEditor();

        editor->setPlainText(tools->readFromFile(fileName));
        fileIsOpen = true;

        this->setWindowTitle(fileName + " - Turnip Editor 16.03 Preview");

        tools->writeSessionLog(fileName + " was successfully opened");
    }
}

void MainWindow::newFile()
{
    fileName = QFileDialog::getSaveFileName(this, tr("New File..."), "", tr("Turnip source code (*.txt);;Turnip source code (*.trnp);;Turnip source code (*.ext)"));

    if(fileName != "")
    {
        tools->writeToFile(fileName, tools->readFromFile("configs/newFileDefault.config"));

        if(tools->fileExits(fileName))
        {
            tools->writeSessionLog(fileName + " was successfully created");

            if(!editorSetuped)
                setupEditor();

            editor->setPlainText(tools->readFromFile(fileName));
            fileIsOpen = true;

            this->setWindowTitle(fileName + " - Turnip Editor 16.03 Preview");

            tools->writeSessionLog(fileName + " was successfully opened");
        }
        else
        {
            QMessageBox::critical(this, "Turnip Editor", tr("File cannot be saved!"));
            tools->writeSessionLog("Error of creating " + fileName);
        }
    }
}

void MainWindow::save()
{
    if(fileName != "" && fileIsOpen == true)
    {
        tools->writeToFile(fileName, editor->toPlainText());

        if(tools->fileSaved(fileName, editor->toPlainText()))
            tools->writeSessionLog(fileName + " was successfully saved");
        else
        {
            QMessageBox::critical(this, "Turnip Editor", tr("File cannot be saved!"));
            tools->writeSessionLog("Error of saving " + fileName);
        }
    }
}

void MainWindow::saveAs()
{
    fileName = QFileDialog::getSaveFileName(this, tr("Save As..."), "", tr("Turnip source code (*.txt);;Turnip source code (*.trnp);;Turnip source code (*.ext)"));

    if(fileName != "" && fileIsOpen == true)
    {
        tools->writeToFile(fileName, editor->toPlainText());

        if(tools->fileSaved(fileName, editor->toPlainText()))
            tools->writeSessionLog(fileName + " was successfully saved");
        else
        {
            QMessageBox::critical(this, "Turnip Editor", tr("File cannot be saved!"));
            tools->writeSessionLog("Error of saving " + fileName);
        }
    }
}

void MainWindow::exit()
{
    QMessageBox* quit = new QMessageBox(this);
    //quit->setIcon();
    quit->setText(tr("Do you want to exit?"));
    quit->setInformativeText(tr("Are you sure?"));
    quit->setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    quit->setDefaultButton(QMessageBox::Yes);
    int *q = new int(quit->exec());
    delete quit;
    switch(*q)
    {
    case QMessageBox::Yes:
        if(fileIsOpen && !tools->fileSaved(fileName, editor->toPlainText()))
        {
            QMessageBox* save = new QMessageBox(this);
            save->setText(tr("File ") + fileName + tr(" has been changed"));
            save->setInformativeText(tr("Do you want to save it?"));
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

void MainWindow::cut()
{
    editor->cut();
}

void MainWindow::copy()
{
    editor->copy();
}

void MainWindow::paste()
{
    editor->paste();
}

void MainWindow::undo()
{
    editor->undo();
}

void MainWindow::redo()
{
    editor->redo();
}

void MainWindow::comment()
{
    editor->comment();
}

void MainWindow::interpret()
{
    save();

    if(tools->fileSaved(fileName, editor->toPlainText()))
    {
        if(!interpreterPath.isEmpty())
        {
            QString* bufPath = new QString(interpreterPath);
            QString* arg = new QString(bufPath->append(" " + fileName));
            delete bufPath;
            system(arg->toStdString().c_str());
            delete arg;
            tools->writeSessionLog(fileName + " was successfully interpreted");
        }
        else
        {
            QMessageBox::critical(this, "Turnip Editor", tr("Turnip Runner not found or is invalid. Please, choose the correct interpreter."));
            tools->writeSessionLog("Error of interpreting " + fileName);
            chooseInterpreter();
            interpret();
        }
    }
    else QMessageBox::critical(this, "Turnip Editor", tr("Error of saving file!"));

}

void MainWindow::fullscr()
{
    if(menuBar->getFullscrState())
    {
        menuBar->setFullscrState(true);
        this->showFullScreen();
        tools->writeSessionLog("Fullscreen mode switched on");
    }
    else if(!menuBar->getFullscrState())
    {
        menuBar->setFullscrState(false);
        this->showNormal();
        tools->writeSessionLog("Fullscreen mode switched off");
    }
}

void MainWindow::chooseInterpreter()
{
    interpreterPath = tools->chooseInterpreter(editorSetuped);
}

void MainWindow::setLanguage(QAction *language)
{
    qtTranslator->load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(qtTranslator);

    if(language->text() == tr("Russian"))
    {
        appTranslator->load(":translations/russian.qm");
        qApp->installTranslator(appTranslator);
        retranslateStrings();
        tools->writeToFile("configs/languageConfig.config", "Russian");
    }
    if(language->text() == tr("English"))
    {
        appTranslator->load(":translations/english.qm");
        qApp->installTranslator(appTranslator);
        retranslateStrings();
        tools->writeToFile("configs/languageConfig.config", "English");
    }
    if(language->text() == tr("German"))
    {
        appTranslator->load(":translations/german.qm");
        qApp->installTranslator(appTranslator);
        retranslateStrings();
        tools->writeToFile("configs/languageConfig.config", "German");
    }
}

void MainWindow::about()
{
    QString aboutText;
    aboutText.insert(0,  "<h1>Turnip Editor 16.03 Preview</h1>"
                         "<h4>" + tr("Next generation of Turnip Editor.") + "</h4>" +
                     "<h3>" + tr("Simple IDE for simple programming language.") + "</h3>"
                                                                                  "<p>" + tr("Welcome to the Turnip Editor! Work with Turnip requiredn't some "
                                                                                             "experience in programming, Turnip Editor is perfect for beginners. More "
                                                                                             "information about Turnip and features of Turnip Editor you can look in "
                                                                                             "Manual (F1).") + "</p>" +
                     "<p>" + tr("Turnip Editor is free software: you can redistribute it and/or modify it "
                                "under the terms of the GNU General Public License as published by the "
                                "Free Software Foundation, either version 3 of the License, or (at your "
                                "option) any later version.") + "</p>" +
                     "<p>" + tr("Turnip Editor is distributed in the hope that it will be useful, but WITHOUT "
                                "ANY WARRANTY; without even the implied warranty of "
                                "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the "
                                "GNU General Public License for more details.") + "</p>" +
                     "<p>" + tr("You should have received a copy of the GNU General Public License along "
                                "with Turnip Editor. If not, see <a href = 'http://www.gnu.org/licenses/'>http://www.gnu.org/licenses/</a>.") +

                     "<p>" + tr("More information at ") + "<a href = 'https://github.com/NEzyaka/Turnip-Editor'>https://github.com/NEzyaka/Turnip-Editor</a></p>"

                                                          "<p><b>" + tr("Found a bug or just have a question? Write ") + "<a href='mailto:nekit2002mir@yandex.ru?subject=Message to the Turnip Editor developer' title='Message to the Turnip Editor developer'>" + tr("here") + "</a>.</b></p>");

    "<p>Copyright (С) 2015-2016 " + tr("Nikita Mironov") + ".</p>";


    QMessageBox::about(this, tr("About"), aboutText);
}

void MainWindow::aboutQt()
{
    qApp->aboutQt();
}

void MainWindow::manual()
{
    ManualViewer* viewer = new ManualViewer;
    viewer->show();

    if(viewer->exec() == 0)
        delete viewer;
}

void MainWindow::textChanged()
{
    QTextCursor* lineAndColumn = new QTextCursor(editor->textCursor());
    columnLine->setText(QString::number(lineAndColumn->blockNumber()+1) + ":" + QString::number(lineAndColumn->columnNumber()+1));
    delete lineAndColumn;
    tools->fileSaved(fileName, editor->toPlainText()) ? this->setWindowTitle(fileName + "* - Turnip Editor 16.03 Preview") : this->setWindowTitle(fileName + " - Turnip Editor 16.03 Preview");
}

