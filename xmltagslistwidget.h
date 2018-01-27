/***************************************************************************
 *   copyright       : (C) 2003-2007 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *   inspired by the ktikz (GPL) program from Glad Deschrijver             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef XMLTAGSLISTWIDGET_H
#define XMLTAGSLISTWIDGET_H

#include <QListWidget>

class QDomElement;

struct xmlTag
    {
    QString txt;
    QString tag;
    QString dx;
    QString dy;
    int type;
    };

struct xmlTagList
    {
    QString title;
    QList<xmlTag> tags;
    QList<xmlTagList> children;
    };

class XmlTagsListWidget : public QListWidget  {
  Q_OBJECT
public:
	XmlTagsListWidget(QWidget *parent, QString file);
private:
xmlTagList getTags(const QDomElement &element);
xmlTagList xmlSections;
void addListWidgetItems(const xmlTagList &tagList);
};

#endif
