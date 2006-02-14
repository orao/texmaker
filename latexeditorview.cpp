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

#include "latexeditorview.h"
#include <qlayout.h>
#include <QHBoxLayout>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextEdit>
#include <QTextBlock>

LatexEditorView::LatexEditorView(QWidget *parent,QFont & efont,bool line) : QWidget(parent)
{
  editor=new LatexEditor(this,efont);
  m_lineNumberWidget = new LineNumberWidget( editor, this);
  m_lineNumberWidget->setFont(efont);
  QFontMetrics fm( efont );
  m_lineNumberWidget->setFixedWidth( fm.width( "000000" ) + 10 );
  QHBoxLayout* lay = new QHBoxLayout( this );
  lay->addWidget( m_lineNumberWidget );
  lay->addWidget( editor );
  setFocusProxy( editor );
  setLineNumberWidgetVisible(line);
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
  m_lineNumberWidget->setFixedWidth( fm.width( "000000" ) + 10 );
  setLineNumberWidgetVisible(line);
}
