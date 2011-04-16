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
#ifndef PDFVIEWER_H
#define PDFVIEWER_H

#include <QtGui>
#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QAction>
#include <QListWidgetItem>
#include <QProcess>
#include <QPointer>
#include <QKeyEvent>
#include <QPainterPath>
#include <QFrame>
#include <QHBoxLayout>
#include <QMutex>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QToolBar>
#include <QStack>
#include "pdfdocumentwidget.h"
#include "pdfscrollarea.h"
#include "synctex_parser.h"
#include "paperdialog.h"



class PdfViewer : public QMainWindow
{
    Q_OBJECT
public:
    PdfViewer( const QString fileName, const QString externalCommand, const QString ghostscriptCommand, const QString psize,QWidget* parent = 0, Qt::WFlags flags = 0);
    ~PdfViewer();
    QString pdf_file;
public slots:
void openFile(QString fn, QString ec,QString pc);
void jumpToPdfFromSource(QString sourceFile,int source_line);
private:
void closeEvent(QCloseEvent *e);
PaperDialog *dlg;
QStackedWidget *LeftPanelStackedWidget;
QTreeWidget *StructureTreeWidget;
QToolBar *LeftPanelToolBar;
QHBoxLayout *LeftPanelLayout,*CentralLayout;
QByteArray windowstate;
QList<PdfDocumentWidget*> listPdfWidgets, templist;
Poppler::Document *doc;
QAction *upAct, *downAct, *fitWithAct, *fitPageAct, *zoominAct, *zoomoutAct, *findAct, *historyBackAct, *historyForwardAct,*printAct, *externAct;
QDockWidget *StructureView;
QListWidget *listpagesWidget;
#if defined(Q_WS_WIN)
QComboBox *comboBoxPaper;
#endif
PdfScrollArea  *scrollArea;
QList<int> listPdfWidgetsPos;
QList<int> listPdfWidgetsStatus;
QComboBox *scaleComboBox;
QLineEdit *searchLineEdit;
QPushButton *findButton;
QString viewpdf_command, gswin32c_command, paper_size;
int currentPage;
qreal currentScale, lastScale, previousScale;
bool fileLoaded;
QPointer<QProcess> proc;
QString lastFile;
int lastPage, altern;
synctex_scanner_t scanner;
QStringList scalePercents;
QLineEdit *zoomCustom;
QRectF searchLocation;
QMutex pageMutex;
bool canDisplayPixmap;
int scrollMax, deltaMax;
QPainterPath path;
QStack<int> stack;
QStack<int> forwardStack;
bool showingListPages;


private slots:
void connectActions();
void disconnectActions();
void setScrollMax();
void initPages(bool checkScale);
void initPagesWithNewScale();
void displayPage(int page);
void displayNewPage();
void updatePageStatus(int page);
void slotHighlight();
void gotoPage(int page);
void checkPage(int value);
void updateCurrentPage();
void jumpToDest(int page,int left, int top);
void userZoom();
void scaleDocumentZoom(QString zoom);
void searchDocument();
void enableSearch();
void pageUp();
void pageDown();
void fitWidth();
void fitPage();
void zoomIn();
void zoomOut();
void runExternalViewer();
void printPdf();
void slotItemClicked(QListWidgetItem* item);
void jumpToEditor(int page, const QPointF& pos);
//QRectF searchBackwards(const QString &text);
void searchForwards(const QString &text);
void ShowStructure();
void ShowListPages();
void ParseToc(const QDomNode &parent, QTreeWidget *tree, QTreeWidgetItem *parentItem);
void ClickedOnStructure(QTreeWidgetItem* item,int c);
void historyBack();
void historyForward();
void clearHistory();
void updateHistory(int pos);

protected:
void keyPressEvent ( QKeyEvent * e );

signals:
void openDocAtLine(const QString&, int,bool);
void sendFocusToEditor();
void sendPaperSize(const QString&);
void backwardAvailable(bool);
void forwardAvailable(bool);
};

#endif

