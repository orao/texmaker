/***************************************************************************
 *   copyright       : (C) 2003-2013 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QApplication>

#include "aboutdialog.h"
#if !defined(OLDPOPPLER)
#include <poppler-config.h>
#endif

AboutDialog::AboutDialog(QWidget *parent)
    :QDialog( parent)
{
ui.setupUi(this);
QPixmap pixmap;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
if (qApp->devicePixelRatio()==2)
{
pixmap.load(":/images/splash@2x.png");
pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
}
else pixmap.load(":/images/splash.png");
#else
pixmap.load(":/images/splash.png");
#endif
ui.label->setPixmap(pixmap);
ui.textBrowser->setOpenExternalLinks(true);
QString head=QString::fromUtf8("<b>Texmaker ")+QLatin1String(TEXMAKERVERSION)+QString::fromUtf8("</b><br>(compiled with Qt ")+QLatin1String(QT_VERSION_STR);
#if !defined(OLDPOPPLER)
head+=QString::fromUtf8(" and Poppler ")+QLatin1String(POPPLER_VERSION);
#endif
ui.textBrowser->setHtml(head+QString::fromUtf8(")<br><b>Copyright (c) 2003-2013 by Pascal Brachet</b><br><br>Project home site : <a href=\"http://www.xm1math.net/texmaker/\">http://www.xm1math.net/texmaker/</a><br><br>This program is licensed to you under the terms of the GNU General Public License Version 2 as published by the Free Software Foundation.<br><br>Thanks to Joël Amblard, Andriy Bandura, Merlin Raschtuttis, Sebastián Vanrell, KLEANTHIS MANOLOPOULOS, Володимир Боденчук, Sandris LACIS, Koutheir ATTOUCHI, Andreas Pettersson, George Boumis , Pedro F. Silva, Nikolić Miroslav, Michele Mastropietro, Peter Axt, Torbjörn Klatt , Tomas Olarte Hernandez , Jan Jełowicki, Tamas Orosz, Adrián Yanes Martínez, Koen Wybo, Pavel Fric, Christian Cenker, Seyyed Razi Alavizadeh, Frederic Devernay, Luis Silvestre, Davide Alberelli, Ilya Barygin, Javier Jardón Cabezas, Felipe Bugno, Carles Gumí, Martin Dreher, Andreas Volk. <br>Texmaker contains code from the Hunspell (GPL), qpdfview ((C) Adam Reichold GPL), QtCreator (Copyright (C) Nokia - Licence: GPL), SyncTeX ( (C) Jerome Laurens - Licence: GPL), Texworks ((C) Jonathan Kew - Licence: GPL), AtD ((C) www.afterthedeadline.com - Licence : LGPL), Tiled (Copyright  Vsevolod Klementjev - Licence: GPL) programs.<br>Some piece of code has been inspirated by the Ktikz ((C) Florian Hackenberger and Glad Deschrijver - Licence: GPL) and CLedit ((C) 2010 Heinz van Saanen -  Licence: GPL) programs .<br>Texmaker contains icons from the K Desktop Environment ((C) KDE ev - Licence: GPL)."));
}

AboutDialog::~AboutDialog(){
}
