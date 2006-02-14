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

#ifndef WEBPUBLISHDIALOG_H
#define WEBPUBLISHDIALOG_H

#include <QVariant>
#include <QProcess>
#include <QMap>
#include <QVector> 
#include "ui_webpublishdialog.h"


typedef  QMap<QString, QString> LinkMap;

class WebPublishDialog : public QDialog  {
   Q_OBJECT
public:
	WebPublishDialog(QWidget *parent=0, const char *name=0);
	~WebPublishDialog();
	Ui::WebPublishDialog ui;

    QString PROGRAMDIR, DVIOPT, ADDRESS, ALIGN, CONTENTNAME, COLORLINK, DEPTH, BASE, TITLE, WORKDIR, HTMLDIR, BROWSER, LASTDIR;
    int NAVIGATION, COMPIL, MAXWIDTH, USERWIDTH, STARTINDEX, COLORDEPTH, TOCDEPTH, nb_pages, nb_content_pages, id_page, x1, y1, x2, y2;
    bool NOINDEX, NOINTERLACE, FINPROCESS, TTWPERROR, ERRPROCESS;

protected:
    void closeEvent( QCloseEvent* );

private slots:
    void init();
    void convert( const QString &fileName );
    void RunCommand(QString comd,bool waitendprocess);
    void SlotEndProcess(int err);
    void copyFile(QString from_file, QString to_file);
    void removeFile(QString file);
    void ps2gif(QString input,QString output,int id_page,int w,int h,int maxw);
    void writepages(QString mode);
    void fatalerror(QString msg);
    void latexerror(QString logfile);
    void clean();
    void proceedSlot();
    void browseSlot();
    void writesettings();
    void readsettings();
    void applyusersettings();

private:
    QString filename;
    QProcess *proc;
    int nbpagesps(QString psfile);
    QString header();
    QString footer();
    QString content_navigation(int page,int numpages,QString up_page);
    QString codepic(QString pic_name, QString map_name);
    LinkMap loc;
    QVector<int> x1box, y1box, x2box, y2box;
};


#endif

