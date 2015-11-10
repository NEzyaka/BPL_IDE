/****************************************************************************
**
** Copyright (C) 2015 Nikita Mironov
** Contact: nekit2002mir@yandex.ru
**
** This file is part of BPL IDE source code.
**
** BPL IDE is open-source cross-platform IDE for BPL programming language.
** This program is licensed under the terms of the GNU General Public License version 3.
**
****************************************************************************/


#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
class QAbstractItemModel;
QT_END_NAMESPACE

class QCompleter;
class LineNumberArea;

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget* parent = 0);

    QColor backColor;
    QColor fontColor;

    QString number;

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &, int);
    void insertCompletion( const QString& completion );
private:
    QWidget* lineNumberArea;

    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* commentAction;

    void keyPressEvent( QKeyEvent* event );
    QString textUnderCursor()const;

    QCompleter* completer;

    void contextMenuEvent(QContextMenuEvent *e);
    QAbstractItemModel *modelFromFile(const QString& fileName);

    QStringList* words;
private slots:
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

#endif

