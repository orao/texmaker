/***************************************************************************
 *   copyright       : (C) 2003-2005 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LATEXEDITOR_H
#define LATEXEDITOR_H

#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>

#include "latexhighlighter.h"

typedef  int UserBookmarkList[3];

class LatexEditor : public QTextEdit  {
   Q_OBJECT
public:
LatexEditor(QWidget *parent,QFont & efont);
~LatexEditor();
void gotoLine( int line );
bool search( const QString &expr, bool cs, bool wo, bool forward, bool startAtCursor );
void replace( const QString &r);
void commentSelection();
void uncommentSelection();
void indentSelection();
void changeFont(QFont & new_font);
QString getEncoding();
void setEncoding(QString enc);
void setCursorPosition(int parag, int index);
void removeOptAlt();
int numoflines();
int linefromblock(const QTextBlock& p);
UserBookmarkList UserBookmark;
private:
QString encoding;
LatexHighlighter *highlighter;
protected:
void paintEvent(QPaintEvent *event);

};

#endif
