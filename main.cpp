/***************************************************************************
 *   copyright       : (C) 2003-2007 by Pascal Brachet                     *
 *   addons by Frederic Devernay <frederic.devernay@m4x.org>               *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QSplashScreen>
#include <QDir>
#include <QFileOpenEvent>
#include "texmaker.h"

class TexmakerApp : public QApplication
{
private:
    Texmaker *mw;
protected:
    bool event(QEvent *event);
public:
    TexmakerApp( int & argc, char ** argv );
    ~TexmakerApp();
};

TexmakerApp::TexmakerApp( int & argc, char ** argv ) : QApplication ( argc, argv )
{
QPixmap pixmap(":/images/splash.png");
QSplashScreen *splash = new QSplashScreen(pixmap);
splash->show();
QTranslator* appTranslator=new QTranslator(this);
#if defined( Q_WS_X11 )
QString transdir=PREFIX"/share/texmaker";
#endif
#if defined( Q_WS_MACX )
QString transdir=QCoreApplication::applicationDirPath() + "/../Resources";
#endif
#if defined(Q_WS_WIN)
QString transdir=QCoreApplication::applicationDirPath();
#endif
QString locale = QString(QLocale::system().name()).left(2);
if ( locale.length() < 2 ) locale = "en";
if (appTranslator->load(QString("texmaker_")+locale,transdir)) installTranslator(appTranslator);
mw = new Texmaker();
connect( this, SIGNAL( lastWindowClosed() ), this, SLOT( quit() ) );
splash->finish(mw);
delete splash;
for ( int i = 1; i < argc; ++i )
	{
	QString arg = argv[ i ];
	if ( arg[0] != '-' )    mw->load( arg );
	if ( arg == "-master" ) mw->ToggleMode();
	if (( arg == "-line" ) && (i<argc-1))  mw->setLine( argv[ ++i ] );
	}
}

TexmakerApp::~TexmakerApp()
{
    delete mw;
}

bool TexmakerApp::event ( QEvent * event )
{
    if (event->type() == QEvent::FileOpen) {
        QFileOpenEvent *oe = static_cast<QFileOpenEvent *>(event);
        mw->load(oe->file());
    }
    return QApplication::event(event);
}

int main( int argc, char ** argv )
{
TexmakerApp a( argc, argv );
return a.exec();
}
