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

#include <QtGui>

#include "pageeditor.h"

#include <QFontDatabase>
#include <QTextCodec>
#include <QFileDialog>
#include <QColorDialog>
#include <QDir>

PageEditor::PageEditor(QWidget *parent)
    : QWidget(parent)
{
ui.setupUi(this);

QFontDatabase fdb;
ui.comboBoxFont->addItems( fdb.families() );

ui.comboBoxEncoding->addItem("UTF-8");
foreach (int mib, QTextCodec::availableMibs()) 
	{
	QTextCodec *codec = QTextCodec::codecForMib(mib);
	if (codec->name()!="UTF-8") ui.comboBoxEncoding->addItem(codec->name());
	}
connect( ui.pushButtonAspell, SIGNAL(clicked()), this, SLOT(browseAspell()));

ui.comboBoxAspellEncoding->addItem("utf-8");
ui.comboBoxAspellEncoding->addItem("iso8859-1");

connect( ui.pushButtonColorMath, SIGNAL(clicked()), this, SLOT(configureColorMath()));
connect( ui.pushButtonColorCommand, SIGNAL(clicked()), this, SLOT(configureColorCommand()));
connect( ui.pushButtonColorKeyword, SIGNAL(clicked()), this, SLOT(configureColorKeyword()));
}

void PageEditor::browseAspell()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\"";
	ui.lineEditAspellCommand->setText( location );
	}
}

void PageEditor::configureColorMath()
{
QColor color = QColorDialog::getColor(ui.pushButtonColorMath->palette().background().color(), this);
if (color.isValid()) 
	{
	ui.pushButtonColorMath->setPalette(QPalette(color));
	ui.pushButtonColorMath->setAutoFillBackground(true);
	}
}

void PageEditor::configureColorCommand()
{
QColor color = QColorDialog::getColor(ui.pushButtonColorCommand->palette().background().color(), this);
if (color.isValid()) 
	{
	ui.pushButtonColorCommand->setPalette(QPalette(color));
	ui.pushButtonColorCommand->setAutoFillBackground(true);
	}
}

void PageEditor::configureColorKeyword()
{
QColor color = QColorDialog::getColor(ui.pushButtonColorKeyword->palette().background().color(), this);
if (color.isValid()) 
	{
	ui.pushButtonColorKeyword->setPalette(QPalette(color));
	ui.pushButtonColorKeyword->setAutoFillBackground(true);
	}
}

