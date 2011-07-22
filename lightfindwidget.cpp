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

#include "lightfindwidget.h"

LightFindWidget::LightFindWidget(QWidget* parent)
    : QWidget( parent)
{
ui.setupUi(this);
connect(ui.findButton, SIGNAL( clicked() ), this, SLOT( doFind() ) );
connect(ui.closeButton, SIGNAL( clicked() ), this, SLOT( doHide() ) );
ui.findButton->setShortcut(Qt::Key_Return);
ui.findButton->setToolTip("Return");
ui.closeButton->setShortcut(Qt::Key_Escape);
ui.closeButton->setToolTip("Escape");
 }

LightFindWidget::~LightFindWidget()
{

}

void LightFindWidget::doFind()
{
if ( !editor ) return;
if ( !editor->search( ui.comboFind->currentText(), ui.checkCase->isChecked(),	ui.checkWords->isChecked(), ui.radioForward->isChecked(), !ui.checkBegin->isChecked() ) )
   {
   ui.checkBegin->setChecked( TRUE );
   }
else ui.checkBegin->setChecked( FALSE );
editor->viewport()->repaint();
}

void LightFindWidget::doHide()
{
emit requestHide();
if ( editor ) 
	{
	editor->viewport()->repaint();
	editor->setFocus();
	}
}

void LightFindWidget::SetEditor(LightLatexEditor *ed)
{
editor=ed;
}

