#include "subtools.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDateTime>
#include <QFileInfo>
#include <QApplication>
#include <QDirIterator>
#include <QFileDialog>
#include <QIcon>

SubTools::SubTools()
{

}

bool SubTools::fileExits(QString filePath)
{
    QFile file;
    file.setFileName(filePath);
    return file.exists() ? true : false;
}

QString SubTools::readFromFile(QString filePath)
{
    QString data;

    QFile* file = new QFile(filePath);
    if(file->open(QFile::ReadOnly | QFile::Text))
    {
        data = file->readAll();
        file->close();
        delete file;

        return data;
    }
    else
    {
        writeSessionLog("Error of writing to file");
        return "\0";
    }
}

void SubTools::writeToFile(QString filePath, QString data)
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
    else writeSessionLog("Error of writing to file");
}

bool SubTools::fileSaved(QString filePath, QString content)
{
    return (readFromFile(filePath) == content) ? true : false;
}

QString SubTools::currentTimeDate() //getting current date and time
{
    return QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
}

void SubTools::writeLog(QString begin, QString end) //appending global log
{
    QFile* file = new QFile("log.log");

    if(!file->open(QIODevice::Append))
        return;
    else
    {
        QTextStream* stream = new QTextStream(&*file);
        *stream << "\n" << "Session " << begin << " - " << end << "\n" << readFromFile("sessionLog.log");
        stream->flush();
        file->close();
        delete file;
        delete stream;
    }
}

void SubTools::writeSessionLog(QString message) //writing session-log
{
    QFile* file = new QFile("sessionLog.log");
    if(!file->open(QIODevice::Append))
        return;
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

void SubTools::clearSessionLog() //cleaning session-log
{
    QFile* file = new QFile("sessionLog.log");
    if(!file->open(QIODevice::WriteOnly))
        return;
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

QString SubTools::getLanguage()
{
    QFile languageFromFile;
    languageFromFile.setFileName("configs/languageConfig.config");
    if(!languageFromFile.open(QFile::ReadOnly | QFile::Text) || !languageFromFile.exists())
        throw;
    else
        return languageFromFile.readLine();
    languageFromFile.close();
}

QString SubTools::getInterpreterPath()
{

    QFile* interpreterPathFromFile = new QFile("configs/interpreterConfig.config");
    QString *interpreterpath = new QString (readFromFile("configs/interpreterConfig.config"));
    QString interpreterPath;
    QFileInfo* interpr = new QFileInfo(*interpreterpath);

    if(interpreterpath->isEmpty() || !interpr->isExecutable())
    {
        delete interpr;
        QFileInfo* autoInterpreter;

        if((QSysInfo::kernelType() == "winnt") && (QSysInfo::windowsVersion() != QSysInfo::WV_None)) //if current platform is Windows
            autoInterpreter = new QFileInfo("Turnip-Runner.exe");
        else if(QSysInfo::kernelType() == "linux") //if current platform is Linux
            autoInterpreter = new QFileInfo("Turnip-Runner.run");
        else if((QSysInfo::kernelType() == "darwin") && (QSysInfo::macVersion() != QSysInfo::MV_None)) //if current platform is Macintosh
        { /*Here will be Macintosh executable file format*/ }

        if(autoInterpreter->isExecutable())
        {
            QMessageBox* autoDetectBox = new QMessageBox;
            autoDetectBox->setIconPixmap(QPixmap(":icons/question.png"));
            autoDetectBox->setText(tr("<b>Turnip Editor automatically found Turnip Runner<br> in ") + QString(autoInterpreter->absolutePath()) + "</b>");
            autoDetectBox->setInformativeText(tr("Do you want to use this interpreter?"));
            autoDetectBox->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            autoDetectBox->setDefaultButton(QMessageBox::Yes);
            int* ret = new int(autoDetectBox->exec());
            delete autoDetectBox;
            switch(*ret)
            {
            case QMessageBox::Yes:
                interpreterPath = autoInterpreter->absoluteFilePath();
                break;
            case QMessageBox::Cancel:
                interpreterPath = chooseInterpreter(false);
                break;
            }
            delete ret;
        }
        else
        {
            QString searchpath;

#if defined(Q_OS_WIN) //if current platform is Windows
            searchpath = "C:\\";
#elif defined(Q_OS_UNIX) //if current platform is Linux
            searchpath = "/";
#endif

            QDirIterator* iterator = new QDirIterator(searchpath, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
            while(iterator->hasNext())
            {
                iterator->next();

                bool condition;

#if defined(Q_OS_WIN) //if current platform is Windows
                condition = iterator->fileInfo().absoluteFilePath().contains("Turnip-Runner.exe", Qt::CaseInsensitive);
#elif defined(Q_OS_LINUX)
                condition = iterator->fileInfo().absoluteFilePath().contains("Turnip-Runner.run", Qt::CaseInsensitive);
#endif
                if(condition)
                {
                    QMessageBox* autoDetectBox = new QMessageBox;
                    autoDetectBox->setText(tr("<b>Turnip Editor automatically found Turnip Runner<br> in ") + QString(iterator->fileInfo().absolutePath() + "</b>"));
                    autoDetectBox->setInformativeText(tr("Do you want to use this interpreter?"));
                    autoDetectBox->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                    autoDetectBox->setDefaultButton(QMessageBox::Yes);
                    int* ret = new int(autoDetectBox->exec());
                    delete autoDetectBox;
                    switch(*ret)
                    {
                    case QMessageBox::Yes:
                        interpreterPath = iterator->fileInfo().absoluteFilePath();
                        break;
                    case QMessageBox::Cancel:
                        interpreterPath = chooseInterpreter(false);
                        break;
                    }
                    delete ret;
                    break;
                }
            }
            delete iterator;
            delete autoInterpreter;
        }
    }
    else return *interpreterpath;
    delete interpreterpath;

    interpreterPathFromFile->close();
    delete interpreterPathFromFile;

    writeToFile("configs/interpreterConfig.config", interpreterPath);
    return interpreterPath;
}

QString SubTools::chooseInterpreter(bool editorSetuped)
{
    QWidget buf;
    QString interpreterPath;
    if(editorSetuped == false)
        while(interpreterPath == "")
            interpreterPath = QFileDialog::getOpenFileName(&buf, tr("Choose the interpreter"), "Turnip-Runner");
    else interpreterPath = QFileDialog::getOpenFileName(&buf, tr("Choose the interpreter"), "Turnip-Runner");

    writeToFile("configs/interpreterConfig.config", interpreterPath);
    writeSessionLog("Choosed Turnip Runner in " + interpreterPath);

    return interpreterPath;
}

bool SubTools::configsExists()
{
    try
    {
        //checking of existing completer's model
        if(!fileExits("configs/completerModel.config"))
            throw 1;

        //checking of existing interpreter's config
        if(!fileExits("configs/interpreterConfig.config"))
            throw 1;

        //checking of existing language's config
        if(!fileExits("configs/languageConfig.config"))
            throw 1;

        //checking of existing config of default new file content
        if(!fileExits("configs/newFileDefault.config"))
            throw 1;

        //checking of existing blocks's highlighting list
        if(!fileExits("configs/highlight/blocks.config"))
            throw 1;

        //checking of existing conditions's highlighting list
        if(!fileExits("configs/highlight/conditions.config"))
            throw 1;

        //checking of existing dataTypes's highlighting list
        if(!fileExits("configs/highlight/dataTypes.config"))
            throw 1;

        //checking of existing operators's highlighting list
        if(!fileExits("configs/highlight/operators.config"))
            throw 1;

        //checking of existing values's highlighting list
        if(!fileExits("configs/highlight/values.config"))
            throw 1;

        return true;
    }
    catch(...)
    {
        return false;
    }
}
