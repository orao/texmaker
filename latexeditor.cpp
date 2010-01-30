/***************************************************************************
 *   copyright       : (C) 2003-2009 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *   addons by Luis Silvestre                                              *
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

LatexEditor::LatexEditor(QWidget *parent,QFont & efont, QColor colMath, QColor colCommand, QColor colKeyword,bool inlinespelling,QString ignoredWords,Hunspell *spellChecker) : QTextEdit(parent),c(0)
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
setTabChangesFocus(false);

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
highlighter->setColors(colMath,colCommand,colKeyword);

//c=0;
connect(this, SIGNAL(cursorPositionChanged()), viewport(), SLOT(update()));
 matcher = new ParenMatcher;
 connect(this, SIGNAL(cursorPositionChanged()), matcher, SLOT(matchFromSender()));
//grabShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Tab), Qt::WidgetShortcut);
setFocus();
}
LatexEditor::~LatexEditor(){
delete matcher;
//delete pChecker;
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
/*******************************************/
if (inlinecheckSpelling && pChecker)
      {
      QFont spellmenufont (qApp->font());
      spellmenufont.setBold(true);
      QTextCursor c = cursorForPosition(e->pos());
      BlockData* data;
      QTextCodec *codec = QTextCodec::codecForName(spell_encoding.toLatin1());
      QByteArray encodedString;
      QTextBlock block;
      QString text,word;
      bool gonext=true;
      QByteArray t;
      int li,cols,cole,colstart,colend,check,ns;
      char ** wlst;
      QStringList suggWords;
      //c.movePosition(QTextCursor::NextCharacter,QTextCursor::MoveAnchor);
      c.movePosition(QTextCursor::StartOfWord,QTextCursor::MoveAnchor);
      data = (BlockData*)c.block().userData();
      li=c.blockNumber();
      block=c.block();
      colstart=c.position()-block.position();
      c.movePosition(QTextCursor::EndOfWord,QTextCursor::KeepAnchor);
      block=c.block();
      colend=c.position()-block.position()-1;
      cols=colstart;
      text=c.selectedText();
      while ((cols<colend && cols<data->code.count() && data->code[cols]==1) || text.mid(cols-colstart,1)==" " || text.mid(cols-colstart,1)=="\t" )
	      {
	      cols++;
	      }
      cole=colend;
      while (cole>colstart && cole<data->code.count() && data->code[cole]==1)
	      {
	      cole--;
	      }
      if (text.length()>1 && cole>cols)
	      {
	      word=text.mid(cols-colstart,cole-cols+1);
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
a = menu->addAction(tr("Check Spelling Word"), this, SLOT(checkSpellingWord()));
a->setEnabled(!document()->isEmpty());
a = menu->addAction(tr("Check Spelling Selection"), this, SLOT(checkSpellingDocument()));
a->setEnabled(textCursor().hasSelection());
a = menu->addAction(tr("Check Spelling Document"), this, SLOT(checkSpellingDocument()));
a->setEnabled(!document()->isEmpty() && !textCursor().hasSelection());
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
QTextCursor cur=textCursor();
cur.select(QTextCursor::WordUnderCursor);
setTextCursor(cur);
if (cur.hasSelection()) emit spellme();
}

void LatexEditor::checkSpellingDocument()
{
emit spellme();
}

QString LatexEditor::textUnderCursor() const
 {
QTextCursor tc = textCursor();
int oldpos=tc.position();
tc.select(QTextCursor::WordUnderCursor);
int newpos = tc.selectionStart();
tc.setPosition(newpos, QTextCursor::MoveAnchor);
tc.setPosition(oldpos, QTextCursor::KeepAnchor);
QString word=tc.selectedText();
QString sword=word.trimmed();
if (word.right(1)!=sword.right(1)) word="";
return word;
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
    QTextEdit::keyPressEvent(e);
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
	QTextEdit::keyPressEvent(e);
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
else QTextEdit::keyPressEvent(e);

const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
if (!c || (ctrlOrShift && e->text().isEmpty())) return;

bool hasModifier = (e->modifiers() & ( Qt::ControlModifier | Qt::AltModifier ));
QString completionPrefix = textUnderCursor();

if (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3)
	{
	c->popup()->hide();
	return;
	}

QChar firstchar=e->text().at(0);
if ( isWordSeparator(firstchar) || isSpace(firstchar))
	{
	c->popup()->hide();
	return;
	}

if (completionPrefix != c->completionPrefix()) 
	{
	c->setCompletionPrefix(completionPrefix);
	c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
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
tc.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor, textUnderCursor().size());
tc.removeSelectedText();
int pos=tc.position();
QString insert_word = completion;
QRegExp rbb("begin\\{\\s*([A-Za-z_]+)\\}");
if (completion.contains(rbb)) 
	{
	tc.insertText(insert_word);
	insertNewLine();
	insertNewLine();
	tc.insertText("\\end{"+rbb.cap(1)+"}");
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
QTextEdit::keyPressEvent(&e);
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
QTextEdit::focusInEvent(e);
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
    case '[':
    case ']':
    case '(':
    case ')':
    case '{':
    case '}':
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
