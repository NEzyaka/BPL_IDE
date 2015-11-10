/****************************************************************************
**
** Copyright (C) 2015 Nikita Mironov
** Contact: nekit2002mir@yandex.ru
**
** This file is part of BPL IDE source code.
** BPL IDE is open-source cross-platform IDE for BPL programming language.
**
** BPL IDE is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** Foobar is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with BPL IDE. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

#include <QtWidgets>

#include "codeeditor.h"
#include "mainwindow.h"

#include "icons/copy.xpm"
#include "icons/redo.xpm"
#include "icons/undo.xpm"
#include "icons/paste.xpm"
#include "icons/cut.xpm"
#include "icons/comment.xpm"

#include <QtGui>
#include <QRect>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) //constructor
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);

    completer = new QCompleter(this);
    completer->setWidget(this);

    completer = new QCompleter(this);
    completer->setWidget(this);

    completer->setModel(modelFromFile("configs/completerModel.config"));
    completer->setCompletionMode(QCompleter::PopupCompletion);
    connect(completer, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
}

void CodeEditor::comment() //commenting line
{
    QTextCursor* tc = new QTextCursor(this->textCursor());
    tc->select(QTextCursor::WordUnderCursor);
    tc->insertText("//" + tc->selectedText());
    delete tc;
}


//line number area

void CodeEditor::contextMenuEvent(QContextMenuEvent *e) //setting up context menu
{
    QMenu* contextMenu;

    undoAction = new QAction(this);
    redoAction = new QAction(this);
    cutAction = new QAction(this);
    copyAction = new QAction(this);
    pasteAction = new QAction(this);
    commentAction = new QAction(this);

    undoAction->setShortcut(Qt::CTRL + Qt::Key_Z);
    redoAction->setShortcut(Qt::CTRL + Qt::Key_Y);
    cutAction->setShortcut(Qt::CTRL + Qt::Key_X);
    copyAction->setShortcut(Qt::CTRL + Qt::Key_C);
    pasteAction->setShortcut(Qt::CTRL + Qt::Key_V);

    undoAction->setIcon(QPixmap(undoXpm));
    redoAction->setIcon(QPixmap(redoXpm));
    cutAction->setIcon(QPixmap(cutXpm));
    copyAction->setIcon(QPixmap(copyXpm));
    pasteAction->setIcon(QPixmap(pasteXpm));
    commentAction->setIcon(QPixmap(commentXpm));

    contextMenu = new QMenu(this);

    contextMenu->addAction(cutAction);
    contextMenu->addAction(copyAction);
    contextMenu->addAction(pasteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(undoAction);
    contextMenu->addAction(redoAction);
    contextMenu->addSeparator();
    contextMenu->addAction(commentAction);

    connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
    connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));
    connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));
    connect(commentAction, SIGNAL(triggered()), this, SLOT(comment()));

    undoAction->setText("Отменить");
    redoAction->setText("Повторить");
    cutAction->setText("Вырезать");
    copyAction->setText("Копировать");
    pasteAction->setText("Вставить");
    commentAction->setText("Закомментировать строку");

    contextMenu->exec(e->globalPos());

    delete contextMenu;
    delete undoAction;
    delete redoAction;
    delete cutAction;
    delete copyAction;
    delete pasteAction;
}

int CodeEditor::lineNumberAreaWidth() //setting up line number area width
{
    int digits = 2;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int) //updating width of line number area
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) //updating line number area
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) //resizing line number area
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) //painting line number area
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor(backColor));

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(/*231, 76, 60*/ fontColor));
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


//completer

QAbstractItemModel *CodeEditor::modelFromFile(const QString& fileName) //getting completer model from file
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
    QStringList words;

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return new QStringListModel(words, completer);
}

void CodeEditor::insertCompletion(const QString& completion) //inserting complted operator
{
    QTextCursor cursor = textCursor();
    int extra = completion.length() - completer->completionPrefix().length();
    cursor.movePosition(QTextCursor::Left);
    cursor.movePosition(QTextCursor::EndOfWord);
    cursor.insertText(completion.right(extra));
    setTextCursor(cursor);
}

QString CodeEditor::textUnderCursor() const //getting text under cursor
{
    QTextCursor* cursor = new QTextCursor(textCursor());
    cursor->select(QTextCursor::WordUnderCursor);
    return cursor->selectedText();
    delete cursor;
}

void CodeEditor::keyPressEvent(QKeyEvent* event) //key-press event
{
    if(completer->popup()->isVisible())
    {
        switch (event->key())
        {
        case Qt::Key_Enter:
        case Qt::Key_Return:
        case Qt::Key_Escape:
        case Qt::Key_Tab:
            event->ignore();
            return;
        }
    }

    QPlainTextEdit::keyPressEvent(event);

    const QString completionPrefix = textUnderCursor();

    if(completionPrefix != completer->completionPrefix())
    {
        completer->setCompletionPrefix(completionPrefix);
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
    }

    if (!event->text().isEmpty() && completionPrefix.length() > 2)
    {
        QRect* cr = new QRect(cursorRect());
        cr->setWidth(completer->popup()->sizeHintForColumn(0) + completer->popup()->verticalScrollBar()->sizeHint().width());
        completer->complete(*cr);
        delete cr;
    }
}

