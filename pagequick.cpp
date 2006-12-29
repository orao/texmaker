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

#include <QtGui>

#include "pagequick.h"

#include <QFontDatabase>
#include <QTextCodec>
#include <QFileDialog>
#include <QColorDialog>
#include <QDir>

PageQuick::PageQuick(QWidget *parent)
    : QWidget(parent)
{
ui.setupUi(this);
connect(ui.radioButton6, SIGNAL(toggled(bool)),ui.lineEditUserquick, SLOT(setEnabled(bool)));
}
