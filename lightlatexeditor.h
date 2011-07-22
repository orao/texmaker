/***************************************************************************
 *   copyright       : (C) 2003-2011 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *   addons by Luis Silvestre                                              *
 *   contains some code from CLedit (C) 2010 Heinz van Saanen -GPL         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LIGHTLATEXEDITOR_H
#define LIGHTLATEXEDITOR_H

#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>

#include "lightlatexhighlighter.h"
#include "encodingprober/qencodingprober.h"


typedef  int UserBookmarkList[3];

class LightLatexEditor : public QPlainTextEdit  {
   Q_OBJECT
public:
LightLatexEditor(QWidget *parent,QFont & efont, QColor colMath, QColor colCommand, QColor colKeyword);
~LightLatexEditor();
static void clearMarkerFormat(const QTextBlock &block, int markerId);
void gotoLine( int line );
bool search( const QString &expr, bool cs, bool wo, bool forward, bool startAtCursor );
void changeFont(QFont & new_font);
QString getEncoding();
void setEncoding(QString enc);
int getCursorPosition(int parag, int index);
void setCursorPosition(int para, int index);
int numoflines();
int searchLine( const QString &expr);
int linefromblock(const QTextBlock& p);
UserBookmarkList UserBookmark;
void selectword(int line, int col, QString word);
LightLatexHighlighter *highlighter;
void insertNewLine();
QRectF blockGeometry(const QTextBlock & block) {return blockBoundingGeometry(block).translated(contentOffset());}
int getLastNumLines();
void setLastNumLines(int n);

QString beginningLine();

public slots:
void matchAll();
void load( const QString &f );

private:
QString encoding;
int lastnumlines;

bool matchLeftPar ( QTextBlock currentBlock, int index, int numRightPar );
bool matchRightPar( QTextBlock currentBlock, int index, int numLeftPar );
void createParSelection( int pos );

int endBlock;


private slots:
void jumpToEndBlock();
void matchPar();
void ensureFinalNewLine();//Qt 4.7.1 bug
void gotoBookmark1();
void gotoBookmark2();
void gotoBookmark3();
void editFind();
void editGotoLine();


protected:
void paintEvent(QPaintEvent *event);
void contextMenuEvent(QContextMenuEvent *e);

signals:
void updatelineWidget();
void requestFind();
void requestGotoLine();
};

#endif
