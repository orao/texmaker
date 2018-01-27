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

#ifndef ARRAYDIALOG_H
#define ARRAYDIALOG_H

#include "ui_arraydialog.h"

class ArrayDialog : public QDialog  {
   Q_OBJECT
public:
	ArrayDialog(QWidget *parent=0, const char *name=0);
	~ArrayDialog();
	Ui::ArrayDialog ui;

protected slots:
  void NewRows(int num);
  void NewColumns(int num);
};


#endif
