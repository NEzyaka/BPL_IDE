#ifndef SUBTOOLS_H
#define SUBTOOLS_H

#include <QString>
#include <QTime>
#include <QObject>

class SubTools : public QObject
{
public:
    SubTools();

    bool fileExits(QString filePath);
    QString readFromFile(QString filePath);
    void writeToFile(QString filePath, QString data);
    bool fileSaved(QString filePath, QString content);
    QString currentTimeDate();
    void writeLog(QString begin, QString end);
    void writeSessionLog(QString message);
    void clearSessionLog();
    QString getLanguage();
    QString getInterpreterPath();
    QString chooseInterpreter(bool editorSetuped);
    bool configsExists();
};

#endif // SUBTOOLS_H
