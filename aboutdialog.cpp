/***************************************************************************
 *   copyright       : (C) 2003-2009 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    :QDialog( parent)
{
ui.setupUi(this);
ui.textBrowser->setOpenExternalLinks(true);
ui.textBrowser->setHtml(QString::fromUtf8("<b>Texmaker "TEXMAKERVERSION"</b><br><b>Copyright (c) 2004-2010 by Pascal Brachet</b><br><br>Project home site : <a href=\"http://www.xm1math.net/texmaker/\">http://www.xm1math.net/texmaker/</a><br><br>This program is licensed to you under the terms of the GNU General Public License Version 2 as published by the Free Software Foundation.<br><br>Thanks to Joël Amblard (html conversion), Seyyed Razi Alavizadeh, Frederic Devernay, Denis Bitouzé, Jean-Côme Charpentier, Luis Silvestre, Davide Alberelli, Ilya Barygin, Javier Jardón Cabezas, Felipe Bugno, Carles Gumí).<br>Texmaker contains code from the Hunspell (GPL) and QtCreator (Copyright (C) Nokia - Licence: GPL) programs.<br>Some piece of code has been inspirated by the Ktikz program (Florian Hackenberger and Glad Deschrijver - Licence: GPL)."));
}

AboutDialog::~AboutDialog(){
}
