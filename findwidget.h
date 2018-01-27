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

#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include "ui_findwidget.h"
#include "latexeditor.h"

class FindWidget : public QWidget
{ 
    Q_OBJECT

public:
    FindWidget(QWidget* parent = 0);
    ~FindWidget();
    Ui::FindWidget ui;
public slots:
    virtual void doFind();
    void SetEditor(LatexEditor *ed);
    void doHide();
private slots:
    void expand(bool e);
    void updateSelection(bool e);
private :
  int startpos, endpos;
protected:
    LatexEditor *editor;
void keyPressEvent ( QKeyEvent * e );
signals:
void requestHide();
void requestExtension();
};

#endif
