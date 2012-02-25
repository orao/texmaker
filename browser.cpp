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
 
#include "browser.h"

#include <QtGui>
#include <QPrinter>


Browser::Browser( const QString home, bool showToolBar, QWidget* parent, Qt::WFlags flags)
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
fileMenu->addAction(tr("Print"), this, SLOT(Print()));
fileMenu->addSeparator();
fileMenu->addAction(tr("Exit"), this, SLOT(close()));
if (showToolBar)
    {
    QToolBar *toolBar = addToolBar("Navigation");
    toolBar->setIconSize(QSize(22,22 ));
    QAction *Act;
    Act = new QAction(QIcon(":/images/home.png"), tr("Index"), this);
    connect(Act, SIGNAL(triggered()), this, SLOT(Index()));
    toolBar->addAction(Act);
    toolBar->addAction(view->pageAction(QWebPage::Back));
    toolBar->addAction(view->pageAction(QWebPage::Forward));

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    toolBar->addWidget(spacer);
    searchLineEdit = new QLineEdit(toolBar);
    connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT(Find()));
    toolBar->addWidget(searchLineEdit);

    findButton=new QPushButton(tr("Find"),toolBar);
    connect(findButton, SIGNAL(clicked()), this, SLOT(Find()));
    toolBar->addWidget(findButton);
    }
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

void Browser::Index()
{
view->page()->mainFrame()->evaluateJavaScript("window.location.href='#top';");
}

void Browser::Print()
{
QPrinter printer;
QPrintDialog *dialog = new QPrintDialog(&printer, this);
dialog->setWindowTitle(tr("Print"));
if (dialog->exec() != QDialog::Accepted) return;
view->page()->mainFrame()->print(&printer);
}

void Browser::Find()
{
if (searchLineEdit->text().isEmpty()) return;
view->findText(searchLineEdit->text(),QWebPage::FindWrapsAroundDocument);
}
