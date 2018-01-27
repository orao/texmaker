/***************************************************************************
 *   copyright       : (C) 2003-2009 by Pascal Brachet                     *
 *   addons by Luis Silvestre                                              *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QDebug>
#include <QLocale>
#include <QSplashScreen>
#include <QDir>
#include <QFileOpenEvent>
#include <QSettings>
#include "texmakerapp.h"

TexmakerApp *TexmakerApp::theAppInstance = NULL;

TexmakerApp::TexmakerApp(const QString &appId, int & argc, char ** argv ) : QtSingleApplication (appId, argc, argv )
{
mw = NULL;
theAppInstance = this;
language=QString(QLocale::system().name());
}

TexmakerApp::~TexmakerApp()
{
    if (mw) delete mw;
SaveSettings();
}

void TexmakerApp::makeTranslation(const QString &lang)
{
QString locale=lang;
foreach (QTranslator* tr, translatorsList) 
    {
    removeTranslator(tr);
    delete tr;
    }
translatorsList.clear();
QTranslator* appTranslator=new QTranslator(this);
QTranslator* basicTranslator=new QTranslator(this);
#if defined( Q_WS_X11 )
#ifdef USB_VERSION
QString transdir=QCoreApplication::applicationDirPath();
#else
QString transdir=PREFIX"/share/texmaker";
#endif
#endif
#if defined( Q_WS_MACX )
QString transdir=QCoreApplication::applicationDirPath() + "/../Resources";
#endif
#if defined(Q_WS_WIN)
QString transdir=QCoreApplication::applicationDirPath();
#endif
if ( locale.length() < 2 ) locale = "en";
if (appTranslator->load(QString("texmaker_")+locale,transdir)) 	
    {
    installTranslator(appTranslator);
    translatorsList.append(appTranslator);
    }
if (basicTranslator->load(QString("qt_")+locale,transdir)) 
    {
    installTranslator(basicTranslator);
    translatorsList.append(basicTranslator);
    }
}
void TexmakerApp::init( QStringList args )
{
QPixmap pixmap(":/images/splash.png");
QSplashScreen *splash = new QSplashScreen(pixmap);
splash->show();
ReadSettings();
makeTranslation(language);
mw = new Texmaker();
connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
splash->finish(mw);
delete splash;
#if defined( Q_WS_MACX )
if (!MacFile.isEmpty()) mw->load(MacFile);
#endif
for (QStringList::Iterator it = ++(args.begin()); it != args.end(); it++)
    {
    if ( (*it)[0] != '-') mw->load( *it );
    else if ( *it == "-master" ) mw->ToggleMode();
    else if ( ( *it == "-line") && (++it != args.end())) mw->setLine( *it );
    }
}


#if defined( Q_WS_MACX )
bool TexmakerApp::event ( QEvent * event )
{
if (event->type() == QEvent::FileOpen) 
    {
    QFileOpenEvent *oe = static_cast<QFileOpenEvent *>(event);
    if (mw) mw->load(oe->file());
    else MacFile=oe->file();
    }
return QApplication::event(event);
}
#endif

void TexmakerApp::ReadSettings()
{
#ifdef USB_VERSION
QSettings *config=new QSettings(QCoreApplication::applicationDirPath()+"/texmakerapp.ini",QSettings::IniFormat); //for USB-stick version
#else
QSettings *config=new QSettings(QSettings::IniFormat,QSettings::UserScope,"xm1","texmakerapp");
#endif

language=config->value("Language",QString(QLocale::system().name())).toString();
}

void TexmakerApp::SaveSettings()
{
#ifdef USB_VERSION
QSettings config(QCoreApplication::applicationDirPath()+"/texmakerapp.ini",QSettings::IniFormat); //for USB-stick version 
#else
QSettings config(QSettings::IniFormat,QSettings::UserScope,"xm1","texmakerapp");
#endif
config.setValue( "Language",language);
}
