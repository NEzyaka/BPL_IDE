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

#ifndef CODEEDITOR_H
#define CODEEDITOR_H


#include <QPlainTextEdit>
#include <QObject>

#include "widget.h"

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QAbstractItemModel;
QT_END_NAMESPACE

class QCompleter;
class LineNumberArea;

class CodeEditor : public QPlainTextEdit, public Widget
{
    Q_OBJECT

public:
    CodeEditor(QWidget* parent = 0);

    QColor backColor;
    QColor fontColor;

    QString number;
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void retranslateStrings();
    void setupScheme();
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);
    void insertCompletion(const QString& completion);

public slots:
    void setUndoEnabled(bool state);
    void setRedoEnabled(bool state);
    void setCopyEnabled(bool state);
    void setCutEnabled(bool state);

private:
    QWidget* lineNumberArea;

    void keyPressEvent(QKeyEvent* event);
    QString textUnderCursor() const;
    QStringList* words;
    QCompleter* completer;

    void contextMenuEvent(QContextMenuEvent *e);
    QAbstractItemModel *modelFromFile(const QString& fileName);

    void createActions();
    void createMenus();
    QMenu* contextMenu;
    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* commentAction;

    void setupFont();

public slots:
    void comment();
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};

#endif // CODEEDITOR_H
