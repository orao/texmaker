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
 
#include "helpwidget.h"
#include <QAction>

HelpWidget::HelpWidget( const QString home, QWidget* parent, Qt::WFlags flags)
    : QMainWindow( parent, flags )
{
QAction *Acthome, *Actback, *Actforward;

setWindowIcon(QIcon(":/images/appicon.png"));
setIconSize(QSize(22,22 ));

browser = new QTextBrowser( this );
setCentralWidget( browser );
if ( !home.isEmpty() ) browser->setSource(QUrl(home));

browseToolBar = addToolBar("Browse");
Acthome = new QAction(QIcon(":/images/home.png"), tr("Home"), this);;
connect(Acthome, SIGNAL(triggered()),browser, SLOT(home()));
browseToolBar->addAction(Acthome);

Actback = new QAction(QIcon(":/images/back.png"), tr("Backward"), this);
Actback->setShortcut(Qt::CTRL+Qt::Key_Left);
connect(Actback, SIGNAL(triggered()),browser, SLOT(backward()));
browseToolBar->addAction(Actback);

Actforward = new QAction(QIcon(":/images/forward.png"), tr("Forward"), this);
Actforward->setShortcut(Qt::CTRL+Qt::Key_Right);
connect(Actforward, SIGNAL(triggered()),browser, SLOT(forward()));
browseToolBar->addAction(Actforward);

connect( browser, SIGNAL( backwardAvailable( bool ) ), Actback, SLOT( setEnabled( bool ) ) );
connect( browser, SIGNAL( forwardAvailable( bool ) ), Actforward, SLOT( setEnabled( bool ) ) );

browser->setFocus();
resize(780,580 );
}

HelpWidget::~HelpWidget()
{

}



