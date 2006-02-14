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

#include "configdialog.h"

ConfigDialog::ConfigDialog(QWidget* parent): QDialog( parent)
{
setModal(true);
ui.setupUi(this);
ui.contentsWidget->setIconSize(QSize(96, 96));
ui.contentsWidget->setViewMode(QListView::IconMode);
ui.contentsWidget->setMovement(QListView::Static);

pt=new PageTools(ui.pagesWidget);
pq=new PageQuick(ui.pagesWidget);
pe=new PageEditor(ui.pagesWidget);


ui.pagesWidget->addWidget(pt);
ui.pagesWidget->addWidget(pq);
ui.pagesWidget->addWidget(pe);


createIcons();
ui.contentsWidget->setCurrentRow(0);
}

void ConfigDialog::createIcons()
{
QListWidgetItem *commandButton = new QListWidgetItem(ui.contentsWidget);
commandButton->setIcon(QIcon(":/images/configtools.png"));
commandButton->setText(tr("Commands"));
commandButton->setTextAlignment(Qt::AlignHCenter);
commandButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

QListWidgetItem *quickButton = new QListWidgetItem(ui.contentsWidget);
quickButton->setIcon(QIcon(":/images/configquick.png"));
quickButton->setText(tr("Quick Build"));
quickButton->setTextAlignment(Qt::AlignHCenter);
quickButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

QListWidgetItem *editorButton = new QListWidgetItem(ui.contentsWidget);
editorButton->setIcon(QIcon(":/images/configeditor.png"));
editorButton->setText(tr("Editor"));
editorButton->setTextAlignment(Qt::AlignHCenter);
editorButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

connect(ui.contentsWidget,
	SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
	this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    ui.pagesWidget->setCurrentIndex(ui.contentsWidget->row(current));
}
