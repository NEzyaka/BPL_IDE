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

    //operators
    operatorFormat.setForeground(QColor(63, 81, 181));
    operatorFormat.setFontWeight(QFont::Bold);
    QStringList operatorPatterns;

    operatorPatterns << "\\bSIZE\\b" << "\\bSETVAL\\b" << "\\bDELETE\\b"
                     << "\\bSWAP\\b" << "\\bPRINT\\b" << "\\bDUMP\\b"
                     << "\\bDUMPVAR\\b" << "\\bINPUTVAR\\b" << "\\bCLEARSCREEN\\b"
                     << "\\bALERT\\b" << "\\bNEXTLINE\\b" << "\\bCOMMAND\\b"
                     << "\\bFONTCOLOR\\b" << "\\bDOBLOCK\\b" << "\\bIMPORT\\b"
                     << "\\bBLOCKLIST\\b" << "\\bADD\\b" << "\\bSUBTRACT\\b" << "\\bMULTIPLY\\b"
                     << "\\bDIVISE\\b" << "AND" << "OR" << "\\+" << "\\-" << "\\*" << "\\/" << "\\=";
    foreach(const QString &pattern, operatorPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = operatorFormat;
        highlightingRules.append(rule);
    }

    //single-line comment
    singleLineCommentFormat.setForeground(QColor(4, 147, 114));
    singleLineCommentFormat.setFontWeight(QFont::Bold);
    singleLineCommentFormat.setFontItalic(true);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(42, 187, 155));
    multiLineCommentFormat.setFontWeight(QFont::Bold);
    multiLineCommentFormat.setFontItalic(true);

    //data types
    variableFormat.setForeground(QColor(245, 215, 110));
    variableFormat.setFontWeight(QFont::Bold);
    QStringList variablePatterns;
    variablePatterns << "\\bLINE\\b" << "\\bINTEGER\\b"
                     << "\\bBOOL\\b" << "\\bDOUBLE\\b";
    foreach (const QString &pattern, variablePatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = variableFormat;
        highlightingRules.append(rule);
    }

    //errors
    errorsFormat.setBackground(QColor(255, 10, 0));
    errorsFormat.setFontWeight(QFont::Bold);
    QStringList errorsPatterns;
    errorsPatterns << "\\bprint\\b";

    foreach(const QString &pattern, errorsPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = errorsFormat;
        highlightingRules.append(rule);
    }

    //values
    valueFormat.setForeground(QColor(241, 169, 160));
    valueFormat.setFontWeight(QFont::Bold);
    valueFormat.setFontItalic(true);
    QStringList valuePatterns;
    valuePatterns << "\\bLIGHTGREEN\\b" << "\\bLIGHTWHITE\\b" << "\\bLIGHTPURPLE\\b"
                  << "\\bLIGHTYELLOW\\b" << "\\bLIGHTRED\\b" << "\\bLIGHTBLUE\\b"
                  << "\\blightgreen\\b" << "\\blightwhite\\b" << "\\blightpurple\\b"
                  << "\\blightyellow\\b" << "\\blightred\\b" << "\\blightblue\\b"
                  << "\\blightgray\\b" << "\\bgray\\b" << "\\bwhite\\b"
                  << "\\byellow\\b" << "\\bpurple\\b" << "\\bred\\b"
                  << "\\bcyan\\b" << "\\bgreen\\b" << "\\bblack\\b"
                  << "\\bblue\\b" << "\\bLIGHTGRAY\\b" << "\\bGRAY\\b"
                  << "\\bWHITE\\b" << "\\bYELLOW\\b" << "\\bPURPLE\\b"
                  << "\\bRED\\b" << "\\bCYAN\\b" << "\\bGREEN\\b"
                  << "\\bBLACK\\b" << "\\bBLUE\\b" << "\\btrue\\b"
                  << "\\bfalse\\b" << "\\bTRUE\\b" << "\\bFALSE\\b"
                  << "\\bMAIN\\b" << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "0";
    foreach(const QString &pattern, valuePatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = valueFormat;
        highlightingRules.append(rule);
    }

    //conditions
    conditionsFormat.setForeground(QColor(54, 215, 183));
    conditionsFormat.setFontWeight(QFont::Bold);
    QStringList conditionsPatterns;
    conditionsPatterns << "\\bIF\\b" << "\\bELSE\\b" << "\\bENDIF\\b";

    foreach(const QString &pattern, conditionsPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = conditionsFormat;
        highlightingRules.append(rule);
    }

    //conditions
    cyclesFormat.setForeground(QColor(145, 61, 136));
    cyclesFormat.setFontWeight(QFont::Bold);
    QStringList cyclesPatterns;
    cyclesPatterns << "\\bFOR\\b" << "\\bENDFOR\\b";

    foreach(const QString &pattern, cyclesPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = cyclesFormat;
        highlightingRules.append(rule);
    }

    //blocks
    blocksFormat.setForeground(QColor(235, 149, 50));
    blocksFormat.setFontWeight(QFont::Bold);
    QStringList blocksPatterns;
    blocksPatterns << "\\bBEGINBLOCK\\b" << "\\bENDBLOCK\\b";

    foreach(const QString &pattern, blocksPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = blocksFormat;
        highlightingRules.append(rule);
    }

    //multi-line comment
    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");

    //lines
    quotationFormat.setForeground(QColor(174, 168, 211));
    quotationFormat.setFontWeight(QFont::Bold);
    quotationFormat.setFontItalic(true);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text) //syntax highlighting
{
    foreach(const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while(index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if(previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);


    while(startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if(endIndex == -1)
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

