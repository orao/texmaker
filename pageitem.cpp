/***************************************************************************
 *   copyright       : (C) 2012 by Pascal Brachet                          *
 *   http://www.xm1math.net/texmaker/                                      *
 *   based on qpdfview  Copyright 2012 Adam Reichold GPL                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/*

Copyright 2012 Adam Reichold

This file is part of qpdfview.

modified by Pascal Brachet

*/

#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>
#include <QApplication>
#include <QToolTip>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtConcurrent>
#endif
#include "pageitem.h"

QCache< PageItem*, QImage > PageItem::s_cache(32 * 1024 * 1024);

bool PageItem::s_decoratePages = true;
bool PageItem::s_decorateLinks = false;

bool PageItem::s_invertColors = false;

int PageItem::cacheSize()
{
    return s_cache.totalCost();
}

void PageItem::setCacheSize(int cacheSize)
{
    s_cache.setMaxCost(cacheSize);
}

bool PageItem::decoratePages()
{
    return s_decoratePages;
}

void PageItem::setDecoratePages(bool decoratePages)
{
    s_decoratePages = decoratePages;
}

bool PageItem::decorateLinks()
{
    return s_decorateLinks;
}

void PageItem::setDecorateLinks(bool decorateLinks)
{
    s_decorateLinks = decorateLinks;
}

bool PageItem::invertColors()
{
    return s_invertColors;
}

void PageItem::setInvertColors(bool invertColors)
{
    s_invertColors = invertColors;
}

PageItem::PageItem(QMutex* mutex, Poppler::Page* page, int index, QGraphicsItem* parent) : QGraphicsObject(parent),
    m_mutex(0),
    m_page(0),
    m_index(-1),
    m_size(),
    m_links(),
    m_highlights(),
    m_rubberBand(),
    m_physicalDpiX(72),
    m_physicalDpiY(72),
    m_scaleFactor(1.0),
    m_rotation(Poppler::Page::Rotate0),
    m_transform(),
    m_normalizedTransform(),
    m_boundingRect(),
    m_image(),
    m_render(0)
{
    setAcceptHoverEvents(true);
    setCursor(Qt::ArrowCursor);
    highlightRemover.setSingleShot(true);
    connect(&highlightRemover, SIGNAL(timeout()), this, SLOT(clearHighlight()));    
    
    m_render = new QFutureWatcher< void >(this);
    connect(m_render, SIGNAL(finished()), SLOT(on_render_finished()));

    connect(this, SIGNAL(imageReady(QImage,bool)), SLOT(on_imageReady(QImage,bool)));

    m_mutex = mutex;
    m_page = page;

    m_index = index;
    m_size = m_page->pageSizeF();

    foreach(Poppler::Link* link, m_page->links())
    {
        if(link->linkType() == Poppler::Link::Goto)
        {
            if(!static_cast< Poppler::LinkGoto* >(link)->isExternal())
            {
                m_links.append(link);
                continue;
            }
        }
        else if(link->linkType() == Poppler::Link::Browse)
        {
            m_links.append(link);
            continue;
        }

        delete link;
    }

    prepareGeometry();
}

PageItem::~PageItem()
{
    m_render->cancel();
    m_render->waitForFinished();

    s_cache.remove(this);

    if(m_page != 0)
    {
        delete m_page;
    }

    qDeleteAll(m_links);
}


void PageItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
if (m_page == 0) return;
QMenu menu;
#if defined(Q_OS_MAC)
QAction *act = menu.addAction(tr("Click to jump to the line")+" (cmd+click)");
#else
QAction *act = menu.addAction(tr("Click to jump to the line")+" (ctrl+click)");
#endif

//act->setShortcut(Qt::CTRL+Qt::Key_Space);
act->setData(QVariant(event->pos()));
connect(act, SIGNAL(triggered()), this, SLOT(jumpToSourceFromPdf()));

act = menu.addAction(tr("Number of words in the document"));
act->setData(QVariant(event->pos()));
connect(act, SIGNAL(triggered()), this, SLOT(requestNumWords()));
act = menu.addAction(tr("Convert page to png image"));
act->setData(QVariant(event->pos()));
connect(act, SIGNAL(triggered()), this, SLOT(requestPngExport()));

menu.exec(event->screenPos());
}

QRectF PageItem::boundingRect() const
{
    return m_boundingRect;
}

void PageItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QImage image;

    if(s_cache.contains(this))
    {
        image = *s_cache.object(this);
    }
    else
    {
        if(!m_image.isNull())
        {
            image = m_image;
            m_image = QImage();

            s_cache.insert(this, new QImage(image), image.byteCount());
        }
        else
        {
            startRender();
        }
    }

    // page
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
if (qApp->devicePixelRatio()==2) image.setDevicePixelRatio(2);
#endif
    if(s_decoratePages)
    {
        painter->fillRect(m_boundingRect, QBrush(s_invertColors ? Qt::black : Qt::white));

        painter->drawImage(m_boundingRect.topLeft(), image);

        painter->setPen(QPen(s_invertColors ? Qt::white : Qt::black));
        painter->drawRect(m_boundingRect);
    }
    else
    {
        painter->drawImage(m_boundingRect.topLeft(), image);
    }

    // links

    if(s_decorateLinks)
    {
        painter->save();

        painter->setTransform(m_normalizedTransform, true);
        painter->setPen(QPen(Qt::red));

        foreach(Poppler::Link* link, m_links)
        {
            painter->drawRect(link->linkArea().normalized());
        }

        painter->restore();
    }

    // highlights

    painter->save();

    painter->setTransform(m_transform, true);

    QColor highlightColor = QApplication::palette().color(QPalette::Highlight);

    highlightColor.setAlpha(127);

    foreach(QRectF highlight, m_highlights)
    {
        painter->fillRect(highlight.normalized(), QBrush(highlightColor));
    }

    QColor selBlendColor=QColor("#FF0000");
    QColor blCol = selBlendColor.dark( 140 );
    blCol.setAlphaF( 0.2 );

    if (!searchPath.isEmpty()) 
      {
      QRect highlightRect = matrix().mapRect(searchPath.boundingRect()).toRect();
      highlightRect.adjust(-3, -2, 3, 0);
//      QImage highlight = image.copy(highlightRect);
//      painter.begin(&image);
      painter->fillRect(highlightRect, blCol);
      painter->setPen( selBlendColor );
      painter->drawRect( highlightRect );
//      painter.end();
      }
    if (!highlightPath.isEmpty()) 
      {
      QRect highlightRect = matrix().mapRect(highlightPath.boundingRect()).toRect();
      highlightRect.adjust(-3, -2, 3, 0);
//      QImage highlight = image.copy(highlightRect);
//      painter.begin(&image);
      painter->fillRect(highlightRect, blCol);
      painter->setPen( selBlendColor );
      painter->drawRect( highlightRect );
//      painter.end();
      }    
    
    painter->restore();

    // rubber band

    if(!m_rubberBand.isNull())
    {
        QPen pen;
        pen.setColor(s_invertColors ? Qt::white : Qt::black);
        pen.setStyle(Qt::DashLine);

        painter->setPen(pen);
        painter->drawRect(m_rubberBand);
    }
}

int PageItem::index() const
{
    return m_index;
}

QSizeF PageItem::size() const
{
    return m_size;
}

const QList< QRectF >& PageItem::highlights() const
{
    return m_highlights;
}

void PageItem::setHighlights(const QList< QRectF >& highlights)
{
    m_highlights = highlights;

    update();
}

int PageItem::physicalDpiX() const
{
    return m_physicalDpiX;
}

int PageItem::physicalDpiY() const
{
    return m_physicalDpiY;
}

void PageItem::setPhysicalDpi(int physicalDpiX, int physicalDpiY)
{
    if(m_physicalDpiX != physicalDpiX || m_physicalDpiY != physicalDpiY)
    {
        refresh();

        m_physicalDpiX = physicalDpiX;
        m_physicalDpiY = physicalDpiY;

        prepareGeometryChange();
        prepareGeometry();
    }
}

qreal PageItem::scaleFactor() const
{
    return m_scaleFactor;
}

void PageItem::setScaleFactor(qreal scaleFactor)
{
    if(m_scaleFactor != scaleFactor)
    {
        refresh();

        m_scaleFactor = scaleFactor;

        prepareGeometryChange();
        prepareGeometry();
    }
}

Poppler::Page::Rotation PageItem::rotation() const
{
    return m_rotation;
}

void PageItem::setRotation(Poppler::Page::Rotation rotation)
{
    if(m_rotation != rotation)
    {
        refresh();

        m_rotation = rotation;

        prepareGeometryChange();
        prepareGeometry();
    }
}

const QTransform& PageItem::transform() const
{
    return m_transform;
}

const QTransform& PageItem::normalizedTransform() const
{
    return m_normalizedTransform;
}

void PageItem::refresh()
{
    cancelRender();

    s_cache.remove(this);

    update();
}

void PageItem::startRender(bool prefetch)
{
    if(!m_render->isRunning())
    {
        m_render->setFuture(QtConcurrent::run(this, &PageItem::render, m_physicalDpiX, m_physicalDpiY, m_scaleFactor, m_rotation, prefetch));
    }
}

void PageItem::cancelRender()
{
    m_render->cancel();

    m_image = QImage();
}

void PageItem::on_render_finished()
{
    update();
}

void PageItem::on_imageReady(QImage image, bool prefetch)
{
    if(s_invertColors)
    {
        image.invertPixels();
    }

    if(!prefetch)
    {
        if(!m_render->isCanceled())
        {
            m_image = image;
        }
    }
    else
    {
        s_cache.insert(this, new QImage(image), image.byteCount());
    }
}

void PageItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    Q_UNUSED(event);
}

void PageItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
    QApplication::restoreOverrideCursor();

    if(event->modifiers() == Qt::NoModifier)
    {
        foreach(Poppler::Link* link, m_links)
        {
            if(m_normalizedTransform.mapRect(link->linkArea().normalized()).contains(event->pos()))
            {
                if(link->linkType() == Poppler::Link::Goto)
                {
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
                    //QToolTip::showText(event->screenPos(), tr("Go to page %1.").arg(static_cast< Poppler::LinkGoto* >(link)->destination().pageNumber()));

                    return;
                }
                else if(link->linkType() == Poppler::Link::Browse)
                {
                    QApplication::setOverrideCursor(Qt::PointingHandCursor);
                    //QToolTip::showText(event->screenPos(), tr("Open %1.").arg(static_cast< Poppler::LinkBrowse* >(link)->url()));

                    return;
                }
            }
        }

    }

    QToolTip::hideText();
}

void PageItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{

    Q_UNUSED(event);
}

void PageItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QApplication::restoreOverrideCursor();
clearPaths();
    if(event->modifiers() == Qt::NoModifier && event->button() == Qt::LeftButton)
    {
        foreach(Poppler::Link* link, m_links)
        {
            if(m_normalizedTransform.mapRect(link->linkArea().normalized()).contains(event->pos()))
            {
                if(link->linkType() == Poppler::Link::Goto)
                {
                    Poppler::LinkGoto* linkGoto = static_cast< Poppler::LinkGoto* >(link);

                    int page = linkGoto->destination().pageNumber();
                    qreal left = linkGoto->destination().isChangeLeft() ? linkGoto->destination().left() : 0.0;
                    qreal top = linkGoto->destination().isChangeTop() ? linkGoto->destination().top() : 0.0;

                    emit linkClicked(page, left, top);

                    event->accept();
                    return;
                }
                else if(link->linkType() == Poppler::Link::Browse)
                {
                    emit linkClicked(static_cast< Poppler::LinkBrowse* >(link)->url());

                    event->accept();
                    return;
                }
            }
        }

    }
    else if((event->modifiers() == Qt::ShiftModifier /*|| event->modifiers() == Qt::ControlModifier*/) && event->button() == Qt::LeftButton)
    {
        QApplication::setOverrideCursor(Qt::CrossCursor);

        m_rubberBand = QRectF(event->pos(), QSizeF());

        update();

        event->accept();
        return;
    }
     else  if ((event->modifiers() & ~Qt::ShiftModifier) == Qt::ControlModifier)
    {
    emit syncpage(m_index, QPointF (event->pos().x(),event->pos().y()));
    }
    else if(event->modifiers() == Qt::NoModifier && event->button() == Qt::RightButton)
    {

    }

    event->ignore();
}

void PageItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(!m_rubberBand.isNull())
    {
        if(m_boundingRect.contains(event->pos()))
        {
            m_rubberBand.setBottomRight(event->pos());

            update();
        }
    }
}

void PageItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QApplication::restoreOverrideCursor();
    if(!m_rubberBand.isNull())
    {
        m_rubberBand = m_rubberBand.normalized();

        if(event->modifiers() == Qt::ShiftModifier)
        {
            copyToClipboard(event->screenPos());
        }


        m_rubberBand = QRectF();

        update();

        event->accept();
    }
    else
    {
        event->ignore();
    }

}

void PageItem::copyToClipboard(const QPoint& screenPos)
{
    QMenu* menu = new QMenu();

    QAction* copyTextAction = menu->addAction(tr("Copy text"));
    QAction* copyImageAction = menu->addAction(tr("Copy image"));

    QAction* action = menu->exec(screenPos);

    if(action == copyTextAction)
    {
        QString text;

        m_mutex->lock();

        text = m_page->text(m_transform.inverted().mapRect(m_rubberBand));

        m_mutex->unlock();

        if(!text.isEmpty())
        {
            QApplication::clipboard()->setText(text);
        }
    }
    else if(action == copyImageAction)
    {
        QImage image;

        m_mutex->lock();

        switch(m_rotation)
        {
        case Poppler::Page::Rotate0:
        case Poppler::Page::Rotate90:
            image = m_page->renderToImage(m_scaleFactor * m_physicalDpiX, m_scaleFactor * m_physicalDpiY, m_rubberBand.x(), m_rubberBand.y(), m_rubberBand.width(), m_rubberBand.height(), m_rotation);
            break;
        case Poppler::Page::Rotate180:
        case Poppler::Page::Rotate270:
            image = m_page->renderToImage(m_scaleFactor * m_physicalDpiY, m_scaleFactor * m_physicalDpiX, m_rubberBand.x(), m_rubberBand.y(), m_rubberBand.width(), m_rubberBand.height(), m_rotation);
            break;
        }

        m_mutex->unlock();

        if(!image.isNull())
        {
            QApplication::clipboard()->setImage(image);
        }
    }

    delete menu;
}



void PageItem::prepareGeometry()
{
    m_transform.reset();
    m_normalizedTransform.reset();

    switch(m_rotation)
    {
    case Poppler::Page::Rotate0:
        break;
    case Poppler::Page::Rotate90:
        m_transform.rotate(90.0);
        m_normalizedTransform.rotate(90.0);
        break;
    case Poppler::Page::Rotate180:
        m_transform.rotate(180.0);
        m_normalizedTransform.rotate(180.0);
        break;
    case Poppler::Page::Rotate270:
        m_transform.rotate(270.0);
        m_normalizedTransform.rotate(270.0);
        break;
    }
qreal extra=1.0;
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
if (qApp->devicePixelRatio()==2) extra=2.0;
#endif
    switch(m_rotation)
    {
    case Poppler::Page::Rotate0:
    case Poppler::Page::Rotate90:
        m_transform.scale(m_scaleFactor * m_physicalDpiX / 72.0/extra, m_scaleFactor * m_physicalDpiY / 72.0/extra);
        m_normalizedTransform.scale(m_scaleFactor * m_physicalDpiX / 72.0/extra * m_size.width(), m_scaleFactor * m_physicalDpiY / 72.0/extra * m_size.height());
        break;
    case Poppler::Page::Rotate180:
    case Poppler::Page::Rotate270:
        m_transform.scale(m_scaleFactor * m_physicalDpiY / 72.0/extra, m_scaleFactor * m_physicalDpiX / 72.0/extra);
        m_normalizedTransform.scale(m_scaleFactor * m_physicalDpiY / 72.0/extra * m_size.width(), m_scaleFactor * m_physicalDpiX / 72.0/extra * m_size.height());
        break;
    }

    m_boundingRect = m_transform.mapRect(QRectF(QPointF(), m_size));
}

void PageItem::render(int physicalDpiX, int physicalDpiY, qreal scaleFactor, Poppler::Page::Rotation rotation, bool prefetch)
{
    QMutexLocker mutexLocker(m_mutex);

    if(m_render->isCanceled() && !prefetch)
    {
        return;
    }

    QImage image;

    switch(rotation)
    {
    case Poppler::Page::Rotate0:
    case Poppler::Page::Rotate90:
        image = m_page->renderToImage(scaleFactor * physicalDpiX, scaleFactor * physicalDpiY, -1, -1, -1, -1, rotation);
        break;
    case Poppler::Page::Rotate180:
    case Poppler::Page::Rotate270:
        image = m_page->renderToImage(scaleFactor * physicalDpiY, scaleFactor * physicalDpiX, -1, -1, -1, -1, rotation);
        break;
    }

    if(m_render->isCanceled() && !prefetch)
    {
        return;
    }

    emit imageReady(image, prefetch);
}

void PageItem::setHighlightPath(const QPainterPath& path)
{
highlightRemover.stop();
highlightPath = path;
if (!path.isEmpty()) 
  {
  update();
  highlightRemover.start(2000);
  }
}

void PageItem::setSearchPath(const QPainterPath& path)
{
searchPath = path;
if (!path.isEmpty()) 
  {
    update();
  }
}

void PageItem::clearHighlight()
{
highlightPath = QPainterPath();
update();
}

void PageItem::clearPaths()
{
searchPath = QPainterPath();
highlightPath = QPainterPath();
update();
}

void PageItem::requestNumWords()
{
emit wantNumWords();
}

void PageItem::requestPngExport()
{
emit wantPngExport(m_index);
}

void PageItem::jumpToSourceFromPdf()
{
QAction *act = qobject_cast<QAction*>(sender());
if (act != NULL) 
  {
    
  QPoint eventPos = act->data().toPoint();
  QPointF pagePos(eventPos.x(),eventPos.y());
   emit syncpage(m_index, pagePos);
  }
}
