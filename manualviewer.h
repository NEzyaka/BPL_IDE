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

#ifndef MANUALVIEWER_H
#define MANUALVIEWER_H

#include <QDialog>
#include <QTableWidget>
#include <QWidget>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QFont>

class ManualViewer : public QDialog
{
    Q_OBJECT
public:
    explicit ManualViewer(QWidget *parent = 0);
private:
    QTabWidget* parts;

    QWidget* startWidget;
    QWidget* operatorsWidget;
    QWidget* varsWidget;
    QWidget* ifWidget;
    QWidget* blocksWidget;

    QVBoxLayout* helpLayout;
    QVBoxLayout* startLayout;
    QVBoxLayout* operatorsLayout;
    QVBoxLayout* varsLayout;
    QVBoxLayout* ifLayout;
    QVBoxLayout* blocksLayout;

    QFont font;
    QTextBrowser* startView;
    QTextBrowser* operatorsView;
    QTextBrowser* varsView;
    QTextBrowser* ifView;
    QTextBrowser* blocksView;

    QString readFromFile(QString filePath);
    void setupTabs();
};

#endif // MANUALVIEWER_H
