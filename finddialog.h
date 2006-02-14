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

#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include "ui_finddialog.h"
#include "latexeditor.h"

class FindDialog : public QDialog
{ 
    Q_OBJECT

public:
    FindDialog(QWidget* parent = 0, const char* name = 0, Qt::WFlags fl = 0 );
    ~FindDialog();
    Ui::FindDialog ui;
public slots:
    virtual void doFind();
    void SetEditor(LatexEditor *ed);

protected:
    LatexEditor *editor;
};

#endif // FINDDIALOG_H
