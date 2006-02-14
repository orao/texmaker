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

#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QMainWindow>
#include <QTextBrowser>
#include <QToolBar>

class HelpWidget : public QMainWindow
{
    Q_OBJECT
public:
    HelpWidget( const QString home, QWidget* parent = 0, Qt::WFlags flags = 0);
    ~HelpWidget();

private:
QTextBrowser* browser;
QToolBar *browseToolBar;
};

#endif

