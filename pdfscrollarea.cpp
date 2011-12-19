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
 
#include "pdfscrollarea.h"

#include <QtGui>
#include <QDebug>

PdfScrollArea::PdfScrollArea( QWidget *parent)
    : QScrollArea( parent )
{
setFrameStyle(QFrame::NoFrame);
scrollAreaWidgetContents = new QWidget();
verticalLayout = new QVBoxLayout(scrollAreaWidgetContents);
setWidget(scrollAreaWidgetContents);
connect(horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(sChanged(int)));
connect(verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(scrollChanged(int)));
connect(verticalScrollBar(), SIGNAL(rangeChanged(int,int)), this, SLOT(rChanged(int,int)));
handmode=false;
}

PdfScrollArea::~PdfScrollArea()
{

}

void PdfScrollArea::pressHere(QPoint p)
{
scrollPos = p;

}

void PdfScrollArea::moveHere(QPoint p)
{
QPoint delta = p - scrollPos;
scrollPos = p;
int oldX = horizontalScrollBar()->value();
horizontalScrollBar()->setValue(oldX - delta.x());
int oldY = verticalScrollBar()->value();
verticalScrollBar()->setValue(oldY - delta.y());
}

void PdfScrollArea::mousePressEvent(QMouseEvent *event)
{
QApplication::setOverrideCursor(Qt::ClosedHandCursor);
//setCursor(Qt::ClosedHandCursor);
handmode=true;
pressHere(event->globalPos());
}

void PdfScrollArea::mouseMoveEvent(QMouseEvent *event)
{
if (handmode) {setCursor(Qt::ClosedHandCursor);moveHere(event->globalPos());}
else setCursor(Qt::ArrowCursor);
event->accept();
}

void PdfScrollArea::mouseReleaseEvent(QMouseEvent *event)
{
QApplication::restoreOverrideCursor();
//setCursor(Qt::ArrowCursor);
handmode=false;
}

void PdfScrollArea::wheelEvent( QWheelEvent *e )
{
int delta = e->delta();
int vScroll = verticalScrollBar()->value();
e->accept();
if ( (e->modifiers() & Qt::ControlModifier) == Qt::ControlModifier )
  {
  if ( delta < 0 ) emit pagezoomOut();
  else emit pagezoomIn();  
  }
  
else QAbstractScrollArea::wheelEvent( e );

}

void PdfScrollArea::scrollChanged(int value)
{
//qDebug() << "scroll" << value << verticalScrollBar()->maximum();
emit doScroll(value);
}

void PdfScrollArea::sChanged(int value)
{
//qDebug() << "scroll" << value << horizontalScrollBar()->maximum() << horizontalScrollBar()->pageStep() << horizontalScrollBar()->maximum()+horizontalScrollBar()->pageStep() << viewport()->width();
if ( horizontalScrollBar()->maximum()!=0) emit doHScroll(value);
}

void PdfScrollArea::rChanged(int min,int max)
{
//qDebug() << "range" << max ;
emit doRange();
}

void PdfScrollArea::setVisible(int x, int y, int margin, int maxv)
{
int logicalX = QStyle::visualPos(layoutDirection(), viewport()->rect(), QPoint(x, y)).x();
int xmargin=20;
int ymargin=margin;
if (logicalX - xmargin < horizontalScrollBar()->value()) 
{
    horizontalScrollBar()->setValue(qMax(0, logicalX - xmargin));
} 
else if (logicalX > horizontalScrollBar()->value() + viewport()->width() - xmargin) 
{
    horizontalScrollBar()->setValue(qMin(logicalX - viewport()->width() + xmargin, horizontalScrollBar()->maximum()));
}

if (y - ymargin < verticalScrollBar()->value()) 
{
    verticalScrollBar()->setValue(qMax(0, y - ymargin));
} 
else if (y > verticalScrollBar()->value() + viewport()->height() - ymargin) 
{
  verticalScrollBar()->setValue(qMin(y  - ymargin, maxv));
//    verticalScrollBar()->setValue(qMin(y  - ymargin, verticalScrollBar()->maximum()));
}
}
