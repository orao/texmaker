/***************************************************************************
 *   copyright       : (C) 2003-2012 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "addtagdialog.h"


AddTagDialog::AddTagDialog(QWidget *parent)
    : QDialog( parent)
{
setModal(true);
ui.setupUi(this);
}
AddTagDialog::~AddTagDialog()
{
}

