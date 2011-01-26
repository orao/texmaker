/***************************************************************************
 *   copyright       : (C) 2003-2011 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PAPERDIALOG_H
#define PAPERDIALOG_H

#include "ui_paperdialog.h"



class PaperDialog : public QDialog  {
   Q_OBJECT
public:
	PaperDialog(QWidget *parent=0, const char *name=0);
	~PaperDialog();
	Ui::PaperDialog ui;

};


#endif
