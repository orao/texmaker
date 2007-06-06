/***************************************************************************
 *   copyright       : (C) 2007 by Pascal Brachet                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "spellerdialog.h"

#include <QDebug>
#include <QTextCursor>
#include <QTextBlock>
#include <QMessageBox>
#include <QTimer>
#include <QCloseEvent>

SpellerDialog::SpellerDialog(QWidget *parent,LatexEditor *ed,QString AspellCommand, QString lang, QString encoding)
    :QDialog( parent)
{
editor=ed;
spell_command=AspellCommand;
spell_lang=lang;
spell_encoding=encoding;
ui.setupUi(this);
setModal(true);
show();

connect(ui.pushButtonIgnore, SIGNAL(clicked()), this, SLOT(slotIgnore()));
connect(ui.pushButtonReplace, SIGNAL(clicked()), this, SLOT(slotReplace()));
connect(ui.listWidget, SIGNAL(itemSelectionChanged()),this, SLOT(updateItem()));
connect(this, SIGNAL(scanfinished()), this, SLOT(spellingInit()));

miswordList.clear();
suggestionList.clear();
ignoredwordList.clear();
lineList.clear();
colList.clear();
ui.listWidget->setEnabled(false);
ui.lineEditNew->setEnabled(false);
ui.pushButtonIgnore->setEnabled(false);
ui.pushButtonReplace->setEnabled(false);
ui.lineEditOriginal->setEnabled(false);
ui.progressBar->setEnabled(false);
spellproc = new QProcess( this );
connect(spellproc, SIGNAL(readyReadStandardOutput()),this, SLOT(readOutput()));
connect(spellproc, SIGNAL(finished(int)),this, SLOT(endProcess(int)));
spellproc->start(spell_command+" -a -t --encoding="+spell_encoding+" --lang="+spell_lang);
if (!spellproc->waitForStarted(1000)) 
	{
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	QTimer::singleShot(100, this, SLOT(accept()));
	}
else
	{
	scanDocument();
	}
}

SpellerDialog::~SpellerDialog(){

}

void SpellerDialog::closeEvent( QCloseEvent* ce )
{
if (spellproc && spellproc->state()==QProcess::Running) 
	{
	spellproc->disconnect();
	spellproc->kill(); 
	delete spellproc ;
	}
if (progressDialog) 
	{
	delete progressDialog;
	}
ce->accept();
}

void SpellerDialog::accept()
{
if (spellproc && spellproc->state()==QProcess::Running) 
	{
	spellproc->disconnect();
	spellproc->kill(); 
	delete spellproc ;
	}
if (progressDialog) 
	{
	delete progressDialog;
	}
QDialog::accept();
}

void SpellerDialog::updateItem()
{
int current=-1;
QList<QListWidgetItem *> items;
items = ui.listWidget->selectedItems();
if (items.count() > 0) 
	{
	ui.listWidget->setCurrentItem(items[0]);
	current=ui.listWidget->row(items[0]);
	}
if (current>=0) 
	{
	ui.lineEditNew->setText(ui.listWidget->currentItem()->text());
	}
}

void SpellerDialog::scanDocument()
{
QTextBlock block;
QString text;
bool go=true;
QTextCursor c = editor->textCursor();
if (c.hasSelection()) 
	{
	startpos=c.selectionStart();
	endpos=c.selectionEnd();
	c.setPosition(endpos,QTextCursor::MoveAnchor);
	block=c.block();
	numlines=editor->linefromblock(block);
	c.setPosition(startpos,QTextCursor::MoveAnchor);
	block=c.block();
	currentline=editor->linefromblock(block)-1;
	if (!progressDialog) progressDialog = new QProgressDialog(tr("Scanning document..."),0,currentline,numlines,this);
	progressDialog->show();
	progressDialog->raise();
	progressDialog->setValue(currentline);
	if (currentline>=0)
		{
		while ( c.position() < endpos && go)
			{
			text="^"+block.text()+"\n";
			if (spell_encoding=="iso8859-1") spellproc->write(text.toLatin1());
			else spellproc->write(text.toUtf8());
			go=c.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor);
			block=c.block();
			}
		}
	}
else
	{
	c.movePosition(QTextCursor::Start,QTextCursor::MoveAnchor);
	startpos=c.position();
	c.movePosition(QTextCursor::End,QTextCursor::MoveAnchor);
	endpos=c.position();
	numlines=editor->numoflines();
	if (!progressDialog) progressDialog = new QProgressDialog(tr("Scanning document..."),0,0,numlines,this);
	progressDialog->show();
	progressDialog->raise();
	progressDialog->setValue(0);
	currentline=0;
	block=editor->document()->begin();
	while (block.isValid())
		{
		text="^"+block.text()+"\n";
		if (spell_encoding=="iso8859-1") spellproc->write(text.toLatin1());
		else spellproc->write(text.toUtf8());
		block = block.next();
		}
	}
}

void SpellerDialog::spellingInit()
{
ui.listWidget->setEnabled(true);
ui.lineEditNew->setEnabled(true);
ui.pushButtonIgnore->setEnabled(true);
ui.pushButtonReplace->setEnabled(true);
ui.lineEditOriginal->setEnabled(true);
ui.progressBar->setEnabled(true);
ui.progressBar->setMinimum(0);
wordcount=0;
deltacol=0;
spellingline=0;
if (miswordList.count()>0)
	{
	ui.progressBar->setMaximum(miswordList.count());
	spellingNext();
	}
else 
	{
	QMessageBox::information( this,tr("Check Spelling"),tr("No misspelled words were found."));
	accept();
	}
}

void SpellerDialog::spellingNext()
{
QString misword;
int line,col;
if (wordcount>=miswordList.count()) 
	{
//	QMessageBox::information( this,tr("Check Spelling"),tr("The spelling check is complete."));
	accept();
	}
else
	{
	ui.progressBar->setValue(wordcount+1);
	QStringList suggWords;
	line=lineList.at(wordcount);
	if (line!=spellingline) deltacol=0;
	spellingline=line;
	col=colList.at(wordcount)+deltacol;
	misword=miswordList.at(wordcount);
	if (ignoredwordList.contains(misword))
		{
		wordcount+=1;
		spellingNext();
		}
	else
		{
		editor->selectword(line,col,misword);
		ui.lineEditOriginal->setText(misword);
		ui.listWidget->clear();
		ui.lineEditNew->clear();
		if (!suggestionList.at(wordcount).isEmpty())
			{
			suggWords=suggestionList.at(wordcount).split(",");
			ui.listWidget->addItems(suggWords);
			ui.lineEditNew->setText(suggWords.at(0));
			}
		}
	}
}

void SpellerDialog::slotIgnore()
{
ignoredwordList.append(ui.lineEditOriginal->text());
wordcount+=1;
spellingNext();
}

void SpellerDialog::slotReplace()
{
QString selectedword="";
QTextCursor c = editor->textCursor();
if (c.hasSelection()) selectedword=c.selectedText();
QString newword=ui.lineEditNew->text();
if (!newword.isEmpty()) 
	{
	deltacol=deltacol+newword.length()-selectedword.length();
	editor->replace(newword);
	}
wordcount+=1;
spellingNext();
}

void SpellerDialog::endProcess(int err)
{
//trouble with aspell on win32 system
// if (err) 
// 	{
// 	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
// 	accept();
// 	}
// else 
emit scanfinished();
}

void SpellerDialog::readOutput()
{
QRegExp rxsug("^&\\s+(.+)\\s+(\\d+)\\s+(\\d+):\\s+(.+)");
QRegExp rxnosug("^#\\s+(.+)\\s+(\\d+)");
QByteArray result=spellproc->readAllStandardOutput();
QString l;
QTextStream stream(&result);
int col,pos;
while (!stream.atEnd()) 
	{
	l = stream.readLine();
	if (rxsug.indexIn(l)>-1)
		{
		col=rxsug.cap(3).toInt()-1;
		pos=editor->getCursorPosition(currentline,col);
		if (pos>=startpos && pos<=endpos)
			{
			lineList.append(currentline);
			colList.append(col);
			miswordList.append(rxsug.cap(1));
			suggestionList.append(rxsug.cap(4).remove(" "));
			}
		}
	else if (rxnosug.indexIn(l)>-1)
		{
		col=rxnosug.cap(2).toInt()-1;
		pos=editor->getCursorPosition(currentline,col);
		if (pos>=startpos && pos<=endpos)
			{
			lineList.append(currentline);
			colList.append(col);
			miswordList.append(rxnosug.cap(1));
			suggestionList.append("");
			}
		}
	else if (l.isEmpty()) 
		{
		currentline+=1;
		if (progressDialog) progressDialog->setValue(currentline);
		}
	}
if (currentline==numlines) 
	{
#ifdef Q_WS_WIN
	spellproc->kill();
#else
	spellproc->terminate();
#endif
	}
}
