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

#ifndef LIGHTLINENUMBERWIDGET_H
#define LIGHTLINENUMBERWIDGET_H

#include <QtGui>
#include <QWidget>
#include <QFont>
#include <QPaintEvent>
#include "lightlatexeditor.h"


class LightLineNumberWidget: public QWidget
{
    Q_OBJECT
public:
    LightLineNumberWidget(LightLatexEditor*, QWidget* parent);
    virtual ~LightLineNumberWidget();

public slots:
    void doRepaint() { repaint(); }
    void setFont(QFont efont);

protected:
    virtual bool event(QEvent *event);
    virtual void paintEvent( QPaintEvent* event);
    virtual void mousePressEvent(QMouseEvent *e);

private:
    LightLatexEditor* m_editor;
    QFont numfont;
    int start, end;
};

#endif
