/***************************************************************************
 *   copyright       : (C) 2003-2009 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef LIGHTLATEXHIGHLIGHTER_H
#define LIGHTLATEXHIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QHash>
#include <QTextCharFormat>
#include <QColor>
#include <QTextBlockUserData>
//#include "latexeditor.h"

class QTextDocument;
class LightLatexEditor;

class LightLatexHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    LightLatexHighlighter(QTextDocument *parent = 0);
    ~LightLatexHighlighter();
    QColor ColorStandard, ColorComment, ColorMath, ColorCommand, ColorKeyword, ColorVerbatim;
    QStringList KeyWords;
public slots:
void setColors(QColor colMath, QColor colCommand, QColor colKeyword);
void SetEditor(LightLatexEditor *ed);
private :
bool isWordSeparator(QChar c) const;
protected:
LightLatexEditor *editor;
void highlightBlock(const QString &text);
};


#endif
