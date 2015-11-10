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

#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat operatorFormat;
    QTextCharFormat variableFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat valueFormat;
    QTextCharFormat errorsFormat;
};

#endif // HIGHLIGHTER_H
