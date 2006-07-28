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

#include "pageconfig.h"

#include <QFontDatabase>
#include <QTextCodec>
#include <QFileDialog>
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

PageQuick::PageQuick(QWidget *parent)
    : QWidget(parent)
{
ui.setupUi(this);
connect(ui.radioButton6, SIGNAL(toggled(bool)),ui.lineEditUserquick, SLOT(setEnabled(bool)));
}

PageTools::PageTools(QWidget *parent)
    : QWidget(parent)
{
ui.setupUi(this);
connect( ui.pushButtonLatex, SIGNAL(clicked()), this, SLOT(browseLatex()));
connect( ui.pushButtonDvips, SIGNAL(clicked()), this, SLOT(browseDvips()));
connect( ui.pushButtonBibtex, SIGNAL(clicked()), this, SLOT(browseBibtex()));
connect( ui.pushButtonMakeindex, SIGNAL(clicked()), this, SLOT(browseMakeindex()));
connect( ui.pushButtonDviviewer, SIGNAL(clicked()), this, SLOT(browseDviviewer()));
connect( ui.pushButtonPsviewer, SIGNAL(clicked()), this, SLOT(browsePsviewer()));
connect( ui.pushButtonPdflatex, SIGNAL(clicked()), this, SLOT(browsePdflatex()));
connect( ui.pushButtonDvipdfm, SIGNAL(clicked()), this, SLOT(browseDvipdfm()));
connect( ui.pushButtonPs2pdf, SIGNAL(clicked()), this, SLOT(browsePs2pdf()));
connect( ui.pushButtonPdfviewer, SIGNAL(clicked()), this, SLOT(browsePdfviewer()));
connect( ui.pushButtonMetapost, SIGNAL(clicked()), this, SLOT(browseMetapost()));
connect( ui.pushButtonGhostscript, SIGNAL(clicked()), this, SLOT(browseGhostscript()));
}

void PageTools::browseLatex()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" -interaction=nonstopmode %.tex";
	ui.lineEditLatex->setText( location );
	}
}

void PageTools::browseDvips()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" -o %.ps %.dvi";
	ui.lineEditDvips->setText( location );
	}
}

void PageTools::browseBibtex()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.aux";
	ui.lineEditBibtex->setText( location );
	}
}

void PageTools::browseMakeindex()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.idx";
	ui.lineEditMakeindex->setText( location );
	}
}

void PageTools::browseDviviewer()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.dvi";
	ui.lineEditDviviewer->setText( location );
	}
}

void PageTools::browsePsviewer()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.ps";
	ui.lineEditPsviewer->setText( location );
	}
}

void PageTools::browsePdflatex()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" -interaction=nonstopmode %.tex";
	ui.lineEditPdflatex->setText( location );
	}
}

void PageTools::browseDvipdfm()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.dvi";
	ui.lineEditDvipdfm->setText( location );
	}
}

void PageTools::browsePs2pdf()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.ps";
	ui.lineEditPs2pdf->setText( location );
	}
}

void PageTools::browsePdfviewer()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.pdf";
	ui.lineEditPdfviewer->setText( location );
	}
}

void PageTools::browseMetapost()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" --interaction nonstopmode ";
	ui.lineEditMetapost->setText( location );
	}
}

void PageTools::browseGhostscript()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\"";
	ui.lineEditGhostscript->setText( location );
	}
}
