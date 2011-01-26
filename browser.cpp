/***************************************************************************
 *   copyright       : (C) 2003-2010 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
#include "browser.h"

#include <QtGui>


Browser::Browser( const QString home, QWidget* parent, Qt::WFlags flags)
    : QMainWindow( parent, flags )
{
setWindowTitle("Texmaker");
#ifdef Q_WS_MACX
setWindowIcon(QIcon(":/images/logo128.png"));
#else
setWindowIcon(QIcon(":/images/appicon.png"));
#endif
progress = 0;
view = new QWebView(this);
if ( !home.isEmpty()) view->load(QUrl(home));
connect(view, SIGNAL(titleChanged(QString)), SLOT(adjustTitle()));
connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));

QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
fileMenu->addAction(tr("Exit"), this, SLOT(close()));

QToolBar *toolBar = addToolBar("Navigation");
toolBar->addAction(view->pageAction(QWebPage::Back));
toolBar->addAction(view->pageAction(QWebPage::Forward));
toolBar->addAction(view->pageAction(QWebPage::Reload));
toolBar->addAction(view->pageAction(QWebPage::Stop));

setCentralWidget(view);
setUnifiedTitleAndToolBarOnMac(true);
    
//resize(780,580 );
}

Browser::~Browser()
{

}

void Browser::adjustTitle()
{
if (progress <= 0 || progress >= 100)
    setWindowTitle(view->title());
else
    setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
}

void Browser::setProgress(int p)
{
progress = p;
adjustTitle();
}

void Browser::finishLoading(bool)
{
progress = 100;
adjustTitle();
}

