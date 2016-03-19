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
