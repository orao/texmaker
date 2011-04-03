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

#ifndef LATEXEDITOR_H
#define LATEXEDITOR_H

#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>
#include <QCompleter>
#include <QDateTime>
#include <QTimer>

#include "latexhighlighter.h"
#include "hunspell/hunspell.hxx"

typedef  int UserBookmarkList[3];

class LatexEditor : public QPlainTextEdit  {
   Q_OBJECT
public:
LatexEditor(QWidget *parent,QFont & efont, QColor colMath, QColor colCommand, QColor colKeyword,bool inlinespelling=false, QString ignoredWords="",Hunspell *spellChecker=0);
~LatexEditor();
static void clearMarkerFormat(const QTextBlock &block, int markerId);
void gotoLine( int line );
bool search( const QString &expr, bool cs, bool wo, bool forward, bool startAtCursor );
void replace( const QString &r);
void commentSelection();
void uncommentSelection();
void indentSelection();
void unindentSelection();
void changeFont(QFont & new_font);
QString getEncoding();
void setEncoding(QString enc);
int getCursorPosition(int parag, int index);
void setCursorPosition(int para, int index);
void removeOptAlt();
int numoflines();
int searchLine( const QString &expr);
int linefromblock(const QTextBlock& p);
UserBookmarkList UserBookmark;
void selectword(int line, int col, QString word);
LatexHighlighter *highlighter;
void setCompleter(QCompleter *completer);
QCompleter *completer() const;
QStringList alwaysignoredwordList;
void setSpellChecker(Hunspell * checker);
void activateInlineSpell(bool enable);
Hunspell * pChecker;
void insertNewLine();
void fold(int start, int end);
void unfold(int start, int end);
QRectF blockGeometry(const QTextBlock & block) {return blockBoundingGeometry(block).translated(contentOffset());}
//const QRectF blockGeometry(const QTextBlock & block);
//const QRectF blockRect(const QTextBlock & block) {return blockBoundingRect(block).translated(contentOffset());};
QMap<int,int> foldedLines;
QMap<int,int> foldableLines;


QStringList structlist, structitem;
QList<int> structtype, structpos, structlen;
QList<QTextCursor> structcursor;

void removeStructureItem(int offset,int len, int line);
void appendStructureItem(int line,QString item,int type,const QTextCursor& cursor);

int getLastNumLines();
void setLastNumLines(int n);

QDateTime getLastSavedTime();
void setLastSavedTime(QDateTime t);

class StructItem {
public:
int line;
QString item;
int type;
QTextCursor cursor;
StructItem(int l, const QString& it, int t,const QTextCursor& curs): line(l),item(it),type(t),cursor(curs) { };
};
const QList<StructItem> getStructItems() const { return StructItemsList; }

public slots:
void matchAll();
void setHightLightLine();
void clearHightLightLine();


private:
QDateTime lastSavedTime;
QList<StructItem> StructItemsList;
QString encoding;
int lastnumlines;
//QString textUnderCursor() const;
QString commandUnderCursor() const;
QStringList fullcommandUnderCursor();
QCompleter *c;
QString spell_dic, spell_encoding;
QStringList ignoredwordList, hardignoredwordList;
bool inlinecheckSpelling;
bool isWordSeparator(QChar c) const;
bool isSpace(QChar c) const;

bool matchLeftPar ( QTextBlock currentBlock, int index, int numRightPar );
bool matchRightPar( QTextBlock currentBlock, int index, int numLeftPar );
void createParSelection( int pos );

bool matchLeftLat ( QTextBlock currentBlock, int index, int numRightLat, int bpos );
bool matchRightLat( QTextBlock currentBlock, int index, int numLeftLat, int epos );
void createLatSelection(int start, int end );
int endBlock;
QTimer highlightRemover;
bool highlightLine;

private slots:
void correctWord();
void checkSpellingWord();
void checkSpellingDocument();
void insertCompletion(const QString &completion);
void jumpToPdf();
void jumpToEndBlock();


void matchPar();
void matchLat();

void ensureFinalNewLine();//Qt 4.7.1 bug

protected:
void paintEvent(QPaintEvent *event);
void contextMenuEvent(QContextMenuEvent *e);
void keyPressEvent ( QKeyEvent * e );
void focusInEvent(QFocusEvent *e);
signals:
void spellme();
void tooltiptab();
void requestpdf(int );
void setBlockRange(int,int);
void updatelineWidget();
void requestUpdateStructure();
void requestGotoStructure(int);
};

#endif
