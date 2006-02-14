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

#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QSplashScreen>
#include <QDir>
#include "texmaker.h"

int main( int argc, char ** argv )
{
QApplication a( argc, argv );
QPixmap pixmap(":/images/splash.png");
QSplashScreen *splash = new QSplashScreen(pixmap);
splash->show();
QTranslator appTranslator;
#if defined( Q_WS_X11 )
QString transdir=PREFIX"/share/texmaker";
#endif
#if defined( Q_WS_MACX )
QString transdir="/Applications/texmaker.app/Contents/Resources";
#endif
#if defined(Q_WS_WIN)
QString transdir=QCoreApplication::applicationDirPath();
#endif
appTranslator.load(QString("texmaker_")+QLocale::system().name(),transdir);
a.installTranslator(&appTranslator);
Texmaker * mw = new Texmaker();
a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
splash->finish(mw);
delete splash;
for ( int i = 1; i < a.argc(); ++i )
	{
	QString arg = a.argv()[ i ];
	if ( arg[0] != '-' )    mw->load( arg );
	if (( arg == "-line" ) && (i<a.argc()-1))  mw->setLine( a.argv()[ ++i ] );
	}
return a.exec();
}
