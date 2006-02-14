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

#include "webpublishdialog.h"

#include <QVariant>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QRegExp>
#include <QSettings>
#include <QStringList>
#include <QEventLoop>
#include <QApplication>
#include <QCloseEvent>

WebPublishDialog::WebPublishDialog(QWidget *parent, const char *name)
    :QDialog( parent)
{
setWindowTitle(name);
setModal(true);
ui.setupUi(this);
ui.alignmentcomboBox->insertItem(0, tr("Left") );
ui.alignmentcomboBox->insertItem(1, tr("Center") );
ui.alignmentcomboBox->insertItem(2, tr("Right") );
ui.interlacecomboBox->insertItem(0, tr("Yes") );
ui.interlacecomboBox->insertItem(1, tr("No") );
ui.colordepthcomboBox->insertItem(0, "4" );
ui.colordepthcomboBox->insertItem(1, "8" );
ui.colordepthcomboBox->insertItem(2, "32" );
ui.colordepthcomboBox->insertItem(3, "128" );
ui.colordepthcomboBox->insertItem(4, "256" );
ui.navigationBox->insertItem(0, tr("Icons") );
ui.navigationBox->insertItem(1, tr("Page numbers") );
ui.indexcomboBox->insertItem(0, tr("Yes") );
ui.indexcomboBox->insertItem(1, tr("No") );
connect( ui.quitButton, SIGNAL( clicked() ), this, SLOT( close() ) );
connect( ui.LaunchButton, SIGNAL( clicked() ), this, SLOT( proceedSlot() ) );
connect( ui.inputfileButton, SIGNAL( clicked() ), this, SLOT( browseSlot() ) );
init();
}

WebPublishDialog::~WebPublishDialog(){
}

void WebPublishDialog::init()
{
readsettings();
ui.titleEdit->setText(TITLE );
ui.footnoteEdit->setText(ADDRESS);
ui.browserEdit->setText(BROWSER);
ui.contentEdit->setText(CONTENTNAME);
if (NOINDEX) ui.indexcomboBox->setCurrentIndex(1);
else ui.indexcomboBox->setCurrentIndex(0);
if(ALIGN=="left") ui.alignmentcomboBox->setCurrentIndex(0);
if(ALIGN=="center") ui.alignmentcomboBox->setCurrentIndex(1);
if(ALIGN=="right") ui.alignmentcomboBox->setCurrentIndex(2);
if (NAVIGATION==1) ui.navigationBox->setCurrentIndex(0);
else ui.navigationBox->setCurrentIndex(1);
if (NOINTERLACE) ui.interlacecomboBox->setCurrentIndex(1);
else ui.interlacecomboBox->setCurrentIndex(0);
if(COLORDEPTH==4) ui.colordepthcomboBox->setCurrentIndex(0);
if(COLORDEPTH==8) ui.colordepthcomboBox->setCurrentIndex(1);
if(COLORDEPTH==32) ui.colordepthcomboBox->setCurrentIndex(2);
if(COLORDEPTH==128) ui.colordepthcomboBox->setCurrentIndex(3);
if(COLORDEPTH==256) ui.colordepthcomboBox->setCurrentIndex(4);
ui.widthspinBox->setValue( USERWIDTH );
ui.compilationspinBox->setValue( COMPIL );
ui.tocdepthspinBox->setValue(TOCDEPTH);
ui.startindexspinBox->setValue(STARTINDEX);
MAXWIDTH=0;
COLORLINK="1 0 0"; 
nb_pages=0;
nb_content_pages=0;
id_page=1;
DEPTH="\\{part\\}|\\{chapter\\}|\\{section\\}";
}

void WebPublishDialog::convert( const QString &fileName )
{
TTWPERROR=false;
ERRPROCESS=false;
ui.messagetextEdit->clear();
QString firstpage;
QFileInfo fi(fileName);
if (fi.exists() && fi.isReadable()) 
	{
	WORKDIR=fi.absolutePath();
	LASTDIR=WORKDIR;
	BASE=fi.baseName();
	HTMLDIR=WORKDIR+"/"+BASE+"_html";
	QDir HDir(HTMLDIR);
	if (HDir.exists())
		{
		QFileInfoList files = HDir.entryInfoList();
		if ( !files.empty() ) 
			{
			for (int i = 0; i < files.size(); ++i) 
				{
 				QString fn=files.at(i).filePath();
 				QFile f(fn);
 				f.remove();
				}
			}
		}
		else
		{
		HDir.mkdir(HTMLDIR);
		}
	copyFile(PROGRAMDIR+"/style.css",HTMLDIR+"/style.css");
	copyFile(PROGRAMDIR+"/up.gif",HTMLDIR+"/up.gif");
	copyFile(PROGRAMDIR+"/up_d.gif",HTMLDIR+"/up_d.gif");
	copyFile(PROGRAMDIR+"/psheader.txt",WORKDIR+"/psheader.txt");
	if (NAVIGATION==1) 
		{
		copyFile(PROGRAMDIR+"/next.gif",HTMLDIR+"/next.gif");
		copyFile(PROGRAMDIR+"/next_d.gif",HTMLDIR+"/next_d.gif");
		copyFile(PROGRAMDIR+"/prev.gif",HTMLDIR+"/prev.gif");
		copyFile(PROGRAMDIR+"/prev_d.gif",HTMLDIR+"/prev_d.gif");
		}
	if (TITLE=="") TITLE=BASE;
	if (TOCDEPTH==2) 
		{
		DEPTH=DEPTH+"|\\{subsection\\}";
		}
	else if (TOCDEPTH==3) 
		{
		DEPTH=DEPTH+"|\\{subsection\\}|\\{subsubsection\\}";
		}
	writepages("content");
	if (TTWPERROR || ERRPROCESS) return;
	if (!NOINDEX) writepages("index");
	if (TTWPERROR || ERRPROCESS) return;
	clean();
	ui.messagetextEdit->append("Conversion done.\nThe html files are located in the "+HTMLDIR+" directory.");
	if (!NOINDEX) firstpage=HTMLDIR+"/index.html";
	else firstpage=HTMLDIR+"/page1.html";
	QFileInfo fip(firstpage);
	if (fip.exists() && fip.isReadable() && (!BROWSER.isEmpty()) && (!ERRPROCESS)) 
		{
		ui.messagetextEdit->append("Running browser .");
		RunCommand(BROWSER+" "+firstpage,false);
		}
	}
else 
	{
	ui.messagetextEdit->append("Input file not found.");
	}
}




void WebPublishDialog::closeEvent( QCloseEvent* ce )
{
writesettings();
ce->accept();
return;
}


void WebPublishDialog::RunCommand(QString comd,bool waitendprocess)
{
QByteArray result;
proc = new QProcess( this );
proc->setWorkingDirectory(WORKDIR);
connect(proc, SIGNAL(finished(int)),this, SLOT(SlotEndProcess(int)));
if (waitendprocess) proc->start("/bin/sh",QStringList() << "-c" << comd);
else proc->start(comd);
if (!proc->waitForStarted(1000)) 
	{
	ui.messagetextEdit->append("Error : could not start the command");
	ERRPROCESS=true;
	}
FINPROCESS=false;
if (waitendprocess)
	{
	while (!FINPROCESS) 
		{
		qApp->instance()->processEvents(QEventLoop::ExcludeUserInputEvents);
		}
	}
}

void WebPublishDialog::copyFile(QString from_file, QString to_file)
{
if (!ERRPROCESS) RunCommand("cp -f "+from_file+" "+to_file,true);
}

void WebPublishDialog::removeFile(QString file)
{
if (QFile::exists(file))
	{
	QFile fi(file);
	fi.open( QIODevice::ReadOnly );
	fi.remove();
	}
}

QString WebPublishDialog::header()
{
QString result="<!DOCTYPE HTML PUBLIC '-//W3C//DTD HTML 4.01 Transitional//EN'> \n";
result+="<html>\n";
result+="<head>\n";
result+="<META NAME='Generator' CONTENT='TeXToWebPublishing (ttwp)'>\n";
result+="<TITLE>"+TITLE+"</TITLE>\n";
result+="<link rel=StyleSheet href='style.css' type='text/css'>\n";
result+="</head>\n";
result+="<body bgcolor='white'>\n";
return result;
}

QString WebPublishDialog::footer()
{
QString result="<div align='"+ALIGN+"' id='address'> "+ADDRESS+" </div>\n";
result+="</body>\n";
result+="</html>";
return result;
}

QString WebPublishDialog::content_navigation(int page,int numpages,QString up_page)
{
QString result;
QString leftcode="";
QString rightcode="";
QString upcode ="";
int prev_page=page-1;
int next_page=page+1;
if (NAVIGATION==1) 
	{
	if (page > 1) 
		{
		leftcode= "<A HREF='page"+QString::number(prev_page)+".html'><IMG src='prev.gif' align='middle' border='0'></A>";
		}
	else
		{
		leftcode="<IMG src='prev_d.gif' align='middle' border='0'>";
		}
		
	if (page < numpages) 
		{
		rightcode="<A HREF='page"+QString::number(next_page)+".html'><IMG src='next.gif' align='middle' border='0'></A>";
		}
	else
		{
		rightcode="<IMG src='next_d.gif' align='middle' border='0'>" ;
		}
	}
else
	{
	if (page > 1)
		{
		leftcode ="<A HREF='page"+QString::number(prev_page)+".html'>&nbsp;"+QString::number(prev_page)+"/"+QString::number(nb_content_pages)+"&nbsp;<</A>" ;
		}
	else
		{
		leftcode ="&nbsp;";
		}
	if (page < numpages) 
		{
		rightcode ="<A HREF='page"+QString::number(next_page)+".html'>>&nbsp;"+QString::number(next_page)+"/"+QString::number(nb_content_pages)+"&nbsp;</A>" ;
		}
	else
		{
		rightcode ="&nbsp;";
		}
	}
if (page>0) 
	{
	upcode ="<A HREF='"+up_page+"'><IMG src='up.gif' align='middle' border='0'></A>" ;
	}
else
	{
	upcode ="<IMG src='up_d.gif' align='middle' border='0'>";
	}
result="<div id='navigation'>\n <table border=0 cellpadding=2 cellspacing=0 width='100%'>\n ";
result+="<tr valign='middle'>\n ";
result+="<td align='left' valign='middle' width='100'>"+leftcode+"</td>\n";
result+="<td align='center' valign='middle' width='100'>"+upcode+"</td>\n";
result+="<td align='right' valign='middle' width='100'>"+rightcode+"</td>\n";
result+="</tr>\n";
result+="</table>\n";
result+="</div>\n";
return result;
}

int WebPublishDialog:: nbpagesps(QString psfile)
{
QString line;
QString captured="0";
int result;
bool ok;
QFile f( psfile);
QRegExp rx( "^\%\%Pages:\\s+(-?\\d+)" );
if ( !f.open( QIODevice::ReadOnly ) ) 
	{
	fatalerror(psfile+" not found.");
	return 0; 
	}
else
	{
	QTextStream PS( &f );
	while ( !PS.atEnd() ) 
		{
		line=PS.readLine();
		if (rx.indexIn(line)>-1) 
			{
			captured = rx.cap( 1 );
			break;
			}
		}
	f.close();
	result=captured.toInt( &ok );
	if (ok  && (result>0)) 
		{
		return result;
		}
	else 
		{
		return 0;
		}
	}
}

QString WebPublishDialog::codepic(QString pic_name, QString map_name)
{
QString result="<div id='content' align='"+ALIGN+"'><IMG src='"+pic_name+"' align='middle' border='0'";
if (map_name!="") result+=" usemap='#"+map_name+"'"; 
result+="></div>\n";
return result;
}

void WebPublishDialog::ps2gif(QString input,QString output,int id_page,int w,int h,int maxw)
{
if (!ERRPROCESS) RunCommand("psselect -p"+QString::number(id_page)+" "+input+" page.ps",true);
if (TTWPERROR  || ERRPROCESS) return;
float resolution=float (72*USERWIDTH)/maxw;
float scale=float (USERWIDTH)/maxw;
int gx=int(w*resolution/72+0.5); 
int gy=int(h*resolution/72+0.5); 
QFile psf(WORKDIR+"/page.ps");
if ( !psf.open( QIODevice::ReadOnly ) ) {fatalerror(WORKDIR+"/page.ps"+" not found.");return;}
else
	{
	QTextStream PS( &psf );
	QFile outf(WORKDIR+"/tmp.ps");
	if ( !outf.open( QIODevice::WriteOnly ) ) {fatalerror("Can't open "+WORKDIR+"/tmp.ps");return;}
	else
		{
		QTextStream OUTMP( &outf );
		bool flag=false;
		QString line;
		while ( !PS.atEnd() ) 
			{
			line=PS.readLine();
			if (line.indexOf(QRegExp("^%%BoundingBox:\\s(\\d+)\\s(\\d+)\\s(\\d+)\\s(\\d+)/)"),0) !=-1) 
				{
				OUTMP << "%%BoundingBox: 0 0 "+QString::number(w)+" "+QString::number(h)+"\n";
				}
			else if ( (!flag) && ( (line.indexOf(QRegExp("^%%EndSetup"),0) >-1) || (line.indexOf(QRegExp("^%%Page:"),0) >-1) ) )
				{
				OUTMP << line+"\n";
				OUTMP << QString::number(x1)+" neg "+QString::number(y1)+" neg translate\n";
				flag=true;
				}
			else if (line.indexOf(QRegExp("/uscale\\s+([0-9eE\\.\\-]+)\\s+N"),0) !=-1)
				{
				OUTMP << "/uscale "+QString::number(scale)+" N\n";
				}
			else
				{
				OUTMP << line+"\n";
				}
			}
		psf.close();
		outf.close();
		if (w!=0) 
			{
			if (!ERRPROCESS) RunCommand("gs -q -dSAFER -dBATCH -dNOPAUSE -sDEVICE=ppmraw  -g"+QString::number(gx)+"x"+QString::number(gy)+" -r"+QString::number(resolution)+" -dTextAlphaBits=4 -dGraphicsAlphaBits=4 -sOutputFile=tmp.ppm tmp.ps > link.txt",true);
			if (TTWPERROR || ERRPROCESS) return;
			if (NOINTERLACE) 
				{
				if (!ERRPROCESS) RunCommand("ppmquant -quiet -floyd "+QString::number(COLORDEPTH)+" tmp.ppm | ppmtogif -quiet > "+output+QString::number(id_page)+".gif",true);
				if (TTWPERROR || ERRPROCESS) return;
				}
			else
				{
				if (!ERRPROCESS) RunCommand("ppmquant -quiet -floyd "+QString::number(COLORDEPTH)+" tmp.ppm | ppmtogif -interlace -quiet > "+output+QString::number(id_page)+".gif",true);
				if (TTWPERROR || ERRPROCESS) return;
				}
			}
		else 
			{
				copyFile(PROGRAMDIR+"/blank.gif",HTMLDIR+"/"+output+QString::number(id_page)+".gif");
			}
		removeFile(WORKDIR+"/page.ps");
		removeFile(WORKDIR+"/tmp.ps");
		removeFile(WORKDIR+"/tmp.ppm");
		}
	}
}

void WebPublishDialog::writepages(QString mode)
{
bool match=false;
bool ok;
int counter=1;
MAXWIDTH=1;
QString line;
QString captured1, captured2, captured3, captured4;
x1box.clear();
y1box.clear();
x2box.clear();
y2box.clear();

if (mode=="content")
	{
	QFile outf(WORKDIR+"/"+BASE+"_"+mode+".tex");
	if ( !outf.open( QIODevice::WriteOnly ) ) {fatalerror("Can't open "+WORKDIR+"/"+BASE+"_"+mode+".tex");return;}
	else
		{
		QTextStream OUTMP( &outf );
		QFile texf(WORKDIR+"/"+BASE+".tex");
		if ( !texf.open( QIODevice::ReadOnly ) ) {fatalerror(WORKDIR+"/"+BASE+".tex"+" not found.");return;}
		else
			{
			QTextStream TEX( &texf );
			while ( !TEX.atEnd() ) 
				{
				line=TEX.readLine();
				if (  !match && (line.indexOf(QRegExp("^\\\\begin\\{document\\}"),0)!=-1)) 
					{
					OUTMP << "\\newbox\\bwk\\edef\\tempd#1pt{#1\\string p\\string t}\\tempd\\def\\nbextr#1pt{#1}\n";
					OUTMP << "\\def\\npts#1{\\expandafter\\nbextr\\the#1\\space}\n";
					OUTMP << "\\def\\ttwplink#1#2{\\special{ps:"+COLORLINK+" setrgbcolor}#2\\special{ps:0 0 0 setrgbcolor}\\setbox\\bwk=\\hbox{#2}\\special{ps:( linkto #1)\\space\\npts{\\wd\\bwk} \\npts{\\dp\\bwk} -\\npts{\\ht\\bwk} true\\space Cpos}}\n";
					match=true;
					}
				OUTMP << line+"\n";
				}
			texf.close();
			outf.close();
			ui.messagetextEdit->append("Compiling input file. Please wait...");
			while (counter <= COMPIL) 
				{
				if (!ERRPROCESS) RunCommand("latex -interaction=nonstopmode "+BASE+"_"+mode+".tex",true);
				latexerror(WORKDIR+"/"+BASE+"_"+mode+".log");
				if (TTWPERROR  || ERRPROCESS) break;
				counter++;
				}
			if (TTWPERROR  || ERRPROCESS) return;
			QFile auxf(WORKDIR+"/"+BASE+"_content.aux");
			if ( !auxf.open( QIODevice::ReadOnly ) ) {fatalerror(mode+WORKDIR+"/"+BASE+"_content.aux"+" not found.");return;}
			else
				{
				QTextStream AUX( &auxf );
				QRegExp rx( "\\\\newlabel\\{(.*)\\}\\{\\{.*\\}\\{(\\d+)\\}\\}" );
				while ( !AUX.atEnd() ) 
					{
					QString line=AUX.readLine();
					if (rx.indexIn(line)!=-1) 
						{
						QString captured1 = rx.cap( 1 );
						QString captured2 = rx.cap( 2 );
						loc[captured1] = "page"+captured2+".html";
						}
					}
				auxf.close();
				}
			}
		}
	}
else if (mode=="index")
	{
	QFile outf(WORKDIR+"/"+BASE+"_"+mode+".tex");
	if ( !outf.open( QIODevice::WriteOnly ) ) {fatalerror("Can't open "+WORKDIR+"/"+BASE+"_"+mode+".tex");return;}
	else
		{
		QTextStream OUTMP( &outf );
		QFile texf(WORKDIR+"/"+BASE+".tex");
		if ( !texf.open( QIODevice::ReadOnly ) ) {fatalerror(WORKDIR+"/"+BASE+".tex"+" not found.");return;}
		else
			{
			QTextStream TEX( &texf );
			match=false;
			while ( !TEX.atEnd() ) 
				{
				line=TEX.readLine();
				if (line.indexOf(QRegExp("^\\\\begin\\{document\\}"),0) >-1) 
					{
					match=true;
					}
				if (!match) 
					{
					OUTMP << line+"\n";
					}
				}
			texf.close();
			OUTMP << "\\pagestyle{empty}\n";
			OUTMP << "\\setcounter{tocdepth}{"+QString::number(TOCDEPTH)+"} \n";
			OUTMP << "\\newbox\\bwk\\edef\\tempd#1pt{#1\\string p\\string t}\\tempd\\def\\nbextr#1pt{#1}\n";
			OUTMP << "\\def\\npts#1{\\expandafter\\nbextr\\the#1\\space}\n";
			OUTMP << "\\def\\ttwplink#1#2{#2\\setbox\\bwk=\\vbox{#2}\\special{ps:( linkto #1)\\space\\npts{\\wd\\bwk} \\npts{\\dp\\bwk} -\\npts{\\ht\\bwk} false\\space Cpos}}\n";
			OUTMP << "\\begin{document}\n";
			OUTMP << "\\section*{"+CONTENTNAME+"}\n";
			OUTMP << "\\makeatletter\n\\parindent = 0.0 in\n";
			QFile auxf(WORKDIR+"/"+BASE+"_content.aux");
			if ( !auxf.open( QIODevice::ReadOnly ) ) {fatalerror(mode+WORKDIR+"/"+BASE+"_content.aux"+" not found.");return;}
			else
				{
				QTextStream AUX( &auxf );
				QRegExp rx( "\\\\@writefile\\{toc\\}.*("+DEPTH+").*\\{(\\d+)\\}\\}" );
				while ( !AUX.atEnd() ) 
					{
					line=AUX.readLine();
					if (rx.indexIn(line)>-1) 
						{
						captured2 = rx.cap( 2 );
						id_page=captured2.toInt( &ok );
						line.remove(QRegExp("\\\\@writefile\\{toc\\}"));
						if (line.indexOf(QRegExp("\\\\numberline"),0) >-1) 
							{
							id_page=id_page+STARTINDEX-1;
							OUTMP << "\\ttwplink{page"+QString::number(id_page)+".html}";
							}
						else
							{
							OUTMP << "\\ttwplink{none}";
							}
						OUTMP << line+"\n\n";
						}
					}
				auxf.close();
				OUTMP << "\\end{document}" ;  
				outf.close();
				if (!ERRPROCESS) RunCommand("latex -interaction=nonstopmode "+BASE+"_"+mode+".tex",true);
				latexerror(WORKDIR+"/"+BASE+"_"+mode+".log");
				if (TTWPERROR  || ERRPROCESS) return;
				}
			}
		}
	}
if (!ERRPROCESS) RunCommand("dvips -q "+DVIOPT+" -h psheader.txt -o "+BASE+"_"+mode+".ps "+BASE+"_"+mode+".dvi",true) ;
if (TTWPERROR || ERRPROCESS) return;
QString link="none";
int nb_link=0;
int W,H;
nb_pages=nbpagesps(WORKDIR+"/"+BASE+"_"+mode+".ps");
if (TTWPERROR  || ERRPROCESS) return;
id_page=1;
ui.messagetextEdit->append("Scanning files. Please wait...");
while ( id_page <= nb_pages ) 
	{
	if (!ERRPROCESS) RunCommand("psselect -p"+QString::number(id_page)+" "+BASE+"_"+mode+".ps page.ps",true);
	if (TTWPERROR  || ERRPROCESS) return;
	if (!ERRPROCESS) RunCommand("gs -q -dBATCH -dNOPAUSE -sDEVICE=bbox -sOutputFile=-_ -f page.ps -c quit 2> bbox.txt",true);
	if (TTWPERROR  || ERRPROCESS) return;
	QFile bboxf(WORKDIR+"/bbox.txt");
	if ( !bboxf.open( QIODevice::ReadOnly ) ) {fatalerror(WORKDIR+"/bbox.txt"+" not found.");return;}
	else
		{
		QTextStream BBOX( &bboxf );
		QRegExp rx( "%%BoundingBox:\\s*([0-9eE\\.\\-]+)\\s+([0-9eE\\.\\-]+)\\s+([0-9eE\\.\\-]+)\\s+([0-9eE\\.\\-]+)" );
		while ( !BBOX.atEnd() ) 
			{
			QString line=BBOX.readLine();
			if (rx.indexIn(line)!=-1) 
					{
					captured1 = rx.cap( 1 );
					x1=captured1.toInt( &ok );
					captured2 = rx.cap( 2 );
					y1=captured2.toInt( &ok );
					captured3 = rx.cap( 3 );
					x2=captured3.toInt( &ok );
					captured4 = rx.cap( 4 );
					y2=captured4.toInt( &ok );
					}
			}
		bboxf.close();
		x1box.append(x1);
		y1box.append(y1);
		x2box.append(x2);
		y2box.append(y2);
		W=x2-x1;
		if ( W > MAXWIDTH ) 
			{
			MAXWIDTH=W ;
			}
		}
	removeFile(WORKDIR+"/page.ps");
	removeFile(WORKDIR+"/bbox.txt");
	id_page++;  
	}
if (mode=="content")
	{
	id_page=1;
	nb_content_pages=nb_pages;
	while ( id_page <= nb_pages ) 
		{
		ui.messagetextEdit->append("Writing page "+QString::number(id_page)+" of "+QString::number(nb_pages));
		x1=x1box[id_page-1];
		y1=y1box[id_page-1];
		x2=x2box[id_page-1];
		y2=y2box[id_page-1];
		W=x2-x1;
		H=y2-y1;
		ps2gif(BASE+"_"+mode+".ps",HTMLDIR+"/image",id_page,W,H,MAXWIDTH);
		if (TTWPERROR || ERRPROCESS) return;
		QFile htmf(HTMLDIR+"/page"+QString::number(id_page)+".html");
		if ( !htmf.open( QIODevice::WriteOnly ) ) {fatalerror("Can't open "+HTMLDIR+"/page"+QString::number(id_page)+".html");return;}
		else
			{
			QTextStream HTM( &htmf );
			HTM << header();
			if (NOINDEX)
				{
				HTM << content_navigation(id_page,nb_pages,"page1.html");
				}
			else
				{
				HTM << content_navigation(id_page,nb_pages,"index.html");
				}
			QFile linkf(WORKDIR+"/link.txt");
			if ( !linkf.open( QIODevice::ReadOnly ) ) {fatalerror(WORKDIR+"/link.txt"+" not found.");return;}
			else
				{
				QTextStream LINK( &linkf );
				QRegExp rx( "x1=([0-9eE\\.\\-]+)\\s+y1=([0-9eE\\.\\-]+)\\s+x2=([0-9eE\\.\\-]+)\\s+y2=([0-9eE\\.\\-]+)\\s+linkto\\s+(.*)$" );
				nb_link = 0 ;
				HTM << "<map name='carte"+QString::number(id_page)+"'>\n" ;
				while ( !LINK.atEnd() ) 
					{
					line=LINK.readLine();
					if (rx.indexIn(line)>-1) 
						{
						captured1 = rx.cap( 1 );
						x1=int(captured1.toFloat( &ok )+0.5);
						captured2 = rx.cap( 2 );
						y1=int(captured2.toFloat( &ok )+0.5);
						captured3 = rx.cap( 3 );
						x2=int(captured3.toFloat( &ok )+0.5);
						captured4 = rx.cap( 4 );
						y2=int(captured4.toFloat( &ok )+0.5);
						link=rx.cap( 5 );
						if (link !="none")
							{
							LinkMap::Iterator it=loc.find(link);
								if (it !=loc.end())
								{
								HTM << "<area shape='rect' coords='"+QString::number(x1)+","+QString::number(y1)+","+QString::number(x2)+","+QString::number(y2)+"' href='"+it.value()+"' >\n"  ;
								}
								else
								{
								HTM << "<area shape='rect' coords='"+QString::number(x1)+","+QString::number(y1)+","+QString::number(x2)+","+QString::number(y2)+"' href='"+link+"' >\n"  ;
								}
							}
						nb_link++;
						}
					}
				linkf.close();
				HTM << "</map>\n" ;
				if (nb_link >0) 
					{
					HTM << codepic("image"+QString::number(id_page)+".gif","carte"+QString::number(id_page));
					}
				else
					{
					HTM << codepic("image"+QString::number(id_page)+".gif","");
					}
				if (NOINDEX)
					{
					HTM << content_navigation(id_page,nb_pages,"page1.html");
					}
				else
					{
					HTM << content_navigation(id_page,nb_pages,"index.html");
					}
				HTM << footer();
				htmf.close() ;
				
				}
			}
		id_page++; 
		}
	}
else if (mode=="index")
	{
	ui.messagetextEdit->append("Writing index page");
	QFile htmf(HTMLDIR+"/index.html");
	if ( !htmf.open( QIODevice::WriteOnly ) ) {fatalerror("Can't open "+HTMLDIR+"/index.html");return;}
	else
		{
		QTextStream HTM( &htmf );
		HTM << header();
		HTM << content_navigation(0,1,"");
		id_page=1;
		while ( id_page <= nb_pages ) 
			{
			x1=x1box[id_page-1];
			y1=y1box[id_page-1];
			x2=x2box[id_page-1];
			y2=y2box[id_page-1];
			W=x2-x1;
			H=y2-y1;
			ps2gif(BASE+"_"+mode+".ps",HTMLDIR+"/"+mode+"image",id_page,W,H,MAXWIDTH);
			if (TTWPERROR || ERRPROCESS) return;
			QFile linkf(WORKDIR+"/link.txt");
			if ( !linkf.open( QIODevice::ReadOnly ) ) {fatalerror(WORKDIR+"/link.txt"+" not found.");return;}
			else
				{
				QTextStream LINK( &linkf );
				QRegExp rx( "x1=([0-9eE\\.\\-]+)\\s+y1=([0-9eE\\.\\-]+)\\s+x2=([0-9eE\\.\\-]+)\\s+y2=([0-9eE\\.\\-]+)\\s+linkto\\s+(.*)$" );
				nb_link = 0 ;
				HTM << "<map name='carte"+QString::number(id_page)+"'>\n" ;
				while ( !LINK.atEnd() ) 
					{
					line=LINK.readLine();
					if (rx.indexIn(line)>-1) 
						{
						captured1 = rx.cap( 1 );
						x1=int(captured1.toFloat( &ok )+0.5);
						captured2 = rx.cap( 2 );
						y1=int(captured2.toFloat( &ok )+0.5);
						captured3 = rx.cap( 3 );
						x2=int(captured3.toFloat( &ok )+0.5);
						captured4 = rx.cap( 4 );
						y2=int(captured4.toFloat( &ok )+0.5);
						link=rx.cap( 5 );
						if (link !="none")
							{
							HTM << "<area shape='rect' coords='"+QString::number(x1)+","+QString::number(y1)+","+QString::number(x2)+","+QString::number(y2)+"' href='"+link+"' >\n"  ;
							}
						nb_link++;
						}
					}
				linkf.close();
				HTM << "</map>\n" ;
				}
			id_page++; 
			}
		id_page=1;
		while ( id_page <= nb_pages ) 
		{
			HTM << codepic(mode+"image"+QString::number(id_page)+".gif","carte"+QString::number(id_page));
			id_page++; 
		}
		HTM << content_navigation(0,1,"");
		HTM << footer();
		htmf.close();
		}
	}
}

void WebPublishDialog::SlotEndProcess(int err)
{
if (err) 
	{
	ui.messagetextEdit->append("Error : a process has failed");
	ERRPROCESS=true;
	}
FINPROCESS=true;
}

void WebPublishDialog::fatalerror(QString msg)
{
clean();
ui.messagetextEdit->append("Fatal error : "+msg);
TTWPERROR=true;
}

void WebPublishDialog::clean()
{
removeFile(WORKDIR+"/psheader.txt");
removeFile(WORKDIR+"/link.txt");
QStringList extension=QString(".tex,.log,.aux,.dvi,.lof,.lot,.bit,.idx,.glo,.bbl,.ilg,.toc,.ind,.ps").split(",");
for ( QStringList::Iterator it = extension.begin(); it != extension.end(); ++it ) 
		{
		removeFile(WORKDIR+"/"+BASE+"_content"+*it);
		removeFile(WORKDIR+"/"+BASE+"_index"+*it);
		}
}

void WebPublishDialog::latexerror(QString logfile)
{
bool ok=true;
QRegExp rx( "^!" );
QString line;
QFile logf(logfile);
if ( !logf.open( QIODevice::ReadOnly ) ) {fatalerror(logfile+" not found.");}
else
	{
	QTextStream LOG( &logf );
	while ( !LOG.atEnd() ) 
		{
		line=LOG.readLine();
		if (rx.indexIn(line)>-1) 
			{
			ok=false;
			}
		}
	logf.close();
	if (!ok) fatalerror("LaTeX errors detected.");
	}
}

void WebPublishDialog::proceedSlot()
{
ui.messagetextEdit->clear();
applyusersettings();
convert(ui.inputfileEdit->text());
}

void WebPublishDialog::browseSlot()
{
QString fn = QFileDialog::getOpenFileName(this,tr("Open File"),LASTDIR,"TeX files (*.tex);;All files (*.*)");
if ( !fn.isEmpty() ) 
	{
	ui.inputfileEdit->setText( fn );
	}
}

void WebPublishDialog::writesettings()
{
applyusersettings();
QSettings settings("xm1","qttwp");
settings.beginGroup( "qttwp" );
settings.setValue("colordepth",COLORDEPTH);
settings.setValue("userwidth",USERWIDTH);
settings.setValue("compil",COMPIL);
settings.setValue("tocdepth",TOCDEPTH);
settings.setValue("startindex",STARTINDEX);
settings.setValue("navigation",NAVIGATION);
settings.setValue("nointerlace",NOINTERLACE);
settings.setValue("noindex",NOINDEX);
settings.setValue("title",TITLE);
settings.setValue("address",ADDRESS);
settings.setValue("browser",BROWSER);
settings.setValue("contentname",CONTENTNAME);
settings.setValue("align",ALIGN);
settings.setValue("lastdir",LASTDIR);
settings.setValue("dviopt",DVIOPT);
settings.endGroup();
}

void WebPublishDialog::readsettings()
{
QSettings settings("xm1","qttwp");
settings.beginGroup( "qttwp" );
COLORDEPTH=settings.value("/colordepth",32).toInt();
USERWIDTH=settings.value("/userwidth",700).toInt();
COMPIL=settings.value("/compil",1).toInt();
TOCDEPTH=settings.value("/tocdepth",2).toInt();
STARTINDEX=settings.value("/startindex",1).toInt();
NAVIGATION=settings.value("/navigation",1).toInt();
NOINTERLACE=settings.value("/nointerlace",false).toBool();
NOINDEX=settings.value("/noindex",false).toBool();
TITLE=settings.value("/title","").toString();
ADDRESS=settings.value("/address","").toString();
#ifdef Q_WS_X11
BROWSER=settings.value("/browser","firefox").toString();
#endif
#ifdef Q_WS_MACX
BROWSER=settings.value("/browser","open").toString();
#endif
CONTENTNAME=settings.value("/contentname","\\contentsname").toString();
ALIGN=settings.value("/align","center").toString();
LASTDIR=settings.value("/lastdir",QDir::homePath()).toString();
DVIOPT=settings.value("/dviopt"," -Ppk -V").toString();
#ifdef Q_WS_X11
PROGRAMDIR=PREFIX"/share/texmaker";
#endif
#ifdef Q_WS_MACX
PROGRAMDIR="/Applications/texmaker.app/Contents/Resources";
#endif
settings.endGroup();
}

void WebPublishDialog::applyusersettings()
{
TITLE=ui.titleEdit->text();
ADDRESS=ui.footnoteEdit->text();
BROWSER=ui.browserEdit->text();
CONTENTNAME=ui.contentEdit->text();
if (ui.indexcomboBox->currentIndex()==0) NOINDEX=false;
else NOINDEX=true;
if (ui.alignmentcomboBox->currentIndex()==0) ALIGN="left";
if (ui.alignmentcomboBox->currentIndex()==1) ALIGN="center";
if (ui.alignmentcomboBox->currentIndex()==2) ALIGN="right";
if (ui.navigationBox->currentIndex()==0) NAVIGATION=1;
else NAVIGATION=0;
if (ui.interlacecomboBox->currentIndex()==0) NOINTERLACE=false;
else NOINTERLACE=true;
if (ui.colordepthcomboBox->currentIndex()==0) COLORDEPTH=4;
if (ui.colordepthcomboBox->currentIndex()==1) COLORDEPTH=8;
if (ui.colordepthcomboBox->currentIndex()==2) COLORDEPTH=32;
if (ui.colordepthcomboBox->currentIndex()==3) COLORDEPTH=128;
if (ui.colordepthcomboBox->currentIndex()==4) COLORDEPTH=256;
USERWIDTH=ui.widthspinBox->value();
COMPIL=ui.compilationspinBox->value();
TOCDEPTH=ui.tocdepthspinBox->value();
STARTINDEX=ui.startindexspinBox->value();  
}
