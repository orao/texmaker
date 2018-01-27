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
#ifndef PDFVIEWERWIDGET_H
#define PDFVIEWERWIDGET_H

#include <QtGui>
#include <QDockWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QToolButton>
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
#include "minisplitter.h"
#include "browser.h"



class PdfViewerWidget : public QWidget
{
    Q_OBJECT
public:
    PdfViewerWidget( const QString fileName, const QString externalCommand, const QString ghostscriptCommand, const QString psize,const QKeySequence edfocus, const QString SpellLang, const qreal startScale, QWidget* parent = 0);
    ~PdfViewerWidget();
    QString pdf_file;
    QToolBar *centralToolBarBis;
public slots:
void openFile(QString fn, QString ec,QString pc);
void jumpToPdfFromSource(QString sourceFile,int source_line);
void getFocus();
void setKeyEditorFocus(QKeySequence s);
void setGSCommand(QString c);
qreal getScale() {return currentScale;};
private:
QKeySequence KeySequenceEditorFocus;
//void closeEvent(QCloseEvent *e);
MiniSplitter *splitter;
QTreeView *StructureTreeView;

DocumentView* pdfview;
Poppler::Document *doc;
QAction *upAct, *downAct, *fitWithAct, *fitPageAct, *zoominAct, *zoomoutAct, *findAct, *historyBackAct, *printAct, *externAct, *historyForwardAct, *toggleStructAct, *checkerAct;
QAction *continuousModeAction, *twoPagesModeAction, *rotateLeftAction, *rotateRightAction, *presentationAction;
QHBoxLayout *CentralLayout;
QVBoxLayout *CentralLayoutBis;

QListWidget *listpagesWidget;
QToolButton *optionsButton;
QComboBox *scaleComboBox;
QLineEdit *searchLineEdit;
QPushButton *findButton;
QString viewpdf_command, gswin32c_command, paper_size, spell_lang;
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
QPainterPath path;
QStack<int> stack;
QStack<int> forwardStack;
bool showingStructure;
int lastHpos;
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
void searchForwards(const QString &text);
void ToggleStructure();

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

