/****************************************************************************
**
**   copyright       : (C) 2003-2011 by Pascal Brachet                     
**   http://www.xm1math.net/texmaker/                                      
**
** addons by Adrián Yanes Martínez
** Parts of this file come from the documentation of Qt. It was originally
** published as part of Qt Quarterly.
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
**
****************************************************************************/


#include <QtGui>
#include <QDebug>
#include <QScrollArea>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include "pdfdocumentwidget.h"

PdfDocumentWidget::PdfDocumentWidget(QWidget *parent, int page,Poppler::Page *dp)
    : QLabel(parent)
{
    numPage=page;
    doc_page = dp;
    scaleFactor = 1.0;
    setAlignment(Qt::AlignCenter);
    currentLink=0;
    setMouseTracking(true);
    highlightRemover.setSingleShot(true);
    connect(&highlightRemover, SIGNAL(timeout()), this, SLOT(clearHighlight()));
    handmode=false;
}

PdfDocumentWidget::~PdfDocumentWidget()
{
    delete doc_page;
}

/*Poppler::Page *PdfDocumentWidget::document_page()
{
    return doc_page;
}*/

QMatrix PdfDocumentWidget::matrix() const
{
    return QMatrix(scaleFactor * physicalDpiX() / 72.0, 0,
                   0, scaleFactor * physicalDpiY() / 72.0,
                   0, 0);
}

void PdfDocumentWidget::mousePressEvent(QMouseEvent *event)
{
if (!doc_page) return;
currentLink=0;
clearPaths();
updatePixmap();
//setPage();

foreach (Poppler::Link* link, doc_page->links())
  {
  QPointF scaledPos((event->pos().x()- (width() - pixmap()->width()) / 2.0) / scaleFactor / physicalDpiX() * 72.0 / doc_page->pageSizeF().width(),(event->pos().y()- (height() - pixmap()->height()) / 2.0) / scaleFactor / physicalDpiY() * 72.0 / doc_page->pageSizeF().height());
  if (link->linkArea().contains(scaledPos)) 
    {
    currentLink = link;
    break;
    }
  }
  if (currentLink==0)
  {
  setCursor(Qt::ClosedHandCursor);
  QApplication::setOverrideCursor(Qt::ClosedHandCursor);
  handmode=true;
  emit pressOnPoint(event->globalPos());
  }
  else setCursor(Qt::ArrowCursor);
}

void PdfDocumentWidget::mouseMoveEvent(QMouseEvent *event)
{
if (!doc_page) return;
if (handmode) {setCursor(Qt::ClosedHandCursor);emit moveOnPoint(event->globalPos());}
else {setCursor(Qt::ArrowCursor);}
foreach (Poppler::Link* link, doc_page->links())
  {
  QPointF scaledPos((event->pos().x()- (width() - pixmap()->width()) / 2.0) / scaleFactor / physicalDpiX() * 72.0 / doc_page->pageSizeF().width(),(event->pos().y()- (height() - pixmap()->height()) / 2.0) / scaleFactor / physicalDpiY() * 72.0 / doc_page->pageSizeF().height());
  if (link->linkArea().contains(scaledPos)) 
    {
    setCursor(Qt::PointingHandCursor);
    break;
    }
  }

event->accept();
}

void PdfDocumentWidget::mouseReleaseEvent(QMouseEvent *event)
{
if (!doc_page) return;
if (currentLink)
  {
   QPointF scaledPos((event->pos().x()- (width() - pixmap()->width()) / 2.0) / scaleFactor / physicalDpiX() * 72.0 / doc_page->pageSizeF().width(),(event->pos().y()- (height() - pixmap()->height()) / 2.0) / scaleFactor / physicalDpiY() * 72.0 / doc_page->pageSizeF().height());
    if ((currentLink->linkArea().contains(scaledPos)) && (currentLink->linkType()==Poppler::Link::Goto))
      {
      const Poppler::LinkGoto *gotoLink = dynamic_cast<const Poppler::LinkGoto*>(currentLink);
      if (gotoLink)
	{
	const Poppler::LinkDestination dest = gotoLink->destination();
	int destPage=0;
	int destLeft=0;
	int destTop=0;
	if ((dest.pageNumber() > 0)) destPage=dest.pageNumber();
	if (dest.isChangeLeft()) destLeft = (int)floor(dest.left() * scaleFactor * physicalDpiX() / 72.0 * doc_page->pageSizeF().width());
	if (dest.isChangeTop()) destTop = (int)floor(dest.top() * scaleFactor * physicalDpiY() / 72.0 * doc_page->pageSizeF().height());
	emit gotoDest(destPage,destLeft,destTop);
	} 
      }
  }
currentLink=0;
handmode=false;
setCursor(Qt::ArrowCursor);
QApplication::restoreOverrideCursor();
if ((event->modifiers() & ~Qt::ShiftModifier) == Qt::ControlModifier)
{
  QPointF pagePos((event->pos().x()- (width() - pixmap()->width()) / 2.0) / scaleFactor / physicalDpiX() * 72.0,(event->pos().y()- (height() - pixmap()->height()) / 2.0) / scaleFactor / physicalDpiY() * 72.0 );
  emit syncpage(numPage, pagePos);
}

}

void PdfDocumentWidget::contextMenuEvent(QContextMenuEvent *event)
{
if (!doc_page) return;
handmode=false;
setCursor(Qt::ArrowCursor);
QApplication::restoreOverrideCursor();
QMenu *menu = new QMenu(this);
QAction *act = new QAction(tr("Click to jump to the line")+" (ctrl+clic)", menu);
//act->setShortcut(Qt::CTRL+Qt::Key_Space);
act->setData(QVariant(event->pos()));
connect(act, SIGNAL(triggered()), this, SLOT(jumpToSourceFromPdf()));
menu->addAction(act);
menu->exec(mapToGlobal(event->pos()));
delete menu;
}

qreal PdfDocumentWidget::scale() const
{
    return scaleFactor;
}

void PdfDocumentWidget::updatePixmap()
{
//    QColor selBlendColor=QColor("#43ACE8");
    QColor selBlendColor=QColor("#FF0000");
    QColor blCol = selBlendColor.dark( 140 );
    blCol.setAlphaF( 0.2 );
    QImage image = doc_page->renderToImage(scaleFactor * physicalDpiX(), scaleFactor * physicalDpiY());
    QPainter painter;
    if (!searchPath.isEmpty()) 
      {
      QRect highlightRect = matrix().mapRect(searchPath.boundingRect()).toRect();
      highlightRect.adjust(-3, -2, 3, 0);
      QImage highlight = image.copy(highlightRect);
      painter.begin(&image);
      painter.fillRect(highlightRect, blCol);
      painter.setPen( selBlendColor );
      painter.drawRect( highlightRect );
      painter.end();
      }
    if (!highlightPath.isEmpty()) 
      {
      QRect highlightRect = matrix().mapRect(highlightPath.boundingRect()).toRect();
      highlightRect.adjust(-3, -2, 3, 0);
      QImage highlight = image.copy(highlightRect);
      painter.begin(&image);
      painter.fillRect(highlightRect, blCol);
      painter.setPen( selBlendColor );
      painter.drawRect( highlightRect );
      painter.end();
      }
    setPixmap(QPixmap::fromImage(image));
    emit updateDone(numPage);
}

void PdfDocumentWidget::createblankPixmap(int w, int h)
{
    QPixmap pix=QPixmap(w,h);
    pix.fill(Qt::transparent);
    setPixmap(pix);
}

void PdfDocumentWidget::changeScale(qreal scale)
{
    if (scaleFactor != scale) {
        scaleFactor = scale;
        updatePixmap();
    }
}

void PdfDocumentWidget::setScale(qreal scale)
{
    if (scaleFactor != scale) {
        scaleFactor = scale;
    }
}

void PdfDocumentWidget::jumpToSourceFromPdf()
{
QAction *act = qobject_cast<QAction*>(sender());
if (act != NULL) 
  {
  QPoint eventPos = act->data().toPoint();
  QPointF pagePos((eventPos.x()- (width() - pixmap()->width()) / 2.0) / scaleFactor / physicalDpiX() * 72.0,(eventPos.y()- (height() - pixmap()->height()) / 2.0) / scaleFactor / physicalDpiY() * 72.0 );
  emit syncpage(numPage, pagePos);
  }
}

void PdfDocumentWidget::setHighlightPath(const QPainterPath& path)
{
highlightRemover.stop();
highlightPath = path;
if (!path.isEmpty()) 
  {
  updatePixmap();
  highlightRemover.start(2000);
  }
}

void PdfDocumentWidget::setSearchPath(const QPainterPath& path)
{
searchPath = path;
if (!path.isEmpty()) 
  {
    updatePixmap();
  }
}

void PdfDocumentWidget::clearHighlight()
{
highlightPath = QPainterPath();
updatePixmap();
}

void PdfDocumentWidget::clearPaths()
{
searchPath = QPainterPath();
highlightPath = QPainterPath();
}

