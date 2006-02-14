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

#include "latexeditor.h"
#include <QPainter>

LatexEditor::LatexEditor(QWidget *parent,QFont & efont) : QTextEdit(parent)
{
for (int i = 0; i < 3; ++i) UserBookmark[i]=0;
encoding="";
setFont(efont);
highlighter = new LatexHighlighter(document());
connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(update()));
}
LatexEditor::~LatexEditor(){

}

void LatexEditor::paintEvent(QPaintEvent *event)
{
QRect rect = cursorRect();
rect.setX(0);
rect.setWidth(viewport()->width());
QPainter painter(viewport());
const QBrush brush(QColor(0xE9, 0xE9, 0xE9));
painter.fillRect(rect, brush);
painter.end();
QTextEdit::paintEvent(event);
}

bool LatexEditor::search( const QString &expr, bool cs, bool wo, bool forward, bool startAtCursor )
{
QTextDocument::FindFlags flags = 0;
if (cs) flags |= QTextDocument::FindCaseSensitively;
if (wo) flags |= QTextDocument::FindWholeWords;
QTextCursor c = textCursor();
if (!c.hasSelection()) 
	{
	if (forward) c.movePosition(QTextCursor::Start);
	else c.movePosition(QTextCursor::End);
	setTextCursor(c);
	}
QTextDocument::FindFlags options;
if (! startAtCursor) 
	{
	c.movePosition(QTextCursor::Start);
	setTextCursor(c);
	}
if (forward == false) flags |= QTextDocument::FindBackward;
QTextCursor found = document()->find(expr, c, flags);

if (found.isNull()) return false;
else 
	{
	setTextCursor(found);
	return true;
	}
}

void LatexEditor::replace( const QString &r)
{
QTextCursor c = textCursor();
if (c.hasSelection()) 
	{
	c.removeSelectedText();
	c.insertText(r);
	}
}

void LatexEditor::gotoLine( int line )
{
if (line<=numoflines()) setCursorPosition( line, 0 );
}

void LatexEditor::commentSelection()
{
QTextCursor cur=textCursor();
if (cur.hasSelection())
	{
	int start=cur.selectionStart();
	int end=cur.selectionEnd();
	cur.setPosition(start,QTextCursor::MoveAnchor);
	cur.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
	while ( cur.position() < end )
		{
		cur.insertText("%");
		end++;
		cur.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
		}
}	
}

void LatexEditor::indentSelection()
{
QTextCursor cur=textCursor();
if (cur.hasSelection())
	{
	int start=cur.selectionStart();
	int end=cur.selectionEnd();
	cur.setPosition(start,QTextCursor::MoveAnchor);
	cur.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
	while ( cur.position() < end )
		{
		cur.insertText("\t");
		end++;
		cur.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
		}
	}
}

void LatexEditor::uncommentSelection()
{
QTextCursor cur=textCursor();
if (cur.hasSelection())
	{
	int start=cur.selectionStart();
	int end=cur.selectionEnd();
	cur.setPosition(start,QTextCursor::MoveAnchor);
	cur.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
	while ( cur.position() < end )
		{
		cur.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor);
		if (cur.selectedText()=="%") 
			{
			cur.removeSelectedText();
			end--;
			}
		cur.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
		}
	}
}

void LatexEditor::changeFont(QFont & new_font)
{
setFont(new_font);
}

QString LatexEditor::getEncoding()
{
 return encoding;
}

void LatexEditor::setEncoding(QString enc)
{
 encoding=enc;
}  

void LatexEditor::setCursorPosition(int para, int index)
{
QTextCursor cur=textCursor();
int i = 0;
QTextBlock p = document()->begin();
while ( p.isValid() ) {
	if (para==i) break;
	i++;
p = p.next();
}
int pos=p.position();
cur.setPosition(pos+index,QTextCursor::MoveAnchor);
setTextCursor(cur);
ensureCursorVisible();
setFocus();
}

void LatexEditor::removeOptAlt()
{
QTextCursor cur=textCursor();
QTextBlock p = document()->begin();
QString s;
while (p.isValid())
   {
    s = p.text();
    s=s.left(3);
    if (s=="OPT" || s=="ALT")
        {
	int pos=p.position();
	p = p.next();
	cur.setPosition(pos,QTextCursor::MoveAnchor);
	cur.select(QTextCursor::BlockUnderCursor);
	cur.removeSelectedText();	
        }
    else 
	{
	p = p.next();
	}
   }
setFocus();
}

int LatexEditor::numoflines()
{
int num=0;
QTextBlock p;
for (p = document()->begin(); p.isValid(); p = p.next()) ++num;
return num;
}

int LatexEditor::linefromblock(const QTextBlock& p)
{
if (!p.isValid()) return -1;
int num = 1;
QTextBlock block=document()->begin();
while (block.isValid())
	{
	if ( p == block ) return num;
	num++;
	block = block.next();
	}
return -1;
}
