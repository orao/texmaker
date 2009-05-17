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

#ifndef LATEXEDITOR_H
#define LATEXEDITOR_H

#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>
#include <QCompleter>

#include "latexhighlighter.h"
#include "parenmatcher.h"
#include "hunspell/hunspell.hxx"

//class QCompleter;
//class ParenMatcher;
//class QTextBlock;

#define LRM 0x200E //add By S. R. Alavizadeh

typedef  int UserBookmarkList[3];

class LatexEditor : public QTextEdit  {
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

private:
QString encoding;
QString textUnderCursor() const;
QCompleter *c;
ParenMatcher *matcher;
QString spell_dic, spell_encoding;
QStringList ignoredwordList, hardignoredwordList;
bool inlinecheckSpelling;

private slots:
void correctWord();
void checkSpellingWord();
void checkSpellingDocument();
void insertCompletion(const QString &completion);

void contextMenuAddLRM();//add by S. R. Alavizadeh
void remLRMfromSelection();//add by S. R. Alavizadeh

protected:
void paintEvent(QPaintEvent *event);
void contextMenuEvent(QContextMenuEvent *e);
void keyPressEvent ( QKeyEvent * e );
void focusInEvent(QFocusEvent *e);
signals:
void removeLRM();//add by S. R. Alavizadeh
void spellme();
};

#endif
