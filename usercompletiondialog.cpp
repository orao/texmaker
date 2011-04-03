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

#include <QDebug>

#include "usercompletiondialog.h"

UserCompletionDialog::UserCompletionDialog(QWidget *parent,QStringList extraCompletion)
    :QDialog( parent)
{
ui.setupUi(this);

connect(ui.addButton, SIGNAL( clicked() ), this, SLOT( actionAddCommand() ) );
connect(ui.deleteButton, SIGNAL( clicked() ), this, SLOT( actionDelete() ) );

ui.listWidget->clear();
ui.listWidget->setAlternatingRowColors(true);
userlist=extraCompletion;
ui.listWidget->addItems(userlist);
}

UserCompletionDialog::~UserCompletionDialog(){
}





void UserCompletionDialog::actionDelete()
{
int current=ui.listWidget->currentRow();
if (current<0) return;
delete ui.listWidget->currentItem();
userlist.removeAt(current);
if (current==ui.listWidget->count())
	{
	ui.listWidget->setCurrentItem(ui.listWidget->item(current-1));
	ui.listWidget->setItemSelected(ui.listWidget->item(current-1),true);
	}
else
	{
	ui.listWidget->setCurrentItem(ui.listWidget->item(current));
	ui.listWidget->setItemSelected(ui.listWidget->item(current),true);
	}
}

void UserCompletionDialog::actionAddCommand()
{
QString newcommand=ui.lineEdit->text();
newcommand.replace("@",QString(0x2022));
ui.listWidget->addItem(newcommand);
userlist.append(newcommand);
if (ui.listWidget->count()>0)
	{
	ui.listWidget->setCurrentItem(ui.listWidget->item(ui.listWidget->count()-1));
	ui.listWidget->setItemSelected(ui.listWidget->currentItem(), true);
	}
}




