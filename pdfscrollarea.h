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

#ifndef PDFSCROLLAREA_H
#define PDFSCROLLAREA_H

#include <QScrollArea>
#include <QVBoxLayout>
class PdfScrollArea : public QScrollArea
{
Q_OBJECT

public:
    PdfScrollArea( QWidget *parent);
    ~PdfScrollArea();
QWidget *scrollAreaWidgetContents;
QVBoxLayout *verticalLayout;
signals:
  void doScroll(int);
  void doRange();
  void pagezoomOut();
  void pagezoomIn();
protected:
  void wheelEvent( QWheelEvent *e );
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
private slots:
  void scrollChanged(int value);
  void sChanged(int value);
    void rChanged(int min,int max);
public slots:
void setVisible(int x, int y, int margin, int maxv);
void pressHere(QPoint p);
void moveHere(QPoint p);
private:
QPoint scrollPos;
bool handmode;
};

#endif

