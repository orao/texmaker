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

#include "latexeditor.h"

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
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>
#include <QTextCodec>
#include <QFile>
#include "blockdata.h"

LatexEditor::LatexEditor(QWidget *parent,QFont & efont, QColor colMath, QColor colCommand, QColor colKeyword,bool inlinespelling,QString ignoredWords,Hunspell *spellChecker) : QPlainTextEdit(parent),c(0)
{
QPalette p = palette();
p.setColor(QPalette::Inactive, QPalette::Highlight,p.color(QPalette::Active, QPalette::Highlight));
p.setColor(QPalette::Inactive, QPalette::HighlightedText,p.color(QPalette::Active, QPalette::HighlightedText));
setPalette(p);
setFrameStyle(QFrame::NoFrame);
//setAcceptRichText(false);
setLineWidth(0);
setFrameShape(QFrame::NoFrame);
for (int i = 0; i < 3; ++i) UserBookmark[i]=0;
encoding="";
setFont(efont);
setTabStopWidth(fontMetrics().width("    "));
setTabChangesFocus(false);
endBlock=-1;
foldableLines.clear();
lastnumlines=0;
/*********************************/
inlinecheckSpelling=inlinespelling;
pChecker = spellChecker;
if (pChecker) spell_encoding=QString(pChecker->get_dic_encoding());
if (!ignoredWords.isEmpty()) alwaysignoredwordList=ignoredWords.split(",");
else alwaysignoredwordList.clear();
ignoredwordList=alwaysignoredwordList;
QFile wordsfile(":/spell/spellignore.txt");
QString line;
if (wordsfile.open(QFile::ReadOnly))
    {
    while (!wordsfile.atEnd()) 
	    {
	    line = wordsfile.readLine();
	    if (!line.isEmpty()) hardignoredwordList.append(line.trimmed());
	    }
    }
/********************************/

highlighter = new LatexHighlighter(document(),inlinespelling,ignoredWords,spellChecker);
highlighter->SetEditor(this);
highlighter->setColors(colMath,colCommand,colKeyword);

//c=0;
//connect(this, SIGNAL(cursorPositionChanged()), viewport(), SLOT(update()));
connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(matchAll()));
//connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(matchLat()));
//grabShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Tab), Qt::WidgetShortcut);
//setCenterOnScroll(true);
setFocusPolicy(Qt::WheelFocus);
setFocus();
lastSavedTime=QDateTime::currentDateTime();
highlightLine=false;
highlightRemover.setSingleShot(true);
connect(&highlightRemover, SIGNAL(timeout()), this, SLOT(clearHightLightLine()));
}
LatexEditor::~LatexEditor(){
//delete pChecker;
}

void LatexEditor::paintEvent(QPaintEvent *event)
{
QColor selBlendColor=QColor("#FF0000");
QColor blCol = selBlendColor.dark( 140 );
blCol.setAlphaF( 0.2 );
QRect rect = cursorRect();
rect.setX(0);
rect.setWidth(viewport()->width());
QPainter painter(viewport());
if (!highlightLine)
  {
  const QBrush brush(QColor("#ececec"));
  painter.fillRect(rect, brush);
  painter.end();
  }
else
  {
  const QBrush brush(blCol);
  painter.fillRect(rect, brush);
  painter.end();
  }
QPlainTextEdit::paintEvent(event);
}

void LatexEditor::contextMenuEvent(QContextMenuEvent *e)
{
QMenu *menu=new QMenu(this);
QAction *a;
/*******************************************/
if (inlinecheckSpelling && pChecker)
      {
      QFont spellmenufont (qApp->font());
      spellmenufont.setBold(true);
      QTextCursor c = cursorForPosition(e->pos());
      int cpos=c.position();
      QTextBlock block=c.block();
      int bpos=block.position();
      BlockData *data = static_cast<BlockData *>(c.block().userData() );
      QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
      QByteArray encodedString;
      QString text,word;
      bool gonext=true;
      QByteArray t;
      int li,cols,cole,colstart,colend,check,ns;
      char ** wlst;
      QStringList suggWords;
      colstart=cpos-bpos;
      if (data->misspelled[colstart]==true)
      {
 	while ((colstart>=0) && (colstart<data->misspelled.count()) && (data->misspelled[colstart]==true))
	{
	colstart--;
	}
	colstart++;
	colend=cpos-bpos;
	while ((colend>=0) && (colend<data->misspelled.count()) && (data->misspelled[colend]==true))
	{
	colend++;
	}
	c.setPosition(bpos+colstart,QTextCursor::MoveAnchor);
	c.setPosition(bpos+colend,QTextCursor::KeepAnchor);
	text=c.selectedText();
 
	li=c.blockNumber();
	cols=colstart;
        if (text.length()>1 && colend>colstart)
		{
		word=text;
		if (!ignoredwordList.contains(word) && !hardignoredwordList.contains(word))
			{
			encodedString = codec->fromUnicode(word);
			check = pChecker->spell(encodedString.data());
			if (!check)
				{
				selectword(li,cols,word);
				gonext=false;
				ns = pChecker->suggest(&wlst,encodedString.data());
				if (ns > 0)
					{
					suggWords.clear();
					for (int i=0; i < ns; i++) 
						{
						suggWords.append(codec->toUnicode(wlst[i]));
					//free(wlst[i]);
						} 
				//free(wlst);
					pChecker->free_list(&wlst, ns);
					if (!suggWords.isEmpty())
						{
						if (suggWords.contains(word)) gonext=true;
						else
							{
							foreach (const QString &suggestion, suggWords)
							    {
							    a = menu->addAction(suggestion, this, SLOT(correctWord()));
							    a->setFont(spellmenufont);
							    }
							}
						}
					}
				}
			}
		}
	menu->addSeparator();
	}
      }
/*******************************************/
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
if (!document()->isEmpty() && !inlinecheckSpelling)
{
a = menu->addAction(tr("Check Spelling Word"), this, SLOT(checkSpellingWord()));
//a->setEnabled(!document()->isEmpty() && !inlinecheckSpelling);
}
if (textCursor().hasSelection())
{
a = menu->addAction(tr("Check Spelling Selection"), this, SLOT(checkSpellingDocument()));
//a->setEnabled(textCursor().hasSelection());
}
a = menu->addAction(tr("Check Spelling Document"), this, SLOT(checkSpellingDocument()));
a->setEnabled(!document()->isEmpty() /*&& !textCursor().hasSelection()*/);
if (endBlock>=0)
  {
  if (linefromblock(textCursor().block())-1!=endBlock)
    {
    menu->addSeparator();
    a=new QAction(tr("Jump to the end of this block"),menu);
    connect(a, SIGNAL(triggered()), this, SLOT(jumpToEndBlock()));
    menu->addAction(a);
    }
  }
menu->addSeparator();
a=new QAction(tr("Jump to pdf"),menu);
a->setShortcut(Qt::CTRL+Qt::Key_Space);
a->setData(QVariant(cursorForPosition(e->pos()).blockNumber() + 1));
connect(a, SIGNAL(triggered()), this, SLOT(jumpToPdf()));
menu->addAction(a);
menu->exec(e->globalPos());
delete menu;
}

void LatexEditor::correctWord()
{
QAction *action = qobject_cast<QAction *>(sender());
if (action)
	{
	QString newword = action->text();
	replace(newword);
	}
}

bool LatexEditor::search( const QString &expr, bool cs, bool wo, bool forward, bool startAtCursor )
{
QTextDocument::FindFlags flags = 0;
if (cs) flags |= QTextDocument::FindCaseSensitively;
if (wo) flags |= QTextDocument::FindWholeWords;
QTextCursor c = textCursor();
//if (!c.hasSelection()) 
//	{
//	if (forward) c.movePosition(QTextCursor::Start);
//	else c.movePosition(QTextCursor::End);
//	setTextCursor(c);
//	}
QTextDocument::FindFlags options;
if (! startAtCursor) 
	{
	  c.setPosition(0);
	//c.movePosition(QTextCursor::Start);//Qt 4.7.1 bug
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

int LatexEditor::searchLine( const QString &expr)
{
int result=-1;
QTextCursor c = textCursor();
c.setPosition(0);
//c.movePosition(QTextCursor::Start); //Qt 4.7.1 bug
QTextCursor found = document()->find(expr, c, QTextDocument::FindCaseSensitively|QTextDocument::FindWholeWords);
if (found.isNull()) return result;
QTextBlock p = document()->findBlock(found.position());
if (p.blockNumber()>=0) result=p.blockNumber();
return result;
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
if (line<=numoflines())  
  {
  setCursorPosition( line, 0 );
  if (textCursor().block().isVisible()) setHightLightLine();
  }
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
		cur.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
		if (cur.selectedText()=="%") 
			{
			cur.removeSelectedText();
			end--;
			}
		go=cur.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
		}
	}
}

void LatexEditor::unindentSelection()
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
		cur.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
		if (cur.selectedText()=="\t") 
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
return document()->findBlockByNumber(para).position()+index;
}

void LatexEditor::setCursorPosition(int para, int index)
{
int pos=getCursorPosition(para,index);
QTextCursor cur=textCursor();
cur.setPosition(pos,QTextCursor::MoveAnchor);
if (cur.block().isVisible()) setTextCursor(cur);
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
return document()->blockCount();
}

int LatexEditor::linefromblock(const QTextBlock& p)
{
return p.blockNumber()+1;
}

void LatexEditor::selectword(int line, int col, QString word)
{
QTextCursor cur=textCursor();
int i = 0;
QTextBlock p = document()->begin();
while ( p.isValid() ) 
	{
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
QTextCursor c=textCursor();
int cpos=c.position();
QTextBlock block=c.block();
int bpos=block.position();
BlockData *data = static_cast<BlockData *>(c.block().userData() );
QString text;
int li,cols,colstart,colend;
colstart=cpos-bpos;
if (data->misspelled[colstart]==true)
  {
    while ((colstart>=0) && (colstart<data->misspelled.count()) && (data->misspelled[colstart]==true))
    {
    colstart--;
    }
    colstart++;
    colend=cpos-bpos;
    while ((colend>=0) && (colend<data->misspelled.count()) && (data->misspelled[colend]==true))
    {
    colend++;
    }
    c.setPosition(bpos+colstart,QTextCursor::MoveAnchor);
    c.setPosition(bpos+colend,QTextCursor::KeepAnchor);
    text=c.selectedText();
      li=c.blockNumber();
  cols=colstart;
  if (text.length()>1 && colend>colstart) 
    {
    selectword(li,cols,text);
    emit spellme();
    }
  }

}

void LatexEditor::checkSpellingDocument()
{
QTextCursor cur=textCursor();
if (!cur.hasSelection())
{
cur.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
setTextCursor(cur);
}
emit spellme();
}

/*QString LatexEditor::textUnderCursor() const
 {
QTextCursor tc = textCursor();
int oldpos=tc.position();
tc.select(QTextCursor::WordUnderCursor);
int newpos = tc.selectionStart();
tc.setPosition(newpos, QTextCursor::MoveAnchor);
tc.setPosition(oldpos, QTextCursor::KeepAnchor);
QString word=tc.selectedText();
QString prevword="";
QString nextword="";
tc.setPosition(newpos, QTextCursor::MoveAnchor);
tc.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor);
//tc.setPosition(oldpos, QTextCursor::KeepAnchor);
QString sep=tc.selectedText();
if (sep=="{")
    {
    tc.movePosition(QTextCursor::PreviousWord,QTextCursor::MoveAnchor);
    tc.movePosition(QTextCursor::PreviousWord,QTextCursor::MoveAnchor);
    tc.setPosition(oldpos, QTextCursor::KeepAnchor);
    prevword=tc.selectedText();
    prevword=prevword.trimmed();
    }
if ((!prevword.isEmpty()) && (prevword.startsWith("\\"))) {word=prevword;}
//if ((nextword.contains("{")) || (nextword.contains("(")) || (nextword.contains("["))) word=""; 
//if (!nextword.isEmpty()) word=""; 
QString sword=word.trimmed();
if (word.right(1)!=sword.right(1)) word="";
return word;
 }*/

QString LatexEditor::commandUnderCursor() const
{
QString result="";
QTextCursor tc = textCursor();
const QTextBlock block = document()->findBlock(tc.position());
int index=tc.position()-block.position()-1;
if (index<0) return "";
const QString text = block.text();
if (text.length() < 1) return "";
if (index >= text.length()) return "";
int start=index;
int end=index;
QChar	ch = text.at(index);
#define IS_WORD_BACK(ch) (ch.isLetter() || ch.isMark() || ch=='{' || ch=='[')
#define IS_WORD_FORWARD(ch) (ch.isLetter() || ch.isMark() )
bool isControlSeq = false; // becomes true if we include an @ sign or a leading backslash
bool includesApos = false; // becomes true if we include an apostrophe
if (IS_WORD_BACK(ch) || ch == '@' /* || ch == '\'' || ch == 0x2019 */) 
{
if (ch == '@') isControlSeq = true;
while (start > 0) 
  {
  --start;
  ch = text.at(start);
  if (IS_WORD_BACK(ch))
	  continue;
  if (!includesApos && ch == '@') 
    {
    isControlSeq = true;
    continue;
    }
  if (!isControlSeq && (ch == '\'' || ch == 0x2019) && start > 0 && IS_WORD_BACK(text.at(start - 1))) 
    {
    includesApos = true;
    continue;
    }
  ++start;
  break;
  }
if (start > 0 && text.at(start - 1) == '\\') 
  {
  isControlSeq = true;
  --start;
  }
while (++end < text.length()) 
  {
  ch = text.at(end);
  if (IS_WORD_FORWARD(ch))
	  continue;
  if (!includesApos && ch == '@') 
    {
    isControlSeq = true;
    continue;
    }
  if (!isControlSeq && (ch == '\'' || ch == 0x2019) && end < text.length() - 1 && IS_WORD_FORWARD(text.at(end + 1))) 
  {
  includesApos = true;
  continue;
  }
  break;
  }
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return result;
}

if (index > 0 && text.at(index - 1) == '\\') 
  {
  start = index - 1;
  end = index + 1;
  return "";
  }

if (ch.isNumber()) 
  {
  while (start > 0) {
	  --start;
	  ch = text.at(start);
	  if (ch.isNumber())
		  continue;
	  ++start;
	  break;
  }
  while (++end < text.length()) {
	  ch = text.at(end);
	  if (ch.isNumber())
		  continue;
	  break;
  }
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return result;
  }

if (ch == ' ' || ch == '\t') 
  {
  while (start > 0) {
	  --start;
	  ch = text.at(start);
	  if (!(ch == ' ' || ch == '\t')) {
		  ++start;
		  break;
	  }
  }
  while (++end < text.length()) {
	  ch = text.at(end);
	  if (!(ch == ' ' || ch == '\t'))
		  break;
  }
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return result;
  }

if (ch == '\\') 
  {
  if (++end < text.length()) {
	  ch = text.at(end);
	  if (IS_WORD_FORWARD(ch) || ch == '@')
		  while (++end < text.length()) {
			  ch = text.at(end);
			  if (IS_WORD_FORWARD(ch) || ch == '@')
				  continue;
			  break;
		  }
	  else
		  ++end;
  }
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return result;
  }
end = index + 1;
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return result;
}

QStringList LatexEditor::fullcommandUnderCursor()
{
QString result="";
QTextCursor tc = textCursor();
const QTextBlock block = document()->findBlock(tc.position());
int index=tc.position()-block.position()-1;
int start=index;
int end=index;
if (index<0) return (QStringList() << "" << QString::number(index) << QString::number(start) << QString::number(end));
const QString text = block.text();
if (text.length() < 1) return (QStringList() << "" << QString::number(index) << QString::number(start) << QString::number(end));
if (index >= text.length()) return (QStringList() << "" << QString::number(index) << QString::number(start) << QString::number(end));
QChar	ch = text.at(index);
#define IS_WORD_FORMING(ch) (ch.isLetter() || ch.isMark() || ch=='{' || ch=='[' || ch=='}' || ch==']')
//#define IS_WORD_FORMING(ch) (ch.isLetter() || ch.isMark() || ch=='{' || ch=='[' || ch=='}' || ch==']')
bool isControlSeq = false; // becomes true if we include an @ sign or a leading backslash
bool includesApos = false; // becomes true if we include an apostrophe
bool first=true;
if (IS_WORD_FORMING(ch) || ch == '@' /* || ch == '\'' || ch == 0x2019 */) 
{
if (ch == '@') isControlSeq = true;
while (start > 0) 
  {
  --start;
  ch = text.at(start);

  if (IS_WORD_FORMING(ch))
	  continue;
  if (!includesApos && ch == '@') 
    {
    isControlSeq = true;
    continue;
    }
  if (!isControlSeq && (ch == '\'' || ch == 0x2019) && start > 0 && IS_WORD_FORMING(text.at(start - 1))) 
    {
    includesApos = true;
    continue;
    }
  ++start;
  break;
  }
if (start > 0 && text.at(start - 1) == '\\') 
  {
  isControlSeq = true;
  --start;
  }
while (++end < text.length()) 
  {
  ch = text.at(end);

  if (IS_WORD_FORMING(ch))
  {
    if ((first) && (ch=='}' || ch==']')) break;
    first=false;
    continue;
	  
  }
  if (!includesApos && ch == '@') 
    {
    isControlSeq = true;
    continue;
    }
  if (!isControlSeq && (ch == '\'' || ch == 0x2019) && end < text.length() - 1 && IS_WORD_FORMING(text.at(end + 1))) 
  {
  includesApos = true;
  continue;
  }
  break;
  }
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return (QStringList() << result << QString::number(index) << QString::number(start) << QString::number(end));
}

if (index > 0 && text.at(index - 1) == '\\') 
  {
  start = index - 1;
  end = index + 1;
  return (QStringList() << "" << QString::number(index) << QString::number(start) << QString::number(end));;
  }

if (ch.isNumber()) 
  {
  while (start > 0) {
	  --start;
	  ch = text.at(start);
	  if (ch.isNumber())
		  continue;
	  ++start;
	  break;
  }
  while (++end < text.length()) {
	  ch = text.at(end);
	  if (ch.isNumber())
		  continue;
	  break;
  }
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return (QStringList() << result << QString::number(index) << QString::number(start) << QString::number(end));;
  }

if (ch == ' ' || ch == '\t') 
  {
  while (start > 0) {
	  --start;
	  ch = text.at(start);
	  if (!(ch == ' ' || ch == '\t')) {
		  ++start;
		  break;
	  }
  }
  while (++end < text.length()) {
	  ch = text.at(end);
	  if (!(ch == ' ' || ch == '\t'))
		  break;
  }
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return (QStringList() << result << QString::number(index) << QString::number(start) << QString::number(end));
  }

if (ch == '\\') 
  {
  if (++end < text.length()) {
	  ch = text.at(end);
	  if (IS_WORD_FORMING(ch) || ch == '@')
		  while (++end < text.length()) {
			  ch = text.at(end);
			  if (IS_WORD_FORMING(ch) || ch == '@')
				  continue;
			  break;
		  }
	  else
		  ++end;
  }
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return (QStringList() << result << QString::number(index) << QString::number(start) << QString::number(end));
  }
end = index + 1;
tc.setPosition(block.position() + start,QTextCursor::MoveAnchor);
tc.setPosition(block.position() + end, QTextCursor::KeepAnchor);
result=tc.selectedText();
return (QStringList() << result << QString::number(index) << QString::number(start) << QString::number(end));
}


void LatexEditor::keyPressEvent ( QKeyEvent * e ) 
{
if (c && c->popup()->isVisible()) 
	{
	switch (e->key()) 
		{
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Escape:
		case Qt::Key_Tab:
		case Qt::Key_Backtab:
		e->ignore();
		return; 
		default:
		break;
		}
	}
if ( e->key()==Qt::Key_Tab) 
    {
    QTextCursor cursor=textCursor();
    QTextBlock block=cursor.block();
    if (block.isValid()) 
	{
	QString txt=block.text();
	if (txt.contains(QString(0x2022)))
	    {
	    //e->ignore();
	    search(QString(0x2022) ,true,true,true,true);
	    return;		
	    }
	}
    QTextBlock blocknext=block.next();
    if (blocknext.isValid()) 
	{
	QString txtnext=blocknext.text();
	if (txtnext.contains(QString(0x2022)))
	    {
	    //e->ignore();
	    search(QString(0x2022) ,true,true,true,true);
	    return;		
	    }
	}
    QPlainTextEdit::keyPressEvent(e);
    }
else if ( e->key()==Qt::Key_Backtab) 
    {
    QTextCursor cursor=textCursor();
    QTextBlock block=cursor.block();
    if (block.isValid()) 
	{
	QString txt=block.text();
	if (txt.contains(QString(0x2022)))
	    {
	    //e->ignore();
	    search(QString(0x2022) ,true,true,false,true);
	    return;		
	    }
	}
    cursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor);
    if (cursor.selectedText()=="\t") 
	    {
	    cursor.removeSelectedText();
	    }
    }
// if (((e->modifiers() & ~Qt::ShiftModifier) == Qt::ControlModifier) && e->key()==Qt::Key_Tab) 
//   {
//   e->ignore();
//   search(QString(0x2022) ,true,true,true,true);
//   return;
//   }
// if (((e->modifiers() & ~Qt::ShiftModifier) == Qt::ControlModifier) && e->key()==Qt::Key_Backtab) 
//   {
//   e->ignore();
//   search(QString(0x2022) ,true,true,false,true);
//   return;
//   }
// if ((e->key()==Qt::Key_Backtab))
// 	{
// 	QTextCursor cursor=textCursor();
// 	cursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor);
// 	if (cursor.selectedText()=="\t") 
// 			{
// 			cursor.removeSelectedText();
// 			}
// 	}
else if ((e->key()==Qt::Key_Enter)||(e->key()==Qt::Key_Return))
	{
	QPlainTextEdit::keyPressEvent(e);
	QTextCursor cursor=textCursor();
	cursor.joinPreviousEditBlock();
	QTextBlock block=cursor.block();
	QTextBlock blockprev=block.previous();
	if (blockprev.isValid()) 
		{
		QString txt=blockprev.text();
		int j=0;
		while ( (j<txt.count()) && ((txt[j]==' ') || txt[j]=='\t') ) 
			{
			cursor.insertText(QString(txt[j]));
			j++;
			}

		}
	cursor.endEditBlock();
	}
else if (((e->modifiers() & ~Qt::ShiftModifier) == Qt::ControlModifier) && e->key()==Qt::Key_Space) 
  {
  emit requestpdf(textCursor().blockNumber() + 1);
  }
else QPlainTextEdit::keyPressEvent(e);
if (c && !c->popup()->isVisible()) 
	{
	switch (e->key()) 
		{
		case Qt::Key_Down:
		case Qt::Key_Right:
		case Qt::Key_Left:
		case Qt::Key_Up:
		case Qt::Key_PageUp:
		case Qt::Key_PageDown:
		return; 
		default:
		break;
		}
	}
const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
if (!c || (ctrlOrShift && e->text().isEmpty())) {return;}

bool hasModifier = (e->modifiers() & ( Qt::ControlModifier | Qt::AltModifier ));
QString completionPrefix = commandUnderCursor();
if ( completionPrefix.length() < 3)
	{
	c->popup()->hide();
	return;
	}
if (!e->text().isEmpty())
	{
	QChar firstchar=e->text().at(0);
	if ( isWordSeparator(firstchar) || isSpace(firstchar))
		{
		c->popup()->hide();
		return;
		}
	}
if ((c->completionModel()->rowCount()==1) && (c->completionModel()->data(c->completionModel()->index(0,0))==completionPrefix))
	  {
	  c->popup()->hide();
	  return;
	  }
if (completionPrefix != c->completionPrefix()) 
	{
	c->setCompletionPrefix(completionPrefix);
	if ((c->completionModel()->rowCount()==1) && (c->completionModel()->data(c->completionModel()->index(0,0))==completionPrefix))
		  {
		  c->popup()->hide();
		  return;
		  }
	if (completionPrefix.startsWith("\\beg")) c->popup()->setCurrentIndex(c->completionModel()->index(c->completionModel()->rowCount()-1,0));
	else c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
	}
QRect cr = cursorRect();
cr.setWidth(c->popup()->sizeHintForColumn(0)+ c->popup()->verticalScrollBar()->sizeHint().width());
c->complete(cr); 
}

QCompleter *LatexEditor::completer() const
 {
     return c;
 }

void LatexEditor::setCompleter(QCompleter *completer)
{
if (c) QObject::disconnect(c, 0, this, 0);
c = completer;
if (!c) return;
c->setWidget(this);
c->setCompletionMode(QCompleter::PopupCompletion);
c->setCaseSensitivity(Qt::CaseSensitive);
QObject::connect(c, SIGNAL(activated(const QString&)),this, SLOT(insertCompletion(const QString&)));
}

 void LatexEditor::insertCompletion(const QString& completion)
{
if (c->widget() != this) return;
QTextCursor tc = textCursor();
QStringList check=fullcommandUnderCursor();
int t=check.at(1).toInt();
int r=check.at(2).toInt();
int w=check.at(0).size();
tc.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor,t-r+1);
tc.removeSelectedText();
tc.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor,w-t-1+r);
tc.removeSelectedText();
int pos=tc.position();
QString insert_word = completion;
QString env;
QRegExp rbb("begin\\{\\s*([A-Za-z_]+)\\}");
//QRegExp rbb("begin\\{\\s*(.*)\\}");
if (completion.contains(rbb)) 
	{
	tc.insertText(insert_word);
	insertNewLine();
	insertNewLine();
	tc.select(QTextCursor::WordUnderCursor);
	env=rbb.cap(1);
	tc.insertText("\\end{"+env+"}");
	tc.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,1);
	setTextCursor(tc);
	if (insert_word.contains(QString(0x2022)))
	    {
	    tc.movePosition(QTextCursor::Up,QTextCursor::MoveAnchor,1);
	    setTextCursor(tc);
	    search(QString(0x2022) ,true,true,true,true);
	    emit tooltiptab();
	    }

	}
else
	{
	tc.insertText(insert_word);
	tc.setPosition(pos,QTextCursor::MoveAnchor);
	setTextCursor(tc);
	if (!search(QString(0x2022) ,true,true,true,true))
	  {
	  tc.setPosition(pos+completion.length(),QTextCursor::MoveAnchor);
	  setTextCursor(tc);
	  }
	 else emit tooltiptab();
	} 

}

void LatexEditor::insertNewLine()
{
QKeyEvent e(QEvent::KeyPress,Qt::Key_Enter,Qt::NoModifier);
QPlainTextEdit::keyPressEvent(&e);
QTextCursor cursor=textCursor();
cursor.joinPreviousEditBlock();
QTextBlock block=cursor.block();
QTextBlock blockprev=block.previous();
if (blockprev.isValid()) 
	{
	QString txt=blockprev.text();
	int j=0;
	while ( (j<txt.count()) && ((txt[j]==' ') || txt[j]=='\t') ) 
		{
		cursor.insertText(QString(txt[j]));
		j++;
		}
	}
cursor.endEditBlock();  
}

 void LatexEditor::focusInEvent(QFocusEvent *e)
{
if (c) c->setWidget(this);
QPlainTextEdit::focusInEvent(e);
}

 void LatexEditor::setSpellChecker(Hunspell * checker)
{
pChecker = checker;
if (pChecker) spell_encoding=QString(pChecker->get_dic_encoding());
}

void LatexEditor::activateInlineSpell(bool enable)
{
inlinecheckSpelling=enable;
}

void LatexEditor::jumpToPdf()
{
QAction *act = qobject_cast<QAction*>(sender());
if (act != NULL) 
  {
  int i=act->data().toInt();
  emit requestpdf(i);
  }
}

bool LatexEditor::isWordSeparator(QChar c) const
{
    switch (c.toLatin1()) {
    case '.':
    case ',':
    case '?':
    case '!':
    case ':':
    case ';':
    case '-':
    case '<':
    case '>':
//    case '[':
//    case ']':
//    case '(':
//    case ')':
//    case '{':
//    case '}':
    case '=':
    case '/':
    case '+':
    case '%':
    case '&':
    case '^':
    case '*':
    case '\'':
    case '"':
    case '~':
        return true;
    default:
        return false;
    }
}

bool LatexEditor::isSpace(QChar c) const
{
    return c == QLatin1Char(' ')
        || c == QChar::Nbsp
        || c == QChar::LineSeparator
        || c == QLatin1Char('\t')
        ;
}

void LatexEditor::matchAll() 
{
viewport()->update();
QList<QTextEdit::ExtraSelection> selections;
setExtraSelections(selections);
matchPar();
matchLat();
if (foldableLines.keys().contains(textCursor().block().blockNumber())) emit requestGotoStructure(textCursor().block().blockNumber());
}

void LatexEditor::matchPar() 
{

//QList<QTextEdit::ExtraSelection> selections;
//setExtraSelections(selections);
QTextBlock textBlock = textCursor().block();
BlockData *data = static_cast<BlockData *>( textBlock.userData() );
if ( data ) {
	QVector<ParenthesisInfo *> infos = data->parentheses();
	int pos = textCursor().block().position();

	for ( int i=0; i<infos.size(); ++i ) {
		ParenthesisInfo *info = infos.at(i);
		int curPos = textCursor().position() - textBlock.position();
		// Clicked on a left parenthesis?
		if ( info->position == curPos-1 && info->character == '{' ) {
			if ( matchLeftPar( textBlock, i+1, 0 ) )
				createParSelection( pos + info->position );
		}

		// Clicked on a right parenthesis?
		if ( info->position == curPos-1 && info->character == '}' ) {
			if ( matchRightPar( textBlock, i-1, 0 ) )
				createParSelection( pos + info->position );
		}
	}
}
}

bool LatexEditor::matchLeftPar(	QTextBlock currentBlock, int index, int numLeftPar ) 
{

BlockData *data = static_cast<BlockData *>( currentBlock.userData() );
QVector<ParenthesisInfo *> infos = data->parentheses();
int docPos = currentBlock.position();

// Match in same line?
for ( ; index<infos.size(); ++index ) {
	ParenthesisInfo *info = infos.at(index);

	if ( info->character == '{' ) {
		++numLeftPar;
		continue;
	}

	if ( info->character == '}' && numLeftPar == 0 ) {
		createParSelection( docPos + info->position );
		return true;
	} else
		--numLeftPar;
}

// No match yet? Then try next block
currentBlock = currentBlock.next();
if ( currentBlock.isValid() )
	return matchLeftPar( currentBlock, 0, numLeftPar );

// No match at all
return false;
}

bool LatexEditor::matchRightPar(QTextBlock currentBlock, int index, int numRightPar) 
{

BlockData *data = static_cast<BlockData *>( currentBlock.userData() );
QVector<ParenthesisInfo *> infos = data->parentheses();
int docPos = currentBlock.position();

// Match in same line?
for (int j=index; j>=0; --j ) {
	ParenthesisInfo *info = infos.at(j);

	if ( info->character == '}' ) {
		++numRightPar;
		continue;
	}

	if ( info->character == '{' && numRightPar == 0 ) {
		createParSelection( docPos + info->position );
		return true;
	} else
		--numRightPar;
}

// No match yet? Then try previous block
currentBlock = currentBlock.previous();
if ( currentBlock.isValid() ) {

	// Recalculate correct index first
	BlockData *data = static_cast<BlockData *>( currentBlock.userData() );
	QVector<ParenthesisInfo *> infos = data->parentheses();

	return matchRightPar( currentBlock, infos.size()-1, numRightPar );
}

// No match at all
return false;
}

void LatexEditor::createParSelection( int pos ) 
{
QList<QTextEdit::ExtraSelection> selections = extraSelections();
QTextEdit::ExtraSelection selection;
QTextCharFormat format = selection.format;
format.setBackground( QColor("#FFFF99") );
format.setForeground( QColor("#FF0000") );
selection.format = format;

QTextCursor cursor = textCursor();
cursor.setPosition( pos );
cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
selection.cursor = cursor;
selections.append( selection );
setExtraSelections( selections );
}

/********************************************************/
void LatexEditor::matchLat() 
{
//QList<QTextEdit::ExtraSelection> selections;
//setExtraSelections(selections);

QTextBlock textBlock = textCursor().block();
if (foldableLines.keys().contains(textBlock.blockNumber())) createLatSelection(textBlock.blockNumber(),foldableLines[textBlock.blockNumber()]);
else
  {
  BlockData *data = static_cast<BlockData *>( textBlock.userData() );
  if ( data ) 
    {
    QVector<LatexBlockInfo *> infos = data->latexblocks();
    int pos = textCursor().block().position();
    if (infos.size()==0) 
      {
      emit setBlockRange(-1,-1);
      endBlock=-1;
      }
    for ( int i=0; i<infos.size(); ++i ) 
      {
      LatexBlockInfo *info = infos.at(i);
      int curPos = textCursor().position() - textBlock.position();
      if ( info->position <= curPos && info->character == 'b' ) matchLeftLat( textBlock, i+1, 0, textBlock.blockNumber());
      if ( info->position <= curPos && info->character == 'e' ) matchRightLat( textBlock, i-1, 0,textBlock.blockNumber());
      }
    }
  }

}

bool LatexEditor::matchLeftLat(	QTextBlock currentBlock, int index, int numLeftLat, int bpos ) 
{

BlockData *data = static_cast<BlockData *>( currentBlock.userData() );
QVector<LatexBlockInfo *> infos = data->latexblocks();
int docPos = currentBlock.position();

// Match in same line?
for ( ; index<infos.size(); ++index ) {
	LatexBlockInfo *info = infos.at(index);

	if ( info->character == 'b' ) {
		++numLeftLat;
		continue;
	}

	if ( info->character == 'e' && numLeftLat == 0 ) {
		createLatSelection( bpos,currentBlock.blockNumber() );
		return true;
	} else
		--numLeftLat;
}

// No match yet? Then try next block
currentBlock = currentBlock.next();
if ( currentBlock.isValid() )
	return matchLeftLat( currentBlock, 0, numLeftLat, bpos );

// No match at all
return false;
}

bool LatexEditor::matchRightLat(QTextBlock currentBlock, int index, int numRightLat, int epos) 
{

BlockData *data = static_cast<BlockData *>( currentBlock.userData() );
QVector<LatexBlockInfo *> infos = data->latexblocks();
int docPos = currentBlock.position();

// Match in same line?
for (int j=index; j>=0; --j ) {
	LatexBlockInfo *info = infos.at(j);

	if ( info->character == 'e' ) {
		++numRightLat;
		continue;
	}

	if ( info->character == 'b' && numRightLat == 0 ) {
		createLatSelection( epos, currentBlock.blockNumber() );
		return true;
	} else
		--numRightLat;
}

// No match yet? Then try previous block
currentBlock = currentBlock.previous();
if ( currentBlock.isValid() ) {

	// Recalculate correct index first
	BlockData *data = static_cast<BlockData *>( currentBlock.userData() );
	QVector<LatexBlockInfo *> infos = data->latexblocks();

	return matchRightLat( currentBlock, infos.size()-1, numRightLat, epos );
}

// No match at all
return false;
}

void LatexEditor::createLatSelection( int start, int end ) 
{
int s=qMin(start,end);
int e=qMax(start,end);
emit setBlockRange(s,e);
endBlock=e;
}

void LatexEditor::jumpToEndBlock()
{
gotoLine(endBlock);
QTextCursor c = textCursor();
c.movePosition(QTextCursor::EndOfBlock);
setTextCursor(c);
}

void LatexEditor::fold(int start, int end)
{
foldedLines.insert(start, end);
ensureFinalNewLine();//Qt 4.7.1 bug
for (int i = start + 1; i <= end; i++) {document()->findBlockByNumber(i).setVisible(false);}
update();
resizeEvent(new QResizeEvent(QSize(0, 0), size()));
viewport()->update();
emit updatelineWidget();
ensureCursorVisible();
}
 
void LatexEditor::unfold(int start, int end)
{
if (!foldedLines.keys().contains(start)) return;
foldedLines.remove(start);
int i=start+1;
while (i<=end)
{
if (foldedLines.keys().contains(i)) 
  {
  document()->findBlockByNumber(i).setVisible(true);
  i=foldedLines[i]; 
  }
else document()->findBlockByNumber(i).setVisible(true);
i++;
}
update();
resizeEvent(new QResizeEvent(QSize(0, 0), size()));
viewport()->update();
emit updatelineWidget();
ensureCursorVisible();
}

void LatexEditor::removeStructureItem(int offset,int len, int line)
{
bool r=false;
for ( int j=StructItemsList.count()-1;j>=0; --j ) 
  {
  //if (StructItemsList[j].cursor.selectionStart() < offset) break;
  //if (StructItemsList[j].cursor.selectionStart() < offset+len)
  //if (StructItemsList[j].cursor.position() < offset) break;
  //if (StructItemsList[j].cursor.position() < offset+len)
    if ( StructItemsList[j].cursor.block().blockNumber()==line)
    {
      //qDebug() << "remove" << StructItemsList[j].item << StructItemsList[j].cursor.selectionStart() << offset << len;
      //qDebug() << "remove" << StructItemsList[j].item << StructItemsList[j].cursor.block().blockNumber() << line;
      StructItemsList.removeAt(j);
    r=true;
    }
  }
if (r) {emit requestUpdateStructure();}
}

void LatexEditor::appendStructureItem(int line,QString item,int type,const QTextCursor& cursor)
{
int j = 0;
while (j < StructItemsList.count()) 
  {
  if (StructItemsList[j].cursor.position() > cursor.position()) break;
  //if (StructItemsList[j].cursor.selectionStart() > cursor.selectionStart()) break;
  ++j;
  }
StructItemsList.insert(j,StructItem(line,item,type,cursor));
//qDebug() << "add" << StructItemsList[j].item;
emit requestUpdateStructure();
}

void LatexEditor::ensureFinalNewLine()//Qt 4.7.1 bug
{
QTextCursor cursor = textCursor();
cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
bool emptyFile = !cursor.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor);
if (!emptyFile && cursor.selectedText().at(0) != QChar::ParagraphSeparator)
    {
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.insertText(QLatin1String("\n"));
    }
}

int LatexEditor::getLastNumLines()
{
return lastnumlines;
}

void LatexEditor::setLastNumLines(int n)
{
lastnumlines=n;  
}

void LatexEditor::setHightLightLine()
{
highlightRemover.stop();
highlightLine=true;
update();
highlightRemover.start(1000);
}

void LatexEditor::clearHightLightLine()
{
highlightLine=false;
emit cursorPositionChanged();
update();
}

QDateTime LatexEditor::getLastSavedTime()
{
return lastSavedTime;  
}

void LatexEditor::setLastSavedTime(QDateTime t)
{
lastSavedTime=t;  
}

QString LatexEditor::beginningLine()
{
QString result="";
QTextCursor cursor=textCursor();
QTextBlock block=cursor.block();
if (block.isValid()) 
	{
	QString txt=block.text();
	int j=0;
	while ( (j<txt.count()) && ((txt[j]==' ') || txt[j]=='\t') ) 
		{
		result+=QString(txt[j]);
		j++;
		}

	}
return result;
}
/*const QRectF LatexEditor::blockGeometry(const QTextBlock & block) 
{
qDebug() << "ok1" << block.isValid() << block.isVisible();
//if (block.isVisible()) return blockBoundingGeometry(block).translated(contentOffset());
//else return QRectF();
return blockBoundingGeometry(block).translated(contentOffset());
//const QRectF rec=blockBoundingGeometry(block);
//qDebug() << "ok2";
//if (rec.isValid()) return rec.translated(contentOffset());
//else return QRectF();
}*/
