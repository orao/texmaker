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

#ifndef PAGECONFIG_H
#define PAGECONFIG_H

#include "ui_pageeditor.h"
#include "ui_pagequick.h"
#include "ui_pagetools.h"
#include <QWidget>

class PageEditor : public QWidget
{ 
    Q_OBJECT

public:
    PageEditor(QWidget* parent = 0);
    Ui::PageEditor ui;
};

class PageQuick : public QWidget
{ 
    Q_OBJECT

public:
    PageQuick(QWidget* parent = 0);
    Ui::PageQuick ui;
};

class PageTools : public QWidget
{ 
    Q_OBJECT

public:
    PageTools(QWidget* parent = 0);
    Ui::PageTools ui;
};
#endif 

