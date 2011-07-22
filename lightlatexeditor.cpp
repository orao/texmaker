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

#include "lightlatexeditor.h"

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
#include <QTextCodec>
#include <QFileInfo>
#include <QMessageBox>
#include "blockdata.h"
#include "encodingdialog.h"

LightLatexEditor::LightLatexEditor(QWidget *parent,QFont & efont, QColor colMath, QColor colCommand, QColor colKeyword) : QPlainTextEdit(parent)
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
lastnumlines=0;

highlighter = new LightLatexHighlighter(document());
highlighter->SetEditor(this);
highlighter->setColors(colMath,colCommand,colKeyword);

connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(matchAll()));

setReadOnly(true);
setEnabled(true);
setFocusPolicy(Qt::WheelFocus);
setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
setFocus();

}
LightLatexEditor::~LightLatexEditor(){
}

bool LightLatexEditor::search( const QString &expr, bool cs, bool wo, bool forward, bool startAtCursor )
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

int LightLatexEditor::searchLine( const QString &expr)
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

void LightLatexEditor::gotoLine( int line )
{
if (line<=numoflines())  
  {
  setCursorPosition( line, 0 );
  }
}


void LightLatexEditor::changeFont(QFont & new_font)
{
setFont(new_font);
}

QString LightLatexEditor::getEncoding()
{
 return encoding;
}

void LightLatexEditor::setEncoding(QString enc)
{
 encoding=enc;
}  

int LightLatexEditor::getCursorPosition(int para, int index)
{
return document()->findBlockByNumber(para).position()+index;
}

void LightLatexEditor::setCursorPosition(int para, int index)
{
int pos=getCursorPosition(para,index);
QTextCursor cur=textCursor();
cur.setPosition(pos,QTextCursor::MoveAnchor);
if (cur.block().isVisible()) setTextCursor(cur);
ensureCursorVisible();
setFocus();
}



int LightLatexEditor::numoflines()
{
return document()->blockCount();
}

int LightLatexEditor::linefromblock(const QTextBlock& p)
{
return p.blockNumber()+1;
}

void LightLatexEditor::selectword(int line, int col, QString word)
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

void LightLatexEditor::insertNewLine()
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


void LightLatexEditor::matchAll() 
{
viewport()->update();
QList<QTextEdit::ExtraSelection> selections;
setExtraSelections(selections);
matchPar();
emit updatelineWidget();
}

void LightLatexEditor::matchPar() 
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

bool LightLatexEditor::matchLeftPar(	QTextBlock currentBlock, int index, int numLeftPar ) 
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

bool LightLatexEditor::matchRightPar(QTextBlock currentBlock, int index, int numRightPar) 
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

void LightLatexEditor::createParSelection( int pos ) 
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

void LightLatexEditor::jumpToEndBlock()
{
gotoLine(endBlock);
QTextCursor c = textCursor();
c.movePosition(QTextCursor::EndOfBlock);
setTextCursor(c);
}

void LightLatexEditor::ensureFinalNewLine()//Qt 4.7.1 bug
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

int LightLatexEditor::getLastNumLines()
{
return lastnumlines;
}

void LightLatexEditor::setLastNumLines(int n)
{
lastnumlines=n;  
}

QString LightLatexEditor::beginningLine()
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

void LightLatexEditor::load( const QString &f )
{
QFile file( f );
if ( !file.open( QIODevice::ReadOnly ) )
	{
	QMessageBox::warning( this,tr("Error"), tr("You do not have read permission to this file."));
	return;
	}
bool hasDecodingError=false;
QByteArray buf = file.readAll();
int bytesRead = buf.size();
file.close();
QTextCodec* codec = QTextCodec::codecForName(encoding.toLatin1());
if(!codec) codec = QTextCodec::codecForLocale();
#if 0 // should work, but does not, Qt bug with "system" codec
QTextDecoder *decoder = codec->makeDecoder();
QString text = decoder->toUnicode(buf);
hasDecodingError = (decoder->hasFailure());
delete decoder;
#else
QString text = codec->toUnicode(buf);
QByteArray verifyBuf = codec->fromUnicode(text); // slow
// the minSize trick lets us ignore unicode headers
int minSize = qMin(verifyBuf.size(), buf.size());
hasDecodingError = (minSize < buf.size()- 4 || memcmp(verifyBuf.constData() + verifyBuf.size() - minSize,buf.constData() + buf.size() - minSize, minSize));
#endif
QString new_encoding;
QEncodingProber prober (QEncodingProber::Universal);
if (hasDecodingError)
  {
  prober.feed (buf.constData());
  QTextCodec* detected_codec;
  if (prober.confidence() > 0.5) //Kencodingprober works very bad with tex documents
    {
    detected_codec = QTextCodec::codecForName(prober.encoding());
    if (detected_codec) new_encoding=detected_codec->name();
    else if (encoding=="UTF-8") new_encoding="ISO-8859-1";
    else if (encoding=="ISO-8859-1") new_encoding="UTF-8";
    else new_encoding=QString(QTextCodec::codecForLocale()->name());
    }
  else if (encoding=="UTF-8") new_encoding="ISO-8859-1";
  else if (encoding=="ISO-8859-1") new_encoding="UTF-8";
  else new_encoding=QString(QTextCodec::codecForLocale()->name());
  EncodingDialog *encDlg = new EncodingDialog(this);
  encDlg->ui.comboBoxEncoding->setCurrentIndex(encDlg->ui.comboBoxEncoding->findText(new_encoding, Qt::MatchExactly));
  encDlg->ui.label->setText(encDlg->ui.label->text()+ " ("+encoding+").");
  if (encDlg->exec())
	  {
	  new_encoding=encDlg->ui.comboBoxEncoding->currentText();
	  codec = QTextCodec::codecForName(new_encoding.toLatin1());
	  text = codec->toUnicode(buf);
	  }
  else return;
  }
if (hasDecodingError) setEncoding(new_encoding);
else setEncoding(encoding);
setPlainText(text);
}

void LightLatexEditor::paintEvent(QPaintEvent *event)
{
QRect rect = cursorRect();
QRect rectbis=rect;
rectbis.setX(0);
rectbis.setWidth(viewport()->width());
QPainter painter(viewport());
const QBrush brush(QColor("#000000"));
const QBrush brushbis(QColor("#ececec"));
painter.fillRect(rectbis, brushbis);
painter.fillRect(rect, brush);
painter.end();
QPlainTextEdit::paintEvent(event);
}

void LightLatexEditor::contextMenuEvent(QContextMenuEvent *e)
{
QMenu *menu=createStandardContextMenu();
menu->addSeparator();
QAction *Act;
Act = new QAction(tr("Click to jump to the bookmark")+"1", this);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoBookmark1()));
menu->addAction(Act);
Act = new QAction(tr("Click to jump to the bookmark")+"2", this);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoBookmark2()));
menu->addAction(Act);
Act = new QAction(tr("Click to jump to the bookmark")+"3", this);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoBookmark3()));
menu->addAction(Act);
menu->addSeparator();
Act = new QAction( tr("Find"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(editFind()));
menu->addAction(Act);
Act = new QAction(tr("Goto Line"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(editGotoLine()));
menu->addAction(Act);
menu->exec(e->globalPos());
delete menu;
}

void LightLatexEditor::gotoBookmark1()
{
int l=UserBookmark[0];
if (l>0) gotoLine(l-1);
}

void LightLatexEditor::gotoBookmark2()
{
int l=UserBookmark[1];
if (l>0) gotoLine(l-1);
}

void LightLatexEditor::gotoBookmark3()
{
int l=UserBookmark[2];
if (l>0) gotoLine(l-1);
}

void LightLatexEditor::editFind()
{
emit requestFind();
}

void LightLatexEditor::editGotoLine()
{
emit requestGotoLine();
}
