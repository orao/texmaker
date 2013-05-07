/***************************************************************************
 *   copyright       : (C) 2003-2009 by Pascal Brachet                     *
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


int main( int argc, char ** argv )
{
TexmakerApp app("TexMaker", argc, argv );

QStringList args = QCoreApplication::arguments();
app.setApplicationName("TexMaker");
app.setApplicationVersion(TEXMAKERVERSION);
app.setOrganizationName("xm1");
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
app.setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif
if ( app.isRunning() ) 
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
