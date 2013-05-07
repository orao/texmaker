/***************************************************************************
 *   copyright       : (C) 2003-2013 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation  either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>
#include "geticon.h"
QIcon getIcon(QString name)
{
QIcon dIcon=QIcon();
QString base=name.remove(".png");
dIcon.addFile(base+".png");
#if (QT_VERSION >= QT_VERSION_CHECK(5, 1, 0))
if (qApp->devicePixelRatio()==2)
{
dIcon.addFile(base+"@2x.png");
}
#endif
return dIcon;
}


