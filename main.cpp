/***************************************************************************
 *   copyright       : (C) 2003-2017 by Pascal Brachet                     *
 *   addons by Frederic Devernay <frederic.devernay@m4x.org>               *
 *   addons by Luis Silvestre                                              *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "texmakerapp.h"
#include <QStringList>
#define STRINGIFY_INTERNAL(x) #x
#define STRINGIFY(x) STRINGIFY_INTERNAL(x)

#define VERSION_STR STRINGIFY(TEXMAKERVERSION)

int main( int argc, char ** argv )
{
#if !defined(Q_OS_MAC)
QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
TexmakerApp app("TexMaker", argc, argv );
QStringList args = QCoreApplication::arguments();
app.setApplicationName("TexMaker");
app.setApplicationVersion(VERSION_STR);
app.setOrganizationName("xm1");
app.setAttribute(Qt::AA_DontShowIconsInMenus, true);
app.setAttribute(Qt::AA_UseHighDpiPixmaps);

#if defined(Q_OS_UNIX) && !defined(Q_OS_MAC)
QApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
#endif

if ( app.isRunning() && !args.contains("-n")) 
    {
    QString msg;
    msg = args.join("#!#");
    msg += "#!#";
    app.sendMessage( msg );
    return 0;
    }

app.init(args); // Initialization takes place only if there is no other instance running.

QObject::connect( &app, SIGNAL( messageReceived(const QString &) ), 
                  app.mw,   SLOT( onOtherInstanceMessage(const QString &) ) );

return app.exec();
}
