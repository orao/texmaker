/***************************************************************************
 *   copyright       : (C) 2003-2017 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "ui_aboutdialog.h"

class AboutDialog : public QDialog  {
   Q_OBJECT
public:
	AboutDialog(QWidget *parent=0);
	~AboutDialog();
	Ui::AboutDialog ui;
};


#endif
