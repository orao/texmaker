/***************************************************************************
 *   copyright       : (C) 2003-2007 by Pascal Brachet                     *
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
ui.textBrowser->setHtml(QString::fromUtf8("<b>Texmaker 1.8</b><br><b>Copyright (c) 2004-2008 by Pascal Brachet</b><br><i>with the contribution of Joël Amblard (html conversion).</i><br>Texmaker contains code from the Hunspell program (GPL).<br>Texmaker uses the DSingleApplication class (Author: Dima Fedorov Levit - Copyright (C) BioImage Informatics - Licence: GPL)<br>Thanks to Frederic Devernay, Denis Bitouzé, Jean-Côme Charpentier & Luis Silvestre.<br><br>Project home site : <a href=\"http://www.xm1math.net/texmaker/\">http://www.xm1math.net/texmaker/</a><br><br>This program is licensed to you under the terms of the GNU General Public License Version 2 as published by the Free Software Foundation."));
}

AboutDialog::~AboutDialog(){
}
