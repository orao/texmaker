/***************************************************************************
 *   copyright       : (C) 2003-2011 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "replacewidget.h"
#include <QMessageBox>

ReplaceWidget::ReplaceWidget(QWidget* parent)
    : QWidget( parent)
{
ui.setupUi(this);
connect( ui.findButton, SIGNAL( clicked() ), this, SLOT( doReplace() ) );
connect( ui.replaceallButton, SIGNAL( clicked() ), this, SLOT( doReplaceAll() ) );
connect( ui.closeButton, SIGNAL( clicked() ), this, SLOT( doHide() ) );
ui.findButton->setShortcut(Qt::Key_Return);
ui.findButton->setToolTip("Return");
ui.closeButton->setShortcut(Qt::Key_Escape);
ui.closeButton->setToolTip("Escape");
ui.moreButton->setCheckable(true);
ui.moreButton->setAutoDefault(false);
connect(ui.moreButton, SIGNAL(toggled(bool)), this, SLOT(expand(bool)));
ui.checkRegExp->setChecked( false );
ui.extension->hide();
updateGeometry();
}


ReplaceWidget::~ReplaceWidget()
{
}

void ReplaceWidget::doReplace()
{
doHide();
if ( !editor ) return;
if (ui.checkRegExp->isChecked())
  {
  QRegExp regex(ui.comboFind->currentText());
  if (!regex.isValid()) 
    {
    QMessageBox::warning( this,tr("Error"), tr("Invalid regular expression."));
    return;
    }
  }
bool go=true;
while (go && editor->search( ui.comboFind->currentText(), ui.checkCase->isChecked(),
	ui.checkWords->isChecked(), ui.radioForward->isChecked(), !ui.checkBegin->isChecked(),ui.checkRegExp->isChecked()) )
       {
       switch(  QMessageBox::warning(this, "Texmaker",tr("Replace this occurence ? "),tr("Yes"), tr("No"), tr("Cancel"), 0,2 ) )
         {
         case 0:
         editor->replace(ui.comboReplace->currentText(),ui.checkRegExp->isChecked(),ui.comboFind->currentText() );
         ui.checkBegin->setChecked( false );
    	   break;
         case 1:
         ui.checkBegin->setChecked( false );
    	   break;
         case 2:
         go=false;
    	   break;
         }
       }
if (go) ui.checkBegin->setChecked( true );
}

void ReplaceWidget::doReplaceAll()
{
if ( !editor ) return;
if (ui.checkRegExp->isChecked())
  {
  QRegExp regex(ui.comboFind->currentText());
  if (!regex.isValid()) 
    {
    QMessageBox::warning( this,tr("Error"), tr("Invalid regular expression."));
    return;
    }
  }
while ( editor->search( ui.comboFind->currentText(), ui.checkCase->isChecked(),
ui.checkWords->isChecked(), ui.radioForward->isChecked(), !ui.checkBegin->isChecked(),ui.checkRegExp->isChecked()) )
    {
    editor->replace(ui.comboReplace->currentText(),ui.checkRegExp->isChecked(),ui.comboFind->currentText() );
    ui.checkBegin->setChecked( false );
    }
ui.checkBegin->setChecked( true );
}

void ReplaceWidget::SetEditor(LatexEditor *ed)
{
editor=ed;
}

void ReplaceWidget::doHide()
{
emit requestHide();
if ( editor ) 
	{
	editor->viewport()->repaint();
	editor->setFocus();
	}
}

void ReplaceWidget::expand(bool e)
{
 ui.extension->setVisible(e);
 if (!e) ui.checkRegExp->setChecked( false );
 updateGeometry();
 emit requestExtension();
 editor->viewport()->repaint();
}

