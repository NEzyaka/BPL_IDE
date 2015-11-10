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

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument* parent) : QSyntaxHighlighter(parent) //constructor
{
    HighlightingRule rule;

    operatorFormat.setForeground(QColor(21, 96, 189, 255)); //operators
    operatorFormat.setFontWeight(QFont::Bold);
    QStringList operatorPatterns;
    operatorPatterns << "\\bSWAP\\b" << "\\bPRINTNUM\\b" << "\\bPRINT\\b" << "\\bPRINTVAR\\b" << "\\bDUMP\\b" << "\\bDUMPVAR\\b" << "\\bINPUTVAR\\b" << "\\bCLEARSCREEN\\b" << "\\bALERT\\b" << "\\bNEXTLINE\\b" << "\\bCOMMAND\\b" << "\\bFONTCOLOR\\b" << "\\+" << "\\-" << "\\*" << "\\/" << "\\=";
    foreach (const QString &pattern, operatorPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = operatorFormat;
        highlightingRules.append(rule);
    }

    singleLineCommentFormat.setForeground(QColor(80, 200, 120, 255)); //single-line comment
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(80, 200, 120, 255));

    variableFormat.setForeground(QColor(255, 216, 0)); //data types
    variableFormat.setFontWeight(QFont::Bold);
    QStringList variablePatterns;
    variablePatterns << "\\bLINE\\b" << "\\bINTEGER\\b" << "\\bBOOL\\b" << "\\bDOUBLE\\b";
    foreach (const QString &pattern, variablePatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = variableFormat;
        highlightingRules.append(rule);
    }

    errorsFormat.setBackground(QColor(255, 10, 0)); //errors
    errorsFormat.setFontWeight(QFont::Bold);
    QStringList errorsPatterns;
    errorsPatterns << "\\bprint\\b";

    foreach (const QString &pattern, errorsPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = errorsFormat;
        highlightingRules.append(rule);
    }

    valueFormat.setForeground(QColor(231, 84, 128)); //values
    valueFormat.setFontWeight(QFont::Bold);
    QStringList valuePatterns;
    valuePatterns << "\\bLIGHTGREEN\\b" << "\\bLIGHTWHITE\\b" << "\\bLIGHTPURPLE\\b" << "\\bLIGHTYELLOW\\b" << "\\bLIGHTRED\\b" << "\\bLIGHTBLUE\\b" << "\\blightgreen\\b" << "\\blightwhite\\b" << "\\blightpurple\\b" << "\\blightyellow\\b" << "\\blightred\\b" << "\\blightblue\\b" << "\\blightgray\\b" << "\\bgray\\b" << "\\bwhite\\b" << "\\byellow\\b" << "\\bpurple\\b" << "\\bred\\b" << "\\bcyan\\b" << "\\bgreen\\b" << "\\bblack\\b" << "\\bblue\\b" << "\\bLIGHTGRAY\\b" << "\\bGRAY\\b" << "\\bWHITE\\b" << "\\bYELLOW\\b" << "\\bPURPLE\\b" << "\\bRED\\b" << "\\bCYAN\\b" << "\\bGREEN\\b" << "\\bBLACK\\b" << "\\bBLUE\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bTRUE\\b" << "\\bFALSE\\b" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "0";
    foreach (const QString &pattern, valuePatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = valueFormat;
        highlightingRules.append(rule);
    }

    commentStartExpression = QRegExp("/\\*"); //multi-line comment
    commentEndExpression = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text) //syntax highlighting
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);


    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                    + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

