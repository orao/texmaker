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

#include "configdialog.h"

#include <QFontDatabase>
#include <QTextCodec>
#include <QFileDialog>
#include <QColorDialog>
#include <QDir>
#include <QDebug>

ConfigDialog::ConfigDialog(QWidget* parent): QDialog( parent)
{
setModal(true);
ui.setupUi(this);

ui.contentsWidget->setIconSize(QSize(96, 96));
ui.contentsWidget->setViewMode(QListView::IconMode);
ui.contentsWidget->setMovement(QListView::Static);

//pageditor
QFontDatabase fdb;
ui.comboBoxFont->addItems( fdb.families() );

ui.comboBoxEncoding->addItem("UTF-8");
foreach (int mib, QTextCodec::availableMibs()) 
	{
	QTextCodec *codec = QTextCodec::codecForMib(mib);
	if (codec->name()!="UTF-8") ui.comboBoxEncoding->addItem(codec->name());
	}
connect( ui.pushButtonAspell, SIGNAL(clicked()), this, SLOT(browseAspell()));

ui.labelGetDic->setText( tr("Get dictionary at: %1").arg("<br><a href=\"http://wiki.services.openoffice.org/wiki/Dictionaries\">http://wiki.services.openoffice.org/wiki/Dictionaries</a>") );
ui.labelGetDic->setOpenExternalLinks(true);


connect( ui.pushButtonColorMath, SIGNAL(clicked()), this, SLOT(configureColorMath()));
connect( ui.pushButtonColorCommand, SIGNAL(clicked()), this, SLOT(configureColorCommand()));
connect( ui.pushButtonColorKeyword, SIGNAL(clicked()), this, SLOT(configureColorKeyword()));

//pagequick
connect(ui.radioButton6, SIGNAL(toggled(bool)),ui.lineEditUserquick, SLOT(setEnabled(bool)));
connect(ui.radioButton6, SIGNAL(toggled(bool)),ui.pushButtonWizard, SLOT(setEnabled(bool)));

//pagetools
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
connect( ui.pushButtonAsymptote, SIGNAL(clicked()), this, SLOT(browseAsymptote()));
connect( ui.pushButtonLatexmk, SIGNAL(clicked()), this, SLOT(browseLatexmk()));

connect( ui.pushButtonWizard, SIGNAL(clicked()), this, SLOT(userQuickWizard()));

connect(ui.shorttableWidget, SIGNAL(itemClicked ( QTableWidgetItem*)), this, SLOT(configureShortCut(QTableWidgetItem*)));

createIcons();
ui.contentsWidget->setCurrentRow(0);


}

ConfigDialog::~ConfigDialog(){
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

//pageditor
void ConfigDialog::browseAspell()
{
#if defined( Q_WS_X11 )

#ifdef USB_VERSION
QDir spelldir(QCoreApplication::applicationDirPath());
#else
#ifdef DEBIAN_SPELLDIR
QDir spelldir(PREFIX"/share/myspell/dicts");
#else
QDir spelldir(PREFIX"/share/texmaker");
#endif
#endif

#endif
#if defined( Q_WS_MACX )
QDir spelldir(QCoreApplication::applicationDirPath() + "/../Resources");
#endif
#if defined(Q_WS_WIN)
QDir spelldir(QCoreApplication::applicationDirPath());
#endif
QString location=QFileDialog::getOpenFileName(this,tr("Browse dictionary"),spelldir.absolutePath(),"Dictionary (*.dic)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
//	location="\""+location+"\"";
	ui.lineEditAspellCommand->setText( location );
	}
}

void ConfigDialog::configureColorMath()
{
QColor color = QColorDialog::getColor(ui.pushButtonColorMath->palette().background().color(), this);
if (color.isValid()) 
	{
	ui.pushButtonColorMath->setPalette(QPalette(color));
	ui.pushButtonColorMath->setAutoFillBackground(true);
	}
}

void ConfigDialog::configureColorCommand()
{
QColor color = QColorDialog::getColor(ui.pushButtonColorCommand->palette().background().color(), this);
if (color.isValid()) 
	{
	ui.pushButtonColorCommand->setPalette(QPalette(color));
	ui.pushButtonColorCommand->setAutoFillBackground(true);
	}
}

void ConfigDialog::configureColorKeyword()
{
QColor color = QColorDialog::getColor(ui.pushButtonColorKeyword->palette().background().color(), this);
if (color.isValid()) 
	{
	ui.pushButtonColorKeyword->setPalette(QPalette(color));
	ui.pushButtonColorKeyword->setAutoFillBackground(true);
	}
}

//pagetools
void ConfigDialog::browseLatex()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" -interaction=nonstopmode %.tex";
	ui.lineEditLatex->setText( location );
	}
}

void ConfigDialog::browseDvips()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" -o %.ps %.dvi";
	ui.lineEditDvips->setText( location );
	}
}

void ConfigDialog::browseBibtex()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.aux";
	ui.lineEditBibtex->setText( location );
	}
}

void ConfigDialog::browseMakeindex()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.idx";
	ui.lineEditMakeindex->setText( location );
	}
}

void ConfigDialog::browseDviviewer()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.dvi";
	ui.lineEditDviviewer->setText( location );
	}
}

void ConfigDialog::browsePsviewer()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.ps";
	ui.lineEditPsviewer->setText( location );
	}
}

void ConfigDialog::browsePdflatex()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" -interaction=nonstopmode %.tex";
	ui.lineEditPdflatex->setText( location );
	}
}

void ConfigDialog::browseDvipdfm()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.dvi";
	ui.lineEditDvipdfm->setText( location );
	}
}

void ConfigDialog::browsePs2pdf()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.ps";
	ui.lineEditPs2pdf->setText( location );
	}
}

void ConfigDialog::browsePdfviewer()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.pdf";
	ui.lineEditPdfviewer->setText( location );
	}
}

void ConfigDialog::browseMetapost()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" --interaction nonstopmode ";
	ui.lineEditMetapost->setText( location );
	}
}

void ConfigDialog::browseGhostscript()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\"";
	ui.lineEditGhostscript->setText( location );
	}
}

void ConfigDialog::browseAsymptote()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" %.asy";
	ui.lineEditAsymptote->setText( location );
	}
}

void ConfigDialog::browseLatexmk()
{
QString location=QFileDialog::getOpenFileName(this,tr("Browse program"),QDir::rootPath(),"Program (*)",0,QFileDialog::DontResolveSymlinks);
if ( !location.isEmpty() ) 
	{
	location.replace(QString("\\"),QString("/"));
	location="\""+location+"\" -e \"$pdflatex=q/pdflatex -interaction=nonstopmode/\" -pdf %.tex";
	ui.lineEditLatexmk->setText( location );
	}
}

void ConfigDialog::configureShortCut(QTableWidgetItem *item)
{
QString shortcut,data,newshortcut;
if (item)
	{
	shortcut=item->text();
	data=item->data(Qt::UserRole).toString();
	if (data=="key") 
	    {
	    keydlg = new KeySequenceDialog(this);
	    keydlg->setKeySequence(QKeySequence(shortcut));
	    if ( keydlg->exec() )
		{
		newshortcut=keydlg->ui.lineEdit->text();
		if (!newshortcut.isEmpty()) 
		    {
		    item->setText(newshortcut);
		    item->setData(Qt::UserRole,QString("key"));
		    }
		else
		    {
		    item->setText("none");
		    item->setData(Qt::UserRole,QString("key"));
		    }
		}
	    delete (keydlg);
	    }
	}
}

void ConfigDialog::userQuickWizard()
{
QStringList usualNames, usualCommands;

usualNames.append(tr("LaTeX"));
usualCommands.append(ui.lineEditLatex->text());

usualNames.append(tr("PdfLaTeX"));
usualCommands.append(ui.lineEditPdflatex->text());

usualNames.append(tr("dvips"));
usualCommands.append(ui.lineEditDvips->text());

usualNames.append(tr("Dvi Viewer"));
usualCommands.append(ui.lineEditDviviewer->text());

usualNames.append(tr("PS Viewer"));
usualCommands.append(ui.lineEditPsviewer->text());

usualNames.append(tr("Dvipdfm"));
usualCommands.append(ui.lineEditDvipdfm->text());

usualNames.append(tr("ps2pdf"));
usualCommands.append(ui.lineEditPs2pdf->text());

usualNames.append(tr("Bibtex"));
usualCommands.append(ui.lineEditBibtex->text());

usualNames.append(tr("Makeindex"));
usualCommands.append(ui.lineEditMakeindex->text());

usualNames.append(tr("Pdf Viewer"));
usualCommands.append(ui.lineEditPdfviewer->text());

usualNames.append(tr("metapost"));
usualCommands.append(ui.lineEditMetapost->text());

usualNames.append(tr("ghostscript"));
usualCommands.append(ui.lineEditGhostscript->text());

usualNames.append(tr("Asymptote"));
usualCommands.append(ui.lineEditAsymptote->text());

usualNames.append(tr("Latexmk"));
usualCommands.append(ui.lineEditLatexmk->text());

userquickdlg= new UserQuickDialog(this,usualNames,usualCommands);
if ( userquickdlg->exec() )
   {
   ui.lineEditUserquick->setText(userquickdlg->userQuickCommand);
   }
delete (userquickdlg);
}
