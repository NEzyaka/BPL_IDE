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

#include "outputgetter.h"
#include <QDebug>

OutputGetter::OutputGetter(QWidget *parent) : QWidget(parent)
{
    setupViewer();
    setupGetter();
    setupButton();

    writerLay = new QHBoxLayout(this);
    writerLay->addWidget(edit);
    writerLay->addWidget(btn);
    writer = new QWidget(this);
    writer->setLayout(writerLay);

    lay = new QVBoxLayout(this);
    lay->addWidget(viewer);
    lay->addWidget(writer);
    this->setLayout(lay);
}

void OutputGetter::setupScheme()
{
    setupViewerScheme();
    setupGetterScheme();
    setupButtonScheme();
    setupFont();
}

void OutputGetter::retranslateStrings()
{
    btn->setText(tr("Enter"));
}

void OutputGetter::setupViewer()
{
    viewer = new QPlainTextEdit(this);
    viewer->setReadOnly(true);
}

void OutputGetter::setupViewerScheme()
{
    viewer->setStyleSheet("QPlainTextEdit { background: rgb(43, 48, 59); color: rgb(192, 197, 206); border: none; selection-background-color: rgb(192, 197, 206); selection-color: rgb(43, 48, 49) }"
                        "QScrollBar:vertical { background: transparent; border-top-right-radius: 4px; border-bottom-right-radius: 4px; width: 12px; margin: 0px; }"
                        "QScrollBar::handle:vertical { background-color: rgb(61, 70, 79); border-radius: 3px; min-height: 20px; margin: 0px 2px 0px 2px; }"
                        "QScrollBar::add-line:vertical { background: none; height: 0px; subcontrol-position: right; subcontrol-origin: margin; }"
                        "QScrollBar::sub-line:vertical { background: none; height: 0px; subcontrol-position: left; subcontrol-origin: margin; }"
                        "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }");
}

void OutputGetter::setupGetter()
{
    edit = new QLineEdit(this);
}

void OutputGetter::setupGetterScheme()
{
    edit->setStyleSheet("QLineEdit { background: rgb(43, 48, 59); color: rgb(192, 197, 206); border-bottom: 1px solid; border-color: rgb(192, 197, 206); border-radius: 0px; padding: 2px; selection-background-color: rgb(192, 197, 206); selection-color: rgb(43, 48, 49) }");
}

void OutputGetter::setupButton()
{
    btn = new QPushButton(this);
    btn->setText(tr("Enter"));
    connect(btn, SIGNAL(clicked(bool)), this, SIGNAL(buttonClicked()));
}

void OutputGetter::setupButtonScheme()
{
    btn->setStyleSheet("QPushButton { background: rgb(43, 48, 59); color: rgb(192, 197, 206); border: 1px solid; border-color: rgb(192, 197, 206); border-radius: 3px; padding: 3px 10px 3px 10px; }"
                       "QPushButton:pressed { background: rgb(192, 197, 206); color: rgb(43, 48, 59); border: none; }");
}

void OutputGetter::append(QString line)
{
    viewer->appendPlainText(line);
}

void OutputGetter::setupFont()
{
    QFont font;
    font.setFamily("Consolas");
    font.setFixedPitch(true);
    font.setPointSize(13);
    viewer->setFont(font);
    edit->setFont(font);
    btn->setFont(font);
}

