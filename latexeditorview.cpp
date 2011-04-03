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

#include "latexeditorview.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QTextDocument>
#include <QTextCursor>
//#include <QTextEdit>
#include <QTextBlock>

LatexEditorView::LatexEditorView(QWidget *parent,QFont & efont,bool line, QColor colMath, QColor colCommand, QColor colKeyword,bool inlinespelling,QString ignoredWords,Hunspell *spellChecker) : QWidget(parent)
{
  
splitter=new MiniSplitter(this);
splitter->setOrientation(Qt::Vertical);

QFrame *framebis=new QFrame(splitter);
framebis->setLineWidth(0);
framebis->setFrameShape(QFrame::NoFrame);
framebis->setFrameShadow(QFrame::Plain);
framebis->setFrameStyle(QFrame::NoFrame);


QVBoxLayout* mainlay = new QVBoxLayout(framebis );
mainlay->setSpacing(0);
mainlay->setMargin(0);


QFrame *frame=new QFrame(framebis);
frame->setLineWidth(1);
frame->setFrameShape(QFrame::NoFrame);
frame->setFrameShadow(QFrame::Plain);
frame->setFrameStyle(QFrame::NoFrame);
mainlay->addWidget(frame);

editor=new LatexEditor(frame,efont,colMath,colCommand,colKeyword,inlinespelling,ignoredWords,spellChecker);
m_lineNumberWidget = new LineNumberWidget( editor, frame);
m_lineNumberWidget->setFont(efont);
QFontMetrics fm( efont );
m_lineNumberWidget->setFixedWidth( fm.width( "00000" ) + 32 );
QHBoxLayout* lay = new QHBoxLayout( frame );
lay->setSpacing(0);
lay->setMargin(0);
lay->addWidget( m_lineNumberWidget );
lay->addWidget( editor );
setFocusProxy( editor );
setLineNumberWidgetVisible(line);

findwidget=new FindWidget(splitter);
mainlay->addWidget(findwidget);
findwidget->SetEditor(editor);
findwidget->hide();

splitter->addWidget(framebis);
splitter->addWidget(findwidget);
QVBoxLayout *mainlayout= new QVBoxLayout(this);
mainlayout->setSpacing(0);
mainlayout->setMargin(0);
mainlayout->addWidget(splitter);
QList<int> sizes;
sizes << findwidget->width() << height()-findwidget->width();
splitter->setSizes( sizes );
}

LatexEditorView::~LatexEditorView()
{
}

void LatexEditorView::setLineNumberWidgetVisible( bool b )
{
    if( b ){
	m_lineNumberWidget->show();
    } else {
	m_lineNumberWidget->hide();
    }
}

void LatexEditorView::changeSettings(QFont & new_font,bool line)
{
  editor->changeFont(new_font);
  m_lineNumberWidget->setFont(new_font);
  QFontMetrics fm( new_font );
  m_lineNumberWidget->setFixedWidth( fm.width( "00000" ) + 32 );
  setLineNumberWidgetVisible(line);
}

