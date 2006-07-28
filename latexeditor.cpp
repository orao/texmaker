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
#include "parenmatcher.h"
#include <QPainter>
#include <QTextLayout>
#include <QMetaProperty>
#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QApplication>
#include <QMimeData>
#include <QClipboard>
#include <QPalette>

LatexEditor::LatexEditor(QWidget *parent,QFont & efont) : QTextEdit(parent)
{
QPalette p = palette();
p.setColor(QPalette::Inactive, QPalette::Highlight,p.color(QPalette::Active, QPalette::Highlight));
p.setColor(QPalette::Inactive, QPalette::HighlightedText,p.color(QPalette::Active, QPalette::HighlightedText));
setPalette(p);
setAcceptRichText(false);
setLineWidth(0);
setFrameShape(QFrame::NoFrame);
for (int i = 0; i < 3; ++i) UserBookmark[i]=0;
encoding="";
setFont(efont);
setTabStopWidth(fontMetrics().width("    "));
highlighter = new LatexHighlighter(document());
connect(this, SIGNAL(cursorPositionChanged()), viewport(), SLOT(update()));
 matcher = new ParenMatcher;
 connect(this, SIGNAL(cursorPositionChanged()), matcher, SLOT(matchFromSender()));
setFocus();
}
LatexEditor::~LatexEditor(){
delete matcher;
}

void LatexEditor::clearMarkerFormat(const QTextBlock &block, int markerId)
{
    QTextLayout *layout = block.layout();
    QList<QTextLayout::FormatRange> formats = layout->additionalFormats();
    bool formatsChanged = false;
    for (int i = 0; i < formats.count(); ++i)
        if (formats.at(i).format.hasProperty(markerId)) {
            formats[i].format.clearBackground();
            formats[i].format.clearProperty(markerId);
            if (formats.at(i).format.properties().isEmpty()) {
                formats.removeAt(i);
                --i;
            }
            formatsChanged = true;
        }

    if (formatsChanged)
        layout->setAdditionalFormats(formats);
}

void LatexEditor::paintEvent(QPaintEvent *event)
{
QRect rect = cursorRect();
rect.setX(0);
rect.setWidth(viewport()->width());
QPainter painter(viewport());
const QBrush brush(QColor("#ececec"));
painter.fillRect(rect, brush);
painter.end();
QTextEdit::paintEvent(event);
}

void LatexEditor::contextMenuEvent(QContextMenuEvent *e)
{
QMenu *menu=new QMenu(this);
QAction *a;
a = menu->addAction(tr("Undo"), this, SLOT(undo()));
a->setShortcut(Qt::CTRL+Qt::Key_Z);
a->setEnabled(document()->isUndoAvailable());
a = menu->addAction(tr("Redo") , this, SLOT(redo()));
a->setShortcut(Qt::CTRL+Qt::Key_Y);
a->setEnabled(document()->isRedoAvailable());
menu->addSeparator();
a = menu->addAction(tr("Cut"), this, SLOT(cut()));
a->setShortcut(Qt::CTRL+Qt::Key_X);
a->setEnabled(textCursor().hasSelection());
a = menu->addAction(tr("Copy"), this, SLOT(copy()));
a->setShortcut(Qt::CTRL+Qt::Key_C);
a->setEnabled(textCursor().hasSelection());
a = menu->addAction(tr("Paste") , this, SLOT(paste()));
a->setShortcut(Qt::CTRL+Qt::Key_P);
const QMimeData *md = QApplication::clipboard()->mimeData();
a->setEnabled(md && canInsertFromMimeData(md));
menu->addSeparator();
a = menu->addAction(tr("Select All"), this, SLOT(selectAll()));
a->setShortcut(Qt::CTRL+Qt::Key_A);
a->setEnabled(!document()->isEmpty());
menu->addSeparator();
a = menu->addAction(tr("Check Spelling Word"), this, SLOT(checkSpellingWord()));
a->setEnabled(!document()->isEmpty());
a = menu->addAction(tr("Check Spelling Selection"), this, SLOT(checkSpellingDocument()));
a->setEnabled(textCursor().hasSelection());
a = menu->addAction(tr("Check Spelling Document"), this, SLOT(checkSpellingDocument()));
a->setEnabled(!document()->isEmpty() && !textCursor().hasSelection());
menu->exec(e->globalPos());
delete menu;
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
int start;
QTextCursor c = textCursor();
if (c.hasSelection()) 
	{
	start=c.selectionStart();
	c.removeSelectedText();
	c.insertText(r);
	c.setPosition(start,QTextCursor::MoveAnchor);
	c.setPosition(start+r.length(),QTextCursor::KeepAnchor);
//	c.movePosition(QTextCursor::NextWord,QTextCursor::KeepAnchor);
	setTextCursor(c);
	}
}

void LatexEditor::gotoLine( int line )
{
if (line<=numoflines()) setCursorPosition( line, 0 );
}

void LatexEditor::commentSelection()
{
bool go=true;
QTextCursor cur=textCursor();
if (cur.hasSelection())
	{
	int start=cur.selectionStart();
	int end=cur.selectionEnd();
	cur.setPosition(start,QTextCursor::MoveAnchor);
	cur.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
	while ( cur.position() < end && go)
		{
		cur.insertText("%");
		end++;
		go=cur.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
		}
}	
}

void LatexEditor::indentSelection()
{
bool go=true;
QTextCursor cur=textCursor();
if (cur.hasSelection())
	{
	int start=cur.selectionStart();
	int end=cur.selectionEnd();
	cur.setPosition(start,QTextCursor::MoveAnchor);
	cur.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
	while ( cur.position() < end && go)
		{
		cur.insertText("\t");
		end++;
		go=cur.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
		}
	}
}

void LatexEditor::uncommentSelection()
{
bool go=true;
QTextCursor cur=textCursor();
if (cur.hasSelection())
	{
	int start=cur.selectionStart();
	int end=cur.selectionEnd();
	cur.setPosition(start,QTextCursor::MoveAnchor);
	cur.movePosition(QTextCursor::StartOfBlock,QTextCursor::MoveAnchor);
	while ( cur.position() < end && go)
		{
		cur.movePosition(QTextCursor::Right,QTextCursor::KeepAnchor);
		if (cur.selectedText()=="%") 
			{
			cur.removeSelectedText();
			end--;
			}
		go=cur.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
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

int LatexEditor::getCursorPosition(int para, int index)
{
int i = 0;
QTextBlock p = document()->begin();
while ( p.isValid() ) {
	if (para==i) break;
	i++;
p = p.next();
}
return p.position()+index;
}

void LatexEditor::setCursorPosition(int para, int index)
{
int pos=getCursorPosition(para,index);
QTextCursor cur=textCursor();
cur.setPosition(pos,QTextCursor::MoveAnchor);
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

void LatexEditor::selectword(int line, int col, QString word)
{
QTextCursor cur=textCursor();
int i = 0;
QTextBlock p = document()->begin();
while ( p.isValid() ) {
	if (line==i) break;
	i++;
p = p.next();
}
int pos=p.position();
int offset=word.length();
cur.setPosition(pos+col,QTextCursor::MoveAnchor);
cur.setPosition(pos+col+offset,QTextCursor::KeepAnchor);
setTextCursor(cur);
ensureCursorVisible();
}

void LatexEditor::checkSpellingWord()
{
QTextCursor cur=textCursor();
cur.select(QTextCursor::WordUnderCursor);
setTextCursor(cur);
if (cur.hasSelection()) emit spellme();
}

void LatexEditor::checkSpellingDocument()
{
emit spellme();
}

