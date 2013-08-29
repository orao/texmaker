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
#include <QTreeView>
#include <QToolBar>
#include <QStack>
#include <QKeySequence>

#include "documentview.h"
#include "synctex_parser.h"
#include "browser.h"



class PdfViewer : public QMainWindow
{
    Q_OBJECT
public:
    PdfViewer( const QString fileName, const QString externalCommand, const QString ghostscriptCommand, const QString lpopt,const QKeySequence edfocus, const QString SpellLang, const qreal startScale, QWidget* parent = 0);
    ~PdfViewer();
    QString pdf_file;
public slots:
void openFile(QString fn, QString ec,QString pc);
void jumpToPdfFromSource(QString sourceFile,int source_line);
void setKeyEditorFocus(QKeySequence s);
void setGSCommand(QString c);
qreal getScale() {return currentScale;};
private:
void closeEvent(QCloseEvent *e);
QKeySequence KeySequenceEditorFocus;
QStackedWidget *LeftPanelStackedWidget;
QTreeView *StructureTreeView;
QToolBar *LeftPanelToolBar;
QHBoxLayout *LeftPanelLayout,*CentralLayout;
QByteArray windowstate;

DocumentView* pdfview;
Poppler::Document *doc;
QAction *upAct, *downAct, *fitWithAct, *fitPageAct, *zoominAct, *zoomoutAct, *findAct, *historyBackAct, *historyForwardAct,*printAct, *externAct,*checkerAct, *searchAct;

QAction *continuousModeAction, *twoPagesModeAction, *rotateLeftAction, *rotateRightAction, *presentationAction;
QDockWidget *StructureView;
QListWidget *listpagesWidget;


QComboBox *scaleComboBox;
QLineEdit *searchLineEdit;
//QPushButton *findButton;
QString viewpdf_command, gswin32c_command, lp_options, spell_lang;
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
double searchleft, searchtop, searchright, searchbottom;
QPainterPath path;
QStack<int> stack;
QStack<int> forwardStack;
bool showingListPages;
int lastHpos;
bool islastContinuous;
QPointer<Browser> browserWindow;

private slots:
void connectActions();
void disconnectActions();

void slotHighlight();
void gotoPage(int page);
void checkPage(int value);
void updateCurrentPage();
void jumpToDest(int page,qreal left, qreal top);
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
//void ParseToc(const QDomNode &parent, QTreeWidget *tree, QTreeWidgetItem *parentItem);
void ClickedOnStructure(const QModelIndex& index);
void historyBack();
void historyForward();
void clearHistory();
void updateHistory(int pos);
void setHpos(int pos);
void checkSpellGrammarPage();
void jumptoHpos();

void on_continuousMode_triggered(bool checked);
void on_twoPagesMode_triggered(bool checked);
void on_rotateLeft_triggered();
void on_rotateRight_triggered();
void on_presentation_triggered();

void on_currentTab_continuousModeChanged(bool continuousMode);
void on_currentTab_twoPagesModeChanged(bool twoPagesMode);
    

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

