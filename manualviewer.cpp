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

#include "manualviewer.h"

#include <QFile>

ManualViewer::ManualViewer(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle(tr("Manual"));
    this->setWindowIcon(QPixmap(":icons/logo.png"));
    this->resize(750, 500);
    this->setMinimumSize(750, 500);

    parts = new QTabWidget(this);

    startWidget = new QWidget(this);
    operatorsWidget = new QWidget(this);
    varsWidget = new QWidget(this);
    ifWidget = new QWidget(this);
    blocksWidget = new QWidget(this);

    helpLayout = new QVBoxLayout(this);
    startLayout = new QVBoxLayout(startWidget);
    operatorsLayout = new QVBoxLayout(operatorsWidget);
    varsLayout = new QVBoxLayout(varsWidget);
    ifLayout = new QVBoxLayout(ifWidget);
    blocksLayout = new QVBoxLayout(blocksWidget);

    startView = new QTextBrowser(startWidget);
    operatorsView = new QTextBrowser(operatorsWidget);
    varsView = new QTextBrowser(varsWidget);
    ifView = new QTextBrowser(ifWidget);
    blocksView = new QTextBrowser(blocksWidget);

    setupTabs();
}

QString ManualViewer::readFromFile(QString filePath)
{
    QString data = "";

    QFile* file = new QFile(filePath);
    if(file->open(QFile::ReadOnly | QFile::Text))
        data = file->readAll();
    file->close();
    delete file;

    return data;
}

void ManualViewer::setupTabs()
{
    font.setPointSize(13);

    startLayout->addWidget(startView);
    startView->setHtml(readFromFile(":help/start.html"));
    startView->setFont(font);
    startWidget->setLayout(startLayout);

    operatorsLayout->addWidget(operatorsView);
    operatorsView->setHtml(readFromFile(":help/operators.html"));
    operatorsView->setFont(font);
    operatorsWidget->setLayout(operatorsLayout);

    varsLayout->addWidget(varsView);
    varsView->setHtml(readFromFile(":help/vars.html"));
    varsView->setFont(font);
    varsWidget->setLayout(varsLayout);

    blocksLayout->addWidget(blocksView);
    blocksView->setHtml(readFromFile(":/help/blocks.html"));
    blocksView->setFont(font);
    blocksWidget->setLayout(blocksLayout);

    ifLayout->addWidget(ifView);
    ifView->setHtml(readFromFile(":help/if.html"));
    ifView->setFont(font);
    ifWidget->setLayout(ifLayout);

    parts->addTab(startWidget, tr("Start"));
    parts->addTab(operatorsWidget, tr("Operators"));
    parts->addTab(varsWidget, tr("Variables"));
    parts->addTab(ifWidget, tr("Conditions"));
    parts->addTab(blocksWidget, tr("Blocks"));

    helpLayout->addWidget(parts);
    this->setLayout(helpLayout);

    parts->setStyleSheet("QTabWidget::tab-bar { alignment: center; }"
                         "QTabWidget::pane { padding: -9px -9px -9px -9px; }"
                         "QTabBar::tab { font-family: Segoe UI; font: 9pt; background: rgb(245, 246, 248); border-bottom: none; border-top-right-radius: 4px; border-top-left-radius: 4px; min-width: 80px; padding: 5px; }"
                         "QTabBar::tab:selected, QTabBar::tab:hover { background: rgb(43, 48, 59); color: rgb(245, 246, 248); }"
                         "QTabBar::tab:!selected { margin-top: 5px; }"
                         "QTabBar::tab:selected { margin-left: -4px; margin-right: -4px; }"
                         "QTabBar::tab:first:selected { margin-left: 0; }"
                         "QTabBar::tab:last:selected { margin-right: 0; }"
                         "QTabBar::tab:only-one { margin: 0; }"
                         "QTextBrowser { border-radius: 3px; border:none; background: rgb(43, 48, 59); color: rgb(192, 197, 206); padding: 10px; }"
                         "QScrollBar:vertical { background: rgb(43, 48, 59); width: 12px; margin: 0px; }"
                         "QScrollBar::handle:vertical { background-color: rgb(61, 70, 79); border-radius: 3px; min-height: 20px; margin: 0px 2px 0px 2px; }"
                         "QScrollBar::add-line:vertical { background: none; height: 0px; subcontrol-position: right; subcontrol-origin: margin; }"
                         "QScrollBar::sub-line:vertical { background: none; height: 0px; subcontrol-position: left; subcontrol-origin: margin; }"
                         "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");

    this->setStyleSheet("background: rgb(245, 246, 248);");

}
