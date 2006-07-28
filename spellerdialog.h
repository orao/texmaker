/***************************************************************************
 *   copyright       : (C) 2006 by Pascal Brachet                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SPELLERDIALOG_H
#define SPELLERDIALOG_H

#include "ui_spellerdialog.h"
#include "latexeditor.h"

#include <QProcess>
#include <QPointer>
#include <QProgressDialog>

class SpellerDialog : public QDialog  {
   Q_OBJECT
public:
	SpellerDialog(QWidget *parent=0,LatexEditor *ed=0, QString AspellCommand="", QString lang="", QString encoding="");
	~SpellerDialog();
	Ui::SpellerDialog ui;
private :
QString spell_command, spell_lang, spell_encoding;
LatexEditor *editor;
QPointer<QProcess> spellproc;
int numlines,currentline, wordcount;
QStringList miswordList,suggestionList, ignoredwordList;
QList<int> lineList,colList;
QPointer<QProgressDialog> progressDialog;
int startpos, endpos;
protected:
void closeEvent( QCloseEvent* );
private slots:
void accept();
void scanDocument();
void spellingInit();
void spellingNext();
void endProcess(int err);
void readOutput();
void slotIgnore();
void slotReplace();
void updateItem();
signals:
void scanfinished();
};


#endif
