/****************************************************************************
**
**   copyright       : (C) 2003-2011 by Pascal Brachet                     
**   http://www.xm1math.net/texmaker/                                      
**
** Parts of this file come from Texworks (GPL) Copyright (C) 2007-2010  Jonathan Kew

** Parts of this file come from the documentation of Qt. It was originally
** published as part of Qt Quarterly.
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
**
****************************************************************************/
 
#include "pdfviewer.h"

#include <QtGui>
#include <QAction>
#include <QFontMetrics>
#include <QDebug>
#include <QPrinter>
#include <QPrintDialog>
#include <QSettings>
#include <QTextStream>


#define SYNCTEX_GZ_EXT ".synctex.gz"
#define SYNCTEX_EXT ".synctex"

PdfViewer::PdfViewer( const QString fileName, const QString externalCommand, const QString ghostscriptCommand, const QString psize,QWidget* parent, Qt::WFlags flags)
    : QMainWindow( parent, flags )
{
setWindowTitle("Texmaker : pdf preview");
#ifdef Q_WS_MACX
setWindowIcon(QIcon(":/images/logo128.png"));
#else
setWindowIcon(QIcon(":/images/appicon.png"));
#endif

#ifdef USB_VERSION
QSettings *config=new QSettings(QCoreApplication::applicationDirPath()+"/texmakerpdfview.ini",QSettings::IniFormat); //for USB-stick version :
#else
QSettings *config=new QSettings(QSettings::IniFormat,QSettings::UserScope,"xm1","texmakerpdfview");
#endif
config->beginGroup( "pdfviewer" );
QRect screen = QApplication::desktop()->screenGeometry();
int w= config->value( "Geometries/MainwindowWidth",screen.width()/2).toInt();
int h= config->value( "Geometries/MainwindowHeight",screen.height()-150).toInt() ;
int x= config->value( "Geometries/MainwindowX",screen.width()/2).toInt();
int y= config->value( "Geometries/MainwindowY",30).toInt() ;
resize(w,h);
move(x,y);
windowstate=config->value("MainWindowState").toByteArray();
config->endGroup();

setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
canDisplayPixmap=false;
pdf_file=fileName;
viewpdf_command=externalCommand;
gswin32c_command=ghostscriptCommand;
paper_size=psize;
lastFile=fileName;
lastPage=1;
fileLoaded=false;
currentPage=1;
doc = 0;
scanner=NULL;
path= QPainterPath();
deltaMax=10;

StructureView = new QDockWidget(this);
StructureView->setObjectName("StructureView");
StructureView->setAllowedAreas(Qt::AllDockWidgetAreas);
StructureView->setFeatures(QDockWidget::DockWidgetClosable);
StructureView->setWindowTitle(tr("Structure"));
addDockWidget(Qt::LeftDockWidgetArea, StructureView);

QFrame *LeftPanelFrame=new QFrame(StructureView);
LeftPanelFrame->setLineWidth(0);
LeftPanelFrame->setFrameShape(QFrame::Box);
LeftPanelFrame->setFrameShadow(QFrame::Plain);
LeftPanelFrame->setMinimumWidth(100);

LeftPanelLayout= new QHBoxLayout(LeftPanelFrame);
LeftPanelToolBar=new QToolBar("StructureToolBar",StructureView);
LeftPanelToolBar->setFloatable(false);
LeftPanelToolBar->setOrientation(Qt::Vertical);
LeftPanelToolBar->setMovable(false);
LeftPanelToolBar->setIconSize(QSize(16,16 ));
LeftPanelStackedWidget=new QStackedWidget(StructureView);

listpagesWidget=new QListWidget(LeftPanelStackedWidget);

connect(LeftPanelToolBar->addAction(QIcon(":/images/pages.png"),tr("Structure")), SIGNAL(triggered()), this, SLOT(ShowListPages()));
LeftPanelStackedWidget->addWidget(listpagesWidget);

LeftPanelToolBar->addSeparator();

StructureTreeWidget=new QTreeWidget(LeftPanelStackedWidget);
StructureTreeWidget->setColumnCount(1);
StructureTreeWidget->header()->hide();
StructureTreeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
StructureTreeWidget->header()->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
StructureTreeWidget->header()->setResizeMode(0, QHeaderView::ResizeToContents);
StructureTreeWidget->header()->setStretchLastSection(false);


connect(LeftPanelToolBar->addAction(QIcon(":/images/structure.png"),tr("Structure")), SIGNAL(triggered()), this, SLOT(ShowStructure()));
LeftPanelStackedWidget->addWidget(StructureTreeWidget);

LeftPanelLayout->setSpacing(0);
LeftPanelLayout->setMargin(0);
LeftPanelLayout->addWidget(LeftPanelToolBar);
LeftPanelLayout->addWidget(LeftPanelStackedWidget);
StructureView->setWidget(LeftPanelFrame);
LeftPanelStackedWidget->setCurrentWidget(listpagesWidget);


QFrame *centralFrame=new QFrame(this);
centralFrame->setLineWidth(0);
centralFrame->setFrameShape(QFrame::StyledPanel);
centralFrame->setFrameShadow(QFrame::Plain);
centralFrame->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

scrollArea=new PdfScrollArea(centralFrame);
scrollArea->setWidgetResizable(false);
scrollArea->verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
scrollArea->verticalLayout->setSpacing(10);
scrollArea->verticalLayout->setMargin(0);
scrollArea->setAlignment(Qt::AlignCenter);


CentralLayout= new QHBoxLayout(centralFrame);
CentralLayout->setSpacing(0);
CentralLayout->setMargin(0);
CentralLayout->addWidget(scrollArea);

setCentralWidget(centralFrame);


QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
//printAct = new QAction(QIcon(":/images/print.png"), tr("Print"), this);
//connect(printAct, SIGNAL(triggered()), this, SLOT(printPdf()));
//fileMenu->addAction(printAct);
fileMenu->addAction(tr("Exit"), this, SLOT(close()),Qt::CTRL+Qt::Key_Q);

QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
findAct = new QAction( tr("Find"), this);
findAct->setShortcut(Qt::CTRL+Qt::Key_F);
editMenu->addAction(findAct);



QToolBar *toolBar = addToolBar("Pdf controls");
toolBar->setObjectName("Pdf controls");

upAct = new QAction(QIcon(":/images/up.png"), tr("Previous"), this);
toolBar->addAction(upAct);

downAct = new QAction(QIcon(":/images/down.png"), tr("Next"), this);
toolBar->addAction(downAct);

toolBar->addSeparator();

QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
fitWithAct = new QAction(QIcon(":/images/zoom-fit-width.png"), tr("Fit Width"), this);
toolBar->addAction(fitWithAct);

fitPageAct = new QAction(QIcon(":/images/zoom-fit-best.png"), tr("Fit Page"), this);
toolBar->addAction(fitPageAct);

zoominAct = new QAction(QIcon(":/images/zoom-in.png"), tr("Zoom In"), this);
zoominAct->setShortcut(QKeySequence::ZoomIn);
toolBar->addAction(zoominAct);
viewMenu->addAction(zoominAct);

zoomoutAct = new QAction(QIcon(":/images/zoom-out.png"), tr("Zoom Out"), this);
zoomoutAct->setShortcut(QKeySequence::ZoomOut);
toolBar->addAction(zoomoutAct);
viewMenu->addAction(zoomoutAct);




scaleComboBox = new QComboBox(toolBar);
zoomCustom = new QLineEdit();
QValidator *validatorZoom = new QRegExpValidator(QRegExp("\\d{0,3}%?"), this);
zoomCustom->setValidator(validatorZoom);
scaleComboBox->setInsertPolicy(QComboBox::NoInsert);
scalePercents << "25%" << "50%" << "75%" << "100%" << "125%" << "150%" << "200%" << "300%" << "400%";
scaleComboBox->addItems(scalePercents);
scaleComboBox->setCurrentIndex(3);
scaleComboBox->setEditable(true);
scaleComboBox->setLineEdit(zoomCustom);
toolBar->addWidget(scaleComboBox);

toolBar->addSeparator();

searchLineEdit = new QLineEdit(toolBar);
toolBar->addWidget(searchLineEdit);

findButton=new QPushButton(tr("Find"),toolBar);
toolBar->addWidget(findButton);

toolBar->addSeparator();
QWidget* spacer = new QWidget();
spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
toolBar->addWidget(spacer);

printButton=new QPushButton(tr("Print"),toolBar);
printButton->setShortcut(Qt::CTRL+Qt::Key_P);
connect(printButton, SIGNAL(clicked()), this, SLOT(printPdf()));
toolBar->addWidget(printButton);



externButton=new QPushButton(tr("External Viewer"),toolBar);
connect(externButton, SIGNAL(clicked()), this, SLOT(runExternalViewer()));
toolBar->addWidget(externButton);

searchLineEdit->setEnabled(false);
findButton->setEnabled(false);
scaleComboBox->setEnabled(false);
externButton->setEnabled(false);
upAct->setEnabled(false);
downAct->setEnabled(false);
fitWithAct->setEnabled(false);
fitPageAct->setEnabled(false);
zoominAct->setEnabled(false);
zoomoutAct->setEnabled(false);
connectActions();
restoreState(windowstate, 0);
openFile(pdf_file,viewpdf_command,gswin32c_command);
}

PdfViewer::~PdfViewer()
{
if (scanner != NULL) synctex_scanner_free(scanner);
if (proc && proc->state()==QProcess::Running) 
	{
	proc->kill(); 
	delete proc ;
	}
}

void PdfViewer::closeEvent(QCloseEvent *e)
{
#ifdef USB_VERSION
QSettings config(QCoreApplication::applicationDirPath()+"/texmakerpdfview.ini",QSettings::IniFormat); //for USB-stick version 
#else
QSettings config(QSettings::IniFormat,QSettings::UserScope,"xm1","texmakerpdfview");
#endif
config.beginGroup( "pdfviewer" );
config.setValue("MainWindowState",saveState(0));
config.setValue("Geometries/MainwindowWidth", width() );
config.setValue("Geometries/MainwindowHeight", height() );
config.setValue("Geometries/MainwindowX", x() );
config.setValue("Geometries/MainwindowY", y() );
config.endGroup();
}

void PdfViewer::openFile(QString fn,QString ec,QString pc)
{
show();
if (windowState()==Qt::WindowMinimized) setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
qApp->setActiveWindow(this);
activateWindow();

path= QPainterPath();
altern=1;
currentScale=1;
show();
pdf_file=fn;
viewpdf_command=ec;
gswin32c_command=pc;
int lpage=lastPage;
if (scanner != NULL) 
  {
  synctex_scanner_free(scanner);
  scanner = NULL;
  }
Poppler::Document *oldDocument = doc;
doc = Poppler::Document::load(fn);
if (doc) 
  {
  delete oldDocument;
  doc->setRenderHint(Poppler::Document::Antialiasing);
  doc->setRenderHint(Poppler::Document::TextAntialiasing);
  searchLocation = QRectF();
  }
if (doc!=0) 
    {
    searchLineEdit->setEnabled(true);
    findButton->setEnabled(true);
    scaleComboBox->setEnabled(true);
    externButton->setEnabled(true);
    upAct->setEnabled(false);
    downAct->setEnabled(true);
    fitWithAct->setEnabled(true);
    fitPageAct->setEnabled(true);
    zoominAct->setEnabled(true);
    zoomoutAct->setEnabled(true);
    StructureTreeWidget->clear();
    const QDomDocument *toc = doc->toc();
    if (toc) 
      {
      ParseToc(*toc, StructureTreeWidget, 0);
      delete toc;
      } 
    QString syncFile = QFileInfo(fn).canonicalFilePath();
    scanner = synctex_scanner_new_with_output_file(syncFile.toUtf8().data(), NULL, 1);
    initPages();
    if ((fn==lastFile) && (lpage <= doc->numPages()) && (lpage>0))
      {
      currentPage=lpage;
      }
    else 
      {
      currentPage=1;
      lastPage=1;
      }
    fileLoaded=true;
    lastFile=fn;
    gotoPage(currentPage);
    } 
    else 
      {
      QMessageBox::warning( this,tr("Error"),tr("File not found"));
      fileLoaded=false;
      }
}

void PdfViewer::initPages()
{
canDisplayPixmap=false;
disconnectActions();
qDeleteAll(listPdfWidgets);
disconnect(scrollArea, SIGNAL(doRange()), this, SLOT(setScrollMax()));
disconnect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
listpagesWidget->clear();
listPdfWidgets.clear();
listPdfWidgetsPos.clear();
listPdfWidgetsStatus.clear();
PdfDocumentWidget *pdfWidget;
int pos=0;
pdfWidget = new PdfDocumentWidget(0,0,doc->page(0));
pdfWidget->updatePixmap();
qreal portWidth = scrollArea->viewport()->width();
QSizeF pageSize = doc->page(0)->pageSizeF();
if (pageSize.width()!=0) currentScale = (portWidth-30) / pageSize.width()*72.0/pdfWidget->physicalDpiX();
if (currentScale < 0.25) currentScale = 0.25;
else if (currentScale > 4) currentScale = 4;
pdfWidget->setScale(currentScale);
pdfWidget->updatePixmap();
zoomCustom->setText(QString::number(int(currentScale*100)) + "%");
int widthpix=(int) pdfWidget->pixmap()->width();
int heightpix=(int) pdfWidget->pixmap()->height();
if (pdfWidget) delete pdfWidget;
  for (int i = 0; i < doc->numPages(); ++i)
  {
  listPdfWidgetsStatus.append(0);
  listpagesWidget->addItem(tr("Page")+" "+QString::number(i+1)); 
  pdfWidget = new PdfDocumentWidget(scrollArea,i,doc->page(i));
  pdfWidget->setScale(currentScale);
  connect(pdfWidget, SIGNAL(syncpage(int, const QPointF&)), this, SLOT(jumpToEditor(int, const QPointF&)));
  connect(pdfWidget, SIGNAL(updateDone(int)), this, SLOT(updatePageStatus(int)));
  connect(pdfWidget, SIGNAL(gotoDest(int,int,int)), this, SLOT(jumpToDest(int,int,int)));
  pdfWidget->createblankPixmap(1,heightpix);
  listPdfWidgets.append(pdfWidget);
  listPdfWidgetsPos.append(pos);
  pos+=(int) pdfWidget->pixmap()->height()+scrollArea->verticalLayout->spacing();
  scrollArea->verticalLayout->addWidget(pdfWidget);
  }
scrollArea->viewport()->update();
if (scrollArea->verticalScrollBar()->pageStep()<pos) scrollMax=pos-scrollArea->verticalScrollBar()->pageStep()+scrollArea->verticalLayout->spacing();
else scrollMax=pos;
if (scrollArea->viewport()->height()>pos) scrollMax=0;
scrollArea->verticalScrollBar()->setMaximum(scrollMax);
scrollArea->horizontalScrollBar()->setMaximum(widthpix);
connect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
connect(scrollArea, SIGNAL(doRange()), this, SLOT(setScrollMax()));
connectActions();
deltaMax=10;
if (heightpix!=0) deltaMax=(int) (scrollArea->viewport()->height()/heightpix)+2;
canDisplayPixmap=true;
scrollArea->setFocus();
}

void PdfViewer::initPagesWithNewScale()
{
canDisplayPixmap=false;
for (int i = 0; i < doc->numPages(); ++i) listPdfWidgetsStatus.replace(i,0);
PdfDocumentWidget *pdfWidget;
pdfWidget = new PdfDocumentWidget(0,0,doc->page(0));
pdfWidget->setScale(currentScale);
pdfWidget->updatePixmap();
int widthpix=(int) pdfWidget->pixmap()->width();
int heightpix=(int) pdfWidget->pixmap()->height();
if (pdfWidget) delete pdfWidget;
disconnectActions();
int pos=0;
int w=0;
for (int i = 0; i < doc->numPages(); ++i)
  {
  listPdfWidgetsStatus.replace(i,0);
  listPdfWidgets.at(i)->clearPaths();
  listPdfWidgets.at(i)->setScale(currentScale);
  listPdfWidgets.at(i)->createblankPixmap(1,heightpix);
  listPdfWidgetsPos.replace(i,pos);
  pos+=(int) (listPdfWidgets.at(i)->pixmap()->height()+scrollArea->verticalLayout->spacing());
  }
scrollArea->viewport()->update();
if (scrollArea->verticalScrollBar()->pageStep()<pos) scrollMax=pos-scrollArea->verticalScrollBar()->pageStep()+scrollArea->verticalLayout->spacing();
else scrollMax=pos;
if (scrollArea->viewport()->height()>pos) scrollMax=0;
scrollArea->verticalScrollBar()->setMaximum(scrollMax);
scrollArea->horizontalScrollBar()->setMaximum(widthpix);
connectActions();
deltaMax=10;
if (heightpix!=0) deltaMax=(int) (scrollArea->viewport()->height()/heightpix)+2;
canDisplayPixmap=true;
scrollArea->setFocus();
}

void PdfViewer::displayPage(int page)
{
if ((page>=0) && (page < doc->numPages()))
  {
  listPdfWidgets.at(page)->setScale(currentScale);
  listPdfWidgets.at(page)->updatePixmap();
  }
}

void PdfViewer::displayNewPage()
{
int newpage=-1;
pageMutex.lock();
if (listPdfWidgetsStatus.at(currentPage-1)==0) newpage=currentPage-1;
else
  {
  int delta=1;
  int max=doc->numPages()-currentPage;
  if (currentPage-1>max) max=currentPage-1;
  while (delta<=max) 
      {
      newpage=currentPage-1+delta*altern;
      if ((newpage<doc->numPages()) && (newpage>=0))
	  {
	  if (listPdfWidgetsStatus.at(newpage)==0) 
	    {
	    break;
	    }
	  }
      newpage=currentPage-1-delta*altern;
      if ((newpage<doc->numPages()) && (newpage>=0))
	      {
	      if (listPdfWidgetsStatus.at(newpage)==0) 
		{
		break;
		}
	      }
      delta+=1;
      }
  }
if ((newpage>=0) && (newpage < doc->numPages()) && (abs(newpage-currentPage+1)<=deltaMax) && canDisplayPixmap)
  {
  listPdfWidgets.at(newpage)->setScale(currentScale);
  listPdfWidgets.at(newpage)->updatePixmap();
  pageMutex.unlock();
  QTimer::singleShot( 30,this, SLOT(displayNewPage()) );
  }
else {
    pageMutex.unlock();
    }
altern=-altern;
}

void PdfViewer::updatePageStatus(int page)
{
listPdfWidgetsStatus.replace(page,1);
scrollArea->update();
pageMutex.unlock();
}

void PdfViewer::setScrollMax()
{
disconnect(scrollArea, SIGNAL(doRange()), this, SLOT(setScrollMax()));
if (scrollArea->verticalScrollBar()->maximum()<scrollMax) scrollArea->verticalScrollBar()->setMaximum(scrollMax);
connect(scrollArea, SIGNAL(doRange()), this, SLOT(setScrollMax()));
}

void PdfViewer::connectActions()
{
connect(findAct, SIGNAL(triggered()), this, SLOT(enableSearch()));
connect(upAct, SIGNAL(triggered()), this, SLOT(pageUp()));
connect(downAct, SIGNAL(triggered()), this, SLOT(pageDown()));
connect(listpagesWidget, SIGNAL(itemClicked ( QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));
connect(StructureTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(ClickedOnStructure()));
connect(fitWithAct, SIGNAL(triggered()), this, SLOT(fitWidth()));
connect(fitPageAct, SIGNAL(triggered()), this, SLOT(fitPage()));
connect(zoominAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
connect(zoomoutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
connect(zoomCustom, SIGNAL(returnPressed()),this, SLOT(userZoom()));
connect(scaleComboBox, SIGNAL(currentIndexChanged(QString)),this, SLOT(scaleDocumentZoom(QString)));
connect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT(searchDocument()));
connect(findButton, SIGNAL(clicked()), this, SLOT(searchDocument()));
if (scrollArea) connect(scrollArea, SIGNAL(pagezoomOut()), this, SLOT(zoomOut()));
if (scrollArea) connect(scrollArea, SIGNAL(pagezoomIn()), this, SLOT(zoomIn()));
}

void PdfViewer::disconnectActions()
{
disconnect(findAct, SIGNAL(triggered()), this, SLOT(enableSearch()));
disconnect(upAct, SIGNAL(triggered()), this, SLOT(pageUp()));
disconnect(downAct, SIGNAL(triggered()), this, SLOT(pageDown()));
disconnect(listpagesWidget, SIGNAL(itemClicked ( QListWidgetItem*)), this, SLOT(slotItemClicked(QListWidgetItem*)));
disconnect(StructureTreeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(ClickedOnStructure()));
disconnect(fitWithAct, SIGNAL(triggered()), this, SLOT(fitWidth()));
disconnect(fitPageAct, SIGNAL(triggered()), this, SLOT(fitPage()));
disconnect(zoominAct, SIGNAL(triggered()), this, SLOT(zoomIn()));
disconnect(zoomoutAct, SIGNAL(triggered()), this, SLOT(zoomOut()));
disconnect(zoomCustom, SIGNAL(returnPressed()),this, SLOT(userZoom()));
disconnect(scaleComboBox, SIGNAL(currentIndexChanged(QString)),this, SLOT(scaleDocumentZoom(QString)));
disconnect(searchLineEdit, SIGNAL(returnPressed()), this, SLOT(searchDocument()));
disconnect(findButton, SIGNAL(clicked()), this, SLOT(searchDocument()));
if (scrollArea) disconnect(scrollArea, SIGNAL(pagezoomOut()), this, SLOT(zoomOut()));
if (scrollArea) disconnect(scrollArea, SIGNAL(pagezoomIn()), this, SLOT(zoomIn()));
}

void PdfViewer::jumpToPdfFromSource(QString sourceFile, int source_line)
{
show();
if (windowState()==Qt::WindowMinimized) setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
qApp->setActiveWindow(this);
activateWindow();
if (!fileLoaded) return;
if (scanner == NULL) 
  {
  gotoPage(currentPage);
  return;
  }
const QFileInfo sourceFileInfo(sourceFile);
QDir curDir(QFileInfo(pdf_file).canonicalPath());
synctex_node_t node = synctex_scanner_input(scanner);
QString name;
bool found = false;
while (node != NULL) 
  {
  name = QString::fromUtf8(synctex_scanner_get_name(scanner, synctex_node_tag(node)));
  const QFileInfo fi(curDir, name);
  if (fi == sourceFileInfo) 
    {
    found = true;
    break;
    }
  node = synctex_node_sibling(node);
  }
if (!found) 
  {
  gotoPage(currentPage);
  return;
  }
if (synctex_display_query(scanner, name.toUtf8().data(), source_line, 0) > 0) 
  {
  int page = -1;
  path= QPainterPath();
  while ((node = synctex_next_result(scanner)) != NULL) 
    {
    if (page == -1) page = synctex_node_page(node);
    if (synctex_node_page(node) != page) continue;
    QRectF nodeRect(synctex_node_box_visible_h(node),
				    synctex_node_box_visible_v(node) - synctex_node_box_visible_height(node),
				    synctex_node_box_visible_width(node),
				    synctex_node_box_visible_height(node) + synctex_node_box_visible_depth(node));
    path.addRect(nodeRect);
    }
  if (page > 0) 
    {
     disconnect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
    QRectF r = path.boundingRect();
    currentPage=page;
    lastPage=currentPage;
    int vpos=(int) (listPdfWidgetsPos.at(currentPage-1)-10+(r.top()) * listPdfWidgets.at(currentPage-1)->physicalDpiY() / 72 * currentScale);
    int hpos=(int) (r.left()  * listPdfWidgets.at(currentPage-1)->physicalDpiX() / 72 * currentScale-10);
    scrollArea->setVisible(hpos,vpos,20,scrollMax);
    listPdfWidgets.at(currentPage-1)->clearPaths();
    path.setFillRule(Qt::WindingFill);
    QTimer::singleShot(500,this, SLOT(slotHighlight()) );
    updateCurrentPage();
    connect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
    }
  }
  else 
  {
    gotoPage(currentPage);
  }
}

void PdfViewer::slotHighlight()
{
listPdfWidgets.at(currentPage-1)->setHighlightPath(path); 
}

void PdfViewer::gotoPage(int page)
{
if (!fileLoaded) return;
if ((page <= doc->numPages()) && (page>=1))
  {
  currentPage=page;
  lastPage=currentPage;
  disconnect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
  scrollArea->verticalScrollBar()->setValue(listPdfWidgetsPos.at(currentPage-1));
  updateCurrentPage();
  connect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
  }
}

void PdfViewer::checkPage(int value)
{
if (!fileLoaded) return;
int old=currentPage;
int i=0;
while (i < doc->numPages()) 
	{
	if ((int) (listPdfWidgetsPos.at(i)-scrollArea->viewport()->height()/2)>value) break;
	i++;
	}
currentPage=i;
lastPage=currentPage;
updateCurrentPage();
}

void PdfViewer::updateCurrentPage()
{
if (!fileLoaded) return;
if (currentPage==1) upAct->setEnabled(false);
else upAct->setEnabled(true);
if (currentPage==doc->numPages()) downAct->setEnabled(false);
else downAct->setEnabled(true);
QList<QListWidgetItem *> fItems=listpagesWidget->findItems (tr("Page")+" "+QString::number(currentPage),Qt::MatchRecursive);
if ((fItems.size()>0 ) && (fItems.at(0))) listpagesWidget->setCurrentItem(fItems.at(0));
displayNewPage();
}

void PdfViewer::jumpToDest(int page,int left, int top)
{
if (!fileLoaded) return;
if ((page <= doc->numPages()) && (page>=1))
  {
  currentPage=page;
  lastPage=currentPage;
  disconnect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
    int vpos=listPdfWidgetsPos.at(currentPage-1)-10+top;
    int hpos=left-10;
    scrollArea->setVisible(hpos,vpos,20,scrollMax);
    updateCurrentPage();
  connect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
  }
}

void PdfViewer::userZoom()
{
scaleDocumentZoom(zoomCustom->text());  
}

void PdfViewer::scaleDocumentZoom(QString zoom)
{
if (!fileLoaded) return;
if (zoom.contains("%")) zoom.remove("%");
if (zoom.toInt() > 0 && zoom.toInt() <= 400)
  {
  currentScale=zoom.toFloat() / 100.0;
  if (currentScale < 0.25) currentScale = 0.25;
  else if (currentScale > 4) currentScale = 4;
  disconnect(scrollArea, SIGNAL(doRange()), this, SLOT(setScrollMax()));
  disconnect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
  initPagesWithNewScale();
  connect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
  connect(scrollArea, SIGNAL(doRange()), this, SLOT(setScrollMax()));
  gotoPage(currentPage);
  }
}

void PdfViewer::searchDocument()
{
if (!fileLoaded) return;
QRectF location;
if (searchLineEdit->text().isEmpty()) return;
disconnect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
location = searchForwards(searchLineEdit->text());
connect(scrollArea, SIGNAL(doScroll(int)), this, SLOT(checkPage(int)));
//else location = searchBackwards(searchLineEdit->text());
//QPoint target = pdfWidget->matrix().mapRect(location).center().toPoint();
//scrollArea->ensureVisible(target.x(), target.y());
}

/*QRectF PdfViewer::searchBackwards(const QString &text)
{
    QRectF oldLocation = searchLocation;
    QPainterPath path;

    int page = currentPage;
    if (oldLocation.isNull())
        page -= 1;

    while (page > -1) {

        QList<QRectF> locations;
        searchLocation = QRectF();

        while (doc->page(page)->search(text, searchLocation,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

            if (searchLocation != oldLocation)
                locations.append(searchLocation);
            else
                break;
        }

        int index = locations.indexOf(oldLocation);
        if (index == -1 && !locations.isEmpty()) {
            searchLocation = locations.last();
            //showPage(page + 1);
            return searchLocation;
        } else if (index > 0) {
            searchLocation = locations[index - 1];
            //showPage(page + 1);
            return searchLocation;
        }

        oldLocation = QRectF();
        page -= 1;
    }

    if (currentPage == doc->numPages() - 1)
        return QRectF();

    oldLocation = QRectF();
    page = doc->numPages() - 1;

    while (page > currentPage) {

        QList<QRectF> locations;
        searchLocation = QRectF();

        while (doc->page(page)->search(text, searchLocation,
            Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) {

            locations.append(searchLocation);
        }

        if (!locations.isEmpty()) {
            searchLocation = locations.last();
            //showPage(page + 1);
            return searchLocation;
        }
        page -= 1;
    }

    return QRectF();
}*/

QRectF PdfViewer::searchForwards(const QString &text)
{
int page = currentPage-1;

while (page < doc->numPages()) 
  {
  if (currentPage>=1) 
    {
    listPdfWidgets.at(currentPage-1)->clearPaths();
    displayPage(currentPage-1);
    }
  if (doc->page(page)->search(text, searchLocation,Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) 
    {
    if (!searchLocation.isNull()) 
      {
      path= QPainterPath();
      path.addRect(searchLocation);
      QRectF r = path.boundingRect();
      currentPage=page+1;
      lastPage=currentPage;
      int vpos=(int) (listPdfWidgetsPos.at(currentPage-1)-10+(r.top() + r.bottom()) / 2 * listPdfWidgets.at(currentPage-1)->physicalDpiY() / 72 * currentScale);
      int hpos=(int) ((r.left()+r.right())/2  * listPdfWidgets.at(currentPage-1)->physicalDpiX() / 72 * currentScale-10);
      scrollArea->setVisible(hpos,vpos,20,scrollMax);
      path.setFillRule(Qt::WindingFill);
      listPdfWidgets.at(currentPage-1)->setSearchPath(path);
      updateCurrentPage();
      return searchLocation;
      }
    }
    page += 1;
    searchLocation = QRectF();
  }
page = 0;
while (page < currentPage-1) 
  {
  if (currentPage>=1) 
    {
    listPdfWidgets.at(currentPage-1)->clearPaths();
  displayPage(currentPage-1);
    }
  searchLocation = QRectF();
  if (doc->page(page)->search(text, searchLocation,Poppler::Page::NextResult, Poppler::Page::CaseInsensitive)) 
    {
    if (!searchLocation.isNull()) 
      {
      path= QPainterPath();
      path.addRect(searchLocation);
      QRectF r = path.boundingRect();
      currentPage=page+1;
      lastPage=currentPage;
      int vpos=(int) (listPdfWidgetsPos.at(currentPage-1)-10+(r.top() + r.bottom()) / 2 * listPdfWidgets.at(currentPage-1)->physicalDpiY() / 72 * currentScale);
      int hpos=(int) ((r.left()+r.right())/2  * listPdfWidgets.at(currentPage-1)->physicalDpiX() / 72 * currentScale-10);
      scrollArea->setVisible(hpos,vpos,20,scrollMax);
      path.setFillRule(Qt::WindingFill);
      listPdfWidgets.at(currentPage-1)->setSearchPath(path);
      //if (!path.isEmpty()) displayPage(currentPage-1);
      updateCurrentPage();
      return searchLocation;
      }
    }
  page += 1;
  }
return QRectF();
}

void PdfViewer::enableSearch()
{
searchLineEdit->setFocus();
}

void PdfViewer::pageUp()
{
if (!fileLoaded) return;
if (currentPage>1)
  {
  currentPage--;
  lastPage=currentPage;
  gotoPage(currentPage);
  }
}

void PdfViewer::pageDown()
{
if (!fileLoaded) return;
if (currentPage<doc->numPages())
  {
  currentPage++;
  lastPage=currentPage;
  gotoPage(currentPage);
  }
}

void PdfViewer::fitWidth()
{
if (!fileLoaded) return;
qreal portWidth = scrollArea->viewport()->width();
QSizeF	pageSize = doc->page(currentPage-1)->pageSizeF();
if (pageSize.width()!=0) currentScale = (portWidth-10) / pageSize.width()*72.0/listPdfWidgets.at(currentPage-1)->physicalDpiX();
else return;
if (currentScale < 0.25) currentScale = 0.25;
else if (currentScale > 4) currentScale = 4;
zoomCustom->setText(QString::number(int(currentScale*100)) + "%");
userZoom();
}

void PdfViewer::fitPage()
{
if (!fileLoaded) return;
qreal portWidth = scrollArea->viewport()->width();
qreal portHeight = scrollArea->viewport()->height();
if (scrollArea->horizontalScrollBar()->isVisible()) portHeight+=scrollArea->horizontalScrollBar()->height();
QSizeF	pageSize = doc->page(currentPage-1)->pageSizeF();
qreal scaleW;
qreal scaleH;
if (pageSize.width()!=0) scaleW = (portWidth-20-scrollArea->verticalScrollBar()->width()) / pageSize.width()*72.0/listPdfWidgets.at(currentPage-1)->physicalDpiX();
else return;
if (pageSize.height()!=0) scaleH = (portHeight-scrollArea->verticalLayout->spacing() )/ pageSize.height()*72.0/listPdfWidgets.at(currentPage-1)->physicalDpiY();
else return;
if (scaleH < scaleW) currentScale=scaleH;
else currentScale=scaleW ;
if (currentScale < 0.25) currentScale = 0.25;
else if (currentScale > 4) currentScale = 4;
zoomCustom->setText(QString::number(int(currentScale*100)) + "%");
userZoom();
}

void PdfViewer::zoomIn()
{
if (!fileLoaded) return;
currentScale+=0.1;
if (currentScale > 4) currentScale = 4;
zoomCustom->setText(QString::number(int(currentScale*100)) + "%");
userZoom();
}

void PdfViewer::zoomOut()
{
if (!fileLoaded) return;
currentScale-=0.1;
if (currentScale < 0.25) currentScale = 0.25;
zoomCustom->setText(QString::number(int(currentScale*100)) + "%");
userZoom();
}

void PdfViewer::runExternalViewer()
{
if (!fileLoaded) return;
QString command=viewpdf_command;
command.replace("%.pdf","\""+pdf_file+"\"");
QFileInfo fi(pdf_file);
if (fi.exists()) 
	{
	proc = new QProcess( this );
	proc->setWorkingDirectory(fi.absolutePath());
	proc->start(command);
	}
}

void PdfViewer::printPdf()
{
if (!fileLoaded) return;
QFileInfo fi(pdf_file);
if (!fi.exists()) return; 
QString command;
#ifdef Q_WS_WIN
QString gs="none";
if (QFileInfo(gswin32c_command).exists()) gs=gswin32c_command;
else if (QFileInfo("C:/Program Files/gs/gs9.00/bin/gswin32c.exe").exists()) gs="C:/Program Files/gs/gs9.00/bin/gswin32c.exe";
else if (QFileInfo("C:/Program Files (x86)/gs/gs9.00/bin/gswin32c.exe").exists()) gs="C:/Program Files (x86)/gs/gs9.00/bin/gswin32c.exe";
else if (QFileInfo("C:/Program Files/gs/gs8.71/bin/gswin32c.exe").exists()) gs="C:/Program Files/gs/gs8.71/bin/gswin32c.exe";
else if (QFileInfo("C:/Program Files (x86)/gs/gs8.71/bin/gswin32c.exe").exists()) gs="C:/Program Files (x86)/gs/gs8.71/bin/gswin32c.exe";
else if (QFileInfo("C:/Program Files/gs/gs8.64/bin/gswin32c.exe").exists()) gs="C:/Program Files/gs/gs8.64/bin/gswin32c.exe";
else if (QFileInfo("C:/Program Files (x86)/gs/gs8.64/bin/gswin32c.exe").exists()) gs="C:/Program Files (x86)/gs/gs8.65/bin/gswin32c.exe";
else if (QFileInfo("C:/Program Files/gs/gs8.63/bin/gswin32c.exe").exists()) gs="C:/Program Files/gs/gs8.63/bin/gswin32c.exe";
else if (QFileInfo("C:/Program Files (x86)/gs/gs8.63/bin/gswin32c.exe").exists()) gs="C:/Program Files (x86)/gs/gs8.63/bin/gswin32c.exe";
else if (QFileInfo("C:/Program Files/gs/gs8.62/bin/gswin32c.exe").exists()) gs="C:/Program Files/gs/gs8.62/bin/gswin32c.exe";
else if (QFileInfo("C:/Program Files (x86)/gs/gs8.62/bin/gswin32c.exe").exists()) gs="C:/Program Files (x86)/gs/gs8.62/bin/gswin32c.exe";
if (gs=="none") 
  {
  QMessageBox::warning( this,tr("Error"), tr("Can't print : the ghostscript command (gswin32c.exe) was not found on your system."));
  return;
  }
dlg = new PaperDialog(this,"New");
dlg->ui.comboBoxPaper->setCurrentIndex(dlg->ui.comboBoxPaper->findText(paper_size, Qt::MatchExactly));
int firstp=1;
int lastp=doc->numPages();
dlg->ui.from->setValue(firstp);
dlg->ui.to->setValue(lastp);
if ( dlg->exec() )
  {
  paper_size=dlg->ui.comboBoxPaper->currentText();
  if (dlg->ui.printAll->isChecked()) 
    {
    firstp=1;
    lastp=doc->numPages();
    } 
  else if (dlg->ui.printRange->isChecked()) 
    {
    firstp=dlg->ui.from->value();
    lastp=qMax(dlg->ui.from->value(), dlg->ui.to->value());
    }
  delete (dlg);
  emit sendPaperSize(paper_size);
  }
else
  {
  delete (dlg);
  return;  
  }
command = "\""+gs+"\" -dBATCH -dNOPAUSE -dQUIET -dNoCancel -sPAPERSIZE="+paper_size+" -dFirstPage="+QString::number(firstp)+" -dLastPage="+QString::number(lastp)+" -sDEVICE=mswinpr2 \""+pdf_file+"\"";
qDebug() << command;
#else
unsigned int firstPage, lastPage;
QPrinter printer(QPrinter::HighResolution);
QPrintDialog printDlg(&printer, this);
printer.setDocName(fi.baseName());
printDlg.setMinMax(1, doc->numPages());
printDlg.setFromTo(1, doc->numPages());
printDlg.setOption(QAbstractPrintDialog::PrintToFile, false);
printDlg.setOption(QAbstractPrintDialog::PrintSelection, false);
printDlg.setOption(QAbstractPrintDialog::PrintPageRange, true);
printDlg.setOption(QAbstractPrintDialog::PrintCollateCopies, true);

printDlg.setWindowTitle(tr("Print"));
if(printDlg.exec() != QDialog::Accepted) return;
switch(printDlg.printRange()) 
  {
  case QAbstractPrintDialog::PageRange:
	  firstPage = printDlg.fromPage();
	  lastPage = printDlg.toPage();
	  break;
  default:
	  firstPage = 1;
	  lastPage = doc->numPages();
  }

if(!printer.printerName().isEmpty()) 
  {
  QStringList args;
  args << "lp";
  args << QString("-d %1").arg(printer.printerName().replace(" ","_"));
  args << QString("-n %1").arg(printer.numCopies());
  args << QString("-P %1-%2").arg(firstPage).arg(lastPage);
  switch(printer.duplex()) 
      {
      case QPrinter::DuplexNone:
	      args << "-o sides=one-sided";
	      break;
      case QPrinter::DuplexShortSide:
	      args << "-o sides=two-sided-short-edge";
	      break;
      case QPrinter::DuplexLongSide:
	      args << "-o sides=two-sided-long-edge";
	      break;
      default:
	      break;
      }
  args << "--";
  args << QString("\"%1\"").arg(pdf_file);
  command=args.join(" ");
  }
else return;
#endif
if(QProcess::execute(command) == 0) return;
}

void PdfViewer::slotItemClicked(QListWidgetItem* item)
{
QString txt=item->text().section(" ",-1);
currentPage=txt.toInt();
gotoPage(currentPage);
}

void PdfViewer::jumpToEditor(int page, const QPointF& pos)
{
if (scanner == NULL) return;
if (synctex_edit_query(scanner, page+1, pos.x(), pos.y()) > 0) 
  {
  synctex_node_t node;
  while ((node = synctex_next_result(scanner)) != NULL) 
    {
    QString filename = QString::fromUtf8(synctex_scanner_get_name(scanner, synctex_node_tag(node)));
    QDir curDir(QFileInfo(pdf_file).canonicalPath());
    emit openDocAtLine(QFileInfo(curDir, filename).canonicalFilePath(),synctex_node_line(node));
    break;
    }
  }
}

void PdfViewer::keyPressEvent ( QKeyEvent * e ) 
{
if ( e->key()==Qt::Key_F1) 
    {
    emit sendFocusToEditor();
    }
else QMainWindow::keyPressEvent(e);
}

void PdfViewer::ShowStructure()
{
LeftPanelStackedWidget->setCurrentWidget(StructureTreeWidget);
StructureView->setWindowTitle(tr("Structure"));
}

void PdfViewer::ShowListPages()
{
LeftPanelStackedWidget->setCurrentWidget(listpagesWidget);
StructureView->setWindowTitle(tr("Pages"));
}

void PdfViewer::ParseToc(const QDomNode &parent, QTreeWidget *tree, QTreeWidgetItem *parentItem)
{
QTreeWidgetItem *newitem = 0;
for (QDomNode node = parent.firstChild(); !node.isNull(); node = node.nextSibling()) 
  {
  QDomElement e = node.toElement();
  if (!parentItem) newitem = new QTreeWidgetItem(tree, newitem);
  else newitem = new QTreeWidgetItem(parentItem, newitem);
  newitem->setText(0, e.tagName());
  bool isOpen = false;
  if (e.hasAttribute("Open")) isOpen = QVariant(e.attribute("Open")).toBool();
  if (isOpen) tree->expandItem(newitem);
  if (e.hasAttribute("DestinationName")) newitem->setText(1, e.attribute("DestinationName"));
  if (e.hasChildNodes()) ParseToc(node, tree, newitem);
  }
}
void PdfViewer::ClickedOnStructure()
{
QList<QTreeWidgetItem*> items = StructureTreeWidget->selectedItems();
if (items.count() > 0) 
  {
  QTreeWidgetItem* item = items.first();
  QString destname = item->text(1);
  if (!destname.isEmpty()) 
    {
    const Poppler::LinkDestination *dest = doc->linkDestination(destname);
    if (dest) 
      {
      int destPage=0;
      int destLeft=0;
      int destTop=0;
      if ((dest->pageNumber() > 0) && (dest->pageNumber() <= doc->numPages())) destPage=dest->pageNumber();
      else return;
      if (dest->isChangeLeft()) destLeft = (int)floor(dest->left() * listPdfWidgets.at(destPage-1)->scale() * listPdfWidgets.at(destPage-1)->physicalDpiX() / 72.0 * doc->page(destPage-1)->pageSizeF().width());
      if (dest->isChangeTop()) destTop = (int)floor(dest->top() * listPdfWidgets.at(destPage-1)->scale() * listPdfWidgets.at(destPage-1)->physicalDpiY() / 72.0 * doc->page(destPage-1)->pageSizeF().height());
      jumpToDest(destPage,destLeft,destTop);
      }
    }
  }
}

