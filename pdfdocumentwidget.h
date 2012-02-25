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

#ifndef PDFDOCUMENTWIDGET_H
#define PDFDOCUMENTWIDGET_H

#include <QLabel>
#include <QRectF>
#include <QPainterPath>
#include <QTimer>
#include "poppler-qt4.h"

class PdfDocumentWidget : public QLabel
{
    Q_OBJECT

public:
    PdfDocumentWidget(QWidget *parent = 0, int page=0,Poppler::Page *dp=0 );
    ~PdfDocumentWidget();
    Poppler::Page *doc_page;
    QMatrix matrix() const;
    qreal scale() const;
    int numPage;
public slots:
    void updatePixmap();
    void createblankPixmap(int w, int h);
    void setScale(qreal scale);
    void changeScale(qreal scale);
    void setHighlightPath(const QPainterPath& path);
    void setSearchPath(const QPainterPath& path);
    void clearHighlight();
    void clearPaths();


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
signals:
    void syncpage(int, const QPointF&);
    void gotoDest(int,int,int);
    void updateDone(int);
    void pressOnPoint(QPoint p);
    void moveOnPoint(QPoint p);
private slots:
    void jumpToSourceFromPdf();
private:
    Poppler::Link *currentLink;
    int currentPage;
    qreal scaleFactor;
    QPainterPath highlightPath, searchPath;
    QTimer highlightRemover;
    bool handmode;
};

#endif
