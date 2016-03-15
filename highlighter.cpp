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

#include "highlighter.h"

Highlighter::Highlighter(QTextDocument* parent) : QSyntaxHighlighter(parent) //constructor
{
    HighlightingRule rule;

    //operators
    operatorFormat.setForeground(QColor(63, 81, 181));
    operatorFormat.setFontWeight(QFont::Bold);
    QStringList operatorPatterns;

    QFile* operators = new QFile("configs/highlight/operators.config");
    if(operators->open(QIODevice::ReadOnly |QIODevice::Text))
    {
        while(!operators->atEnd())
        {
            QString str = operators->readLine();
            operatorPatterns << str.remove(str.length() - 1, 1);
        }
    }
    delete operators;

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
    rule.pattern = QRegExp("@[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(QColor(42, 187, 155));
    multiLineCommentFormat.setFontWeight(QFont::Bold);
    multiLineCommentFormat.setFontItalic(true);

    //data types
    variableFormat.setForeground(QColor(245, 215, 110));
    variableFormat.setFontWeight(QFont::Bold);
    QStringList variablePatterns;

    QFile* dataTypes = new QFile("configs/highlight/dataTypes.config");
    if(dataTypes->open(QIODevice::ReadOnly |QIODevice::Text))
    {
        while(!dataTypes->atEnd())
        {
            QString str = dataTypes->readLine();
            variablePatterns << str.remove(str.length() - 1, 1);
        }
    }
    delete dataTypes;

    foreach(const QString &pattern, variablePatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = variableFormat;
        highlightingRules.append(rule);
    }

    //errors
    errorsFormat.setBackground(QColor(255, 10, 0));
    errorsFormat.setFontWeight(QFont::Bold);
    QStringList errorsPatterns;

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

    QFile* values = new QFile("configs/highlight/values.config");
    if(values->open(QIODevice::ReadOnly |QIODevice::Text))
    {
        while(!values->atEnd())
        {
            QString str = values->readLine();
            valuePatterns << str.remove(str.length() - 1, 1);
        }
    }
    delete values;

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

    QFile* conditions = new QFile("configs/highlight/conditions.config");
    if(conditions->open(QIODevice::ReadOnly |QIODevice::Text))
    {
        while(!conditions->atEnd())
        {
            QString str = conditions->readLine();
            conditionsPatterns << str.remove(str.length() - 1, 1);
        }
    }
    delete conditions;

    foreach(const QString &pattern, conditionsPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = conditionsFormat;
        highlightingRules.append(rule);
    }

    //blocks
    blocksFormat.setForeground(QColor(235, 149, 50));
    blocksFormat.setFontWeight(QFont::Bold);
    QStringList blocksPatterns;

    QFile* blocks = new QFile("configs/highlight/blocks.config");
    if(blocks->open(QIODevice::ReadOnly |QIODevice::Text))
    {
        while(!blocks->atEnd())
        {
            QString str = blocks->readLine();
            blocksPatterns << str.remove(str.length() - 1, 1);
        }
    }
    delete blocks;

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
