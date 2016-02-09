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
    clearSessionLog();
    writeSessionLog("Session started");

    qtTranslator = new QTranslator;
    appTranslator = new QTranslator;

    qApp->installTranslator(qtTranslator);
    qApp->installTranslator(appTranslator);

    configsRead();
    setupLogo();
    createStartMenu();

    this->setWindowTitle("Turnip Editor 16.02");
    this->setWindowIcon(QPixmap(":icons/logo.png"));
    this->setMinimumSize(480, 270);
    this->resize(800, 600);
}

MainWindow::~MainWindow()
{
    writeSessionLog("Session finished");
    writeLog();
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
        menuBar->clear();
        menuBar->retranslateStrings();
        editor->retranslateStrings();
        toolBar->retranslateStrings();
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

    writeSessionLog("Logo was successfully setuped");
}

void MainWindow::setupEditor() //setting up editor
{
    createMenu();
    createToolBar();

    editor = new CodeEditor;
    editorSetuped = true;
    highlighter = new Highlighter(editor->document());

    this->setCentralWidget(editor);
    setupTheme();

    delete logoLabel; //clearing RAM after logo
    delete logoLayout;
    delete logoLayout1;
    delete backgroundBox;

    statBar = new QStatusBar(this);
    columnLine = new QLabel(this);
    statBar->addWidget(columnLine);
    this->setStatusBar(statBar);

    connect(editor, SIGNAL(cursorPositionChanged()), this, SLOT(textChanged()));

    writeSessionLog("Editor was successfully setuped");

}

void MainWindow::setupTheme() //theme
{
    editor->setupScheme();

    qApp->setStyleSheet("QStatusBar { background: rgb(43, 48, 59); }"
                        "QStatusBar::item { border: none; padding: 3px 3px 3px 3px; }");

    writeSessionLog("Fitted theme");
}

bool MainWindow::fileSaved() //checking saving of file
{
    bool isSaved = false;

    if(readFromFile(fileName) == editor->toPlainText())
    {
        isSaved = true;
        writeSessionLog("Successful checking of saving " + fileName);
    }

    return isSaved;
}

QString MainWindow::currentTimeDate() //getting current date and time for writing logs
{
    return QDate::currentDate().toString("dd.MM.yyyy") + " " + QTime::currentTime().toString("hh:mm:ss");
}

void MainWindow::writeLog() //appending global log
{
    QFile* file = new QFile("log.log");

    if(!file->open(QIODevice::Append))
    {
        QMessageBox::critical(this, "Turnip Editor", tr("Error of writing to log!"));
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
    if(!file->open(QIODevice::Append))
    {
        QMessageBox::critical(this, "Turnip Editor", tr("Error of writing to log!"));
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
    if(!file->open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "Turnip Editor", tr("Error of writing to log!"));
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

    QString* compilerpath = new QString (readFromFile("configs/compilerConfig.config"));
    QFileInfo* interpr = new QFileInfo(*compilerpath);

    if(compilerpath->isEmpty() || !interpr->isExecutable())
    {
        delete interpr;
        QFileInfo* autoInterpreter = new QFileInfo("Turnip-Runner.exe");
        if(autoInterpreter->isExecutable())
        {
            QMessageBox* autoDetecox = new QMessageBox;
            autoDetecox->setText(tr("<b>Turnip Editor automatically found Turnip Runner<br> in ") + QString(autoInterpreter->absolutePath()) + "</b>");
            autoDetecox->setInformativeText(tr("Do you want to use this interpreter?"));
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
                chooseInterpreter();
                break;
            }
            delete ret;
        }
        else
        {
            QString* searchpath = new QString("C:\\");
            QDirIterator* iterator = new QDirIterator(*searchpath, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
            while(iterator->hasNext())
            {
                iterator->next();
                if(iterator->fileInfo().absoluteFilePath().contains("Turnip-Runner.exe", Qt::CaseInsensitive))
                {
                    QMessageBox* autoDetecox = new QMessageBox;
                    autoDetecox->setText(tr("<b>Turnip Editor automatically found Turnip Runner<br> in ") + QString(iterator->fileInfo().absolutePath() + "</b>"));
                    autoDetecox->setInformativeText(tr("Do you want to use this interpreter?"));
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
                        chooseInterpreter();
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

    interpreterPathFromFile->close();
    delete interpreterPathFromFile;

    try
    {
        //reading completer model
        QFile* completerModelFromFile = new QFile("configs/completerModel.config");
        if(!completerModelFromFile->open(QFile::ReadOnly | QFile::Text))
            throw 1;
        completerModelFromFile->close();
        delete completerModelFromFile;

        //reading blocks highlighting list
        QFile* blocksFromFile = new QFile("configs/highlight/blocks.config");
        if(!blocksFromFile->open(QFile::ReadOnly | QFile::Text))
            throw 1;
        blocksFromFile->close();
        delete blocksFromFile;

        //reading conditions highlighting list
        QFile* conditionsFromFile = new QFile("configs/highlight/conditions.config");
        if(!conditionsFromFile->open(QFile::ReadOnly | QFile::Text))
            throw 1;
        conditionsFromFile->close();
        delete conditionsFromFile;

        //reading dataTypes highlighting list
        QFile* dataTypesFromFile = new QFile("configs/highlight/dataTypes.config");
        if(!dataTypesFromFile->open(QFile::ReadOnly | QFile::Text))
            throw 1;
        dataTypesFromFile->close();
        delete dataTypesFromFile;

        //reading operators highlighting list
        QFile* operatorsFromFile = new QFile("configs/highlight/operators.config");
        if(!operatorsFromFile->open(QFile::ReadOnly | QFile::Text))
            throw 1;
        operatorsFromFile->close();
        delete operatorsFromFile;

        //reading values highlighting list
        QFile* valuesFromFile = new QFile("configs/highlight/values.config");
        if(!valuesFromFile->open(QFile::ReadOnly | QFile::Text))
            throw 1;
        valuesFromFile->close();
        delete valuesFromFile;

        QFile* languageFromFile = new QFile("configs/languageConfig.config");
        if(!languageFromFile->open(QFile::ReadOnly | QFile::Text))
            throw 1;
        else
            setLanguage(languageFromFile->readAll());
        languageFromFile->close();
        delete languageFromFile;

        writeSessionLog("Configuration files were successfully readed");
    }
    catch(...)
    {
        QMessageBox::warning(this, "Turnip Editor", tr("Error of reding configuration file, program may be crashed!"));
        writeSessionLog("Error of reading configs!");
    }
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
    fileName = QFileDialog::getOpenFileName(this, tr("Open"), "", tr("Turnip source code (*.trnp *.txt *.ext)"));

    if(fileName != "")
    {
        if(!editorSetuped)
            setupEditor();

        editor->setPlainText(readFromFile(fileName));
        fileIsOpen = true;

        this->setWindowTitle(fileName + " - Turnip Editor 16.02");

        writeSessionLog(fileName + " was successfully opened");
    }
}

void MainWindow::save()
{
    if(fileName != "" && fileIsOpen == true)
    {
        writeToFile(fileName, editor->toPlainText());

        if(fileSaved())
            writeSessionLog(fileName + " was successfully saved");
        else
        {
            QMessageBox::critical(this, "Turnip Editor", tr("File cannot be saved!"));
            writeSessionLog("Error of saving " + fileName);
        }
    }
}

void MainWindow::saveAs()
{
    fileName = QFileDialog::getSaveFileName(this, tr("Save As..."), "", tr("Turnip source code (*.trnp *.txt *.ext)"));

    if(fileName != "" && fileIsOpen == true)
    {
        writeToFile(fileName, editor->toPlainText());

        if(fileSaved())
            writeSessionLog(fileName + " was successfully saved");
        else
        {
            QMessageBox::critical(this, "Turnip Editor", tr("File cannot be saved!"));
            writeSessionLog("Error of saving " + fileName);
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
        if(fileIsOpen == true && fileSaved() == false)
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

    if(fileSaved())
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
            QMessageBox::critical(this, "Turnip Editor", tr("Turnip Runner not found or is invalid. Please, choose the correct interpreter."));
            writeSessionLog("Error of interpreting " + fileName);
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
        writeSessionLog("Fullscreen mode switched on");
    }
    else if(!menuBar->getFullscrState())
    {
        menuBar->setFullscrState(false);
        this->showNormal();
        writeSessionLog("Fullscreen mode switched off");
    }
}

void MainWindow::chooseInterpreter()
{
    if(editorSetuped == false)
        while(compilerPath == "")
            compilerPath = QFileDialog::getOpenFileName(this, tr("Choose the interpreter"), "Turnip-Runner");
    else compilerPath = QFileDialog::getOpenFileName(this, tr("Choose the interpreter"), "Turnip-Runner");

    writeToFile("configs/compilerConfig.config", compilerPath);
    writeSessionLog("Choosed Turnip Runner in " + compilerPath);
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
        writeToFile("configs/languageConfig.config", "Russian");
    }
    if(language->text() == tr("English"))
    {
        appTranslator->load(":translations/english.qm");
        qApp->installTranslator(appTranslator);
        retranslateStrings();
        writeToFile("configs/languageConfig.config", "English");
    }
    if(language->text() == tr("German"))
    {
        appTranslator->load(":translations/german.qm");
        qApp->installTranslator(appTranslator);
        retranslateStrings();
        writeToFile("configs/languageConfig.config", "German");
    }
}

void MainWindow::about()
{
    QString aboutText;
    aboutText.insert(0,  "<h1>Turnip Editor 16.02</h1>"
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
    columnLine->setStyleSheet("font: 12pt; font-family: Consolas; color: rgb(84, 96, 107)");
    delete lineAndColumn;
}

