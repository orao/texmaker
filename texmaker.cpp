/***************************************************************************
 *   copyright       : (C) 2003-2011 by Pascal Brachet                     *
 *   addons by Luis Silvestre ; Tom Hoffmann                               *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation  either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
//#include <stdlib.h>

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QStatusBar>
#include <QSettings>
#include <QApplication>
#include <QDesktopWidget>
#include <QTextCodec>
#include <QFileInfo>
#include <QLabel>
#include <QTextStream>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QLineEdit>
#include <QProcess>
#include <QComboBox>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <QLocale>
#include <QTabWidget>
#include <QStyleFactory>
#include <QStyle>
#include <QFontDatabase>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextEdit>
#include <QTextBlock>
#include <QDebug>
#include <QDesktopServices>
#include <QAbstractItemModel>
#include <QTextCharFormat>
#include <QTextTableFormat>
#include <QTextLength>
#include <QFrame>
#include <QFontMetrics>
#include <QDateTime>

#ifdef Q_WS_MACX
#if (QT_VERSION >= 0x0406)
#include <QProcessEnvironment>
#endif
#endif

#ifdef Q_WS_WIN
#if (QT_VERSION >= 0x0406)
#include <QProcessEnvironment>
#endif
#endif

//#ifdef Q_WS_WIN
//#include <windows.h>
//#endif


#include "texmaker.h"
#include "texmakerapp.h"
#include "latexeditorview.h"
#include "manhattanstyle.h"
#include "structdialog.h"
#include "filechooser.h"
#include "graphicfilechooser.h"
#include "tabdialog.h"
#include "arraydialog.h"
#include "tabbingdialog.h"
#include "letterdialog.h"
#include "quickdocumentdialog.h"
#include "usermenudialog.h"
#include "usertooldialog.h"
#include "refdialog.h"
#include "configdialog.h"
#include "aboutdialog.h"
#include "spellerdialog.h"
#include "encodingdialog.h"
#include "usercompletiondialog.h"


#if defined( Q_WS_X11 )
#include "x11fontdialog.h"
#endif

Texmaker::Texmaker(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
ReadSettings();

 if (spelldicExist()) 
       {
       QString dic=spell_dic.left(spell_dic.length()-4);
       spellChecker = new Hunspell(dic.toLatin1()+".aff",dic.toLatin1()+".dic");
       }
 else spellChecker=0;

//spellChecker=0;
untitled_id=1;


#ifdef Q_WS_MACX
setWindowIcon(QIcon(":/images/logo128.png"));
#else
setWindowIcon(QIcon(":/images/appicon.png"));
#endif

setIconSize(QSize(22,22 ));
 
completer = new QCompleter(this);
updateCompleter();

QAction *Act;
splitter1=new MiniSplitter;
splitter1->setOrientation(Qt::Horizontal);
splitter2=new MiniSplitter(splitter1);
splitter2->setOrientation(Qt::Vertical);
// PANNEAU STRUCTURE

LeftPanelFrameBis=new QFrame(this);
LeftPanelFrameBis->setLineWidth(0);
LeftPanelFrameBis->setFrameShape(QFrame::NoFrame);
LeftPanelFrameBis->setFrameShadow(QFrame::Plain);

LeftPanelToolBarBis=new QToolBar("TitleBar",LeftPanelFrameBis);
LeftPanelToolBarBis->setFloatable(false);
LeftPanelToolBarBis->setOrientation(Qt::Horizontal);
LeftPanelToolBarBis->setMovable(false);
LeftPanelToolBarBis->setIconSize(QSize(16,16 ));
LeftPanelToolBarBis->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

QFrame *LeftPanelFrame=new QFrame(this);
LeftPanelFrame->setLineWidth(0);
LeftPanelFrame->setFrameShape(QFrame::NoFrame);
LeftPanelFrame->setFrameShadow(QFrame::Plain);

LeftPanelLayout= new QHBoxLayout(LeftPanelFrame);
LeftPanelToolBar=new QToolBar("LogToolBar",LeftPanelFrame);
LeftPanelToolBar->setFloatable(false);
LeftPanelToolBar->setOrientation(Qt::Vertical);
LeftPanelToolBar->setMovable(false);
LeftPanelToolBar->setIconSize(QSize(16,16 ));

LeftPanelStackedWidget=new QStackedWidget(LeftPanelFrame);

StructureTreeWidget=new QTreeWidget(LeftPanelStackedWidget);
StructureTreeWidget->setFrameStyle(QFrame::NoFrame);
StructureTreeWidget->setColumnCount(1);
StructureTreeWidget->header()->hide();
StructureTreeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
StructureTreeWidget->header()->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
StructureTreeWidget->header()->setResizeMode(0, QHeaderView::ResizeToContents);
StructureTreeWidget->header()->setStretchLastSection(false);
StructureTreeWidget->setIndentation(10);

connect( StructureTreeWidget, SIGNAL(itemClicked(QTreeWidgetItem *,int )), SLOT(ClickedOnStructure(QTreeWidgetItem *,int)));

connect(LeftPanelToolBar->addAction(QIcon(":/images/structure.png"),tr("Structure")), SIGNAL(triggered()), this, SLOT(ShowStructure()));
LeftPanelStackedWidget->addWidget(StructureTreeWidget);
LeftPanelToolBar->addSeparator();

RelationListWidget=new SymbolListWidget(LeftPanelStackedWidget,0);
RelationListWidget->setFrameStyle(QFrame::NoFrame);
connect(RelationListWidget, SIGNAL(itemClicked ( QTableWidgetItem*)), this, SLOT(InsertSymbol(QTableWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/math1.png"),tr("Relation symbols")), SIGNAL(triggered()), this, SLOT(ShowRelation()));
connect(RelationListWidget->addAct, SIGNAL(triggered()), this, SLOT(InsertFavoriteSymbols()));
LeftPanelStackedWidget->addWidget(RelationListWidget);


ArrowListWidget=new SymbolListWidget(LeftPanelStackedWidget,1);
ArrowListWidget->setFrameStyle(QFrame::NoFrame);
connect(ArrowListWidget, SIGNAL(itemClicked ( QTableWidgetItem*)), this, SLOT(InsertSymbol(QTableWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/math2.png"),tr("Arrow symbols")), SIGNAL(triggered()), this, SLOT(ShowArrow()));
connect(ArrowListWidget->addAct, SIGNAL(triggered()), this, SLOT(InsertFavoriteSymbols()));
LeftPanelStackedWidget->addWidget(ArrowListWidget);

MiscellaneousListWidget=new SymbolListWidget(LeftPanelStackedWidget,2);
MiscellaneousListWidget->setFrameStyle(QFrame::NoFrame);
connect(MiscellaneousListWidget, SIGNAL(itemClicked ( QTableWidgetItem*)), this, SLOT(InsertSymbol(QTableWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/math3.png"),tr("Miscellaneous symbols")), SIGNAL(triggered()), this, SLOT(ShowMisc()));
connect(MiscellaneousListWidget->addAct, SIGNAL(triggered()), this, SLOT(InsertFavoriteSymbols()));
LeftPanelStackedWidget->addWidget(MiscellaneousListWidget);

DelimitersListWidget=new SymbolListWidget(LeftPanelStackedWidget,3);
DelimitersListWidget->setFrameStyle(QFrame::NoFrame);
connect(DelimitersListWidget, SIGNAL(itemClicked ( QTableWidgetItem*)), this, SLOT(InsertSymbol(QTableWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/math4.png"),tr("Delimiters")), SIGNAL(triggered()), this, SLOT(ShowDelim()));
connect(DelimitersListWidget->addAct, SIGNAL(triggered()), this, SLOT(InsertFavoriteSymbols()));
LeftPanelStackedWidget->addWidget(DelimitersListWidget);

GreekListWidget=new SymbolListWidget(LeftPanelStackedWidget,4);
GreekListWidget->setFrameStyle(QFrame::NoFrame);
connect(GreekListWidget, SIGNAL(itemClicked ( QTableWidgetItem*)), this, SLOT(InsertSymbol(QTableWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/math5.png"),tr("Greek letters")), SIGNAL(triggered()), this, SLOT(ShowGreek()));
connect(GreekListWidget->addAct, SIGNAL(triggered()), this, SLOT(InsertFavoriteSymbols()));
LeftPanelStackedWidget->addWidget(GreekListWidget);

MostUsedListWidget=new SymbolListWidget(LeftPanelStackedWidget,5);
MostUsedListWidget->setFrameStyle(QFrame::NoFrame);
connect(MostUsedListWidget, SIGNAL(itemClicked ( QTableWidgetItem*)), this, SLOT(InsertSymbol(QTableWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/math6.png"),tr("Most used symbols")), SIGNAL(triggered()), this, SLOT(ShowMostUsed()));
SetMostUsedSymbols();
LeftPanelStackedWidget->addWidget(MostUsedListWidget);

FavoriteListWidget=new SymbolListWidget(LeftPanelStackedWidget,6);
FavoriteListWidget->setFrameStyle(QFrame::NoFrame);
connect(FavoriteListWidget, SIGNAL(itemClicked ( QTableWidgetItem*)), this, SLOT(InsertSymbol(QTableWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/math7.png"),tr("Favorites symbols")), SIGNAL(triggered()), this, SLOT(ShowFavorite()));
FavoriteListWidget->SetFavoritePage(favoriteSymbolList);
connect(FavoriteListWidget->remAct, SIGNAL(triggered()), this, SLOT(RemoveFavoriteSymbols()));
LeftPanelStackedWidget->addWidget(FavoriteListWidget);

LeftPanelToolBar->addSeparator();

leftrightWidget=new XmlTagsListWidget(LeftPanelStackedWidget,":/tags/leftright_tags.xml");
leftrightWidget->setFrameStyle(QFrame::NoFrame);
connect(leftrightWidget, SIGNAL(itemClicked ( QListWidgetItem*)), this, SLOT(InsertXmlTag(QListWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/leftright.png"),"left/right"), SIGNAL(triggered()), this, SLOT(ShowLeftRight()));
LeftPanelStackedWidget->addWidget(leftrightWidget);

LeftPanelToolBar->addSeparator();

PsListWidget=new XmlTagsListWidget(LeftPanelStackedWidget,":/tags/pstricks_tags.xml");
PsListWidget->setFrameStyle(QFrame::NoFrame);
connect(PsListWidget, SIGNAL(itemClicked ( QListWidgetItem*)), this, SLOT(InsertXmlTag(QListWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/pstricks.png"),tr("Pstricks Commands")), SIGNAL(triggered()), this, SLOT(ShowPstricks()));
LeftPanelStackedWidget->addWidget(PsListWidget);

MpListWidget=new XmlTagsListWidget(LeftPanelStackedWidget,":/tags/metapost_tags.xml");
MpListWidget->setFrameStyle(QFrame::NoFrame);
connect(MpListWidget, SIGNAL(itemClicked ( QListWidgetItem*)), this, SLOT(InsertXmlTag(QListWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/metapost.png"),tr("MetaPost Commands")), SIGNAL(triggered()), this, SLOT(ShowMplist()));
LeftPanelStackedWidget->addWidget(MpListWidget);

tikzWidget=new XmlTagsListWidget(LeftPanelStackedWidget,":/tags/tikz_tags.xml");
tikzWidget->setFrameStyle(QFrame::NoFrame);
connect(tikzWidget, SIGNAL(itemClicked ( QListWidgetItem*)), this, SLOT(InsertXmlTag(QListWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/tikz.png"),tr("Tikz Commands")), SIGNAL(triggered()), this, SLOT(ShowTikz()));
LeftPanelStackedWidget->addWidget(tikzWidget);

asyWidget=new XmlTagsListWidget(LeftPanelStackedWidget,":/tags/asymptote_tags.xml");
asyWidget->setFrameStyle(QFrame::NoFrame);
connect(asyWidget, SIGNAL(itemClicked ( QListWidgetItem*)), this, SLOT(InsertXmlTag(QListWidgetItem*)));
connect(LeftPanelToolBar->addAction(QIcon(":/images/asymptote.png"),tr("Asymptote Commands")), SIGNAL(triggered()), this, SLOT(ShowAsy()));
LeftPanelStackedWidget->addWidget(asyWidget);


LeftPanelLayout->setSpacing(0);
LeftPanelLayout->setMargin(0);
LeftPanelLayout->addWidget(LeftPanelToolBar);
LeftPanelLayout->addWidget(LeftPanelStackedWidget);

LeftPanelStackedWidget->setCurrentWidget(StructureTreeWidget);
Act = new QAction(QIcon(":/images/empty.png"),"", this);
LeftPanelToolBarBis->addAction(Act);
Act->setEnabled(false);
titleLeftPanel=new QLabel(tr("Structure"),LeftPanelToolBarBis);
LeftPanelToolBarBis->addWidget(titleLeftPanel);

LeftPanelLayoutBis= new QVBoxLayout(LeftPanelFrameBis);
LeftPanelLayoutBis->setSpacing(0);
LeftPanelLayoutBis->setMargin(0);
LeftPanelLayoutBis->addWidget(LeftPanelToolBarBis);
LeftPanelLayoutBis->addWidget(LeftPanelFrame);

splitter1->addWidget(LeftPanelFrameBis);
LeftPanelFrameBis->setMinimumWidth(210);

Outputframe=new QFrame(this);
Outputframe->setLineWidth(0);
Outputframe->setFrameShape(QFrame::NoFrame);
Outputframe->setFrameShadow(QFrame::Plain);

OutputLayoutH= new QHBoxLayout(Outputframe);
OutputLayoutH->setSpacing(0);
OutputLayoutH->setMargin(0);

logToolBar=new QToolBar("LogToolBar",Outputframe);
logToolBar->setFloatable(false);
logToolBar->setOrientation(Qt::Vertical);
logToolBar->setMovable(false);
logToolBar->setIconSize(QSize(16,16 ));

QFrame *Outputframebis=new QFrame(this);
Outputframebis->setLineWidth(0);
Outputframebis->setFrameShape(QFrame::NoFrame);
Outputframebis->setFrameShadow(QFrame::Plain);

OutputLayoutV= new QVBoxLayout(Outputframebis);
OutputLayoutV->setSpacing(0);
OutputLayoutV->setMargin(0);

OutputTableWidget= new QTableWidget (1,5,Outputframebis);
//OutputTableWidget->setFrameShape(QFrame::Box);
OutputTableWidget->setFrameShadow(QFrame::Plain);
//OutputTableWidget->setFrameStyle(QFrame::NoFrame);
QTableWidgetItem *HeaderItem = new QTableWidgetItem(" ");
HeaderItem->setTextAlignment(Qt::AlignLeft);
OutputTableWidget->setHorizontalHeaderItem(0,HeaderItem);
HeaderItem = new QTableWidgetItem("File");
HeaderItem->setTextAlignment(Qt::AlignLeft);
OutputTableWidget->setHorizontalHeaderItem(1,HeaderItem);
HeaderItem = new QTableWidgetItem("Type");
HeaderItem->setTextAlignment(Qt::AlignLeft);
OutputTableWidget->setHorizontalHeaderItem(2,HeaderItem);
HeaderItem = new QTableWidgetItem("Line");
HeaderItem->setTextAlignment(Qt::AlignLeft);
OutputTableWidget->setHorizontalHeaderItem(3,HeaderItem);
HeaderItem = new QTableWidgetItem("Message");
HeaderItem->setTextAlignment(Qt::AlignLeft);
OutputTableWidget->setHorizontalHeaderItem(4,HeaderItem);

//OutputTableWidget->setWordWrap(true);
OutputTableWidget->setSelectionMode (QAbstractItemView::SingleSelection);
QFontMetrics fm(qApp->font());
OutputTableWidget->setColumnWidth(0,fm.width("> "));
OutputTableWidget->setColumnWidth(1,10*fm.width("w"));
OutputTableWidget->setColumnWidth(2,fm.width("WarningWW"));
OutputTableWidget->setColumnWidth(3,fm.width("Line WWWWWWWW"));
OutputTableWidget->setColumnWidth(4,20*fm.width("w"));
connect(OutputTableWidget, SIGNAL(itemClicked ( QTableWidgetItem*)), this, SLOT(ClickedOnLogLine(QTableWidgetItem*)));
OutputTableWidget->horizontalHeader()->setStretchLastSection(true);
OutputTableWidget->setMinimumHeight(5*(fm.lineSpacing()+4));
OutputTableWidget->verticalHeader()->hide();




OutputTextEdit = new LogEditor(Outputframebis);
//OutputTextEdit->setFrameStyle(QFrame::NoFrame);
OutputTextEdit->setFocusPolicy(Qt::ClickFocus);
OutputTextEdit->setMinimumHeight(3*(fm.lineSpacing()+4));
OutputTextEdit->setReadOnly(true);
connect(OutputTextEdit, SIGNAL(clickonline(int )),this,SLOT(ClickedOnOutput(int )));


OutputLayoutV->addWidget(OutputTableWidget);
OutputLayoutV->addWidget(OutputTextEdit);
OutputLayoutH->addWidget(logToolBar);
OutputLayoutH->addWidget(Outputframebis);

OutputTableWidget->hide();


logpresent=false;
listViewerCommands.clear();
checkViewerInstance=false;

errorFileList.clear();
errorTypeList.clear();
errorLineList.clear();
errorMessageList.clear();
errorLogList.clear();
onlyErrorList.clear();
errorIndex=-1;

translationList.clear();
translationList.append(QString("en"));
#if defined( Q_WS_X11 )

#ifdef USB_VERSION
QDir transdir(QCoreApplication::applicationDirPath());
#else
QDir transdir(PREFIX"/share/texmaker");
#endif

#endif
#if defined( Q_WS_MACX )
QDir transdir(QCoreApplication::applicationDirPath() + "/../Resources");
#endif
#if defined(Q_WS_WIN)
QDir transdir(QCoreApplication::applicationDirPath());
#endif
foreach (QFileInfo qmFileInfo, transdir.entryInfoList(QStringList("texmaker_*.qm"),QDir::Files | QDir::Readable, QDir::Name | QDir::IgnoreCase)) 
    {
    QString transName = qmFileInfo.completeBaseName();
    transName.remove("texmaker_");
    translationList.append(transName);
    }
    
    
StackedViewers=new QStackedWidget(this);
StackedViewers->setLineWidth(0);
StackedViewers->setFrameShape(QFrame::NoFrame);
StackedViewers->setFrameShadow(QFrame::Plain);
StackedViewers->setMinimumWidth(200);


// EDITEUR
QFrame *centralFrame=new QFrame(this);
centralFrame->setLineWidth(0);
centralFrame->setFrameShape(QFrame::NoFrame);
centralFrame->setFrameShadow(QFrame::Plain);


centralToolBar=new QToolBar("LogToolBar",centralFrame);
centralToolBar->setFloatable(false);
centralToolBar->setOrientation(Qt::Vertical);
centralToolBar->setMovable(false);
centralToolBar->setIconSize(QSize(16,16 ));

Act = new QAction(QIcon(":/images/text_bold.png"),tr("Bold"), this);
Act->setData("\\textbf{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/text_italic.png"),tr("Italic"), this);
Act->setData("\\textit{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/text_under.png"),tr("Underline"), this);
Act->setData("\\underline{/}/11/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/text_left.png"),tr("Left"), this);
Act->setData("\\begin{flushleft}\n/\n\\end{flushleft}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/text_center.png"),tr("Center"), this);
Act->setData("\\begin{center}\n/\n\\end{center}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/text_right.png"),tr("Right"), this);
Act->setData("\\begin{flushright}\n/\n\\end{flushright}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
centralToolBar->addAction(Act);
centralToolBar->addSeparator();

Act = new QAction(QIcon(":/images/newline.png"),tr("New line"), this);
Act->setData("\\\\\n/0/1/The \\newline command breaks the line right where it is. It can only be used in paragraph mode.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
centralToolBar->addAction(Act);

centralToolBar->addSeparator();

Act = new QAction(QIcon(":/images/mathmode.png"),"$...$", this);
Act->setData("$  $/2/0/The math environment can be used in both paragraph and LR mode");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/indice.png"),"_{} - subscript", this);
Act->setData("_{}/2/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/puissance.png"),"^{} - superscript", this);
Act->setData("^{}/2/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/smallfrac.png"),"\\frac{}{}", this);
Act->setData("\\frac{}{}/6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/dfrac.png"),"\\dfrac{}{}", this);
Act->setData("\\dfrac{}{}/7/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
centralToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/racine.png"),"\\sqrt{}", this);
Act->setData("\\sqrt{}/6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
centralToolBar->addAction(Act);

QFrame *centralFrameBis=new QFrame(this);
centralFrameBis->setLineWidth(0);
centralFrameBis->setFrameShape(QFrame::NoFrame);
centralFrameBis->setFrameShadow(QFrame::Plain);

centralToolBarBis=new QToolBar("FileBar",centralFrameBis);
centralToolBarBis->setFloatable(false);
centralToolBarBis->setOrientation(Qt::Horizontal);
centralToolBarBis->setMovable(false);
centralToolBarBis->setIconSize(QSize(16,16 ));
centralToolBarBis->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
//centralToolBarBis->setStyle(QStyleFactory::create("Plastique"));

ToggleDocAct=new QAction(QIcon(":/images/toggle.png"),tr("Toggle between the master document and the current document"), this);
connect(ToggleDocAct, SIGNAL(triggered()), this, SLOT(ToggleMasterCurrent()));
centralToolBarBis->addAction(ToggleDocAct);

Act = new QAction(QIcon(":/images/errorprev.png"),tr("Previous Document"), this);
Act->setShortcut(Qt::ALT+Qt::Key_PageUp);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoPrevDocument()));
centralToolBarBis->addAction(Act);
Act = new QAction(QIcon(":/images/errornext.png"),tr("Next Document"), this);
Act->setShortcut(Qt::ALT+Qt::Key_PageDown);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoNextDocument()));
centralToolBarBis->addAction(Act);


comboFiles=new QComboBox(centralToolBarBis);
comboFiles->setMaximumWidth(300);
comboFiles->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
comboFiles->setMinimumContentsLength(20);
comboFiles->setMaxVisibleItems(40);
comboFiles->setContextMenuPolicy(Qt::CustomContextMenu);
connect(comboFiles, SIGNAL(activated(int)), this, SLOT(listSelectionActivated(int)));
centralToolBarBis->addWidget(comboFiles);
QWidget* spacer = new QWidget();
spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
centralToolBarBis->addWidget(spacer);

Act = new QAction(QIcon(":/images/fileclose.png"), tr("Close"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(fileClose()));
centralToolBarBis->addAction(Act);
centralToolBarBis->addSeparator();
Act = new QAction(QIcon(":/images/bookmark1.png"),tr("Click to jump to the bookmark"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoBookmark1()));
centralToolBarBis->addAction(Act);
Act = new QAction(QIcon(":/images/bookmark2.png"),tr("Click to jump to the bookmark"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoBookmark2()));
centralToolBarBis->addAction(Act);
Act = new QAction(QIcon(":/images/bookmark3.png"),tr("Click to jump to the bookmark"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoBookmark3()));
centralToolBarBis->addAction(Act);

EditorView=new QStackedWidget(centralFrame);

connect(EditorView, SIGNAL( currentChanged( int ) ), this, SLOT(UpdateCaption()) );

CentralLayout= new QHBoxLayout(centralFrame);
CentralLayout->setSpacing(0);
CentralLayout->setMargin(0);
CentralLayout->addWidget(centralToolBar);
CentralLayout->addWidget(EditorView);

CentralLayoutBis= new QVBoxLayout(centralFrameBis);
CentralLayoutBis->setSpacing(0);
CentralLayoutBis->setMargin(0);
CentralLayoutBis->addWidget(centralToolBarBis);
CentralLayoutBis->addWidget(centralFrame);


splitter2->addWidget(centralFrameBis);
splitter2->addWidget(Outputframe);
splitter1->addWidget(splitter2);
splitter1->addWidget(StackedViewers);
setCentralWidget(splitter1);
splitter2->show();
splitter1->show();


QList<int> sizes;
sizes << height()-200 << 200;
splitter2->setSizes( sizes );
sizes.clear();
sizes << 180 << (int) (width()-180)*0.5 << (int) (width()-180)*0.5;
splitter1->setSizes( sizes );
//setCentralWidget(centralFrameBis);
QRect screen = QApplication::desktop()->screenGeometry();
largescreen=(screen.width()>1400);


createStatusBar();
setupMenus();
setupToolBars();
connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(clipboardDataChanged()));

QPalette pal = QApplication::palette();
QColor col=pal.color(QPalette::Window);

if (new_gui) 
{
restoreState(windowstate, 0);
splitter1->restoreState(splitter1state);
splitter2->restoreState(splitter2state);
}
ShowOutputView(false);
ShowStructView(false);
ShowPdfView(false);

if (largescreen && builtinpdfview && showpdfview ) StackedViewers->show();
else StackedViewers->hide();

UpdateRecentFile();

UpdateCaption();
singlemode=true;
ToggleDocAct->setEnabled(false);
MasterName=getName();

show();




LeftPanelToolBarBis->setMinimumHeight(centralToolBarBis->height());
stat1->setText(QString(" %1 ").arg(tr("Normal Mode")));
//stat2->setText(QString(" %1 ").arg(tr("Ready")));
stat3->setText(QString(" %1 ").arg(input_encoding));

setAcceptDrops(true);
connect(this, SIGNAL(windowActivated()),this, SLOT(mainWindowActivated()));
}

Texmaker::~Texmaker(){
if (spellChecker) delete spellChecker;
}

void Texmaker::setupMenus()
{
QAction *Act;
//file
fileMenu = menuBar()->addMenu(tr("&File"));
Act = new QAction(QIcon(":/images/filenew.png"), tr("New"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_N);
connect(Act, SIGNAL(triggered()), this, SLOT(fileNew()));
fileMenu->addAction(Act);

Act = new QAction(tr("New by copying an existing file"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(fileNewFromFile()));
fileMenu->addAction(Act);

Act = new QAction(QIcon(":/images/fileopen.png"), tr("Open"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_O);
connect(Act, SIGNAL(triggered()), this, SLOT(fileOpen()));
fileMenu->addAction(Act);

recentMenu=fileMenu->addMenu(tr("Open Recent"));
for (int i = 0; i < 10; ++i) 
	{
	recentFileActs[i] = new QAction(this);
	recentFileActs[i]->setVisible(false);
	connect(recentFileActs[i], SIGNAL(triggered()),this, SLOT(fileOpenRecent()));
	recentMenu->addAction(recentFileActs[i]);
	}

Act = new QAction(tr("Restore previous session"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(fileRestoreSession()));
fileMenu->addAction(Act);
Act->setEnabled(!sessionFilesList.isEmpty());

SaveAct = new QAction(QIcon(":/images/filesave.png"), tr("Save"), this);
SaveAct->setShortcut(Qt::CTRL+Qt::Key_S);
connect(SaveAct, SIGNAL(triggered()), this, SLOT(fileSave()));
fileMenu->addAction(SaveAct);
fileMenu->addSeparator();


Act = new QAction(tr("Save As"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(fileSaveAs()));
fileMenu->addAction(Act);

Act = new QAction(tr("Save All"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(fileSaveAll()));
fileMenu->addAction(Act);

Act = new QAction(QIcon(":/images/fileclose.png"), tr("Close"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_W);
connect(Act, SIGNAL(triggered()), this, SLOT(fileClose()));
fileMenu->addSeparator();
fileMenu->addAction(Act);

Act = new QAction(tr("Close All"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(fileCloseAll()));
fileMenu->addAction(Act);

Act = new QAction(tr("Reload document from file"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(fileReload()));
fileMenu->addAction(Act);

Act = new QAction(tr("Print"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_P);
connect(Act, SIGNAL(triggered()), this, SLOT(filePrint()));
fileMenu->addSeparator();
fileMenu->addAction(Act);

Act = new QAction(QIcon(":/images/exit.png"), tr("Exit"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_Q);
connect(Act, SIGNAL(triggered()), this, SLOT(fileExit()));
fileMenu->addSeparator();
fileMenu->addAction(Act);

editMenu = menuBar()->addMenu(tr("&Edit"));
UndoAct = new QAction(QIcon(":/images/undo.png"), tr("Undo"), this);
UndoAct->setShortcut(Qt::CTRL+Qt::Key_Z);
connect(UndoAct, SIGNAL(triggered()), this, SLOT(editUndo()));
editMenu->addAction(UndoAct);

RedoAct = new QAction(QIcon(":/images/redo.png"), tr("Redo"), this);
RedoAct->setShortcut(Qt::CTRL+Qt::Key_Y);
connect(RedoAct, SIGNAL(triggered()), this, SLOT(editRedo()));
editMenu->addAction(RedoAct);
editMenu->addSeparator();

CopyAct = new QAction(QIcon(":/images/editcopy.png"), tr("Copy"), this);
CopyAct->setShortcut(Qt::CTRL+Qt::Key_C);
connect(CopyAct, SIGNAL(triggered()), this, SLOT(editCopy()));
editMenu->addAction(CopyAct);

CutAct = new QAction(QIcon(":/images/editcut.png"), tr("Cut"), this);
CutAct->setShortcut(Qt::CTRL+Qt::Key_X);
connect(CutAct, SIGNAL(triggered()), this, SLOT(editCut()));
editMenu->addAction(CutAct);

PasteAct = new QAction(QIcon(":/images/editpaste.png"), tr("Paste"), this);
PasteAct->setShortcut(Qt::CTRL+Qt::Key_V);
connect(PasteAct, SIGNAL(triggered()), this, SLOT(editPaste()));
editMenu->addAction(PasteAct);

Act = new QAction( tr("Select All"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_A);
connect(Act, SIGNAL(triggered()), this, SLOT(editSelectAll()));
editMenu->addAction(Act);
editMenu->addSeparator();

Act = new QAction( tr("Comment"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_T);
connect(Act, SIGNAL(triggered()), this, SLOT(editComment()));
editMenu->addAction(Act);

Act = new QAction( tr("Uncomment"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_U);
connect(Act, SIGNAL(triggered()), this, SLOT(editUncomment()));
editMenu->addAction(Act);

Act = new QAction( tr("Indent"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_Greater);
connect(Act, SIGNAL(triggered()), this, SLOT(editIndent()));
editMenu->addAction(Act);

Act = new QAction( tr("Unindent"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_Less);
connect(Act, SIGNAL(triggered()), this, SLOT(editUnindent()));
editMenu->addAction(Act);
editMenu->addSeparator();

Act = new QAction( tr("Find"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_F);
connect(Act, SIGNAL(triggered()), this, SLOT(editFind()));
editMenu->addAction(Act);

Act = new QAction( tr("FindNext"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_M);
connect(Act, SIGNAL(triggered()), this, SLOT(editFindNext()));
editMenu->addAction(Act);

Act = new QAction( tr("Replace"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_R);
connect(Act, SIGNAL(triggered()), this, SLOT(editReplace()));
editMenu->addAction(Act);

Act = new QAction(QIcon(":/images/goto.png"), tr("Goto Line"), this);
Act->setShortcut(Qt::CTRL+Qt::Key_G);
connect(Act, SIGNAL(triggered()), this, SLOT(editGotoLine()));
editMenu->addAction(Act);
editMenu->addSeparator();

Act = new QAction(tr("Check Spelling"), this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_F7);
connect(Act, SIGNAL(triggered()), this, SLOT(editSpell()));
editMenu->addAction(Act);
editMenu->addSeparator();

Act = new QAction(tr("Refresh Structure"), this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_F1);
connect(Act, SIGNAL(triggered()), this, SLOT(refreshAll()));
editMenu->addAction(Act);

Act = new QAction(tr("Refresh Bibliography"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(UpdateBibliography()));
editMenu->addAction(Act);

toolMenu = menuBar()->addMenu(tr("&Tools"));
Act = new QAction(QIcon(":/images/quick.png"),tr("Quick Build"), this);
Act->setShortcut(Qt::Key_F1);
connect(Act, SIGNAL(triggered()), this, SLOT(QuickBuild()));
toolMenu->addAction(Act);
toolMenu->addSeparator();
Act = new QAction(QIcon(":/images/latex.png"),"LaTeX", this);
Act->setShortcut(Qt::Key_F2);
connect(Act, SIGNAL(triggered()), this, SLOT(Latex()));
toolMenu->addAction(Act);
Act = new QAction(QIcon(":/images/viewdvi.png"),tr("View Dvi"), this);
Act->setShortcut(Qt::Key_F3);
connect(Act, SIGNAL(triggered()), this, SLOT(ViewDvi()));
toolMenu->addAction(Act);
Act = new QAction(QIcon(":/images/dvips.png"),"Dvi->PS", this);
Act->setShortcut(Qt::Key_F4);
connect(Act, SIGNAL(triggered()), this, SLOT(DviToPS()));
toolMenu->addAction(Act);
Act = new QAction(QIcon(":/images/viewps.png"),tr("View PS"), this);
Act->setShortcut(Qt::Key_F5);
connect(Act, SIGNAL(triggered()), this, SLOT(ViewPS()));
toolMenu->addAction(Act);
Act = new QAction(QIcon(":/images/pdflatex.png"),"PDFLaTeX", this);
Act->setShortcut(Qt::Key_F6);
connect(Act, SIGNAL(triggered()), this, SLOT(PDFLatex()));
toolMenu->addAction(Act);
Act = new QAction(QIcon(":/images/viewpdf.png"),tr("View PDF"), this);
Act->setShortcut(Qt::Key_F7);
connect(Act, SIGNAL(triggered()), this, SLOT(ViewPDF()));
toolMenu->addAction(Act);
Act = new QAction(QIcon(":/images/ps2pdf.png"),"PS->PDF", this);
Act->setShortcut(Qt::Key_F8);
connect(Act, SIGNAL(triggered()), this, SLOT(PStoPDF()));
toolMenu->addAction(Act);
Act = new QAction(QIcon(":/images/dvipdf.png"),"DVI->PDF", this);
Act->setShortcut(Qt::Key_F9);
connect(Act, SIGNAL(triggered()), this, SLOT(DVItoPDF()));
toolMenu->addAction(Act);
Act = new QAction(QIcon(":/images/viewlog.png"),tr("View Log"), this);
Act->setShortcut(Qt::Key_F10);
connect(Act, SIGNAL(triggered()), this, SLOT(ViewLog()));
toolMenu->addAction(Act);
Act = new QAction("BibTeX", this);
Act->setShortcut(Qt::Key_F11);
connect(Act, SIGNAL(triggered()), this, SLOT(MakeBib()));
toolMenu->addAction(Act);
Act = new QAction("MakeIndex", this);
Act->setShortcut(Qt::Key_F12);
connect(Act, SIGNAL(triggered()), this, SLOT(MakeIndex()));
toolMenu->addAction(Act);
toolMenu->addSeparator();
Act = new QAction("MPost", this);
connect(Act, SIGNAL(triggered()), this, SLOT(MetaPost()));
toolMenu->addAction(Act);
Act = new QAction("Asymptote", this);
connect(Act, SIGNAL(triggered()), this, SLOT(Asymptote()));
toolMenu->addAction(Act);
Act = new QAction("Latexmk", this);
connect(Act, SIGNAL(triggered()), this, SLOT(LatexMk()));
toolMenu->addAction(Act);
toolMenu->addSeparator();
Act = new QAction(tr("Clean"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(CleanAll()));
toolMenu->addAction(Act);

Act = new QAction(QIcon(":/images/errorprev.png"),tr("Previous LaTeX Error"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(PreviousError()));
Act = new QAction(QIcon(":/images/errornext.png"),tr("Next LaTeX Error"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(NextError()));

latex1Menu = menuBar()->addMenu(tr("&LaTeX"));
Act = new QAction("\\documentclass", this);
Act->setData("\\documentclass[10pt]{}/21/0/\\documentclass[options]{class}");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\usepackage{}", this);
Act->setData("\\usepackage{} /12/0/\\usepackage[options]{pkg}");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("AMS packages", this);
Act->setData("\\usepackage{amsmath}\n\\usepackage{amsfonts}\n\\usepackage{amssymb}\n/0/3/The main American Mathematical Society packages");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\begin{document}", this);
Act->setData("\\begin{document}\n\n\\end{document}/0/1/Text is allowed only between \\begin{document} and \\end{document}.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\author{}", this);
Act->setData("\\author{}/8/0/\\author{names}\nThe \\author command declares the author(s), where names is a list of authors separated by \\and commands.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\title{}", this);
Act->setData("\\title{}/7/0/\\title{text}\nThe \\title command declares text to be the title.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\maketitle", this);
Act->setData("\\maketitle/10/0/This command generates a title on a separate title page\n- except in the article class, where the title normally goes at the top of the first page.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\tableofcontents", this);
Act->setData("\\tableofcontents/16/0/Put this command where you want the table of contents to go");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);

latex11Menu=latex1Menu->addMenu(tr("&Sectioning"));
Act = new QAction("\\part", this);
Act->setData("\\part");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertStruct()));
latex11Menu->addAction(Act);
Act = new QAction("\\chapter", this);
Act->setData("\\chapter");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertStruct()));
latex11Menu->addAction(Act);
Act = new QAction("\\section", this);
Act->setData("\\section");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertStruct()));
latex11Menu->addAction(Act);
Act = new QAction("\\subsection", this);
Act->setData("\\subsection");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertStruct()));
latex11Menu->addAction(Act);
Act = new QAction("\\subsubsection", this);
Act->setData("\\subsubsection");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertStruct()));
latex11Menu->addAction(Act);
Act = new QAction("\\paragraph", this);
Act->setData("\\paragraph");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertStruct()));
latex11Menu->addAction(Act);
Act = new QAction("\\subparagraph", this);
Act->setData("\\subparagraph");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertStruct()));
latex11Menu->addAction(Act);

latex12Menu=latex1Menu->addMenu(tr("&Environment"));
Act = new QAction(QIcon(":/images/text_center.png"),"\\begin{center} [selection]", this);
Act->setData("\\begin{center}\n/\n\\end{center}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/text_left.png"),"\\begin{flushleft} [selection]", this);
Act->setData("\\begin{flushleft}\n/\n\\end{flushleft}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/text_right.png"),"\\begin{flushright}  [selection]", this);
Act->setData("\\begin{flushright}\n/\n\\end{flushright}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);
Act = new QAction("\\begin{quote}  [selection]", this);
Act->setData("\\begin{quote}\n/\n\\end{quote}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);
Act = new QAction("\\begin{quotation}  [selection]", this);
Act->setData("\\begin{quotation}\n/\n\\end{quotation}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);
Act = new QAction("\\begin{verse}  [selection]", this);
Act->setData("\\begin{verse}\n/\n\\end{verse}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);
Act = new QAction("\\begin{verbatim}  [selection]", this);
Act->setData("\\begin{verbatim}\n/\n\\end{verbatim}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);
Act = new QAction("\\begin{table}  [selection]", this);
Act->setData("\\begin{table}\n/\n\\caption{}\n\\end{table}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);
Act = new QAction("\\begin{figure}  [selection]", this);
Act->setData("\\begin{figure}\n/\n\\caption{}\n\\end{figure}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);
Act = new QAction("\\begin{titlepage}  [selection]", this);
Act->setData("\\begin{titlepage}\n/\n\\end{titlepage}/0/1");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex12Menu->addAction(Act);

latex13Menu=latex1Menu->addMenu(tr("&List Environment"));
Act = new QAction(QIcon(":/images/itemize.png"),"\\begin{itemize}", this);
Act->setData("\\begin{itemize}\n\\item \n\\end{itemize}/6/1/The itemize environment produces a 'bulleted' list.\nEach item of an itemized list begins with an \\item command.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex13Menu->addAction(Act);
Act = new QAction(QIcon(":/images/enumerate.png"),"\\begin{enumerate}", this);
Act->setData("\\begin{enumerate}\n\\item \n\\end{enumerate}/6/1/The enumerate environment produces a numbered list.\nEach item of an enumerated list begins with an \\item command.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex13Menu->addAction(Act);
Act = new QAction("\\begin{description}", this);
Act->setData("\\begin{description}\n\\item[]\n\\end{description}/6/1/The description environment is used to make labelled lists.\nEach item of the list begins with an \\item[label] command.\n");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex13Menu->addAction(Act);
Act = new QAction("\\begin{list}", this);
Act->setData("\\begin{list}{}{}\n\\item \n\\end{list}/13/0/\\begin{list}{label}{spacing}\nThe {label} argument is a piece of text that is inserted in a box to form the label.\nThe {spacing} argument contains commands to change the spacing parameters for the list.\nEach item of the list begins with an \\item command.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex13Menu->addAction(Act);

Act = new QAction(QIcon(":/images/item.png"),"\\item", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_I);
Act->setData("\\item/5/0/\\item[label] Hello");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex13Menu->addAction(Act);

latex14Menu=latex1Menu->addMenu(tr("Font St&yles"));
Act = new QAction(QIcon(":/images/text_italic.png"),"\\textit - Italics  [selection]", this);
Act->setShortcut(Qt::CTRL+Qt::Key_I);
Act->setData("\\textit{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex14Menu->addAction(Act);
Act = new QAction("\\textsl - Slanted  [selection]", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_S);
Act->setData("\\textsl{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex14Menu->addAction(Act);
Act = new QAction(QIcon(":/images/text_bold.png"),"\\textbf - Boldface  [selection]", this);
Act->setShortcut(Qt::CTRL+Qt::Key_B);
Act->setData("\\textbf{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex14Menu->addAction(Act);
Act = new QAction("\\texttt - Typewriter  [selection]", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_T);
Act->setData("\\texttt{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex14Menu->addAction(Act);
Act = new QAction("\\textsc - Small caps  [selection]", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_C);
Act->setData("\\textsc{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex14Menu->addAction(Act);
Act = new QAction("\\textsf - Sans Serif  [selection]", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_A);
Act->setData("\\textsf{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex14Menu->addAction(Act);
Act = new QAction("\\emph - Emphasis  [selection]", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_E);
Act->setData("\\emph{/}/6/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
latex14Menu->addAction(Act);

latex15Menu=latex1Menu->addMenu(tr("&Tabular Environment"));
Act = new QAction("\\begin{tabbing}", this);
Act->setData("\\begin{tabbing}\n\n\\end{tabbing}/0/1/\\begin{tabbing}\ntext \\= more text \\= still more text \\= last text \\\\\nsecond row \\>  \\> more \\\\\n\\end{tabbing}");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex15Menu->addAction(Act);
Act = new QAction("\\begin{tabular}", this);
Act->setData("\\begin{tabular}{}\n\n\\end{tabular}/16/0/\\begin{tabular}[pos]{cols}\ncolumn 1 entry & column 2 entry ... & column n entry \\\\\n...\n\\end{tabular}");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex15Menu->addAction(Act);
Act = new QAction("\\multicolumn", this);
Act->setData("\\multicolumn{}{}{} /13/0/\\multicolumn{cols}{pos}{text}\ncol, specifies the number of columns to span.\npos specifies the formatting of the entry: c for centred, l for flushleft, r for flushright.\ntext specifies what text is to make up the entry.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex15Menu->addAction(Act);
Act = new QAction("\\hline", this);
Act->setData("\\hline /7/0/The \\hline command draws a horizontal line the width of the table.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex15Menu->addAction(Act);
Act = new QAction("\\vline", this);
Act->setData("\\vline /7/0/The \\vline command draws a vertical line extending the full height and depth of its row.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex15Menu->addAction(Act);
Act = new QAction("\\cline", this);
Act->setData("\\cline{-} /7/0/The \\cline{i-j} command draws horizontal lines across the columns specified, beginning in column i and ending in column j");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex15Menu->addAction(Act);

latex16Menu=latex1Menu->addMenu(tr("S&pacing"));
Act = new QAction("\\newpage", this);
Act->setData("\\newpage /9/0/The \\newpage command ends the current page");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex16Menu->addAction(Act);
Act = new QAction("\\linebreak", this);
Act->setData("\\linebreak /11/0/The \\linebreak command tells LaTeX to break the current line at the point of the command.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex16Menu->addAction(Act);
Act = new QAction("\\pagebreak", this);
Act->setData("\\pagebreak /11/0/The \\pagebreak command tells LaTeX to break the current page at the point of the command.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex16Menu->addAction(Act);
Act = new QAction("\\bigskip", this);
Act->setData("\\bigskip /9/0/The \\bigskip command adds a 'big' vertical space.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex16Menu->addAction(Act);
Act = new QAction("\\medskip", this);
Act->setData("\\medskip /9/0/The \\medskip command adds a 'medium' vertical space.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex16Menu->addAction(Act);
Act = new QAction(QIcon(":/images/newline.png"),"New line", this);
Act->setData("\\\\\n/0/1/The \\newline command breaks the line right where it is.");
Act->setShortcut(Qt::CTRL+Qt::Key_Return);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex16Menu->addAction(Act);

latex17Menu=latex1Menu->addMenu(tr("International &Accents"));
Act = new QAction(QIcon(":/images/accent1.png"),"\\'{}", this);
Act->setData("\\'{}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);
Act = new QAction(QIcon(":/images/accent2.png"),"\\`{}", this);
Act->setData("\\`{}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);
Act = new QAction(QIcon(":/images/accent3.png"),"\\^{}", this);
Act->setData("\\^{}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);
Act = new QAction(QIcon(":/images/accent4.png"),"\\\"{}", this);
Act->setData("\\\"{}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);
Act = new QAction(QIcon(":/images/accent5.png"),"\\~{}", this);
Act->setData("\\~{}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);
Act = new QAction(QIcon(":/images/accent6.png"),"\\={}", this);
Act->setData("\\={}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);
Act = new QAction(QIcon(":/images/accent7.png"),"\\.{}", this);
Act->setData("\\.{}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);
Act = new QAction(QIcon(":/images/accent8.png"),"\\v{}", this);
Act->setData("\\v{}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);
Act = new QAction(QIcon(":/images/accent9.png"),"\\u{}", this);
Act->setData("\\u{}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);
Act = new QAction(QIcon(":/images/accent10.png"),"\\H{}", this);
Act->setData("\\H{}/3/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex17Menu->addAction(Act);

Act = new QAction("\\includegraphics{file}", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertImage()));
latex1Menu->addAction(Act);
Act = new QAction("\\include{file}", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertInclude()));
latex1Menu->addAction(Act);
Act = new QAction("\\input{file}", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertInput()));
latex1Menu->addAction(Act);
latex1Menu->addSeparator();

Act = new QAction("\\label{}", this);
Act->setData("\\label{} /7/0/\\label{key}");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\cite{}", this);
Act->setData("\\cite{} /6/0/\\cite{ref} :\nThis command generates an in-text citation to the reference associated with the ref entry in the bib file");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\footnote{}", this);
Act->setData("\\footnote{} /10/0/\\footnote[number]{text}\nThe \\footnote command places the numbered footnote text at the bottom of the current page.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\bibliographystyle{}", this);
Act->setData("\\bibliographystyle{} /19/0/The argument to \\bibliographystyle refers to a file style.bst, which defines how your citations will look");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
latex1Menu->addAction(Act);
Act = new QAction("\\bibliography{}", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib()));
latex1Menu->addAction(Act);

math1Menu = menuBar()->addMenu(tr("&Math"));
Act = new QAction(tr("Inline math mode $...$"), this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_M);
Act->setData("$"+QString(0x2022)+"$/2/0/The math environment can be used in both paragraph and LR mode");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction(tr("Display math mode \\[...\\]"), this);
Act->setShortcut(Qt::ALT+Qt::SHIFT+Qt::Key_M);
Act->setData("\\["+QString(0x2022)+"\\]/3/0/The displaymath environment can be used only in paragraph mode");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction(tr("Numbered equations \\begin{equation}"), this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_N);
Act->setData("\\begin{equation}\n\n\\end{equation}/0/1/The equation environment centres your equation on the page and places the equation number in the right margin.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("\\begin{eqnarray}", this);
Act->setData("\\begin{eqnarray}\n\n\\end{eqnarray}/0/1/\\begin{eqnarray}\nmath formula 1 \\\\\n\\end{eqnarray}\nThe eqnarray environment is used to display a sequence of equations or inequalities.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("\\begin{align} (AMS)", this);
Act->setData("\\begin{align}\n\n\\end{align}/0/1/\\begin{align}\nmath formula 1 \\\\\n\\end{align}\nThe AMS align environment is used to display a sequence of equations or inequalities.");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("_{} - subscript", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_D);
Act->setData("_{}/2/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("^{} - superscript", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_U);
Act->setData("^{}/2/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("\\frac{}{}", this);
Act->setShortcut(Qt::ALT+Qt::SHIFT+Qt::Key_F);
Act->setData("\\frac{}{}/6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("\\dfrac{}{}", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_F);
Act->setData("\\dfrac{}{}/7/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("\\sqrt{}", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_Q);
Act->setData("\\sqrt{}/6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("\\left", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_L);
Act->setData("\\left /6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("\\right", this);
Act->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_R);
Act->setData("\\right /7/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);
Act = new QAction("\\begin{array}", this);
Act->setData("\\begin{array}{}\n\n\\end{array}/14/0/\\begin{array}{col1col2...coln}\ncolumn 1 entry & column 2 entry ... & column n entry \\\\\n...\n\\end{array}");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math1Menu->addAction(Act);

math14Menu=math1Menu->addMenu(tr("Math &Functions"));
Act = new QAction("\\arccos", this);
Act->setData("\\arccos /8/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\arcsin", this);
Act->setData("\\arcsin /8/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\arctan", this);
Act->setData("\\arctan /8/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\cos", this);
Act->setData("\\cos /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\cosh", this);
Act->setData("\\cosh /6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\cot", this);
Act->setData("\\cot /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\coth", this);
Act->setData("\\coth /6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\csc", this);
Act->setData("\\csc /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\deg", this);
Act->setData("\\deg /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\det", this);
Act->setData("\\det /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\dim", this);
Act->setData("\\dim /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\exp", this);
Act->setData("\\exp /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\gcd", this);
Act->setData("\\gcd /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\hom", this);
Act->setData("\\hom /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\inf", this);
Act->setData("\\inf /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\ker", this);
Act->setData("\\ker /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\lg", this);
Act->setData("\\lg /4/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\lim", this);
Act->setData("\\lim /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\liminf", this);
Act->setData("\\liminf /8/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\limsup", this);
Act->setData("\\limsup /8/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\ln", this);
Act->setData("\\ln /4/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\log", this);
Act->setData("\\log /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\max", this);
Act->setData("\\max /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\min", this);
Act->setData("\\min /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\sec", this);
Act->setData("\\sec /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\sin", this);
Act->setData("\\sin /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\sinh", this);
Act->setData("\\sinh /6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\sup", this);
Act->setData("\\sup /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\tan", this);
Act->setData("\\tan /5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);
Act = new QAction("\\tanh", this);
Act->setData("\\tanh /6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math14Menu->addAction(Act);

math11Menu=math1Menu->addMenu(tr("Math Font St&yles"));
Act = new QAction("\\mathrm{}  [selection]", this);
Act->setData("\\mathrm{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
math11Menu->addAction(Act);
Act = new QAction("\\mathit{}  [selection]", this);
Act->setData("\\mathit{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
math11Menu->addAction(Act);
Act = new QAction("\\mathbf{}  [selection]", this);
Act->setData("\\mathbf{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
math11Menu->addAction(Act);
Act = new QAction("\\mathsf{}  [selection]", this);
Act->setData("\\mathsf{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
math11Menu->addAction(Act);
Act = new QAction("\\mathtt{}  [selection]", this);
Act->setData("\\mathtt{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
math11Menu->addAction(Act);
Act = new QAction("\\mathcal{}  [selection]", this);
Act->setData("\\mathcal{/}/9/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
math11Menu->addAction(Act);
Act = new QAction("\\mathbb{}  [selection]", this);
Act->setData("\\mathbb{/}/8/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
math11Menu->addAction(Act);
Act = new QAction("\\mathfrak{}  [selection]", this);
Act->setData("\\mathfrak{/}/10/0");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertWithSelectionFromAction()));
math11Menu->addAction(Act);

math12Menu=math1Menu->addMenu(tr("Math &Accents"));
Act = new QAction(QIcon(":/images/acute.png"),"\\acute{}", this);
Act->setData("\\acute{}/7/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/grave.png"),"\\grave{}", this);
Act->setData("\\grave{}/7/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/tilde.png"),"\\tilde{}", this);
Act->setData("\\tilde{}/7/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/bar.png"),"\\bar{}", this);
Act->setData("\\bar{}/5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/vec.png"),"\\vec{}", this);
Act->setData("\\vec{}/5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/hat.png"),"\\hat{}", this);
Act->setData("\\hat{}/5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/check.png"),"\\check{}", this);
Act->setData("\\check{}/7/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/breve.png"),"\\breve{}", this);
Act->setData("\\breve{}/7/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/dot.png"),"\\dot{}", this);
Act->setData("\\dot{}/5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);
Act = new QAction(QIcon(":/images/ddtot.png"),"\\ddot{}", this);
Act->setData("\\ddot{}/6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math12Menu->addAction(Act);

math13Menu=math1Menu->addMenu(tr("Math S&paces"));
Act = new QAction("small", this);
Act->setData("\\,/2/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math13Menu->addAction(Act);
Act = new QAction("medium", this);
Act->setData("\\:/2/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math13Menu->addAction(Act);
Act = new QAction("large", this);
Act->setData("\\;/2/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math13Menu->addAction(Act);
Act = new QAction("\\quad", this);
Act->setData("\\quad/5/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math13Menu->addAction(Act);
Act = new QAction("\\qquad", this);
Act->setData("\\qquad/6/0/ ");
connect(Act, SIGNAL(triggered()), this, SLOT(InsertFromAction()));
math13Menu->addAction(Act);

wizardMenu = menuBar()->addMenu(tr("&Wizard"));
Act = new QAction(tr("Quick Start"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(QuickDocument()));
wizardMenu->addAction(Act);
Act = new QAction(tr("Quick Letter"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(QuickLetter()));
wizardMenu->addAction(Act);
wizardMenu->addSeparator();
Act = new QAction(tr("Quick Tabular"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(QuickTabular()));
wizardMenu->addAction(Act);
Act = new QAction(tr("Quick Tabbing"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(QuickTabbing()));
wizardMenu->addAction(Act);
Act = new QAction(tr("Quick Array"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(QuickArray()));
wizardMenu->addAction(Act);

bibMenu = menuBar()->addMenu(tr("&Bibliography"));
Act = new QAction("Article in Journal", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib1()));
bibMenu->addAction(Act);
Act = new QAction("Article in Conference Proceedings", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib2()));
bibMenu->addAction(Act);
Act = new QAction("Article in a collection", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib3()));
bibMenu->addAction(Act);
Act = new QAction("Chapter or Pages in a Book", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib4()));
bibMenu->addAction(Act);
Act = new QAction("Conference Proceedings", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib5()));
bibMenu->addAction(Act);
Act = new QAction("Book", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib6()));
bibMenu->addAction(Act);
Act = new QAction("Booklet", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib7()));
bibMenu->addAction(Act);
Act = new QAction("PhD. Thesis", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib8()));
bibMenu->addAction(Act);
Act = new QAction("Master's Thesis", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib9()));
bibMenu->addAction(Act);
Act = new QAction("Technical Report", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib10()));
bibMenu->addAction(Act);
Act = new QAction("Technical Manual", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib11()));
bibMenu->addAction(Act);
Act = new QAction("Unpublished", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib12()));
bibMenu->addAction(Act);
Act = new QAction("Miscellaneous", this);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertBib13()));
bibMenu->addAction(Act);
bibMenu->addSeparator();
Act = new QAction(tr("Clean"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(CleanBib()));
bibMenu->addAction(Act);

user1Menu = menuBar()->addMenu(tr("&User"));
user11Menu=user1Menu->addMenu(tr("User &Tags"));
Act = new QAction("1: "+UserMenuName[0], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F1);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag1()));
user11Menu->addAction(Act);
Act = new QAction("2: "+UserMenuName[1], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F2);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag2()));
user11Menu->addAction(Act);
Act = new QAction("3: "+UserMenuName[2], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F3);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag3()));
user11Menu->addAction(Act);
Act = new QAction("4: "+UserMenuName[3], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F4);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag4()));
user11Menu->addAction(Act);
Act = new QAction("5: "+UserMenuName[4], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F5);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag5()));
user11Menu->addAction(Act);
Act = new QAction("6: "+UserMenuName[5], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F6);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag6()));
user11Menu->addAction(Act);
Act = new QAction("7: "+UserMenuName[6], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F7);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag7()));
user11Menu->addAction(Act);
Act = new QAction("8: "+UserMenuName[7], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F8);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag8()));
user11Menu->addAction(Act);
Act = new QAction("9: "+UserMenuName[8], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F9);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag9()));
user11Menu->addAction(Act);
Act = new QAction("10: "+UserMenuName[9], this);
Act->setShortcut(Qt::SHIFT+Qt::Key_F10);
connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag10()));
user11Menu->addAction(Act);
user11Menu->addSeparator();
Act = new QAction(tr("Edit User &Tags"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(EditUserMenu()));
user11Menu->addAction(Act);
user12Menu=user1Menu->addMenu(tr("User &Commands"));
Act = new QAction("1: "+UserToolName[0], this);
Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F1);
connect(Act, SIGNAL(triggered()), this, SLOT(UserTool1()));
user12Menu->addAction(Act);
Act = new QAction("2: "+UserToolName[1], this);
Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F2);
connect(Act, SIGNAL(triggered()), this, SLOT(UserTool2()));
user12Menu->addAction(Act);
Act = new QAction("3: "+UserToolName[2], this);
Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F3);
connect(Act, SIGNAL(triggered()), this, SLOT(UserTool3()));
user12Menu->addAction(Act);
Act = new QAction("4: "+UserToolName[3], this);
Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F4);
connect(Act, SIGNAL(triggered()), this, SLOT(UserTool4()));
user12Menu->addAction(Act);
Act = new QAction("5: "+UserToolName[4], this);
Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F5);
connect(Act, SIGNAL(triggered()), this, SLOT(UserTool5()));
user12Menu->addAction(Act);
user12Menu->addSeparator();
Act = new QAction(tr("Edit User &Commands"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(EditUserTool()));
user12Menu->addAction(Act);

Act = new QAction(tr("Customize Completion"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(EditUserCompletion()));
user1Menu->addAction(Act);

viewMenu = menuBar()->addMenu(tr("&View"));
Act = new QAction(tr("Next Document"), this);
Act->setShortcut(Qt::ALT+Qt::Key_PageDown);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoNextDocument()));
viewMenu->addAction(Act);
Act = new QAction(tr("Previous Document"), this);
Act->setShortcut(Qt::ALT+Qt::Key_PageUp);
connect(Act, SIGNAL(triggered()), this, SLOT(gotoPrevDocument()));
viewMenu->addAction(Act);
viewMenu->addSeparator();

ViewStructurePanelAct = new QAction(tr("Structure"), this);
ViewStructurePanelAct->setCheckable(true);
connect(ViewStructurePanelAct, SIGNAL(triggered()), this, SLOT(ToggleStructurePanel()));
ViewStructurePanelAct->setChecked(showstructview);
viewMenu->addAction(ViewStructurePanelAct);
ViewLogPanelAct = new QAction(tr("Messages / Log File"), this);
ViewLogPanelAct->setCheckable(true);
ViewLogPanelAct->setChecked(showoutputview);
connect(ViewLogPanelAct, SIGNAL(triggered()), this, SLOT(ToggleLogPanel()));
viewMenu->addAction(ViewLogPanelAct);

ViewPdfPanelAct = new QAction(tr("Pdf Viewer"), this);
ViewPdfPanelAct->setCheckable(true);
ViewPdfPanelAct->setChecked(showpdfview);
connect(ViewPdfPanelAct, SIGNAL(triggered()), this, SLOT(TogglePdfPanel()));
if (largescreen) 
  {
  viewMenu->addAction(ViewPdfPanelAct);
  if (builtinpdfview) ViewPdfPanelAct->setEnabled(true);
  else ViewPdfPanelAct->setEnabled(false);
  }
viewMenu->addSeparator();

FullScreenAct = new QAction(tr("Full Screen"), this);
FullScreenAct->setShortcut(Qt::CTRL+Qt::SHIFT+Qt::Key_F11);
FullScreenAct->setCheckable(true);
FullScreenAct->setChecked(false);
connect(FullScreenAct, SIGNAL(triggered()), this, SLOT(ToggleFullScreen()));
viewMenu->addAction(FullScreenAct);


optionsMenu = menuBar()->addMenu(tr("&Options"));
Act = new QAction(QIcon(":/images/configure.png"), tr("Configure Texmaker"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(GeneralOptions()));
optionsMenu->addAction(Act);
optionsMenu->addSeparator();
ToggleAct = new QAction(tr("Define Current Document as 'Master Document'"), this);
connect(ToggleAct, SIGNAL(triggered()), this, SLOT(ToggleMode()));
optionsMenu->addAction(ToggleAct);
optionsMenu->addSeparator();
appearanceMenu=optionsMenu->addMenu(tr("Interface Appearance"));
appearanceGroup = new QActionGroup(this);
Act = new QAction("Modern", this);
Act->setCheckable(true);
connect(Act, SIGNAL(triggered()), this, SLOT(updateAppearance()));
appearanceGroup->addAction(Act);
if (modern_style) Act->setChecked(true);
appearanceMenu->addAction(Act);
Act = new QAction("Classic", this);
Act->setCheckable(true);
connect(Act, SIGNAL(triggered()), this, SLOT(updateAppearance()));
appearanceGroup->addAction(Act);
if (!modern_style) Act->setChecked(true);
appearanceMenu->addAction(Act);
#if defined( Q_WS_X11 )
Act = new QAction(tr("Change Interface Font"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(SetInterfaceFont()));
optionsMenu->addAction(Act);
#endif
optionsMenu->addSeparator();
translationMenu=optionsMenu->addMenu(tr("Interface Language"));
translationGroup = new QActionGroup(this);
QString currentTranslation=TexmakerApp::instance()->language;
for (int i=0; i < translationList.count(); i++)
	{
	Act = new QAction(translationList.at(i), this);
	Act->setCheckable(true);
	connect(Act, SIGNAL(triggered()), this, SLOT(updateTranslation()));
	translationGroup->addAction(Act);
	if ((currentTranslation==translationList.at(i)) || (currentTranslation.left(2)==translationList.at(i)) )
	    {
	    Act->setChecked(true);
	    }
	translationMenu->addAction(Act);
	}

helpMenu = menuBar()->addMenu(tr("&Help"));
Act = new QAction(QIcon(":/images/help.png"), tr("LaTeX Reference"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(LatexHelp()));
helpMenu->addAction(Act);
Act = new QAction(QIcon(":/images/help.png"), tr("User Manual"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(UserManualHelp()));
helpMenu->addAction(Act);
helpMenu->addSeparator();
Act = new QAction(QIcon(":/images/appicon.png"), tr("About Texmaker"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(HelpAbout()));
helpMenu->addAction(Act);

QList<QAction *> listaction;
if (shortcuts.isEmpty())
	{
	actionstext.clear();
	listaction << latex1Menu->actions();
	listaction << latex11Menu->actions();
	listaction << latex12Menu->actions();
	listaction << latex13Menu->actions();
	listaction << latex14Menu->actions();
	listaction << latex15Menu->actions();
	listaction << latex16Menu->actions();
	listaction << latex17Menu->actions();
	listaction << math1Menu->actions();
	listaction << math11Menu->actions();
	listaction << math12Menu->actions();
	listaction << math13Menu->actions();
	listaction << math14Menu->actions();
	QListIterator<QAction*> iterator(listaction);
	while ( iterator.hasNext() )
		{
		QAction *action = iterator.next();
		if (action && (!action->menu()) && (!action->data().toString().isEmpty())) 
			{
			if (action->shortcut().isEmpty()) shortcuts.insert(action->data().toString(),"none");
			else shortcuts.insert(action->data().toString(),action->shortcut().toString(QKeySequence::PortableText));
			actionstext.insert(action->data().toString(),action->text());
			}
		}
	}
else ModifyShortcuts();
}

void Texmaker::setupToolBars()
{
QAction *Act;
QStringList list;
//file
fileToolBar = addToolBar("File");
fileToolBar->setObjectName("File");

Act = new QAction(QIcon(":/images/filenew.png"), tr("New"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(fileNew()));
fileToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/fileopen.png"), tr("Open"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(fileOpen()));
fileToolBar->addAction(Act);

//Act = new QAction(QIcon(":/images/filesave.png"), tr("Save"), this);
//connect(Act, SIGNAL(triggered()), this, SLOT(fileSave()));
fileToolBar->addAction(SaveAct);

//Act = new QAction(QIcon(":/images/fileclose.png"), tr("Close"), this);
//connect(Act, SIGNAL(triggered()), this, SLOT(fileClose()));
//fileToolBar->addAction(Act);

//edit
editToolBar = addToolBar("Edit");
editToolBar->setObjectName("Edit");

//Act = new QAction(QIcon(":/images/undo.png"), tr("Undo"), this);
//connect(Act, SIGNAL(triggered()), this, SLOT(editUndo()));
editToolBar->addAction(UndoAct);

//Act = new QAction(QIcon(":/images/redo.png"), tr("Redo"), this);
//connect(Act, SIGNAL(triggered()), this, SLOT(editRedo()));
editToolBar->addAction(RedoAct);

//Act = new QAction(QIcon(":/images/editcopy.png"), tr("Copy"), this);
//connect(Act, SIGNAL(triggered()), this, SLOT(editCopy()));
editToolBar->addAction(CopyAct);

//Act = new QAction(QIcon(":/images/editcut.png"), tr("Cut"), this);
//connect(Act, SIGNAL(triggered()), this, SLOT(editCut()));
editToolBar->addAction(CutAct);

//Act = new QAction(QIcon(":/images/editpaste.png"), tr("Paste"), this);
//connect(Act, SIGNAL(triggered()), this, SLOT(editPaste()));
editToolBar->addAction(PasteAct);

//format
formatToolBar = addToolBar("Format");
formatToolBar->setObjectName("Format");
//insertToolBarBreak(formatToolBar);

list.clear();
list.append("part");
list.append("chapter");
list.append("section");
list.append("subsection");
list.append("subsubsection");
list.append("paragraph");
list.append("subparagraph");
QComboBox* combo1 = new QComboBox(formatToolBar);
combo1->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
combo1->addItems(list);
connect(combo1, SIGNAL(activated(const QString&)),this,SLOT(SectionCommand(const QString&)));
formatToolBar->addWidget(combo1);

list.clear();
list.append("label");
list.append("ref");
list.append("pageref");
list.append("index");
list.append("cite");
list.append("footnote");
QComboBox* combo2 = new QComboBox(formatToolBar);
combo2->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
combo2->addItems(list);
connect(combo2, SIGNAL(activated(const QString&)),this,SLOT(OtherCommand(const QString&)));
formatToolBar->addWidget(combo2);

list.clear();
list.append("tiny");
list.append("scriptsize");
list.append("footnotesize");
list.append("small");
list.append("normalsize");
list.append("large");
list.append("Large");
list.append("LARGE");
list.append("huge");
list.append("Huge");
QComboBox* combo3 = new QComboBox(formatToolBar);
combo3->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
combo3->addItems(list);
connect(combo3, SIGNAL(activated(const QString&)),this,SLOT(SizeCommand(const QString&)));
formatToolBar->addWidget(combo3);

//tools
runToolBar = addToolBar("Tools");
runToolBar->setObjectName("Tools");

list.clear();
list.append(tr("Quick Build"));
list.append("LaTeX");
list.append("Dvi->PS");
list.append("PDFLaTeX");
list.append("BibTeX");
list.append("MakeIndex");
list.append("MPost");
list.append("PS->PDF");
list.append("DVI->PDF");
list.append("Asymptote");
list.append("LatexMk");

for ( int i = 0; i <= 4; i++ ) list.append(QString::number(i+1)+": "+UserToolName[i]);

comboCompil = new QComboBox(runToolBar);
comboCompil->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
comboCompil->addItems(list);
comboCompil->setCurrentIndex(runIndex);
connect(runToolBar->addAction(QIcon(":/images/run.png"),tr("Run")), SIGNAL(triggered()), this, SLOT(doCompile()));
runToolBar->addWidget(comboCompil);

list.clear();
list.append(tr("View Dvi"));
list.append(tr("View PS"));
list.append(tr("View PDF"));

comboView = new QComboBox(runToolBar);
comboView->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
comboView->addItems(list);
comboView->setCurrentIndex(viewIndex);
connect(runToolBar->addAction(QIcon(":/images/run.png"),tr("View")), SIGNAL(triggered()), this, SLOT(doView()));
runToolBar->addWidget(comboView);



Act = new QAction(QIcon(":/images/viewlog.png"),tr("View Log"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(ViewLog()));
logToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/errornext.png"),tr("Next LaTeX Error"), this);
connect(Act, SIGNAL(triggered()), this, SLOT(NextError()));
Act->setShortcut(Qt::ALT+Qt::Key_Down);
Act->setToolTip("Alt+Down");
logToolBar->addAction(Act);

Act = new QAction(QIcon(":/images/errorprev.png"),tr("Previous LaTeX Error"), this);
Act->setShortcut(Qt::ALT+Qt::Key_Up);
Act->setToolTip("Alt+Up");
connect(Act, SIGNAL(triggered()), this, SLOT(PreviousError()));
logToolBar->addAction(Act);

StopAct = new QAction(QIcon(":/images/process-stop.png"),tr("Stop Process"), this);
connect(StopAct, SIGNAL(triggered()), this, SLOT(stopProcess()));
logToolBar->addAction(StopAct);
StopAct->setEnabled(false);

}


void Texmaker::createStatusBar()
{
toggleStructureButton=new PlayerButton(statusBar());
toggleStructureButton->setImages("structure_button");
connect(toggleStructureButton, SIGNAL( clicked() ), this, SLOT(ToggleStructurePanel() ) );
statusBar()->addPermanentWidget(toggleStructureButton,0);
toggleLogButton=new PlayerButton(statusBar());
toggleLogButton->setImages("log_button");
connect(toggleLogButton, SIGNAL( clicked() ), this, SLOT(ToggleLogPanel() ) );
statusBar()->addPermanentWidget(toggleLogButton,0);
togglePdfButton=new PlayerButton(statusBar());
togglePdfButton->setImages("pdf_button");
connect(togglePdfButton, SIGNAL( clicked() ), this, SLOT(TogglePdfPanel() ) );
statusBar()->addPermanentWidget(togglePdfButton,0);
//if (largescreen && builtinpdfview) ViewPdfPanelAct->setEnabled(true);
//else ViewPdfPanelAct->setEnabled(false);
if (largescreen && builtinpdfview) togglePdfButton->show();
else togglePdfButton->hide();
statusBar()->addPermanentWidget(new QLabel(),1);
stat1=new QLabel(statusBar());
//stat2=new QLabel( status );
stat3=new QLabel(statusBar() );

statusBar()->addPermanentWidget(stat3,0);
//status->addPermanentWidget(stat2,0);
statusBar()->addPermanentWidget(stat1,0);

toggleStructureButton->setEnabled(showstructview);
toggleLogButton->setEnabled(showoutputview);
togglePdfButton->setEnabled(showpdfview);
}

void Texmaker::UpdateCaption()
{
QString title;
if   ( !currentEditorView() )	{title="Texmaker";}
else
	{
	title="Document : "+getName();
	//input_encoding=currentEditorView()->editor->getEncoding();
	}
setWindowTitle(title);
UpdateStructure();
if (singlemode)
	{
	OutputTextEdit->clear();
	OutputTableWidget->hide();
	logpresent=false;
	}
QString finame=getName();
comboFiles->setCurrentIndex(comboFiles->findData(finame,Qt::UserRole,Qt::MatchExactly | Qt::MatchCaseSensitive));
if (!finame.startsWith("untitled") && finame!="") 
  {
  lastDocument=finame;
  }
if   (currentEditorView())
  {
   SaveAct->setEnabled(currentEditorView()->editor->document()->isModified());
   UndoAct->setEnabled(currentEditorView()->editor->document()->isUndoAvailable());
   RedoAct->setEnabled(currentEditorView()->editor->document()->isRedoAvailable());
   CopyAct->setEnabled(currentEditorView()->editor->textCursor().hasSelection());
   CutAct->setEnabled(currentEditorView()->editor->textCursor().hasSelection());
   stat3->setText(QString(" %1 ").arg(currentEditorView()->editor->getEncoding()));
  }
else
  {
   SaveAct->setEnabled(false);
   UndoAct->setEnabled(false);
   RedoAct->setEnabled(false);
   CopyAct->setEnabled(false);
   CutAct->setEnabled(false);    
  }
if (currentEditorView()) currentEditorView()->editor->setFocus();
}

void Texmaker::NewDocumentStatus(bool m)
{
if ( !currentEditorView() )	return;
QString finame=getName();
if (m)
	{
	//EditorView->setTabIcon(EditorView->indexOf(currentEditorView()),QIcon(":/images/modified.png"));
	//EditorView->setTabText(EditorView->indexOf(currentEditorView()),QFileInfo( getName() ).fileName());
	comboFiles->setItemIcon(comboFiles->findData(finame,Qt::UserRole,Qt::MatchExactly | Qt::MatchCaseSensitive),QIcon(":/images/modified.png"));
	SaveAct->setEnabled(true);
	}
else
	{
	//EditorView->setTabIcon(EditorView->indexOf(currentEditorView()),QIcon(":/images/empty.png"));
	//EditorView->setTabText(EditorView->indexOf(currentEditorView()),QFileInfo( getName() ).fileName());
	comboFiles->setItemIcon(comboFiles->findData(finame,Qt::UserRole,Qt::MatchExactly | Qt::MatchCaseSensitive),QIcon(":/images/empty.png"));
	SaveAct->setEnabled(false);
	}
}

LatexEditorView *Texmaker::currentEditorView() const
{
if ( EditorView->currentWidget() && EditorView->currentWidget()->inherits( "LatexEditorView" ) ) return (LatexEditorView*)EditorView->currentWidget();
return 0;
}

QString Texmaker::getName()
{
QString title;
if ( !currentEditorView() )	{title="";}
else {title=filenames[currentEditorView()];}
return title;
}

bool Texmaker::FileAlreadyOpen(QString f)
{
bool rep=false;
FilesMap::Iterator it;
QString fw32,funix,forig;
for( it = filenames.begin(); it != filenames.end(); ++it )
	{
	forig=filenames[it.key()];
	fw32=filenames[it.key()];
	funix=filenames[it.key()];
	fw32.replace(QString("\\"),QString("/"));
	funix.replace(QString("/"),QString("\\"));
	if ( (forig==f) || (fw32==f) || (funix==f)) 
		{
		EditorView->setCurrentIndex(EditorView->indexOf(it.key()));
		rep=true;
		}
	}
return rep;
}

void Texmaker::ComboFilesInsert(const QString & file)
{
int index;
QString fname = QFileInfo( file ).fileName();

for (index=0; index<comboFiles->count(); index++)
    if (comboFiles->itemText(index).localeAwareCompare(fname) > 0) break;

comboFiles->insertItem(index, fname, file);
comboFiles->setItemIcon(index,QIcon(":/images/empty.png"));
}
///////////////////FILE//////////////////////////////////////
void Texmaker::load( const QString &f )
{
if (FileAlreadyOpen(f) || !QFile::exists( f )) return;
QFile file( f );
if ( !file.open( QIODevice::ReadOnly ) )
	{
	QMessageBox::warning( this,tr("Error"), tr("You do not have read permission to this file."));
	return;
	}
bool hasDecodingError=false;
QByteArray buf = file.readAll();
int bytesRead = buf.size();
file.close();
QTextCodec* codec = QTextCodec::codecForName(input_encoding.toLatin1());
if(!codec) codec = QTextCodec::codecForLocale();
#if 0 // should work, but does not, Qt bug with "system" codec
QTextDecoder *decoder = codec->makeDecoder();
QString text = decoder->toUnicode(buf);
hasDecodingError = (decoder->hasFailure());
delete decoder;
#else
QString text = codec->toUnicode(buf);
QByteArray verifyBuf = codec->fromUnicode(text); // slow
// the minSize trick lets us ignore unicode headers
int minSize = qMin(verifyBuf.size(), buf.size());
hasDecodingError = (minSize < buf.size()- 4 || memcmp(verifyBuf.constData() + verifyBuf.size() - minSize,buf.constData() + buf.size() - minSize, minSize));
#endif
QString new_encoding;
QEncodingProber prober (QEncodingProber::Universal);

if (hasDecodingError)
  {
  prober.feed (buf.constData());
  QTextCodec* detected_codec;
  if (prober.confidence() > 0.5) //Kencodingprober works very bad with tex documents
    {
    detected_codec = QTextCodec::codecForName(prober.encoding());
    new_encoding=detected_codec->name();
    }
  else if (input_encoding=="UTF-8") new_encoding="ISO-8859-1";
  else if (input_encoding=="ISO-8859-1") new_encoding="UTF-8";
  else new_encoding=QString(QTextCodec::codecForLocale()->name());
  EncodingDialog *encDlg = new EncodingDialog(this);
  encDlg->ui.comboBoxEncoding->setCurrentIndex(encDlg->ui.comboBoxEncoding->findText(new_encoding, Qt::MatchExactly));
  encDlg->ui.label->setText(encDlg->ui.label->text()+ " ("+input_encoding+").");
  if (encDlg->exec())
	  {
	  new_encoding=encDlg->ui.comboBoxEncoding->currentText();
	  codec = QTextCodec::codecForName(new_encoding.toLatin1());
	  text = codec->toUnicode(buf);
	  }
  else return;
  }
LatexEditorView *edit = new LatexEditorView(0,EditorFont,showline,colorMath,colorCommand,colorKeyword,inlinespellcheck,spell_ignored_words,spellChecker);
EditorView->addWidget( edit);
ComboFilesInsert(f);
EditorView->setCurrentIndex(EditorView->indexOf(edit));
edit->editor->setReadOnly(false);
if (hasDecodingError) edit->editor->setEncoding(new_encoding);
else edit->editor->setEncoding(input_encoding);
if (completion) edit->editor->setCompleter(completer);
else edit->editor->setCompleter(0);
edit->editor->setPlainText(text);

filenames.remove( edit);
filenames.insert( edit, f );
edit->editor->document()->setModified(false);
connect(edit->editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(NewDocumentStatus(bool)));
connect(edit->editor, SIGNAL(spellme()), this, SLOT(editSpell()));
connect(edit->editor, SIGNAL(tooltiptab()), this, SLOT(editTipTab()));
connect(edit->editor, SIGNAL(requestpdf(int)),this, SLOT(jumpToPdfline(int)));
connect(edit->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
currentEditorView()->editor->setLastNumLines(currentEditorView()->editor->numoflines());
connect(edit->editor, SIGNAL(blockCountChanged(int)), this, SLOT(refreshAllFromCursor(int)));
connect(edit->editor->document(), SIGNAL(undoAvailable(bool)),UndoAct, SLOT(setEnabled(bool)));
connect(edit->editor->document(), SIGNAL(redoAvailable(bool)),RedoAct, SLOT(setEnabled(bool)));
connect(edit->editor, SIGNAL(copyAvailable(bool)), CutAct, SLOT(setEnabled(bool)));
connect(edit->editor, SIGNAL(copyAvailable(bool)), CopyAct, SLOT(setEnabled(bool)));
connect(edit->editor, SIGNAL(requestGotoStructure(int)),this, SLOT(jumpToStructure(int)));

if (wordwrap) {edit->editor->setWordWrapMode(QTextOption::WordWrap);}
else {edit->editor->setWordWrapMode(QTextOption::NoWrap);}
UpdateCaption();
NewDocumentStatus(false);
AddRecentFile(f);
ShowStructure();
UpdateStructure();
UpdateBibliography();
#ifndef Q_WS_MACX 
show();
if (windowState()==Qt::WindowMinimized) setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
qApp->setActiveWindow(this);
activateWindow();
setFocus();
//raise();
//#ifdef Q_WS_WIN
//        if (IsIconic (this->winId())) ShowWindow(this->winId(), SW_RESTORE);
//#endif
#endif
edit->editor->setFocus();
}

void Texmaker::setLine( const QString &line )
{
bool ok;
int l=line.toInt(&ok,10);
if (currentEditorView() && ok)
	{
	QTextCursor cur=currentEditorView()->editor->textCursor();
	cur.movePosition(QTextCursor::End);
	currentEditorView()->editor->setTextCursor(cur);
	currentEditorView()->editor->gotoLine(l-1);
	}
}


void Texmaker::fileNew()
{
LatexEditorView *edit = new LatexEditorView(0,EditorFont,showline,colorMath,colorCommand,colorKeyword,inlinespellcheck,spell_ignored_words,spellChecker);
edit->editor->setReadOnly(false);
edit->editor->setEncoding(input_encoding);
if (completion) edit->editor->setCompleter(completer);
else edit->editor->setCompleter(0);
EditorView->addWidget( edit);
EditorView->setCurrentIndex(EditorView->indexOf(edit));
if (wordwrap) {edit->editor->setWordWrapMode(QTextOption::WordWrap);}
else {edit->editor->setWordWrapMode(QTextOption::NoWrap);}
filenames.remove( edit);
filenames.insert( edit, "untitled"+QString::number(untitled_id) );
ComboFilesInsert("untitled"+QString::number(untitled_id));
untitled_id++;
edit->editor->document()->setModified(false);
connect(edit->editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(NewDocumentStatus(bool)));
connect(edit->editor, SIGNAL(spellme()), this, SLOT(editSpell()));
connect(edit->editor, SIGNAL(tooltiptab()), this, SLOT(editTipTab()));
connect(edit->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
connect(edit->editor, SIGNAL(requestpdf(int)),this, SLOT(jumpToPdfline(int)));
currentEditorView()->editor->setLastNumLines(currentEditorView()->editor->numoflines());
connect(edit->editor, SIGNAL(blockCountChanged(int)), this, SLOT(refreshAllFromCursor(int)));
connect(edit->editor->document(), SIGNAL(undoAvailable(bool)),UndoAct, SLOT(setEnabled(bool)));
connect(edit->editor->document(), SIGNAL(redoAvailable(bool)),RedoAct, SLOT(setEnabled(bool)));
connect(edit->editor, SIGNAL(copyAvailable(bool)), CutAct, SLOT(setEnabled(bool)));
connect(edit->editor, SIGNAL(copyAvailable(bool)), CopyAct, SLOT(setEnabled(bool)));
connect(edit->editor, SIGNAL(requestGotoStructure(int)),this, SLOT(jumpToStructure(int)));
UpdateCaption();
NewDocumentStatus(false);
edit->editor->setFocus();
}

void Texmaker::fileNewFromFile()
{
QString currentDir=QDir::homePath();
if (!lastDocument.isEmpty())
	{
	QFileInfo fi(lastDocument);
	if (fi.exists() && fi.isReadable()) currentDir=fi.absolutePath();
	}
QString fn = QFileDialog::getOpenFileName(this,tr("Open File"),currentDir,"TeX files (*.tex *.bib *.sty *.cls *.mp);;All files (*.*)");
if (fn.isEmpty()) return;
QFile file( fn );
if ( !file.open( QIODevice::ReadOnly ) )
	{
	QMessageBox::warning( this,tr("Error"), tr("You do not have read permission to this file."));
	return;
	}
LatexEditorView *edit = new LatexEditorView(0,EditorFont,showline,colorMath,colorCommand,colorKeyword,inlinespellcheck,spell_ignored_words,spellChecker);
edit->editor->setReadOnly(false);
edit->editor->setEncoding(input_encoding);
if (completion) edit->editor->setCompleter(completer);
else edit->editor->setCompleter(0);
EditorView->addWidget( edit);
EditorView->setCurrentIndex(EditorView->indexOf(edit));
if (wordwrap) {edit->editor->setWordWrapMode(QTextOption::WordWrap);}
else {edit->editor->setWordWrapMode(QTextOption::NoWrap);}
filenames.remove( edit);
filenames.insert( edit, "untitled"+QString::number(untitled_id));
ComboFilesInsert("untitled"+QString::number(untitled_id));
untitled_id++;
QTextStream ts( &file );
QTextCodec* codec = QTextCodec::codecForName(input_encoding.toLatin1());
if(!codec) codec = QTextCodec::codecForLocale();
ts.setCodec(codec);
edit->editor->setPlainText( ts.readAll() );
file.close();
edit->editor->document()->setModified(true);
connect(edit->editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(NewDocumentStatus(bool)));
connect(edit->editor, SIGNAL(spellme()), this, SLOT(editSpell()));
connect(edit->editor, SIGNAL(tooltiptab()), this, SLOT(editTipTab()));
currentEditorView()->editor->setLastNumLines(currentEditorView()->editor->numoflines());
connect(edit->editor, SIGNAL(blockCountChanged(int)), this, SLOT(refreshAllFromCursor(int)));
connect(edit->editor, SIGNAL(requestpdf(int)),this, SLOT(jumpToPdfline(int)));
connect(edit->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
connect(edit->editor->document(), SIGNAL(undoAvailable(bool)),UndoAct, SLOT(setEnabled(bool)));
connect(edit->editor->document(), SIGNAL(redoAvailable(bool)),RedoAct, SLOT(setEnabled(bool)));
connect(edit->editor, SIGNAL(copyAvailable(bool)), CutAct, SLOT(setEnabled(bool)));
connect(edit->editor, SIGNAL(copyAvailable(bool)), CopyAct, SLOT(setEnabled(bool)));
connect(edit->editor, SIGNAL(requestGotoStructure(int)),this, SLOT(jumpToStructure(int)));

UpdateCaption();
NewDocumentStatus(true);
UpdateStructure();
UpdateBibliography();
edit->editor->setFocus();
}

void Texmaker::fileOpen()
{
QString currentDir=QDir::homePath();
if (!lastDocument.isEmpty())
	{
	QFileInfo fi(lastDocument);
	if (fi.exists() && fi.isReadable()) currentDir=fi.absolutePath();
	}
QStringList filesNames = QFileDialog::getOpenFileNames(this,tr("Open File"),currentDir,"TeX files (*.tex *.bib *.sty *.cls *.mp);;All files (*.*)");
foreach (const QString& fn, filesNames)
  {
  if ( !fn.isEmpty() ) load( fn );
  }
if ((filesNames.count()==1) && largescreen && builtinpdfview && showpdfview)
  {
  if ( !currentEditorView() || !singlemode ) return;
  QString finame=getName();
  QFileInfo fi(finame);
  QString basename=fi.completeBaseName();
  QString pdfname=fi.absolutePath()+"/"+basename+".pdf";
  QFileInfo pdfi(pdfname);
  if (pdfi.exists() && pdfi.isReadable()) 
    {
      if (pdfviewerWidget)
	{
	pdfviewerWidget->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
	StackedViewers->setCurrentWidget(pdfviewerWidget);
	//pdfviewerWidget->raise();
	pdfviewerWidget->show();
	}
      else
	{
    //    pdfviewerWidget=new PdfViewer(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command, this);
	pdfviewerWidget=new PdfViewerWidget(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command,psize,StackedViewers);
	connect(pdfviewerWidget, SIGNAL(openDocAtLine(const QString&, int, bool)), this, SLOT(fileOpenAndGoto(const QString&, int, bool)));
	connect(pdfviewerWidget, SIGNAL(sendFocusToEditor()), this, SLOT(getFocusToEditor()));
	connect(pdfviewerWidget, SIGNAL(sendPaperSize(const QString&)), this, SLOT(setPrintPaperSize(const QString&)));
	StackedViewers->addWidget(pdfviewerWidget);
	//pdfviewerWidget->raise();
	pdfviewerWidget->show();
	pdfviewerWidget->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
	}
    }
  }
}

bool Texmaker::isCurrentModifiedOutside()
{
if ( !currentEditorView() ) return false;
QString fn=*filenames.find( currentEditorView() );
QFileInfo fi(fn);
fi.refresh();
QDateTime disktime=fi.lastModified();
int delta=disktime.secsTo(currentEditorView()->editor->getLastSavedTime());
if (delta<-2) return true;
else return false;
}

void Texmaker::checkModifiedOutsideAll()
{
if (QApplication::activeWindow() != this) return;
disconnect(this, SIGNAL(windowActivated()),this, SLOT(mainWindowActivated()));


QList<QTreeWidgetItem *> fItems;
bool islabels_expanded=true;
bool isblocks_expanded=true;
fItems=StructureTreeWidget->findItems ("LABELS",Qt::MatchRecursive,0);
if (fItems.size()>0 ) 
  {
  if (fItems.at(0)) 
      {
      islabels_expanded=fItems.at(0)->isExpanded();
      }
  }
fItems.clear();
fItems=StructureTreeWidget->findItems ("BLOCKS",Qt::MatchRecursive,0);
if (fItems.size()>0 ) 
  {
  if (fItems.at(0)) 
      {
      isblocks_expanded=fItems.at(0)->isExpanded();
      }
  }  
LatexEditorView *temp = new LatexEditorView(EditorView,EditorFont,showline,colorMath,colorCommand,colorKeyword,inlinespellcheck,spell_ignored_words,spellChecker);
temp=currentEditorView();
FilesMap::Iterator it;
QString fn;
int choice;
for( it = filenames.begin(); it != filenames.end(); ++it )
	{
	EditorView->setCurrentIndex(EditorView->indexOf(it.key()));
	if ( !getName().startsWith("untitled")) 
	    {
	    fn=*filenames.find( currentEditorView() );
	    if (isCurrentModifiedOutside())
	      {
	      QFileInfo fi(fn);
	      choice= QMessageBox::warning(this, "Texmaker",
					    tr("The document has been changed outside Texmaker."
					    "Do you want to reload it (and discard your changes) or save it (and overwrite the file)?"),
					    tr("Reload the file"), tr("Save"), tr("Cancel"),
					    0,
					    2 );
	      if (choice==0)
		{
		  if (fi.exists() && fi.isReadable())
		    {
		    QFile file( fn );
		    if (file.open( QIODevice::ReadOnly ) )
			{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
			QTextStream ts( &file );
			QTextCodec* codec = QTextCodec::codecForName(currentEditorView()->editor->getEncoding().toLatin1());
			ts.setCodec(codec ? codec : QTextCodec::codecForLocale());
			disconnect(currentEditorView()->editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(NewDocumentStatus(bool)));
			disconnect(currentEditorView()->editor, SIGNAL(spellme()), this, SLOT(editSpell()));
			disconnect(currentEditorView()->editor, SIGNAL(tooltiptab()), this, SLOT(editTipTab()));
			disconnect(currentEditorView()->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
			disconnect(currentEditorView()->editor, SIGNAL(requestpdf(int)),this, SLOT(jumpToPdfline(int)));
			disconnect(currentEditorView()->editor->document(), SIGNAL(undoAvailable(bool)),UndoAct, SLOT(setEnabled(bool)));
			disconnect(currentEditorView()->editor->document(), SIGNAL(redoAvailable(bool)),RedoAct, SLOT(setEnabled(bool)));
			disconnect(currentEditorView()->editor, SIGNAL(copyAvailable(bool)), CutAct, SLOT(setEnabled(bool)));
			disconnect(currentEditorView()->editor, SIGNAL(copyAvailable(bool)), CopyAct, SLOT(setEnabled(bool)));
			disconnect(currentEditorView()->editor, SIGNAL(blockCountChanged(int)), this, SLOT(refreshAllFromCursor(int)));
			disconnect(currentEditorView()->editor, SIGNAL(requestGotoStructure(int)),this, SLOT(jumpToStructure(int)));
			currentEditorView()->editor->setPlainText( ts.readAll() );
			file.close();
			currentEditorView()->editor->setLastSavedTime(QDateTime::currentDateTime());
			connect(currentEditorView()->editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(NewDocumentStatus(bool)));
			connect(currentEditorView()->editor, SIGNAL(spellme()), this, SLOT(editSpell()));
			connect(currentEditorView()->editor, SIGNAL(tooltiptab()), this, SLOT(editTipTab()));
			connect(currentEditorView()->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
			connect(currentEditorView()->editor, SIGNAL(requestpdf(int)),this, SLOT(jumpToPdfline(int)));
			connect(currentEditorView()->editor->document(), SIGNAL(undoAvailable(bool)),UndoAct, SLOT(setEnabled(bool)));
			connect(currentEditorView()->editor->document(), SIGNAL(redoAvailable(bool)),RedoAct, SLOT(setEnabled(bool)));
			connect(currentEditorView()->editor, SIGNAL(copyAvailable(bool)), CutAct, SLOT(setEnabled(bool)));
			connect(currentEditorView()->editor, SIGNAL(copyAvailable(bool)), CopyAct, SLOT(setEnabled(bool)));
			currentEditorView()->editor->setLastNumLines(currentEditorView()->editor->numoflines());
			connect(currentEditorView()->editor, SIGNAL(blockCountChanged(int)), this, SLOT(refreshAllFromCursor(int)));
			connect(currentEditorView()->editor, SIGNAL(requestGotoStructure(int)),this, SLOT(jumpToStructure(int)));
			UpdateStructure();
			UpdateBibliography();
			QApplication::restoreOverrideCursor();
			}
		    }
		}
	      else if (choice==1)
		{
		QFile file(fn);
		if (file.open( QIODevice::WriteOnly ) )
		    {
		    QTextStream ts( &file );
		    QTextCodec* codec = QTextCodec::codecForName(currentEditorView()->editor->getEncoding().toLatin1());
		    ts.setCodec(codec ? codec : QTextCodec::codecForLocale());
		    ts << currentEditorView()->editor->toPlainText();
		    file.close();
		    currentEditorView()->editor->setLastSavedTime(QDateTime::currentDateTime());
		    currentEditorView()->editor->document()->setModified(false);
		    }
		}
		else currentEditorView()->editor->setLastSavedTime(QDateTime::currentDateTime());
	      }
	    }
	}

EditorView->setCurrentIndex(EditorView->indexOf(temp));
QString title;
if   ( !currentEditorView() )	{title="Texmaker";}
else
	{
	title="Document : "+getName();
	//input_encoding=currentEditorView()->editor->getEncoding();
	}
setWindowTitle(title);

UpdateStructure();
fItems.clear();
fItems=StructureTreeWidget->findItems ("LABELS",Qt::MatchRecursive,0);
if (fItems.size()>0 ) 
  {
  if (fItems.at(0)) 
      {
      StructureTreeWidget->setItemExpanded(fItems.at(0),islabels_expanded);
      }
  }
fItems.clear();
fItems=StructureTreeWidget->findItems ("BLOCKS",Qt::MatchRecursive,0);
if (fItems.size()>0 ) 
  {
  if (fItems.at(0)) 
      {
      StructureTreeWidget->setItemExpanded (fItems.at(0),isblocks_expanded);
      }
  }  

QString finame=getName();
comboFiles->setCurrentIndex(comboFiles->findData(finame,Qt::UserRole,Qt::MatchExactly | Qt::MatchCaseSensitive));
if (!finame.startsWith("untitled") && finame!="") 
  {
  lastDocument=finame;

  }
if   (currentEditorView())
  {
   SaveAct->setEnabled(currentEditorView()->editor->document()->isModified());
   UndoAct->setEnabled(currentEditorView()->editor->document()->isUndoAvailable());
   RedoAct->setEnabled(currentEditorView()->editor->document()->isRedoAvailable());
   CopyAct->setEnabled(currentEditorView()->editor->textCursor().hasSelection());
   CutAct->setEnabled(currentEditorView()->editor->textCursor().hasSelection());
   stat3->setText(QString(" %1 ").arg(currentEditorView()->editor->getEncoding()));
  }
else
  {
   SaveAct->setEnabled(false);
   UndoAct->setEnabled(false);
   RedoAct->setEnabled(false);
   CopyAct->setEnabled(false);
   CutAct->setEnabled(false);    
  }
if (currentEditorView()) currentEditorView()->editor->setFocus();
connect(this, SIGNAL(windowActivated()),this, SLOT(mainWindowActivated()));
}

void Texmaker::fileSave()
{
if ( !currentEditorView() )	return;
QString fn;
if ( getName().startsWith("untitled") ) {fileSaveAs();}
else
	{
	fn=*filenames.find( currentEditorView() );
	if (isCurrentModifiedOutside())
	{
	QFileInfo fi(fn);
	switch(  QMessageBox::warning(this, "Texmaker",
					tr("The document has been changed outside Texmaker."
					"Do you want to reload it (and discard your changes) or save it (and overwrite the file)?"),
					tr("Reload the file"), tr("Save"), tr("Cancel"),
					0,
					2 ) )
		{
		case 0:
		  if (fi.exists() && fi.isReadable())
		    {
		    filenames.remove(currentEditorView());
		    comboFiles->removeItem(comboFiles->currentIndex());
		    delete currentEditorView();
		    load(fn);
		    return;
		    }
		  break;
		case 1:
		  break;
		case 2:
		  default:
		  currentEditorView()->editor->setLastSavedTime(QDateTime::currentDateTime());
		  return;
		  break;
		}
	}
	QFile file(fn);
	if ( !file.open( QIODevice::WriteOnly ) )
		{
		QMessageBox::warning( this,tr("Error"),tr("The file could not be saved. Please check if you have write permission."));
		return;
		}
	QTextStream ts( &file );
	QTextCodec* codec = QTextCodec::codecForName(currentEditorView()->editor->getEncoding().toLatin1());
	ts.setCodec(codec ? codec : QTextCodec::codecForLocale());
	ts << currentEditorView()->editor->toPlainText();
	file.close();
	currentEditorView()->editor->setLastSavedTime(QDateTime::currentDateTime());
	currentEditorView()->editor->document()->setModified(false);
	fn=getName();
	AddRecentFile(fn);
	}
UpdateCaption();
}

bool Texmaker::currentfileSaved()
{
if ( !currentEditorView() ) return true;
QString fn;
if ( getName().startsWith("untitled") ) {return false;}
else
	{
	fn=*filenames.find( currentEditorView() );
	if (isCurrentModifiedOutside())
	{
	QFileInfo fi(fn);
	switch(  QMessageBox::warning(this, "Texmaker",
					tr("The document has been changed outside Texmaker."
					"Do you want to reload it (and discard your changes) or save it (and overwrite the file)?"),
					tr("Reload the file"), tr("Save"), tr("Cancel"),
					0,
					2 ) )
		{
		case 0:
		  if (fi.exists() && fi.isReadable())
		    {
		    filenames.remove(currentEditorView());
		    comboFiles->removeItem(comboFiles->currentIndex());
		    delete currentEditorView();
		    load(fn);
		    return true;
		    }
		  break;
		case 1:
		  break;
		case 2:
		  default:
		  currentEditorView()->editor->setLastSavedTime(QDateTime::currentDateTime());
		  return false;
		  break;
		}
	}
	QFile file(fn);
	if ( !file.open( QIODevice::WriteOnly ) )
		{
		if (currentEditorView()->editor->document()->isModified())
		  {
		  QMessageBox::warning( this,tr("Error"),tr("The file could not be saved. Please check if you have write permission."));
		  return false;
		  }
		else return true;
		}
	QTextStream ts( &file );
	QTextCodec* codec = QTextCodec::codecForName(currentEditorView()->editor->getEncoding().toLatin1());
	ts.setCodec(codec ? codec : QTextCodec::codecForLocale());
	ts << currentEditorView()->editor->toPlainText();
	file.close();
	currentEditorView()->editor->setLastSavedTime(QDateTime::currentDateTime());
	currentEditorView()->editor->document()->setModified(false);
	fn=getName();
	AddRecentFile(fn);
	UpdateCaption();
	return true;
	}
}

void Texmaker::fileSaveAs()
{
if ( !currentEditorView() ) 	return;
QString currentDir=QDir::homePath();
if (!lastDocument.isEmpty())
	{
	QFileInfo fi(lastDocument);
	if (fi.exists() && fi.isReadable()) currentDir=fi.absolutePath();
	}
QString fn = QFileDialog::getSaveFileName(this,tr("Save As"),currentDir,"TeX files (*.tex *.bib *.sty *.cls *.mp);;All files (*.*)");
if ( !fn.isEmpty() )
	{
	if (!fn.contains('.')) fn += ".tex";
	QFileInfo fic(fn);
	filenames.remove(currentEditorView());
	comboFiles->removeItem(comboFiles->currentIndex());
	filenames.insert(currentEditorView(), fn );
	fileSave();
	//EditorView->setTabText(EditorView->indexOf(currentEditorView()),fic.fileName());
	ComboFilesInsert(fn);
	}
UpdateCaption();
}

void Texmaker::fileRestoreSession()
{
QString finame;
for (int i=0; i < sessionFilesList.count(); i++)
	{
	finame=sessionFilesList.at(i);
	if (finame.left(15)=="texmakermaster#")
		{
		finame=finame.right(finame.length()-15);
		load(finame);
		ToggleMode();
		}
	else load(finame);
	}
}

void Texmaker::fileSaveAll()
{
LatexEditorView *temp = new LatexEditorView(EditorView,EditorFont,showline,colorMath,colorCommand,colorKeyword,inlinespellcheck,spell_ignored_words,spellChecker);
temp=currentEditorView();
FilesMap::Iterator it;
for( it = filenames.begin(); it != filenames.end(); ++it )
	{
	EditorView->setCurrentIndex(EditorView->indexOf(it.key()));
	fileSave();
	}
EditorView->setCurrentIndex(EditorView->indexOf(temp));
UpdateCaption();
}

void Texmaker::fileClose()
{
if ( !currentEditorView() )	return;
if (currentEditorView()->editor->document()->isModified())
	{
	switch(  QMessageBox::warning(this, "Texmaker",
					tr("The document contains unsaved work. "
					"Do you want to save it before closing?"),
					tr("Save and Close"), tr("Don't Save and Close"), tr("Cancel"),
					0,
					2 ) )
		{
		case 0:
		  fileSave();
		  filenames.remove(currentEditorView());
		  comboFiles->removeItem(comboFiles->currentIndex());
		  delete currentEditorView();
		  break;
		case 1:
		  filenames.remove(currentEditorView());
		  comboFiles->removeItem(comboFiles->currentIndex());
		  delete currentEditorView();
		  break;
		case 2:
		  default:
		  return;
		  break;
		}
	}
else
{
filenames.remove(currentEditorView());
comboFiles->removeItem(comboFiles->currentIndex());
delete currentEditorView();
}
UpdateCaption();
}

void Texmaker::fileCloseRequested( int index)
{
EditorView->setCurrentIndex(index);
fileClose();
}

void Texmaker::fileCloseAll()
{
bool go=true;
while (currentEditorView() && go)
	{
	if (currentEditorView()->editor->document()->isModified())
		{
		switch(  QMessageBox::warning(this, "Texmaker",
						tr("The document contains unsaved work. "
						"Do you want to save it before exiting?"),
						tr("Save and Close"), tr("Don't Save and Close"), tr("Cancel"),
						0,
						2 ) )
			{
			case 0:
			fileSave();
			filenames.remove(currentEditorView());
			comboFiles->removeItem(comboFiles->currentIndex());
			delete currentEditorView();
			break;
			case 1:
			filenames.remove(currentEditorView());
			comboFiles->removeItem(comboFiles->currentIndex());
			delete currentEditorView();
			break;
			case 2:
			default:
			go=false;
			return;
			break;
			}
		}
	else
		{
		filenames.remove(currentEditorView());
		comboFiles->removeItem(comboFiles->currentIndex());
		delete currentEditorView();
		}
	}
UpdateCaption();
}

void Texmaker::fileExit()
{
SaveSettings();
if (browserWindow) browserWindow->close();
if (pdfviewerWidget) {StackedViewers->removeWidget(pdfviewerWidget);delete(pdfviewerWidget);} 
if (pdfviewerWindow) pdfviewerWindow->close(); 
bool accept=true;
while (currentEditorView() && accept)
	{
	if (currentEditorView()->editor->document()->isModified())
		{
		switch(  QMessageBox::warning(this, "Texmaker",
						tr("The document contains unsaved work. "
						"Do you want to save it before exiting?"),
						tr("Save and Close"), tr("Don't Save and Close"),tr( "Cancel"),
						0,
						2 ) )
			{
			case 0:
				fileSave();
			filenames.remove(currentEditorView());
			comboFiles->removeItem(comboFiles->currentIndex());
				delete currentEditorView();
				break;
			case 1:
			filenames.remove(currentEditorView());
			comboFiles->removeItem(comboFiles->currentIndex());
				delete currentEditorView();
				break;
			case 2:
			default:
				accept=false;
				break;
			}
		
		}
	else
		{
		filenames.remove(currentEditorView());
		comboFiles->removeItem(comboFiles->currentIndex());
		delete currentEditorView();
		}
	}
if (accept) qApp->quit();
}

void Texmaker::closeEvent(QCloseEvent *e)
{
SaveSettings();
if (browserWindow) browserWindow->close();
if (pdfviewerWidget) {StackedViewers->removeWidget(pdfviewerWidget);delete(pdfviewerWidget);} 
if (pdfviewerWindow) pdfviewerWindow->close(); 
bool accept=true;
while (currentEditorView() && accept)
	{
	if (currentEditorView()->editor->document()->isModified())
		{
		switch(  QMessageBox::warning(this, "Texmaker",
						tr("The document contains unsaved work. "
						"Do you want to save it before exiting?"),
						tr("Save and Close"), tr("Don't Save and Close"), tr("Cancel"),
						0,
						2 ) )
			{
			case 0:
				fileSave();
			filenames.remove(currentEditorView());
			comboFiles->removeItem(comboFiles->currentIndex());
				delete currentEditorView();
				break;
			case 1:
			filenames.remove(currentEditorView());
			comboFiles->removeItem(comboFiles->currentIndex());
				delete currentEditorView();
				break;
			case 2:
			default: 
				accept=false;
				break;
			}
		}
	else
		{
		filenames.remove(currentEditorView());
		comboFiles->removeItem(comboFiles->currentIndex());
		delete currentEditorView();
		}
	}
if (accept) e->accept();
else e->ignore();
}


void Texmaker::fileOpenRecent()
{
QAction *action = qobject_cast<QAction *>(sender());
if (action) 
  {
  load(action->data().toString());
  if (largescreen && builtinpdfview && showpdfview)
      {
      if ( !currentEditorView() || !singlemode ) return;
      QString finame=getName();
      QFileInfo fi(finame);
      QString basename=fi.completeBaseName();
      QString pdfname=fi.absolutePath()+"/"+basename+".pdf";
      QFileInfo pdfi(pdfname);
      if (pdfi.exists() && pdfi.isReadable()) 
	{
	  if (pdfviewerWidget)
	    {
	    pdfviewerWidget->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
	    StackedViewers->setCurrentWidget(pdfviewerWidget);
	    //pdfviewerWidget->raise();
	    pdfviewerWidget->show();
	    }
	  else
	    {
	//    pdfviewerWidget=new PdfViewer(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command, this);
	    pdfviewerWidget=new PdfViewerWidget(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command,psize,StackedViewers);
	    connect(pdfviewerWidget, SIGNAL(openDocAtLine(const QString&, int, bool)), this, SLOT(fileOpenAndGoto(const QString&, int, bool)));
	    connect(pdfviewerWidget, SIGNAL(sendFocusToEditor()), this, SLOT(getFocusToEditor()));
	    connect(pdfviewerWidget, SIGNAL(sendPaperSize(const QString&)), this, SLOT(setPrintPaperSize(const QString&)));
	    StackedViewers->addWidget(pdfviewerWidget);
	    //pdfviewerWidget->raise();
	    pdfviewerWidget->show();
	    pdfviewerWidget->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
	    }
	}
      }
  }
}

void Texmaker::AddRecentFile(const QString &f)
{
if (recentFilesList.contains(f)) return;

if (recentFilesList.count() < 10) recentFilesList.prepend(f);
else
	{
	recentFilesList.removeLast();
	recentFilesList.prepend(f);
	}
UpdateRecentFile();
}

void Texmaker::UpdateRecentFile()
{
for (int i=0; i < recentFilesList.count(); i++)
	{
        recentFileActs[i]->setText(recentFilesList.at(i));
        recentFileActs[i]->setData(recentFilesList.at(i));
        recentFileActs[i]->setVisible(true);
	}
for (int j = recentFilesList.count(); j < 10; ++j) recentFileActs[j]->setVisible(false);
}

void Texmaker::filePrint()
{
if ( !currentEditorView() ) return;
QTextDocument *document = currentEditorView()->editor->document();
QPrinter printer;
QPrintDialog *dlg = new QPrintDialog(&printer, this);
if (dlg->exec() != QDialog::Accepted) return;
document->print(&printer);
}

void Texmaker::fileOpenAndGoto(const QString &f, int line,bool focus)
{
load(f);
if (currentEditorView())
	{
	QTextCursor cur=currentEditorView()->editor->textCursor();
	cur.movePosition(QTextCursor::End);
	currentEditorView()->editor->setTextCursor(cur);
	currentEditorView()->editor->gotoLine(line-1);
	//currentEditorView()->editor->ensureCursorVisible();
	//currentEditorView()->editor->setHightLightLine();
	}
setLine(QString::number(line));
if (focus) getFocusToEditor();
}

void Texmaker::getFocusToEditor()
{
#ifndef Q_WS_MACX 
show();
if (windowState()==Qt::WindowMinimized) setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
#endif
show();
if (windowState()==Qt::WindowMinimized) setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
qApp->setActiveWindow(this);
activateWindow();
setFocus();
if (currentEditorView()) currentEditorView()->editor->setFocus();
}

void Texmaker::fileReload()
{
if ( !currentEditorView() ) return;
if ( getName().startsWith("untitled") ) return;
QString f=filenames[currentEditorView()];
if (currentEditorView()->editor->document()->isModified())
	{
	switch(  QMessageBox::warning(this, "Texmaker",
					tr("The document contains unsaved work."
					"You will lose changes by reloading the document."),
					tr("Reload the file"), tr("Cancel"),
					0,
					1 ) )
		{
		case 0:
			filenames.remove(currentEditorView());
			comboFiles->removeItem(comboFiles->currentIndex());
			delete currentEditorView();
			load(f);
			break;
		case 1:
		default:
			return;
			break;
		}
	}
else
  {
  filenames.remove(currentEditorView());
comboFiles->removeItem(comboFiles->currentIndex());
  delete currentEditorView();
  load(f);
  }
}

void Texmaker::listSelectionActivated(int index)
{
QString f=comboFiles->itemData(index, Qt::UserRole).toString();
FilesMap::Iterator it;
QString fw32,funix,forig;
for( it = filenames.begin(); it != filenames.end(); ++it )
	{
	forig=filenames[it.key()];
	fw32=filenames[it.key()];
	funix=filenames[it.key()];
	fw32.replace(QString("\\"),QString("/"));
	funix.replace(QString("/"),QString("\\"));
	if ( (forig==f) || (fw32==f) || (funix==f)) 
		{
		EditorView->setCurrentIndex(EditorView->indexOf(it.key()));
		}
	}
}

void Texmaker::ToggleMasterCurrent()
{
QString dest;
FilesMap::Iterator it;
QString fw32,funix,forig;
if (singlemode) 
  {
  lastChild="";
  return;
  }
if (getName()==MasterName)
  {
   dest=lastChild; 
  }
else
  {
  lastChild=getName();
  dest=MasterName;  
  }
if (!dest.isEmpty()) 
  {
  for( it = filenames.begin(); it != filenames.end(); ++it )
	{
	forig=filenames[it.key()];
	fw32=filenames[it.key()];
	funix=filenames[it.key()];
	fw32.replace(QString("\\"),QString("/"));
	funix.replace(QString("/"),QString("\\"));
	if ( (forig==dest) || (fw32==dest) || (funix==dest)) 
		{
		EditorView->setCurrentIndex(EditorView->indexOf(it.key()));
		}
	}
  }
}

//////////////////////////// EDIT ///////////////////////
void Texmaker::editUndo()
{
if ( !currentEditorView() ) return;
currentEditorView()->editor->document()->undo();
}

void Texmaker::editRedo()
{
if ( !currentEditorView() ) return;
currentEditorView()->editor->document()->redo();
}

void Texmaker::editCut()
{
if ( !currentEditorView() ) return;
currentEditorView()->editor->cut();
}

void Texmaker::editCopy()
{
if ( !currentEditorView() ) return;
currentEditorView()->editor->copy();
}

void Texmaker::editPaste()
{
if ( !currentEditorView() ) return;
currentEditorView()->editor->paste();
}

void Texmaker::editSelectAll()
{
if ( !currentEditorView() ) return;
currentEditorView()->editor->selectAll();
}

void Texmaker::editFind()
{
if ( !currentEditorView() )	return;
QTextCursor c =currentEditorView()->editor->textCursor();
if (c.hasSelection()) currentEditorView()->findwidget->ui.comboFind->lineEdit()->setText(c.selectedText());
currentEditorView()->findwidget->show();
currentEditorView()->findwidget->ui.comboFind->setFocus();
currentEditorView()->findwidget->ui.comboFind->lineEdit()->selectAll();
}

void Texmaker::editFindNext()
{
if ( !currentEditorView() )	return;
currentEditorView()->findwidget->show();
if (currentEditorView()->findwidget->ui.comboFind->lineEdit()->text()!="") currentEditorView()->findwidget->doFind();
}


void Texmaker::editReplace()
{
if ( !currentEditorView() )	return;
if ( !replaceDialog )  replaceDialog = new ReplaceDialog(this, 0);
replaceDialog->SetEditor(currentEditorView()->editor);
QTextCursor c =currentEditorView()->editor->textCursor();
if (c.hasSelection()) replaceDialog->ui.comboFind->lineEdit()->setText(c.selectedText());
replaceDialog->show();
replaceDialog->raise();
replaceDialog->ui.comboFind->setFocus();
replaceDialog->ui.comboFind->lineEdit()->selectAll();
}

void Texmaker::editGotoLine()
{
if ( !currentEditorView() )	return;
if ( !gotoLineDialog ) gotoLineDialog = new GotoLineDialog(this, 0);
gotoLineDialog->SetEditor(currentEditorView()->editor);
gotoLineDialog->show();
gotoLineDialog->raise();
gotoLineDialog->ui.spinLine->setFocus();
gotoLineDialog->ui.spinLine->setMinimum( 1 );
gotoLineDialog->ui.spinLine->setMaximum( currentEditorView()->editor->numoflines() );
gotoLineDialog->ui.spinLine->selectAll();
}

void Texmaker::editComment()
{
if ( !currentEditorView() )	return;
currentEditorView()->editor->commentSelection();
}

void Texmaker::editUncomment()
{
if ( !currentEditorView() )	return;
currentEditorView()->editor->uncommentSelection();
}

void Texmaker::editIndent()
{
if ( !currentEditorView() )	return;
currentEditorView()->editor->indentSelection();
}

void Texmaker::editUnindent()
{
if ( !currentEditorView() )	return;
currentEditorView()->editor->unindentSelection();
}

void Texmaker::editSpell()
{
if ( !currentEditorView() )	return;
if (spelldicExist()) 
	{
	SpellerDialog *spellDlg=new SpellerDialog(this,currentEditorView()->editor,spell_ignored_words);
	if (spellDlg->exec())
		{
		spell_ignored_words=spellDlg->alwaysignoredwordList.join(",");
		}
	}
else
	{
	QMessageBox::warning( this,tr("Error"),tr("Error : Can't open the dictionary"));
	return;
	}
}

void Texmaker::editTipTab()
{
if ( !currentEditorView() )	return;
OutputTextEdit->clear();
OutputTextEdit->insertLine("Use the Tab key to reach the next "+QString(0x2022)+" field");
}

/////////////// CONFIG ////////////////////
void Texmaker::ReadSettings()
{
#ifdef USB_VERSION
QSettings *config=new QSettings(QCoreApplication::applicationDirPath()+"/texmaker.ini",QSettings::IniFormat); //for USB-stick version :
#else
QSettings *config=new QSettings(QSettings::IniFormat,QSettings::UserScope,"xm1","texmaker");
#endif
if (!config->contains("IniMode")) 
	{
	delete config;
	config=new QSettings("xm1","texmaker");
	}
config->beginGroup( "texmaker" );
singlemode=true;


QFontDatabase fdb;
QStringList xf = fdb.families();
QString deft;

bool new_user=(!config->contains("GUI/New Version"));

modern_style=config->value( "GUI/Style",true).toBool();
new_gui=config->value( "GUI/New Version",false).toBool();


QRect screen = QApplication::desktop()->screenGeometry();
int w= config->value( "Geometries/MainwindowWidth",screen.width()-150).toInt();
int h= config->value( "Geometries/MainwindowHeight",screen.height()-100).toInt() ;
int x= config->value( "Geometries/MainwindowX",30).toInt();
int y= config->value( "Geometries/MainwindowY",30).toInt() ;
resize(w,h);
move(x,y);
windowstate=config->value("MainWindowState").toByteArray();
splitter1state=config->value("Splitter1State").toByteArray();
splitter2state=config->value("Splitter2State").toByteArray();
psize=config->value( "Print/PaperSize","a4").toString();
#ifdef Q_WS_WIN
if (xf.contains("Courier New",Qt::CaseInsensitive)) deft="Courier New";
else deft=qApp->font().family();
QString fam=config->value("Editor/Font Family",deft).toString();
int si=config->value( "Editor/Font Size",10).toInt();
#else
if (xf.contains("DejaVu Sans Mono",Qt::CaseInsensitive)) deft="DejaVu Sans Mono";
else if (xf.contains("Lucida Sans Typewriter",Qt::CaseInsensitive)) deft="Lucida Sans Typewriter";
else deft=qApp->font().family();
QString fam=config->value("Editor/Font Family",deft).toString();
int si=config->value( "Editor/Font Size",qApp->font().pointSize()).toInt();
#endif
QFont F(fam,si);
EditorFont=F;

wordwrap=config->value( "Editor/WordWrap",true).toBool();
parenmatch=config->value( "Editor/Parentheses Matching",true).toBool();
showline=config->value( "Editor/Line Numbers",true).toBool();
completion=config->value( "Editor/Completion",true).toBool();
userCompletionList=config->value( "Editor/UserCompletion",true).toStringList();
shortcuts.clear();
QStringList data,shortcut;
data=config->value("Shortcuts/data").toStringList();
shortcut=config->value("Shortcuts/shortcut").toStringList();
QStringListIterator dataiterator(data);
QStringListIterator shortcutiterator(shortcut);
while ( dataiterator.hasNext() && shortcutiterator.hasNext())
	{
	QString d=dataiterator.next();
	if (!d.isEmpty()) shortcuts.insert(d,shortcutiterator.next());
	}

showoutputview=config->value("Show/OutputView",false).toBool();
showstructview=config->value( "Show/Structureview",true).toBool();
showpdfview=config->value( "Show/Pdfview",true).toBool();

quickmode=config->value( "Tools/Quick Mode",3).toInt();
QString baseName = qApp->style()->objectName();
builtinpdfview=config->value("Tools/IntegratedPdfViewer",true).toBool();
singleviewerinstance=config->value("Tools/SingleViewerInstance",false).toBool();
#ifdef Q_WS_MACX 
// /usr/local/teTeX/bin/i386-apple-darwin-current
// /usr/local/teTeX/bin/powerpc-apple-darwin-current
// /usr/texbin MACTEX/TEXLIVE2007
latex_command=config->value("Tools/Latex","\"/usr/texbin/latex\" -interaction=nonstopmode %.tex").toString();
dvips_command=config->value("Tools/Dvips","\"/usr/texbin/dvips\" -o %.ps %.dvi").toString();
ps2pdf_command=config->value("Tools/Ps2pdf","\"/usr/local/bin/ps2pdf\" %.ps").toString();
makeindex_command=config->value("Tools/Makeindex","\"/usr/texbin/makeindex\" %.idx").toString();
bibtex_command=config->value("Tools/Bibtex","\"/usr/texbin/bibtex\" %.aux").toString();
pdflatex_command=config->value("Tools/Pdflatex","\"/usr/texbin/pdflatex\" -synctex=1 -interaction=nonstopmode %.tex").toString();
dvipdf_command=config->value("Tools/Dvipdf","\"/usr/texbin/dvipdfm\" %.dvi").toString();
metapost_command=config->value("Tools/Metapost","\"/usr/texbin/mpost\" --interaction nonstopmode ").toString();
viewdvi_command=config->value("Tools/Dvi","open %.dvi").toString();
viewps_command=config->value("Tools/Ps","open %.ps").toString();
viewpdf_command=config->value("Tools/Pdf","open %.pdf").toString();
ghostscript_command=config->value("Tools/Ghostscript","/usr/local/bin/gs").toString();
asymptote_command=config->value("Tools/Asymptote","/usr/bin/asy %.asy").toString();
latexmk_command=config->value("Tools/Latexmk","\"/usr/texbin/latexmk\" -e \"$pdflatex=q/pdflatex -synctex=1 -interaction=nonstopmode/\" -pdf %.tex").toString();
if (modern_style) qApp->setStyle(new ManhattanStyle(baseName));
#endif
#ifdef Q_WS_WIN
latex_command=config->value("Tools/Latex","latex -interaction=nonstopmode %.tex").toString();
dvips_command=config->value("Tools/Dvips","dvips -o %.ps %.dvi").toString();
ps2pdf_command=config->value("Tools/Ps2pdf","ps2pdf %.ps").toString();
makeindex_command=config->value("Tools/Makeindex","makeindex %.idx").toString();
bibtex_command=config->value("Tools/Bibtex","bibtex %").toString();
//bibtex %.aux
pdflatex_command=config->value("Tools/Pdflatex","pdflatex -synctex=1 -interaction=nonstopmode %.tex").toString();
dvipdf_command=config->value("Tools/Dvipdf","dvipdfm %.dvi").toString();
metapost_command=config->value("Tools/Metapost","mpost --interaction nonstopmode ").toString();
viewdvi_command=config->value("Tools/Dvi","\"C:/Program Files/MiKTeX 2.9/miktex/bin/yap.exe\" %.dvi").toString();
viewps_command=config->value("Tools/Ps","\"C:/Program Files/Ghostgum/gsview/gsview32.exe\" %.ps").toString();
viewpdf_command=config->value("Tools/Pdf","\"C:/Program Files/Adobe/Reader 9.0/Reader/AcroRd32.exe\" %.pdf").toString();
ghostscript_command=config->value("Tools/Ghostscript","\"C:/Program Files/gs/gs9.00/bin/gswin32c.exe\"").toString();
asymptote_command=config->value("Tools/Asymptote","\"C:/Asymptote/asy.exe\" %.asy").toString();
latexmk_command=config->value("Tools/Latexmk","latexmk -e \"$pdflatex=q/pdflatex -synctex=1 -interaction=nonstopmode/\" -pdf %.tex").toString();
QString yap="C:/Program Files/MiKTeX 2.9/miktex/bin/yap.exe";
QString gsview="C:/Program Files/Ghostgum/gsview/gsview32.exe";
QString gswin="C:/Program Files/gs/gs9.00/bin/gswin32c.exe";
QString acro="C:/Program Files/Adobe/Reader 9.0/Reader/AcroRd32.exe";

if (new_user)
  {
  if (!QFileInfo(gswin).exists())
    {
    if (QFileInfo("C:/Program Files (x86)/gs/gs9.00/bin/gswin32c.exe").exists()) gswin="C:/Program Files (x86)/gs/gs9.00/bin/gswin32c.exe";
    else if (QFileInfo("C:/Program Files/gs/gs8.71/bin/gswin32c.exe").exists()) gswin="C:/Program Files/gs/gs8.71/bin/gswin32c.exe";
    else if (QFileInfo("C:/Program Files (x86)/gs/gs8.71/bin/gswin32c.exe").exists()) gswin="C:/Program Files (x86)/gs/gs8.71/bin/gswin32c.exe";
    else if (QFileInfo("C:/Program Files/gs/gs8.64/bin/gswin32c.exe").exists()) gswin="C:/Program Files/gs/gs8.64/bin/gswin32c.exe";
    else if (QFileInfo("C:/Program Files (x86)/gs/gs8.64/bin/gswin32c.exe").exists()) gswin="C:/Program Files (x86)/gs/gs8.65/bin/gswin32c.exe";
    else if (QFileInfo("C:/Program Files/gs/gs8.63/bin/gswin32c.exe").exists()) gswin="C:/Program Files/gs/gs8.63/bin/gswin32c.exe";
    else if (QFileInfo("C:/Program Files (x86)/gs/gs8.63/bin/gswin32c.exe").exists()) gswin="C:/Program Files (x86)/gs/gs8.63/bin/gswin32c.exe";
    else if (QFileInfo("C:/Program Files/gs/gs8.62/bin/gswin32c.exe").exists()) gswin="C:/Program Files/gs/gs8.62/bin/gswin32c.exe";
    else if (QFileInfo("C:/Program Files (x86)/gs/gs8.62/bin/gswin32c.exe").exists()) gswin="C:/Program Files (x86)/gs/gs8.62/bin/gswin32c.exe";
    }
  ghostscript_command="\""+gswin+"\"";
  if (!QFileInfo(yap).exists())
    {
    if (QFileInfo("C:/Program Files (x86)/MiKTeX 2.9/miktex/bin/yap.exe").exists()) yap="C:/Program Files (x86)/MiKTeX 2.9/miktex/bin/yap.exe";
    else if (QFileInfo("C:/Program Files/MiKTeX 2.8/miktex/bin/yap.exe").exists()) yap="C:/Program Files/MiKTeX 2.8/miktex/bin/yap.exe";
    else if (QFileInfo("C:/Program Files (x86)/MiKTeX 2.8/miktex/bin/yap.exe").exists()) yap="C:/Program Files (x86)/MiKTeX 2.8/miktex/bin/yap.exe";
    else if (QFileInfo("C:/Program Files/MiKTeX 2.7/miktex/bin/yap.exe").exists()) yap="C:/Program Files/MiKTeX 2.7/miktex/bin/yap.exe";
    else if (QFileInfo("C:/Program Files (x86)/MiKTeX 2.7/miktex/bin/yap.exe").exists()) yap="C:/Program Files (x86)/MiKTeX 2.7/miktex/bin/yap.exe";
    else if (QFileInfo("C:/Program Files/MiKTeX 2.5/miktex/bin/yap.exe").exists()) yap="C:/Program Files/MiKTeX 2.5/miktex/bin/yap.exe";
    else if (QFileInfo("C:/Program Files (x86)/MiKTeX 2.5/miktex/bin/yap.exe").exists()) yap="C:/Program Files (x86)/MiKTeX 2.5/miktex/bin/yap.exe";
    else if (QFileInfo("C:/texlive/2009/bin/win32/dviout.exe").exists()) yap="C:/texlive/2009/bin/win32/dviout.exe";
    else if (QFileInfo("C:/texlive/2010/bin/win32/dviout.exe").exists()) yap="C:/texlive/2010/bin/win32/dviout.exe";
    }
  viewdvi_command="\""+yap+"\" %.dvi";  
  if (!QFileInfo(gsview).exists())
    {
    if (QFileInfo("C:/Program Files (x86)/Ghostgum/gsview/gsview32.exe").exists()) gsview="C:/Program Files (x86)/Ghostgum/gsview/gsview32.exe";
    else if (QFileInfo("C:/texlive/2009/bin/win32/psv.exe").exists()) gsview="C:/texlive/2009/bin/win32/psv.exe";
    else if (QFileInfo("C:/texlive/2010/bin/win32/psv.exe").exists()) gsview="C:/texlive/2010/bin/win32/psv.exe";
    }
  viewps_command="\""+gsview+"\" %.ps";
  if (!QFileInfo(acro).exists())
    {
    if (QFileInfo("C:/Program Files (x86)/Adobe/Reader 9.0/Reader/AcroRd32.exe").exists()) acro="C:/Program Files (x86)/Adobe/Reader 9.0/Reader/AcroRd32.exe";
    else if (QFileInfo("C:/Program Files/Adobe/Reader 8.0/Reader/AcroRd32.exe").exists()) acro="C:/Program Files/Adobe/Reader 8.0/Reader/AcroRd32.exe";
    else if (QFileInfo("C:/Program Files (x86)/Adobe/Reader 8.0/Reader/AcroRd32.exe").exists()) acro="C:/Program Files (x86)/Adobe/Reader 8.0/Reader/AcroRd32.exe";
    }
  viewpdf_command="\""+acro+"\" %.pdf"; 
  }
//\"C:/Program Files/MiKTeX 2.7/miktex/bin/yap.exe\" -1 -s @%.tex %.dvi

if (modern_style) qApp->setStyle(new ManhattanStyle(baseName));
#endif
#ifdef Q_WS_X11
int desktop_env=1; // 1 : no kde ; 2: kde ; 3 : kde4 ; 
QStringList styles = QStyleFactory::keys();
QString kdesession= ::getenv("KDE_FULL_SESSION");
QString kdeversion= ::getenv("KDE_SESSION_VERSION");
if (!kdesession.isEmpty()) desktop_env=2;
if (!kdeversion.isEmpty()) desktop_env=3;
//desktop_env=1;
latex_command=config->value("Tools/Latex","latex -interaction=nonstopmode %.tex").toString();
dvips_command=config->value("Tools/Dvips","dvips -o %.ps %.dvi").toString();
ps2pdf_command=config->value("Tools/Ps2pdf","ps2pdf %.ps").toString();
makeindex_command=config->value("Tools/Makeindex","makeindex %.idx").toString();
bibtex_command=config->value("Tools/Bibtex","bibtex %.aux").toString();
pdflatex_command=config->value("Tools/Pdflatex","pdflatex -synctex=1 -interaction=nonstopmode %.tex").toString();
dvipdf_command=config->value("Tools/Dvipdf","dvipdfm %.dvi").toString();
metapost_command=config->value("Tools/Metapost","mpost --interaction nonstopmode ").toString();
// xdvi %.dvi  -sourceposition @:%.tex
// kdvi "file:%.dvi#src:@ %.tex"
switch (desktop_env)
	{
	case 1:
		{
		viewdvi_command=config->value("Tools/Dvi","evince %.dvi").toString();
		viewps_command=config->value("Tools/Ps","evince %.ps").toString();
		viewpdf_command=config->value("Tools/Pdf","evince %.pdf").toString();
		}break;
	case 2:
		{
		viewdvi_command=config->value("Tools/Dvi","kdvi %.dvi").toString();
		viewps_command=config->value("Tools/Ps","kghostview %.ps").toString();
		viewpdf_command=config->value("Tools/Pdf","kpdf %.pdf").toString();
		}break;
	case 3:
		{
		viewdvi_command=config->value("Tools/Dvi","okular %.dvi").toString();
		viewps_command=config->value("Tools/Ps","okular %.ps").toString();
		viewpdf_command=config->value("Tools/Pdf","okular %.pdf").toString();
		}break;
	}



ghostscript_command=config->value("Tools/Ghostscript","gs").toString();
asymptote_command=config->value("Tools/Asymptote","asy %.asy").toString();
latexmk_command=config->value("Tools/Latexmk","latexmk -e \"$pdflatex=q/pdflatex -synctex=1 -interaction=nonstopmode/\" -pdf %.tex").toString();

x11style=config->value( "X11/Style","Plastique").toString();
if (xf.contains("DejaVu Sans",Qt::CaseInsensitive)) deft="DejaVu Sans";
else if (xf.contains("DejaVu Sans LGC",Qt::CaseInsensitive)) deft="DejaVu Sans LGC";
else if (xf.contains("Bitstream Vera Sans",Qt::CaseInsensitive)) deft="Bitstream Vera Sans";
else if (xf.contains("Luxi Sans",Qt::CaseInsensitive)) deft="Luxi Sans";
else deft=qApp->font().family();
x11fontfamily=config->value("X11/Font Family",deft).toString();
x11fontsize=config->value( "X11/Font Size","10").toInt();

if (modern_style)
    {
    if (desktop_env ==1) //no-kde
	{
	if (styles.contains("GTK+")) qApp->setStyle(new ManhattanStyle(QString("GTK+")));//gtkstyle
	else qApp->setStyle(new ManhattanStyle(QString("Cleanlooks")));
	}
    else if ((desktop_env ==3) && (styles.contains("Oxygen"))) qApp->setStyle(new ManhattanStyle(QString("Oxygen"))); //kde4+oxygen 
    else qApp->setStyle(new ManhattanStyle(QString("Plastique"))); //others
    QString baseStyleName = qApp->style()->objectName(); //fallback
    if (baseStyleName == QLatin1String("Windows")) qApp->setStyle(new ManhattanStyle(QString("Plastique"))); //fallback
    }
else
    {
    if (desktop_env ==1) //no-kde
	{
	if (styles.contains("GTK+")) QApplication::setStyle(QStyleFactory::create("GTK+"));//gtkstyle
	else QApplication::setStyle(QStyleFactory::create("Cleanlooks"));
	}
    else if ((desktop_env ==3) && (styles.contains("Oxygen"))) QApplication::setStyle(QStyleFactory::create("Oxygen")); //kde4+oxygen
    else QApplication::setStyle(QStyleFactory::create("Plastique")); //others
    QString baseStyleName = qApp->style()->objectName(); //fallback
    if (baseStyleName == QLatin1String("Windows")) QApplication::setStyle(QStyleFactory::create("Plastique")); //fallback
    }
   
// QApplication::setPalette(QApplication::style()->standardPalette());
QFont x11Font (x11fontfamily,x11fontsize);
QApplication::setFont(x11Font);

QPalette pal = QApplication::palette();
pal.setColor( QPalette::Active, QPalette::Highlight, QColor("#4490d8") );
pal.setColor( QPalette::Inactive, QPalette::Highlight, QColor("#4490d8") );
pal.setColor( QPalette::Disabled, QPalette::Highlight, QColor("#4490d8") );

pal.setColor( QPalette::Active, QPalette::HighlightedText, QColor("#ffffff") );
pal.setColor( QPalette::Inactive, QPalette::HighlightedText, QColor("#ffffff") );
pal.setColor( QPalette::Disabled, QPalette::HighlightedText, QColor("#ffffff") );

pal.setColor( QPalette::Active, QPalette::Base, QColor("#ffffff") );
pal.setColor( QPalette::Inactive, QPalette::Base, QColor("#ffffff") );
pal.setColor( QPalette::Disabled, QPalette::Base, QColor("#ffffff") );

pal.setColor( QPalette::Active, QPalette::WindowText, QColor("#000000") );
pal.setColor( QPalette::Inactive, QPalette::WindowText, QColor("#000000") );
pal.setColor( QPalette::Disabled, QPalette::WindowText, QColor("#000000") );

pal.setColor( QPalette::Active, QPalette::Text, QColor("#000000") );
pal.setColor( QPalette::Inactive, QPalette::Text, QColor("#000000") );
pal.setColor( QPalette::Disabled, QPalette::Text, QColor("#000000") );

pal.setColor( QPalette::Active, QPalette::ButtonText, QColor("#000000") );
pal.setColor( QPalette::Inactive, QPalette::ButtonText, QColor("#000000") );
pal.setColor( QPalette::Disabled, QPalette::ButtonText, QColor("#000000") );

if (desktop_env ==3)
	{
	pal.setColor( QPalette::Active, QPalette::Window, QColor("#eae9e9") );
	pal.setColor( QPalette::Inactive, QPalette::Window, QColor("#eae9e9") );
	pal.setColor( QPalette::Disabled, QPalette::Window, QColor("#eae9e9") );

	pal.setColor( QPalette::Active, QPalette::Button, QColor("#eae9e9") );
	pal.setColor( QPalette::Inactive, QPalette::Button, QColor("#eae9e9") );
	pal.setColor( QPalette::Disabled, QPalette::Button, QColor("#eae9e9") );
	}
else
	{
	pal.setColor( QPalette::Active, QPalette::Window, QColor("#f6f3eb") );
	pal.setColor( QPalette::Inactive, QPalette::Window, QColor("#f6f3eb") );
	pal.setColor( QPalette::Disabled, QPalette::Window, QColor("#f6f3eb") );

	pal.setColor( QPalette::Active, QPalette::Button, QColor("#f6f3eb") );
	pal.setColor( QPalette::Inactive, QPalette::Button, QColor("#f6f3eb") );
	pal.setColor( QPalette::Disabled, QPalette::Button, QColor("#f6f3eb") );
	}

QApplication::setPalette(pal);
#endif
userquick_command=config->value("Tools/Userquick","latex -interaction=nonstopmode %.tex|bibtex %.aux|latex -interaction=nonstopmode %.tex|latex -interaction=nonstopmode %.tex|xdvi %.dvi").toString();
userClassList=config->value("Tools/User Class").toStringList();
userPaperList=config->value("Tools/User Paper").toStringList();
userEncodingList=config->value("Tools/User Encoding").toStringList();
userOptionsList=config->value("Tools/User Options").toStringList();
runIndex=config->value( "Tools/Run","0").toInt();
viewIndex=config->value( "Tools/View","2").toInt();

lastDocument=config->value("Files/Last Document","").toString();
recentFilesList=config->value("Files/Recent Files New").toStringList();
sessionFilesList=config->value("Files/Session Files").toStringList();
input_encoding=config->value("Files/Input Encoding","UTF-8").toString();
UserMenuName[0]=config->value("User/Menu1","").toString();
UserMenuTag[0]=config->value("User/Tag1","").toString();
UserMenuName[1]=config->value("User/Menu2","").toString();
UserMenuTag[1]=config->value("User/Tag2","").toString();
UserMenuName[2]=config->value("User/Menu3","").toString();
UserMenuTag[2]=config->value("User/Tag3","").toString();
UserMenuName[3]=config->value("User/Menu4","").toString();
UserMenuTag[3]=config->value("User/Tag4","").toString();
UserMenuName[4]=config->value("User/Menu5","").toString();
UserMenuTag[4]=config->value("User/Tag5","").toString();
UserMenuName[5]=config->value("User/Menu6","").toString();
UserMenuTag[5]=config->value("User/Tag6","").toString();
UserMenuName[6]=config->value("User/Menu7","").toString();
UserMenuTag[6]=config->value("User/Tag7","").toString();
UserMenuName[7]=config->value("User/Menu8","").toString();
UserMenuTag[7]=config->value("User/Tag8","").toString();
UserMenuName[8]=config->value("User/Menu9","").toString();
UserMenuTag[8]=config->value("User/Tag9","").toString();
UserMenuName[9]=config->value("User/Menu10","").toString();
UserMenuTag[9]=config->value("User/Tag10","").toString();
UserToolName[0]=config->value("User/ToolName1","").toString();
UserToolCommand[0]=config->value("User/Tool1","").toString();
UserToolName[1]=config->value("User/ToolName2","").toString();
UserToolCommand[1]=config->value("User/Tool2","").toString();
UserToolName[2]=config->value("User/ToolName3","").toString();
UserToolCommand[2]=config->value("User/Tool3","").toString();
UserToolName[3]=config->value("User/ToolName4","").toString();
UserToolCommand[3]=config->value("User/Tool4","").toString();
UserToolName[4]=config->value("User/ToolName5","").toString();
UserToolCommand[4]=config->value("User/Tool5","").toString();

struct_level1=config->value("Structure/Structure Level 1","part").toString();
struct_level2=config->value("Structure/Structure Level 2","chapter").toString();
struct_level3=config->value("Structure/Structure Level 3","section").toString();
struct_level4=config->value("Structure/Structure Level 4","subsection").toString();
struct_level5=config->value("Structure/Structure Level 5","subsubsection").toString();


document_class=config->value("Quick/Class","article").toString();
typeface_size=config->value("Quick/Typeface","10pt").toString();
paper_size=config->value("Quick/Papersize","a4paper").toString();
document_encoding=config->value("Quick/Encoding","latin1").toString();
ams_packages=config->value( "Quick/AMS",true).toBool();
makeidx_package=config->value( "Quick/MakeIndex",false).toBool();
author=config->value("Quick/Author","").toString();

#if defined( Q_WS_X11 )

#ifdef USB_VERSION
QString dicDir=QCoreApplication::applicationDirPath() + "/";
#else
QString dicDir=PREFIX"/share/texmaker/";
#endif

#endif
#if defined( Q_WS_MACX )
QString dicDir=QCoreApplication::applicationDirPath() + "/../Resources/";
#endif
#if defined(Q_WS_WIN)
QString dicDir=QCoreApplication::applicationDirPath() + "/";
#endif
QString defaultDic=dicDir+QString(QLocale::system().name())+".dic";
QFileInfo fi(defaultDic);
if (!fi.exists() || !fi.isReadable()) defaultDic=dicDir+"en_GB.dic";
spell_dic=config->value("Spell/Dic",defaultDic).toString();

spell_ignored_words=config->value("Spell/Words","").toString();
inlinespellcheck=config->value( "Spell/Inline",true).toBool();
for (int i=0; i <412 ; i++)
	{
	symbolScore[i]=config->value( "Symbols/symbol"+QString::number(i),0).toInt();
	}
favoriteSymbolList.clear();
QList<QVariant> favoriteSymbolSettings = config->value( "Symbols/Favorites" ).toList( );
if( !favoriteSymbolSettings.isEmpty())
	{
	for( int i = 0; i < favoriteSymbolSettings.count( ); i++ ) favoriteSymbolList.append(favoriteSymbolSettings.at(i).toInt());
	}

colorMath=config->value("Color/Math",QColor(0x00,0x80, 0x00)).value<QColor>();
colorCommand=config->value("Color/Command",QColor(0x80, 0x00, 0x00)).value<QColor>();
colorKeyword=config->value("Color/Keyword",QColor(0x00, 0x00, 0xCC)).value<QColor>();

config->endGroup();
}

void Texmaker::SaveSettings()
{
#ifdef USB_VERSION
QSettings config(QCoreApplication::applicationDirPath()+"/texmaker.ini",QSettings::IniFormat); //for USB-stick version 
#else
QSettings config(QSettings::IniFormat,QSettings::UserScope,"xm1","texmaker");
#endif

config.setValue( "IniMode",true);
config.beginGroup( "texmaker" );
QList<int> sizes;
QList<int>::Iterator it;

config.setValue( "GUI/Style",modern_style);
config.setValue( "GUI/New Version",true);
#if defined( Q_WS_X11 )
config.setValue( "X11/Style",x11style);
config.setValue("X11/Font Family",x11fontfamily);
config.setValue( "X11/Font Size",x11fontsize);
#endif

config.setValue("MainWindowState",saveState(0));
config.setValue("Splitter1State",splitter1->saveState());
config.setValue("Splitter2State",splitter2->saveState());
config.setValue("Geometries/MainwindowWidth", width() );
config.setValue("Geometries/MainwindowHeight", height() );
config.setValue("Geometries/MainwindowX", x() );
config.setValue("Geometries/MainwindowY", y() );

config.setValue("Print/PaperSize",psize);

config.setValue("Editor/Font Family",EditorFont.family());
config.setValue( "Editor/Font Size",EditorFont.pointSize());
config.setValue( "Editor/WordWrap",wordwrap);
config.setValue( "Editor/Parentheses Matching",parenmatch);
config.setValue( "Editor/Line Numbers",showline);
config.setValue( "Editor/Completion",completion);
config.setValue( "Editor/UserCompletion",userCompletionList);

QStringList data,shortcut;
// data.clear();
// shortcut.clear();
KeysMap::Iterator its;

for( its = shortcuts.begin(); its != shortcuts.end(); ++its )
	{
	data.append(its.key());
	shortcut.append(its.value());
	}
config.setValue("Shortcuts/data",data);
config.setValue("Shortcuts/shortcut",shortcut);

config.setValue("Show/OutputView",showoutputview);
config.setValue( "Show/Structureview",showstructview);
config.setValue( "Show/Pdfview",showpdfview);

config.setValue("Tools/Quick Mode",quickmode);
config.setValue("Tools/Latex",latex_command);
config.setValue("Tools/Dvi",viewdvi_command);
config.setValue("Tools/Dvips",dvips_command);
config.setValue("Tools/Ps",viewps_command);
config.setValue("Tools/Ps2pdf",ps2pdf_command);
config.setValue("Tools/Makeindex",makeindex_command);
config.setValue("Tools/Bibtex",bibtex_command);
config.setValue("Tools/Pdflatex",pdflatex_command);
config.setValue("Tools/Pdf",viewpdf_command);
config.setValue("Tools/Dvipdf",dvipdf_command);
config.setValue("Tools/Metapost",metapost_command);
config.setValue("Tools/Ghostscript",ghostscript_command);
config.setValue("Tools/Asymptote",asymptote_command);
config.setValue("Tools/Latexmk",latexmk_command);
config.setValue("Tools/Userquick",userquick_command);
if (userClassList.count()>0) config.setValue("Tools/User Class",userClassList); 
if (userPaperList.count()>0) config.setValue("Tools/User Paper",userPaperList); 
if (userEncodingList.count()>0) config.setValue("Tools/User Encoding",userEncodingList); 
if (userOptionsList.count()>0) config.setValue("Tools/User Options",userOptionsList); 
config.setValue( "Tools/Run",comboCompil->currentIndex());
config.setValue( "Tools/View",comboView->currentIndex());
config.setValue("Tools/IntegratedPdfViewer",builtinpdfview);
config.setValue("Tools/SingleViewerInstance",singleviewerinstance);


config.setValue("Files/Last Document",lastDocument);
if (recentFilesList.count()>0) config.setValue("Files/Recent Files New",recentFilesList); 
FilesMap::Iterator itf;
sessionFilesList.clear();
for( itf = filenames.begin(); itf != filenames.end(); ++itf )
	{
	if (!filenames[itf.key()].startsWith("untitled"))
	    {
	    if ((!singlemode) && (MasterName==filenames[itf.key()])) sessionFilesList.append("texmakermaster#"+filenames[itf.key()]);
	    else sessionFilesList.append(filenames[itf.key()]);
	    }
	}
config.setValue("Files/Session Files",sessionFilesList); 
config.setValue("Files/Input Encoding", input_encoding);

config.setValue("User/Menu1",UserMenuName[0]);
config.setValue("User/Tag1",UserMenuTag[0]);
config.setValue("User/Menu2",UserMenuName[1]);
config.setValue("User/Tag2",UserMenuTag[1]);
config.setValue("User/Menu3",UserMenuName[2]);
config.setValue("User/Tag3",UserMenuTag[2]);
config.setValue("User/Menu4",UserMenuName[3]);
config.setValue("User/Tag4",UserMenuTag[3]);
config.setValue("User/Menu5",UserMenuName[4]);
config.setValue("User/Tag5",UserMenuTag[4]);
config.setValue("User/Menu6",UserMenuName[5]);
config.setValue("User/Tag6",UserMenuTag[5]);
config.setValue("User/Menu7",UserMenuName[6]);
config.setValue("User/Tag7",UserMenuTag[6]);
config.setValue("User/Menu8",UserMenuName[7]);
config.setValue("User/Tag8",UserMenuTag[7]);
config.setValue("User/Menu9",UserMenuName[8]);
config.setValue("User/Tag9",UserMenuTag[8]);
config.setValue("User/Menu10",UserMenuName[9]);
config.setValue("User/Tag10",UserMenuTag[9]);
config.setValue("User/ToolName1",UserToolName[0]);
config.setValue("User/Tool1",UserToolCommand[0]);
config.setValue("User/ToolName2",UserToolName[1]);
config.setValue("User/Tool2",UserToolCommand[1]);
config.setValue("User/ToolName3",UserToolName[2]);
config.setValue("User/Tool3",UserToolCommand[2]);
config.setValue("User/ToolName4",UserToolName[3]);
config.setValue("User/Tool4",UserToolCommand[3]);
config.setValue("User/ToolName5",UserToolName[4]);
config.setValue("User/Tool5",UserToolCommand[4]);

config.setValue("Structure/Structure Level 1",struct_level1);
config.setValue("Structure/Structure Level 2",struct_level2);
config.setValue("Structure/Structure Level 3",struct_level3);
config.setValue("Structure/Structure Level 4",struct_level4);
config.setValue("Structure/Structure Level 5",struct_level5);

config.setValue( "Quick/Class",document_class);
config.setValue( "Quick/Typeface",typeface_size);
config.setValue( "Quick/Papersize",paper_size);
config.setValue( "Quick/Encoding",document_encoding);
config.setValue( "Quick/AMS",ams_packages);
config.setValue( "Quick/MakeIndex",makeidx_package);
config.setValue( "Quick/Author",author);

config.setValue( "Spell/Dic",spell_dic);
config.setValue( "Spell/Words",spell_ignored_words);
config.setValue( "Spell/Inline",inlinespellcheck);

for (int i=0; i <412 ; i++)
	{
	config.setValue( "Symbols/symbol"+QString::number(i),symbolScore[i]);
	}
QList<QVariant> favoriteSymbolSettings;
if( !favoriteSymbolList.isEmpty())
	{
	for( int i = 0; i < favoriteSymbolList.count( ); i++ ) favoriteSymbolSettings.append(favoriteSymbolList.at(i));
	}
config.setValue("Symbols/Favorites",favoriteSymbolSettings);
config.setValue("Color/Math",colorMath);
config.setValue("Color/Command",colorCommand);
config.setValue("Color/Keyword",colorKeyword);


config.endGroup();
}

void Texmaker::setPrintPaperSize(const QString &p)
{
psize=p;
}

////////////////// STRUCTURE ///////////////////
void Texmaker::ShowStructure()
{
LeftPanelStackedWidget->setCurrentWidget(StructureTreeWidget);
titleLeftPanel->setText(tr("Structure"));
}
void Texmaker::ShowRelation() //RelationListWidget
{
LeftPanelStackedWidget->setCurrentWidget(RelationListWidget);
titleLeftPanel->setText(tr("Relation symbols"));
}
void Texmaker::ShowArrow() //ArrowListWidget
{
LeftPanelStackedWidget->setCurrentWidget(ArrowListWidget);
titleLeftPanel->setText(tr("Arrow symbols"));
}
void Texmaker::ShowMisc() //MiscellaneousListWidget
{
LeftPanelStackedWidget->setCurrentWidget(MiscellaneousListWidget);
titleLeftPanel->setText(tr("Miscellaneous symbols"));
}
void Texmaker::ShowDelim() //DelimitersListWidget
{
LeftPanelStackedWidget->setCurrentWidget(DelimitersListWidget);
titleLeftPanel->setText(tr("Delimiters"));
}
void Texmaker::ShowGreek() //GreekListWidget
{
LeftPanelStackedWidget->setCurrentWidget(GreekListWidget);
titleLeftPanel->setText(tr("Greek letters"));
}
void Texmaker::ShowMostUsed() //MostUsedListWidget
{
LeftPanelStackedWidget->setCurrentWidget(MostUsedListWidget);
titleLeftPanel->setText(tr("Most used symbols"));
}
void Texmaker::ShowFavorite() //FavoriteListWidget
{
LeftPanelStackedWidget->setCurrentWidget(FavoriteListWidget);
titleLeftPanel->setText(tr("Favorites symbols"));
}
void Texmaker::ShowPstricks() //PsListWidget
{
LeftPanelStackedWidget->setCurrentWidget(PsListWidget);
titleLeftPanel->setText(tr("Pstricks Commands"));
}
void Texmaker::ShowLeftRight() //leftrightWidget
{
LeftPanelStackedWidget->setCurrentWidget(leftrightWidget);
titleLeftPanel->setText("left/right");
}
void Texmaker::ShowMplist() //MpListWidget
{
LeftPanelStackedWidget->setCurrentWidget(MpListWidget);
titleLeftPanel->setText(tr("MetaPost Commands"));
}
void Texmaker::ShowTikz() //TikzWidget
{
LeftPanelStackedWidget->setCurrentWidget(tikzWidget);
titleLeftPanel->setText(tr("Tikz Commands"));
}
void Texmaker::ShowAsy() //AsyWidget
{
LeftPanelStackedWidget->setCurrentWidget(asyWidget);
titleLeftPanel->setText(tr("Asymptote Commands"));
}

void Texmaker::UpdateStructure()
{
QFont deft=QFont("DejaVu Sans Condensed",qApp->font().pointSize());

QTreeWidgetItem *Child,*parent_level[5], *theitem;
QString current;
if (StructureTreeWidget->currentItem()) current=StructureTreeWidget->currentItem()->text(0);
StructureTreeWidget->clear();
if ( !currentEditorView() ) return;

QString shortName = getName();
if ((shortName.right(4)!=".tex") && (!shortName.startsWith("untitled")))  return;

int pos;
while ( (pos = (int)shortName.indexOf('/')) != -1 )
shortName.remove(0,pos+1);
QTreeWidgetItem *top = new QTreeWidgetItem(StructureTreeWidget);
top->setIcon(0,QIcon(":/images/doc.png"));
top->setText(0,shortName);
top->setFont(0,deft);
Child=parent_level[0]=parent_level[1]=parent_level[2]=parent_level[3]=parent_level[4]=top;
labelitem.clear();
if (singlemode) 
  {
  listbibfiles.clear();
  listchildfiles.clear();
  }
QTreeWidgetItem *toplabel = new QTreeWidgetItem(top);
toplabel->setText(0,"LABELS");
toplabel->setFont(0,deft);
QTreeWidgetItem *blocklabel = new QTreeWidgetItem(top);
blocklabel->setText(0,"BLOCKS");
blocklabel->setFont(0,deft);
QString s;
QTextBlock p = currentEditorView()->editor->document()->begin();
const QList<LatexEditor::StructItem>& structure = currentEditorView()->editor->getStructItems();
//int i;
for (int j = 0; j < structure.count(); j++)
{
//i=structure.at(j).cursor.block().blockNumber();
      switch (structure.at(j).type)
	{
	case 0:
		{
		s=structure.at(j).item;
		Child = new QTreeWidgetItem(blocklabel);
		Child->setText(0,s);
		Child->setFont(0,deft);
		Child->setText(1,QString::number(j));
		StructureTreeWidget->expandItem(Child);
		}break;
	case 1:
		{
		s=structure.at(j).item;
		labelitem.append(s);
		Child = new QTreeWidgetItem(toplabel);
		Child->setText(0,s);
		Child->setFont(0,deft);
		Child->setText(1,QString::number(j));
		StructureTreeWidget->expandItem(Child);
		}break;
	case 2:
		{
		s=structure.at(j).item;
		Child = new QTreeWidgetItem(top);
		Child->setText(0,s);
		Child->setFont(0,deft);
		Child->setText(1,QString::number(j));
		Child->setIcon(0,QIcon(":/images/include.png"));
		if (listchildfiles.indexOf(s)<0) listchildfiles.append(s);
		}break;
	case 3:
		{
		s=structure.at(j).item;
		Child = new QTreeWidgetItem(top);
		Child->setText(0,s);
		Child->setFont(0,deft);
		Child->setText(1,QString::number(j));
		Child->setIcon(0,QIcon(":/images/include.png"));
		if (listchildfiles.indexOf(s)<0) listchildfiles.append(s);
		}break;
	case 4:
		{
		s=structure.at(j).item;
		parent_level[0] = new QTreeWidgetItem(top);
		parent_level[0]->setText(0,s);
		parent_level[0]->setFont(0,deft);
		parent_level[0]->setText(1,QString::number(j));
		parent_level[0]->setIcon(0,QIcon(":/images/part.png"));
		StructureTreeWidget->expandItem(parent_level[0]);
		parent_level[1]=parent_level[2]=parent_level[3]=parent_level[4]=parent_level[0];
		}break;
	case 5:
		{
		s=structure.at(j).item;
		parent_level[1] = new QTreeWidgetItem(parent_level[0]);
		parent_level[1]->setText(0,s);
		parent_level[1]->setFont(0,deft);
		parent_level[1]->setText(1,QString::number(j));
		parent_level[1]->setIcon(0,QIcon(":/images/chapter.png"));
		StructureTreeWidget->expandItem(parent_level[1]);
		parent_level[2]=parent_level[3]=parent_level[4]=parent_level[1];
		}break;
	case 6:
		{
		s=structure.at(j).item;
		parent_level[2] = new QTreeWidgetItem(parent_level[1]);
		parent_level[2]->setText(0,s);
		parent_level[2]->setFont(0,deft);
		parent_level[2]->setText(1,QString::number(j));
		parent_level[2]->setIcon(0,QIcon(":/images/section.png"));
		StructureTreeWidget->expandItem(parent_level[2]);
		parent_level[3]=parent_level[4]=parent_level[2];
		}break;
	case 7:
		{
		s=structure.at(j).item;
		parent_level[3] = new QTreeWidgetItem(parent_level[2]);
		parent_level[3]->setText(0,s);
		parent_level[3]->setFont(0,deft);
		parent_level[3]->setText(1,QString::number(j));
		//parent_level[3]->setIcon(0,QIcon(":/images/subsection.png"));
		parent_level[4]=parent_level[3];
		}break;
	case 8:
		{
		s=structure.at(j).item;
		parent_level[4] = new QTreeWidgetItem(parent_level[3]);
		parent_level[4]->setText(0,s);
		parent_level[4]->setFont(0,deft);
		parent_level[4]->setText(1,QString::number(j));
		//parent_level[4]->setIcon(0,QIcon(":/images/subsubsection.png"));
		}break;
	case 9:
		{
		s=structure.at(j).item;
		Child = new QTreeWidgetItem(top);
		Child->setText(0,s);
		Child->setFont(0,deft);
		Child->setText(1,QString::number(j));
		Child->setIcon(0,QIcon(":/images/include.png"));
		if (listbibfiles.indexOf(s)<0) listbibfiles.append(s);
		}break;
	}
}	

if (!current.isEmpty()) 
	{
	QList<QTreeWidgetItem *> fItems=StructureTreeWidget->findItems (current,Qt::MatchRecursive,0);
	if ((fItems.size()>0 ) && (fItems.at(0))) 
		{
		StructureTreeWidget->setCurrentItem(fItems.at(0));
		theitem=fItems.at(0)->parent();
		while ((theitem) && (theitem!=top)) 
			{	
			StructureTreeWidget->expandItem(theitem);
			theitem=theitem->parent();
			}
		}
	}
StructureTreeWidget->setItemExpanded (top,true);
StructureTreeWidget->setItemExpanded (toplabel,true);
StructureTreeWidget->setItemExpanded (blocklabel,true);
currentEditorView()->editor->foldableLines.clear();
int endpreamble = currentEditorView()->editor->searchLine("\\begin{document}");
if (endpreamble>1) currentEditorView()->editor->foldableLines.insert(0,endpreamble-1);
ParseTree(top);
UpdateChildsLabels(listchildfiles);
updateCompleter();
if (currentEditorView() && completion) currentEditorView()->editor->setCompleter(completer);
if (currentEditorView() && !completion) currentEditorView()->editor->setCompleter(0);
if (currentEditorView() ) currentEditorView()->editor->matchAll();
}

void Texmaker::UpdateChildsLabels(QStringList listfiles)
{
if ( !currentEditorView() ) return;
QString shortName = getName();
if ((shortName.right(4)!=".tex") && (!shortName.startsWith("untitled")))  return;
//currentEditorView()->editor->highlighter->rehighlight();
int pos;
while ( (pos = (int)shortName.indexOf('/')) != -1 )
shortName.remove(0,pos+1);
/*********/
QString finame;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
QFileInfo fi(finame);
QString name=fi.absoluteFilePath();
QString flname=fi.fileName();
QString basename=name.left(name.length()-flname.length());
if (listfiles.count()>0)
  {
   QString fname;
   QRegExp macroName("\\label\\{(.*)\\}", Qt::CaseInsensitive);
   macroName.setMinimal(true);
   QString line;
  for ( int i = 0; i <listfiles.count(); i++ )
      {
      fname=listfiles.at(i);
      if (fname.right(4)==".tex") fname=basename+fname;
      else fname=basename+fname+".tex";
      QFileInfo fi(fname);
      if (fi.exists() && fi.isReadable()) 
	{
	QFile f(fname);
	if ( f.open(QIODevice::ReadOnly) )
		{
		QTextStream t( &f );
		while ( !t.atEnd() )
			{
			line=t.readLine();
			line=line.simplified();
			if (!line.isEmpty()) 
			    {
			    if (macroName.indexIn(line)>-1) {labelitem.append(macroName.cap(1).trimmed());}
			    }
			}
		}
		f.close();
	}
      }
  }
}

void Texmaker::UpdateBibliography()
{
if ( !currentEditorView() ) return;
QString shortName = getName();
if ((shortName.right(4)!=".tex") && (!shortName.startsWith("untitled")))  return;
//currentEditorView()->editor->highlighter->rehighlight();
int pos;
while ( (pos = (int)shortName.indexOf('/')) != -1 )
shortName.remove(0,pos+1);
/*********/
QString finame;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
QFileInfo fi(finame);
QString name=fi.absoluteFilePath();
QString flname=fi.fileName();
QString basename=name.left(name.length()-flname.length());
bibitem.clear();
if (listbibfiles.count()>0)
  {
   QString fname;
     QStringList types;
      types << QLatin1String("article") << QLatin1String("book")
        << QLatin1String("booklet") << QLatin1String("inbook")
        << QLatin1String("incollection") << QLatin1String("inproceedings")
        << QLatin1String("manual") << QLatin1String("mastersthesis")
        << QLatin1String("misc") << QLatin1String("phdthesis")
        << QLatin1String("proceedings") << QLatin1String("techreport")
        << QLatin1String("unpublished") << QLatin1String("periodical")
        << QLatin1String("conference");
   QRegExp macroName("@("+types.join("|")+")\\s*\\{\\s*(.*),", Qt::CaseInsensitive);
   macroName.setMinimal(true);
   QString line;
  for ( int i = 0; i <listbibfiles.count(); i++ )
      {
      fname=listbibfiles.at(i);
      if (fname.right(4)==".bib") fname=basename+fname;
      else fname=basename+fname+".bib";
      QFileInfo fi(fname);
      if (fi.exists() && fi.isReadable()) 
	{
	QFile f(fname);
	if ( f.open(QIODevice::ReadOnly) )
		{
		QTextStream t( &f );
		while ( !t.atEnd() )
			{
			line=t.readLine();
			line=line.simplified();
			if (!line.isEmpty()) 
			    {
			    if (macroName.indexIn(line)>-1) bibitem.append(macroName.cap(2).trimmed());
			    }
			}
		}
		f.close();
	}
      }
  }
/************/
updateCompleter();
if (currentEditorView() && completion) currentEditorView()->editor->setCompleter(completer);
if (currentEditorView() && !completion) currentEditorView()->editor->setCompleter(0);
if (currentEditorView() ) currentEditorView()->editor->matchAll();
}

void Texmaker::ParseTree(QTreeWidgetItem *item)
{

  if (item->childCount()>0)
	{
	for (int i = 0; i < item->childCount(); i++) 
		{
		ItemToRange(item->child(i));
		}
	}
 
  if (item->childCount()>0)
	{
	for (int i = 0; i < item->childCount(); i++) 
		{
		ParseTree(item->child(i));
		}
	}
 
}

int Texmaker::LevelItem(const QTreeWidgetItem *item)
{
int level=0;
const QList<LatexEditor::StructItem>& structure = currentEditorView()->editor->getStructItems();
if ((item) && (structure.count()>0))
  {
  int index = item->text(1).toInt();
  if (index<structure.count())
    {
    if ((structure.at(index).type<=3) || (structure.at(index).type==9) ) level=0;
    else level=structure.at(index).type-3;
    }
  }
return level;

/*if ((item) && (!structlist.isEmpty()))
  {
  QStringList::ConstIterator it1 = structitem.begin();
  QList<int>::ConstIterator it3 = structlevel.begin();
  for ( ; it1 !=structitem.end(); ++it1 )
	  {
	  if (*it1==item->text(0)) break;
	  ++it3;
	  }
  level=*it3;
  }
return level;*/
}

int Texmaker::LineItem(const QTreeWidgetItem *item)
{
int line=-1;
const QList<LatexEditor::StructItem>& structure = currentEditorView()->editor->getStructItems();
if ((item) && (structure.count()>0))
  {
  int index = item->text(1).toInt();
  if (index<structure.count())
    {
    line=structure.at(index).cursor.block().blockNumber();//line=structure.at(index).line;
    }
  }
return line;

/*if ((item) && (!structlist.isEmpty()))
  {
  QStringList::ConstIterator it1 = structitem.begin();
  QStringList::ConstIterator it2 = structlist.begin();
  for ( ; it1 !=structitem.end(); ++it1 )
	  {
	  if (*it1==item->text(0)) break;
	  ++it2;
	  }
  QString s=*it2;
  bool ok;
  int l=s.toInt(&ok,10);
  if (ok) line=l;
  }
return line;*/
}

void Texmaker::ItemToRange(QTreeWidgetItem *item)
{
if (!item) return;
QTreeWidgetItem *parentitem, *theitem;
theitem=item;
int start=-1;
int end=-1;
int level=LevelItem(item);
int index;
if (level>=1)
  {
  start=LineItem(theitem);
  do 
      {
      parentitem=theitem->parent();
      if (parentitem) 
	{
	index=parentitem->indexOfChild(theitem);
	theitem=parentitem;
	} 
      else index=-1;
      } 
  while ((index>=0)&&(index>=parentitem->childCount()-1)&&(LevelItem(parentitem)>=1));
  if (index>=0 && index<parentitem->childCount()-1) end=LineItem(parentitem->child(index+1));
  else  
    {
    end=currentEditorView()->editor->searchLine("\\end{document}");
    if (end<0) end=currentEditorView()->editor->numoflines();
    }
  if (start<end-1) currentEditorView()->editor->foldableLines.insert(start,end-1);
  }
}

void Texmaker::ClickedOnStructure(QTreeWidgetItem *item,int col)
{
if ( !currentEditorView() ) return;
QString finame;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
QString name,flname,basename;
bool hasname=true;
if ((singlemode && !currentEditorView()) || finame.startsWith("untitled") || finame=="")
{
hasname=false;
}
else
{
QFileInfo fi(finame);
name=fi.absoluteFilePath();
flname=fi.fileName();
basename=name.left(name.length()-flname.length());
}
if (item)
  {
  int index = item->text(1).toInt();
  const QList<LatexEditor::StructItem>& structure = currentEditorView()->editor->getStructItems();
  if (index<structure.count())
      {
      int type=structure.at(index).type;
      if (hasname && (type==2) || (type==3))
	  {
	    QString fname=structure.at(index).item;
	    if (fname.right(4)==".tex") fname=basename+fname;
	    else fname=basename+fname+".tex";
	    QFileInfo fi(fname);
	    if (fi.exists() && fi.isReadable()) load(fname);
	  }
	else  if (hasname && (type==9))
	  {
	    QString fname=structure.at(index).item;
	    if (fname.right(4)==".bib") fname=basename+fname;
	    else fname=basename+fname+".bib";
	    QFileInfo fi(fname);
	    if (fi.exists() && fi.isReadable()) load(fname);
	  }
	else
	  {
	    currentEditorView()->editor->gotoLine(structure.at(index).cursor.block().blockNumber());
	    if (!hasname) return;
	    QFileInfo fic(finame);
	    if (!fic.exists()) return;
	    QString basename=fic.completeBaseName();
	    if (largescreen && builtinpdfview)
	      {
	      if (pdfviewerWidget)
		{
		if (pdfviewerWidget->pdf_file!=fic.absolutePath()+"/"+basename+".pdf") pdfviewerWidget->openFile(fic.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
		StackedViewers->setCurrentWidget(pdfviewerWidget);
		pdfviewerWidget->show();
		if ( (pdflatex_command.contains("synctex=1")) || (latex_command.contains("synctex=1")) ) pdfviewerWidget->jumpToPdfFromSource(finame,structure.at(index).cursor.block().blockNumber()+1);
		}
	      }
	  }
      }
  }

}

//////////TAGS////////////////
void Texmaker::InsertTag(QString Entity, int dx, int dy)
{
if ( !currentEditorView() )	return;
OutputTextEdit->clear();
QTextCursor cur=currentEditorView()->editor->textCursor();
int pos=cur.position();
Entity.replace("{}","{"+QString(0x2022)+"}");
Entity.replace("[]","["+QString(0x2022)+"]");
Entity.replace("\n\n","\n"+QString(0x2022)+"\n");
currentEditorView()->editor->insertPlainText(Entity);
cur.setPosition(pos,QTextCursor::MoveAnchor);
if (Entity.contains(QString(0x2022))) 
    {
    currentEditorView()->editor->setTextCursor(cur);
    currentEditorView()->editor->search(QString(0x2022) ,true,true,true,true);
    OutputTextEdit->insertLine("Use the Tab key to reach the next "+QString(0x2022)+" field");
    }
else
    {
    if (dy>0) cur.movePosition(QTextCursor::Down,QTextCursor::MoveAnchor,dy);
    if (dx>0) cur.movePosition(QTextCursor::NextCharacter,QTextCursor::MoveAnchor,dx);
    currentEditorView()->editor->setTextCursor(cur);
    }
currentEditorView()->editor->setFocus();
OutputTableWidget->hide();
logpresent=false;
}

void Texmaker::InsertSymbol(QTableWidgetItem *item)
{
QString code_symbol;
QRegExp rxnumber(";([0-9]+)");
int number=-1;
int dx;
if (item)
	{
	if ( rxnumber.indexIn(item->text()) != -1) number=rxnumber.cap(1).toInt();
	if ((number>-1) && (number<412)) symbolScore[number]=symbolScore[number]+1;
	code_symbol=item->text().remove(rxnumber);
	dx=code_symbol.indexOf("{}",0);
	if (dx>-1) InsertTag(code_symbol,dx+1,0);
	else InsertTag(code_symbol,code_symbol.length(),0);
	SetMostUsedSymbols();
	}
}

void Texmaker::InsertXmlTag(QListWidgetItem *item)
{
QString txt, code, role;
QStringList tagList;
int dx,dy;
if (item  && !item->font().bold())
	{
	txt=item->text();
	txt.remove(QRegExp("\\[(.*)\\]"));
	txt.replace(QRegExp("([^\\\\])\\\\n"), "\\1\n"); 
	txt.replace(QRegExp("([^\\\\])\\\\n"), "\\1\n"); 
	role=item->data(Qt::UserRole).toString();
	tagList= role.split("#");
	code=tagList.at(0);
	code.remove(QRegExp("\\[(.*)\\]"));
	code.replace(QRegExp("([^\\\\])\\\\n"), "\\1\n"); 
	code.replace(QRegExp("([^\\\\])\\\\n"), "\\1\n"); 
	dx=tagList.at(1).toInt();
	dy=tagList.at(2).toInt();
	if ((dx==0) && (dy==0)) dx=code.length();
	InsertTag(code,dx,dy);
	}
}

void Texmaker::InsertFromAction()
{
bool ok;
QString actData;
QStringList tagList;
QAction *action = qobject_cast<QAction *>(sender());
if ( !currentEditorView() )	return;
if (action)
	{
	actData=action->data().toString();
	tagList= actData.split("/");
	InsertTag(tagList.at(0),tagList.at(1).toInt(&ok, 10),tagList.at(2).toInt(&ok, 10));
	OutputTextEdit->insertLine(tagList.at(3));
	}
}

void Texmaker::InsertWithSelectionFromAction()
{
bool ok;
QString actData;
QStringList tagList;
QAction *action = qobject_cast<QAction *>(sender());
if ( !currentEditorView() )	return;
if (action)
	{
	actData=action->data().toString();
	tagList= actData.split("/");
	if (!currentEditorView()->editor->textCursor().hasSelection())
		{
		OutputTextEdit->insertLine("You can select a text before using this environment.");
		InsertTag(tagList.at(0)+tagList.at(1),tagList.at(2).toInt(&ok, 10),tagList.at(3).toInt(&ok, 10));
		}
	else
		{
		currentEditorView()->editor->cut();
		InsertTag(tagList.at(0)+tagList.at(1),tagList.at(2).toInt(&ok, 10),tagList.at(3).toInt(&ok, 10));
		currentEditorView()->editor->paste();
//		InsertTag(tagList.at(1),0,0);
		}
	}
}

void Texmaker::InsertWithSelectionFromString(const QString& text)
{
bool ok;
QStringList tagList;
if ( !currentEditorView() )	return;
tagList= text.split("/");
if (!currentEditorView()->editor->textCursor().hasSelection())
	{
	InsertTag(tagList.at(0)+QString(0x2022)+tagList.at(1),tagList.at(2).toInt(&ok, 10),tagList.at(3).toInt(&ok, 10));
	}
else
	{
	currentEditorView()->editor->cut();
	InsertTag(tagList.at(0)+QString(0x2022)+tagList.at(1),tagList.at(2).toInt(&ok, 10),tagList.at(3).toInt(&ok, 10));
	currentEditorView()->editor->paste();
//	InsertTag(tagList.at(1),0,0);
	}
}

void Texmaker::InsertFromString(const QString& text)
{
bool ok;
QStringList tagList;
if ( !currentEditorView() ) return;
tagList= text.split("/");
InsertTag(tagList.at(0),tagList.at(1).toInt(&ok, 10),tagList.at(2).toInt(&ok, 10));
}

void Texmaker::InsertBib()
{
if ( !currentEditorView() )	return;
//currentEditorView()->editor->viewport()->setFocus();
QString tag;
QFileInfo fi(getName());
tag=QString("\\bibliography{");
tag +=fi.completeBaseName();
tag +=QString("}\n");
InsertTag(tag,0,1);
OutputTextEdit->clear();
OutputTableWidget->hide();
OutputTextEdit->insertLine("The argument to \\bibliography refers to the bib file (without extension)");
OutputTextEdit->insertLine("which should contain your database in BibTeX format.");
OutputTextEdit->insertLine("Texmaker inserts automatically the base name of the TeX file");
}

void Texmaker::InsertStruct()
{
QString actData, tag;
if ( !currentEditorView() )	return;
//currentEditorView()->editor->viewport()->setFocus();
QAction *action = qobject_cast<QAction *>(sender());
if (action)
	{
	actData=action->data().toString();
	StructDialog *stDlg = new StructDialog(this,actData);
	if ( stDlg->exec() )
		{
		if (stDlg->ui.checkBox->isChecked())
		{tag=actData+"{";}
		else
		{tag=actData+"*{";}
		tag +=stDlg->ui.TitlelineEdit->text();
		tag +=QString("}\n");
		InsertTag(tag,0,1);
		//UpdateStructure();
		}
	}
}

void Texmaker::InsertStructFromString(const QString& text)
{
QString tag;
if ( !currentEditorView() )	return;
//currentEditorView()->editor->viewport()->setFocus();
StructDialog *stDlg = new StructDialog(this,text);
if ( stDlg->exec() )
	{
	if (stDlg->ui.checkBox->isChecked())
	{tag=text+"{";}
	else
	{tag=text+"*{";}
	tag +=stDlg->ui.TitlelineEdit->text();
	tag +=QString("}\n");
	InsertTag(tag,0,1);
	//UpdateStructure();
	}
}

void Texmaker::InsertImage()
{
if ( !currentEditorView() )	return;
QString tag;
QString currentDir=QDir::homePath();
QString finame;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
QFileInfo fi(finame);
if (!finame.startsWith("untitled")) currentDir=fi.absolutePath();
QDir rootdir=fi.dir();
GraphicFileChooser *sfDlg = new GraphicFileChooser(this,tr("Select an image File"));
sfDlg->setFilter("Graphic files (*.eps *.pdf *.png);;All files (*.*)");
sfDlg->setDir(currentDir);
if (sfDlg->exec() )
	{
	QString fn=sfDlg->fileName();
	QFileInfo fi(rootdir.relativeFilePath(fn));
	if (!sfDlg->ui.moreButton->isChecked()) InsertTag("\\includegraphics[scale=1]{"+fi.filePath()+"} ",26,0);
	else
	  {
	  tag = "\\begin{figure}["+sfDlg->ui.lineEditPlacement->text()+"]\n";
	  if(sfDlg->ui.comboBoxCaption->currentIndex()==0) tag+="\\caption{"+sfDlg->ui.lineEditCaption->text()+"}\n";
	  tag+="\\includegraphics[scale=1]{"+fi.filePath()+"}\n";
	  if(sfDlg->ui.comboBoxCaption->currentIndex()==1) tag+="\\caption{"+sfDlg->ui.lineEditCaption->text()+"}\n";
	  tag+="\\end{figure}\n";
	  InsertTag(tag,0,4);
	  }
	}
}

void Texmaker::InsertInclude()
{
if ( !currentEditorView() )	return;
QString currentDir=QDir::homePath();
QString finame;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
QFileInfo fi(finame);
if (!finame.startsWith("untitled")) currentDir=fi.absolutePath();
QDir rootdir=fi.dir();
FileChooser *sfDlg = new FileChooser(this,tr("Select a File"));
sfDlg->setFilter("TeX files (*.tex);;All files (*.*)");
sfDlg->setDir(currentDir);
if (sfDlg->exec() )
	{
	QString fn=sfDlg->fileName();
	QFileInfo fi(rootdir.relativeFilePath(fn));
	QString suff=fi.suffix();
	QString name=fi.filePath();
	name=name.left(name.length()-suff.length()-1);
	InsertTag("\\include{"+name+"}",9,0);
	}
//UpdateStructure();
}

void Texmaker::InsertInput()
{
if ( !currentEditorView() )	return;
QString currentDir=QDir::homePath();
QString finame;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
QFileInfo fi(finame);
if (!finame.startsWith("untitled")) currentDir=fi.absolutePath();
QDir rootdir=fi.dir();
FileChooser *sfDlg = new FileChooser(this,tr("Select a File"));
sfDlg->setFilter("TeX files (*.tex);;All files (*.*)");
sfDlg->setDir(currentDir);
if (sfDlg->exec() )
	{
	QString fn=sfDlg->fileName();
	QFileInfo fi(rootdir.relativeFilePath(fn));
	QString suff=fi.suffix();
	QString name=fi.filePath();
	name=name.left(name.length()-suff.length()-1);
	InsertTag("\\input{"+name+"}",7,0);
	}
//UpdateStructure();
}

void Texmaker::QuickTabular()
{
if ( !currentEditorView() )	return;
QString al="";
QString vs="";
QString hs="";
QString tag;
TabDialog *quickDlg = new TabDialog(this,"Tabular");
QTableWidgetItem *item=new QTableWidgetItem();
if ( quickDlg->exec() )
	{
	int y = quickDlg->ui.spinBoxRows->value();
	int x = quickDlg->ui.spinBoxColumns->value();
	if  ((quickDlg->ui.comboSeparator->currentIndex())==0) vs=QString("|");
	if  ((quickDlg->ui.comboSeparator->currentIndex())==1) vs=QString("||");
	if  ((quickDlg->ui.comboSeparator->currentIndex())==2) vs=QString("");
	if  ((quickDlg->ui.comboSeparator->currentIndex())==3) vs=QString("@{}");
	tag = QString("\\begin{tabular}{")+vs;
	if  ((quickDlg->ui.comboAlignment->currentIndex())==0) al=QString("c")+vs;
	if  ((quickDlg->ui.comboAlignment->currentIndex())==1) al=QString("l")+vs;
	if  ((quickDlg->ui.comboAlignment->currentIndex())==2) al=QString("r")+vs;
	if  ((quickDlg->ui.comboAlignment->currentIndex())==3) al=QString("p{}")+vs;
	if  ((quickDlg->ui.comboAlignment->currentIndex())==4) al=QString(">{\\centering\\arraybackslash}p{}")+vs;
	if  ((quickDlg->ui.comboAlignment->currentIndex())==5) al=QString(">{\\raggedleft\\arraybackslash}p{}")+vs;
	if (quickDlg->ui.checkBox->isChecked()) 
	  {
	  hs=QString("\\hline ");
	  if (quickDlg->ui.checkBoxMargin->isChecked()) hs+="\\rule[-2ex]{0pt}{5.5ex} ";
	  }
	for ( int j=0;j<x;j++) {tag +=al;}
	tag +=QString("}\n");
	for ( int i=0;i<y;i++) 
		{
		tag +=hs;
		for ( int j=0;j<x-1;j++) 
			{
			item =quickDlg->ui.tableWidget->item(i,j);
			if (item) tag +=item->text()+ QString(" & ");
			else tag +=QString(0x2022)+QString(" & ");
			}
		item =quickDlg->ui.tableWidget->item(i,x-1);
		if (item) tag +=item->text()+ QString(" \\\\ \n");
		else tag +=QString(0x2022)+QString(" \\\\ \n");
		}
	if (quickDlg->ui.checkBox->isChecked()) tag +=QString("\\hline \n\\end{tabular} ");
	else tag +=QString("\\end{tabular} ");
	InsertTag(tag,0,0);
	}

}

void Texmaker::QuickArray()
{
if ( !currentEditorView() )	return;
QString al;
ArrayDialog *arrayDlg = new ArrayDialog(this,"Array");
QTableWidgetItem *item=new QTableWidgetItem();
if ( arrayDlg->exec() ) 
	{
	int y = arrayDlg->ui.spinBoxRows->value();
	int x = arrayDlg->ui.spinBoxColumns->value();
	QString env=arrayDlg->ui.comboEnvironment->currentText();
	QString tag = QString("\\begin{")+env+"}";
	if (env=="array")
		{
		tag+="{";
		if  ((arrayDlg->ui.comboAlignment->currentIndex())==0) al=QString("c");
		if  ((arrayDlg->ui.comboAlignment->currentIndex())==1) al=QString("l");
		if  ((arrayDlg->ui.comboAlignment->currentIndex())==2) al=QString("r");
		for ( int j=0;j<x;j++) {tag +=al;} tag+="}";
		}
	tag +=QString("\n");
	for ( int i=0;i<y-1;i++) 
		{
		for ( int j=0;j<x-1;j++) 
			{
			item =arrayDlg->ui.tableWidget->item(i,j);
			if (item) tag +=item->text()+ QString(" & ");
			else tag +=QString(0x2022)+QString(" & ");
			}
		item =arrayDlg->ui.tableWidget->item(i,x-1);
		if (item) tag +=item->text()+ QString(" \\\\ \n");
		else tag +=QString(0x2022)+QString(" \\\\ \n");
		}
	for ( int j=0;j<x-1;j++) 
		{
		item =arrayDlg->ui.tableWidget->item(y-1,j);
		if (item) tag +=item->text()+ QString(" & ");
		else tag +=QString(0x2022)+QString(" & ");
		}
	item =arrayDlg->ui.tableWidget->item(y-1,x-1);
	if (item) tag +=item->text()+ QString("\n\\end{")+env+"} ";
	else tag +=QString(0x2022)+QString("\n\\end{")+env+"} ";
	InsertTag(tag,0,0);
	}
}

void Texmaker::QuickTabbing()
{
if ( !currentEditorView() ) return;
TabbingDialog *tabDlg = new TabbingDialog(this,"Tabbing");
if ( tabDlg->exec() )
	{
	int	x = tabDlg->ui.spinBoxColumns->value();
	int	y = tabDlg->ui.spinBoxRows->value();
	QString s=tabDlg->ui.lineEdit->text();
	QString tag = QString("\\begin{tabbing}\n");
	for ( int j=1;j<x;j++) {tag +="\\hspace{"+s+"}\\=";}
	tag+="\\kill\n";
	for ( int i=0;i<y-1;i++)
		{
		for ( int j=1;j<x;j++) {tag +=" \\> ";}
		tag+="\\\\ \n";
		}
	for ( int j=1;j<x;j++) {tag +=" \\> ";}
	tag += QString("\n\\end{tabbing} ");
	InsertTag(tag,0,2);
	}
}

void Texmaker::QuickLetter()
{
if ( !currentEditorView() )	return;
QString tag=QString("\\documentclass[");
LetterDialog *ltDlg = new LetterDialog(this,"Letter");
if ( ltDlg->exec() )
	{
	tag+=ltDlg->ui.comboBoxPt->currentText()+QString(",");
	tag+=ltDlg->ui.comboBoxPaper->currentText()+QString("]{letter}");
	tag+=QString("\n");
	if (ltDlg->ui.comboBoxEncoding->currentText()!="NONE") tag+=QString("\\usepackage[")+ltDlg->ui.comboBoxEncoding->currentText()+QString("]{inputenc}");
	if (ltDlg->ui.comboBoxEncoding->currentText().startsWith("utf8")) tag+=QString(" \\usepackage{ucs}");
	tag+=QString("\n");
	if (ltDlg->ui.checkBox->isChecked()) tag+=QString("\\usepackage{amsmath}\n\\usepackage{amsfonts}\n\\usepackage{amssymb}\n");
	tag+="\\address{your name and address} \n";
	tag+="\\signature{your signature} \n";
	tag+="\\begin{document} \n";
	tag+="\\begin{letter}{name and address of the recipient} \n";
	tag+="\\opening{saying hello} \n \n";
	tag+="write your letter here \n \n";
	tag+="\\closing{saying goodbye} \n";
	tag+="%\\cc{Cclist} \n";
	tag+="%\\ps{adding a postscript} \n";
	tag+="%\\encl{list of enclosed material} \n";
	tag+="\\end{letter} \n";
	tag+="\\end{document}";
	if (ltDlg->ui.checkBox->isChecked()) {InsertTag(tag,9,5);}
	else {InsertTag(tag,9,2);}
	}
}

void Texmaker::QuickDocument()
{
QString opt="";
int li=3;
int f;
if ( !currentEditorView() ) return;
QString tag=QString("\\documentclass[");
QuickDocumentDialog *startDlg = new QuickDocumentDialog(this,"Quick Start");
startDlg->otherClassList=userClassList;
startDlg->otherPaperList=userPaperList;
startDlg->otherEncodingList=userEncodingList;
startDlg->otherOptionsList=userOptionsList;
startDlg->Init();
f=startDlg->ui.comboBoxClass->findText(document_class,Qt::MatchExactly | Qt::MatchCaseSensitive);
startDlg->ui.comboBoxClass->setCurrentIndex(f);
f=startDlg->ui.comboBoxSize->findText(typeface_size,Qt::MatchExactly | Qt::MatchCaseSensitive);
startDlg->ui.comboBoxSize->setCurrentIndex(f);
f=startDlg->ui.comboBoxPaper->findText(paper_size,Qt::MatchExactly | Qt::MatchCaseSensitive);
startDlg->ui.comboBoxPaper->setCurrentIndex(f);
f=startDlg->ui.comboBoxEncoding->findText(document_encoding,Qt::MatchExactly | Qt::MatchCaseSensitive);
startDlg->ui.comboBoxEncoding->setCurrentIndex(f);
startDlg->ui.checkBoxAMS->setChecked(ams_packages);
startDlg->ui.checkBoxIDX->setChecked(makeidx_package);
startDlg->ui.lineEditAuthor->setText(author);
if ( startDlg->exec() )
	{
	tag+=startDlg->ui.comboBoxSize->currentText()+QString(",");
	tag+=startDlg->ui.comboBoxPaper->currentText();
	QList<QListWidgetItem *> selectedItems=startDlg->ui.listWidgetOptions->selectedItems();
	for (int i = 0; i < selectedItems.size(); ++i) 
		{
		if ( selectedItems.at(i)) opt+=QString(",")+selectedItems.at(i)->text();
		}
	tag+=opt+QString("]{");
	tag+=startDlg->ui.comboBoxClass->currentText()+QString("}");
	tag+=QString("\n");
	if (startDlg->ui.comboBoxEncoding->currentText()!="NONE") tag+=QString("\\usepackage[")+startDlg->ui.comboBoxEncoding->currentText()+QString("]{inputenc}");
	tag+=QString("\n");
	if (startDlg->ui.comboBoxEncoding->currentText().startsWith("utf8"))
		{
		tag+=QString("\\usepackage{ucs}\n");
		li=li+1;
		}
	if (startDlg->ui.checkBoxAMS->isChecked())
		{
		tag+=QString("\\usepackage{amsmath}\n\\usepackage{amsfonts}\n\\usepackage{amssymb}\n");
		li=li+3;
		}
	if (startDlg->ui.checkBoxIDX->isChecked())
		{
		tag+=QString("\\usepackage{makeidx}\n");
		li=li+1;
		}
	if (startDlg->ui.lineEditAuthor->text()!="")
		{
		tag+="\\author{"+startDlg->ui.lineEditAuthor->text()+"}\n";
		li=li+1;
		}
	if (startDlg->ui.lineEditTitle->text()!="")
		{
		tag+="\\title{"+startDlg->ui.lineEditTitle->text()+"}\n";
		li=li+1;
		}
	tag+=QString("\\begin{document}\n\n\\end{document}");
	InsertTag(tag,0,li);
	document_class=startDlg->ui.comboBoxClass->currentText();
	typeface_size=startDlg->ui.comboBoxSize->currentText();
	paper_size=startDlg->ui.comboBoxPaper->currentText();
	document_encoding=startDlg->ui.comboBoxEncoding->currentText();
	ams_packages=startDlg->ui.checkBoxAMS->isChecked();
	makeidx_package=startDlg->ui.checkBoxIDX->isChecked();
	author=startDlg->ui.lineEditAuthor->text();
	userClassList=startDlg->otherClassList;
	userPaperList=startDlg->otherPaperList;
	userEncodingList=startDlg->otherEncodingList;
	userOptionsList=startDlg->otherOptionsList;
	}
}

void Texmaker::InsertBib1()
{
QString tag = QString("@Article{,\n");
tag+="author = {},\n";
tag+="title = {},\n";
tag+="journal = {},\n";
tag+="year = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTvolume = {},\n";
tag+="OPTnumber = {},\n";
tag+="OPTpages = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,9,0);
OutputTextEdit->insertLine("Bib fields - Article in Journal");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib2()
{
QString tag = QString("@InProceedings{,\n");
tag+="author = {},\n";
tag+="title = {},\n";
tag+="booktitle = {},\n";
tag+="OPTcrossref = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTpages = {},\n";
tag+="OPTyear = {},\n";
tag+="OPTeditor = {},\n";
tag+="OPTvolume = {},\n";
tag+="OPTnumber = {},\n";
tag+="OPTseries = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTorganization = {},\n";
tag+="OPTpublisher = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,15,0);
OutputTextEdit->insertLine("Bib fields - Article in Conference Proceedings");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib3()
{
QString tag = QString("@InCollection{,\n");
tag+="author = {},\n";
tag+="title = {},\n";
tag+="booktitle = {},\n";
tag+="OPTcrossref = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTpages = {},\n";
tag+="OPTpublisher = {},\n";
tag+="OPTyear = {},\n";
tag+="OPTeditor = {},\n";
tag+="OPTvolume = {},\n";
tag+="OPTnumber = {},\n";
tag+="OPTseries = {},\n";
tag+="OPTtype = {},\n";
tag+="OPTchapter = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTedition = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,14,0);
OutputTextEdit->insertLine("Bib fields - Article in a Collection");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib4()
{
QString tag = QString("@InBook{,\n");
tag+="ALTauthor = {},\n";
tag+="ALTeditor = {},\n";
tag+="title = {},\n";
tag+="chapter = {},\n";
tag+="publisher = {},\n";
tag+="year = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTvolume = {},\n";
tag+="OPTnumber = {},\n";
tag+="OPTseries = {},\n";
tag+="OPTtype = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTedition = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTpages = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,8,0);
OutputTextEdit->insertLine("Bib fields - Chapter or Pages in a Book");
OutputTextEdit->insertLine( "ALT.... : you have the choice between these two fields");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib5()
{
QString tag = QString("@Proceedings{,\n");
tag+="title = {},\n";
tag+="year = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTeditor = {},\n";
tag+="OPTvolume = {},\n";
tag+="OPTnumber = {},\n";
tag+="OPTseries = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTorganization = {},\n";
tag+="OPTpublisher = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,13,0);
OutputTextEdit->insertLine("Bib fields - Conference Proceedings");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib6()
{
QString tag = QString("@Book{,\n");
tag+="ALTauthor = {},\n";
tag+="ALTeditor = {},\n";
tag+="title = {},\n";
tag+="publisher = {},\n";
tag+="year = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTvolume = {},\n";
tag+="OPTnumber = {},\n";
tag+="OPTseries = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTedition = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,6,0);
OutputTextEdit->insertLine("Bib fields - Book");
OutputTextEdit->insertLine( "ALT.... : you have the choice between these two fields");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib7()
{
QString tag = QString("@Booklet{,\n");
tag+="title = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTauthor = {},\n";
tag+="OPThowpublished = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTyear = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,9,0);
OutputTextEdit->insertLine("Bib fields - Booklet");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib8()
{
QString tag = QString("@PhdThesis{,\n");
tag+="author = {},\n";
tag+="title = {},\n";
tag+="school = {},\n";
tag+="year = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTtype = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,11,0);
OutputTextEdit->insertLine("Bib fields - PhD. Thesis");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib9()
{
QString tag = QString("@MastersThesis{,\n");
tag+="author = {},\n";
tag+="title = {},\n";
tag+="school = {},\n";
tag+="year = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTtype = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,15,0);
OutputTextEdit->insertLine("Bib fields - Master's Thesis");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib10()
{
QString tag = QString("@TechReport{,\n");
tag+="author = {},\n";
tag+="title = {},\n";
tag+="institution = {},\n";
tag+="year = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTtype = {},\n";
tag+="OPTnumber = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,12,0);
OutputTextEdit->insertLine("Bib fields - Technical Report");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib11()
{
QString tag = QString("@Manual{,\n");
tag+="title = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTauthor = {},\n";
tag+="OPTorganization = {},\n";
tag+="OPTaddress = {},\n";
tag+="OPTedition = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTyear = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,8,0);
OutputTextEdit->insertLine("Bib fields - Technical Manual");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib12()
{
QString tag = QString("@Unpublished{,\n");
tag+="author = {},\n";
tag+="title = {},\n";
tag+="note = {},\n";
tag+="OPTkey = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTyear = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,13,0);
OutputTextEdit->insertLine("Bib fields - Unpublished");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}
void Texmaker::InsertBib13()
{
QString tag = QString("@Misc{,\n");
tag+="OPTkey = {},\n";
tag+="OPTauthor = {},\n";
tag+="OPTtitle = {},\n";
tag+="OPThowpublished = {},\n";
tag+="OPTmonth = {},\n";
tag+="OPTyear = {},\n";
tag+="OPTnote = {},\n";
tag+="OPTannote = {}\n";
tag+="}\n";
InsertTag(tag,6,0);
OutputTextEdit->insertLine("Bib fields - Miscellaneous");
OutputTextEdit->insertLine( "OPT.... : optional fields (use the 'Clean' command to remove them)");
}

void Texmaker::CleanBib()
{
if ( !currentEditorView() ) return;
currentEditorView()->editor->removeOptAlt();
}

void Texmaker::InsertUserTag(QString Entity)
{
if ( !currentEditorView() )	return;
QTextCursor cur=currentEditorView()->editor->textCursor();
bool selection=cur.hasSelection();
if (selection) currentEditorView()->editor->cut();
int pos=cur.position();
Entity.replace("@",QString(0x2022));
currentEditorView()->editor->insertPlainText(Entity);
cur.setPosition(pos,QTextCursor::MoveAnchor);
int dx=Entity.length();
if (Entity.contains(QString(0x2022))) 
    {
    currentEditorView()->editor->setTextCursor(cur);
    currentEditorView()->editor->search(QString(0x2022) ,true,true,true,true);
    if (selection) currentEditorView()->editor->paste();
    }
else
    {
    if (dx>0) cur.movePosition(QTextCursor::NextCharacter,QTextCursor::MoveAnchor,dx);
    currentEditorView()->editor->setTextCursor(cur);
    }
currentEditorView()->editor->setFocus();
OutputTextEdit->clear();
OutputTableWidget->hide();
logpresent=false;
}

void Texmaker::InsertUserTag1()
{
if (UserMenuTag[0].left(1)=="%")
	{
	QString t=UserMenuTag[0];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[0]);
	}
}

void Texmaker::InsertUserTag2()
{
if (UserMenuTag[1].left(1)=="%")
	{
	QString t=UserMenuTag[1];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[1]);
	}
}

void Texmaker::InsertUserTag3()
{
if (UserMenuTag[2].left(1)=="%")
	{
	QString t=UserMenuTag[2];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[2]);
	}
}

void Texmaker::InsertUserTag4()
{
if (UserMenuTag[3].left(1)=="%")
	{
	QString t=UserMenuTag[3];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[3]);
	}
}

void Texmaker::InsertUserTag5()
{
if (UserMenuTag[4].left(1)=="%")
	{
	QString t=UserMenuTag[4];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[4]);
	}
}

void Texmaker::InsertUserTag6()
{
if (UserMenuTag[5].left(1)=="%")
	{
	QString t=UserMenuTag[5];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[5]);
	}
}

void Texmaker::InsertUserTag7()
{
if (UserMenuTag[6].left(1)=="%")
	{
	QString t=UserMenuTag[6];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[6]);
	}
}

void Texmaker::InsertUserTag8()
{
if (UserMenuTag[7].left(1)=="%")
	{
	QString t=UserMenuTag[7];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[7]);
	}
}

void Texmaker::InsertUserTag9()
{
if (UserMenuTag[8].left(1)=="%")
	{
	QString t=UserMenuTag[8];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[8]);
	}
}

void Texmaker::InsertUserTag10()
{
if (UserMenuTag[9].left(1)=="%")
	{
	QString t=UserMenuTag[9];
	t=t.remove(0,1);
	QString s="\\begin{"+t+"}\n"+QString(0x2022)+"\n\\end{"+t+"}\n";
	InsertUserTag(s);
	}
else
	{
	InsertUserTag(UserMenuTag[9]);
	}
}

void Texmaker::EditUserMenu()
{
QAction *Act;
UserMenuDialog *umDlg = new UserMenuDialog(this,tr("Edit User &Tags"));
for ( int i = 0; i <= 9; i++ )
    {
    umDlg->Name[i]=UserMenuName[i];
    umDlg->Tag[i]=UserMenuTag[i];
    }
umDlg->init();
if ( umDlg->exec() )
	{
	for ( int i = 0; i <= 9; i++ )
		{
		UserMenuName[i]=umDlg->Name[i];
		UserMenuTag[i]=umDlg->Tag[i];
		}
	user11Menu->clear();
	Act = new QAction("1: "+UserMenuName[0], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F1);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag1()));
	user11Menu->addAction(Act);
	Act = new QAction("2: "+UserMenuName[1], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F2);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag2()));
	user11Menu->addAction(Act);
	Act = new QAction("3: "+UserMenuName[2], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F3);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag3()));
	user11Menu->addAction(Act);
	Act = new QAction("4: "+UserMenuName[3], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F4);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag4()));
	user11Menu->addAction(Act);
	Act = new QAction("5: "+UserMenuName[4], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F5);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag5()));
	user11Menu->addAction(Act);
	Act = new QAction("6: "+UserMenuName[5], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F6);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag6()));
	user11Menu->addAction(Act);
	Act = new QAction("7: "+UserMenuName[6], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F7);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag7()));
	user11Menu->addAction(Act);
	Act = new QAction("8: "+UserMenuName[7], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F8);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag8()));
	user11Menu->addAction(Act);
	Act = new QAction("9: "+UserMenuName[8], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F9);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag9()));
	user11Menu->addAction(Act);
	Act = new QAction("10: "+UserMenuName[9], this);
	Act->setShortcut(Qt::SHIFT+Qt::Key_F10);
	connect(Act, SIGNAL(triggered()), this, SLOT(InsertUserTag10()));
	user11Menu->addAction(Act);
	user11Menu->addSeparator();
	Act = new QAction(tr("Edit User &Tags"), this);
	connect(Act, SIGNAL(triggered()), this, SLOT(EditUserMenu()));
	user11Menu->addAction(Act);
	}
}

void Texmaker::SectionCommand(const QString& text)
{
if ( !currentEditorView() ) return;
InsertStructFromString("\\"+text);
}

void Texmaker::OtherCommand(const QString& text)
{
if ( !currentEditorView() ) return;
if (text=="label") 
	{
	InsertFromString("\\label{} /7/0");
	return;
	}
if (text=="ref") 
	{
	InsertRef();
	return;
	}
if (text=="pageref") 
	{
	InsertPageRef();
	return;
	}
if (text=="index") 
	{
	InsertFromString("\\index{}/7/0");
	return;
	}
if (text=="cite") 
	{
	InsertCite();
	//InsertFromString("\\cite{}/6/0");
	return;
	}
if (text=="footnote") 
	{
	InsertFromString("\\footnote{}/10/0");
	return;
	}
}

void Texmaker::InsertCite()
{
//UpdateStructure();
QString tag="";
RefDialog *refDlg = new RefDialog(this,"Bibliography Items");
refDlg->ui.comboBox->addItems(bibitem);
refDlg->ui.label->setText("Items");
if (!bibitem.isEmpty() && refDlg->exec() )
	{
	tag="\\cite{"+refDlg->ui.comboBox->currentText()+"}";
	InsertTag(tag,tag.length(),0);
	}
else InsertTag("\\cite{}",6,0);
OutputTextEdit->insertLine( "\\cite{bibiliography item}");
}

void Texmaker::InsertRef()
{
//UpdateStructure();
QString tag="";
RefDialog *refDlg = new RefDialog(this,"Labels");
refDlg->ui.comboBox->addItems(labelitem);
if (!labelitem.isEmpty() && refDlg->exec() )
	{
	tag="\\ref{"+refDlg->ui.comboBox->currentText()+"}";
	InsertTag(tag,tag.length(),0);
	}
else InsertTag("\\ref{}",5,0);
OutputTextEdit->insertLine( "\\ref{key}");
}

void Texmaker::InsertPageRef()
{
//UpdateStructure();
QString tag="";
RefDialog *refDlg = new RefDialog(this,"Labels");
refDlg->ui.comboBox->addItems(labelitem);
if (!labelitem.isEmpty() && refDlg->exec() )
	{
	tag="\\pageref{"+refDlg->ui.comboBox->currentText()+"}";
	InsertTag(tag,tag.length(),0);
	}
else InsertTag("\\pageref{}",9,0);
OutputTextEdit->insertLine( "\\pageref{key}");
}

void Texmaker::SizeCommand(const QString& text)
{
if ( !currentEditorView() ) return;
if (text=="tiny")
	{
	InsertWithSelectionFromString("\\begin{tiny}\n/\n\\end{tiny}/12/0");
	return;
	}
if (text=="scriptsize")
	{
	InsertWithSelectionFromString("\\begin{scriptsize}\n/\n\\end{scriptsize}/18/0");
	return;
	}
if (text=="footnotesize")
	{
	InsertWithSelectionFromString("\\begin{footnotesize}\n/\n\\end{footnotesize}/20/0");
	return;
	}
if (text=="small")
	{
	InsertWithSelectionFromString("\\begin{small}\n/\n\\end{small}/13/0");
	return;
	}
if (text=="normalsize")
	{
	InsertWithSelectionFromString("\\begin{normalsize}\n/\n\\end{normalsize}/18/0");
	return;
	}
if (text=="large")
	{
	InsertWithSelectionFromString("\\begin{large}\n/\n\\end{large}/13/0");
	return;
	}
if (text=="Large")
	{
	InsertWithSelectionFromString("\\begin{Large}\n/\n\\end{Large}/13/0");
	return;
	}
if (text=="LARGE")
	{
	InsertWithSelectionFromString("\\begin{LARGE}\n/\n\\end{LARGE}/13/0");
	return;
	}
if (text=="huge")
	{
	InsertWithSelectionFromString("\\begin{huge}\n/\n\\end{huge}/12/0");
	return;
	}
if (text=="Huge")
	{
	InsertWithSelectionFromString("\\begin{Huge}\n/\n\\end{Huge}/12/0");
	return;
	}
}

///////////////TOOLS////////////////////
void Texmaker::RunCommand(QString comd,bool waitendprocess)
{
QString finame;
QString commandline=comd;
QByteArray result;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
if ((singlemode && !currentEditorView()) || finame.startsWith("untitled") || finame=="")
	{
	QMessageBox::warning( this,tr("Error"),tr("Can't detect the file name"));
	return;
	}
if (!currentfileSaved()) 
  {
  ERRPROCESS=true;
  return;
  }

QFileInfo fi(finame);
QString basename=fi.completeBaseName();
commandline.replace("%","\""+basename+"\"");
int currentline=1;
if (currentEditorView() )
  {
  currentline=currentEditorView()->editor->linefromblock(currentEditorView()->editor->textCursor().block());
  }
commandline.replace("@",QString::number(currentline));

if (builtinpdfview && (comd==viewpdf_command))
  {
    if (largescreen)
      {
      if (pdfviewerWidget)
	{
	pdfviewerWidget->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
	StackedViewers->setCurrentWidget(pdfviewerWidget);
	//pdfviewerWidget->raise();
	pdfviewerWidget->show();
	if ( (pdflatex_command.contains("synctex=1")) || (latex_command.contains("synctex=1")) ) pdfviewerWidget->jumpToPdfFromSource(getName(),currentline);
	}
      else
	{
    //    pdfviewerWidget=new PdfViewer(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command, this);
	pdfviewerWidget=new PdfViewerWidget(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command,psize,StackedViewers);
	connect(pdfviewerWidget, SIGNAL(openDocAtLine(const QString&, int, bool)), this, SLOT(fileOpenAndGoto(const QString&, int, bool)));
	connect(pdfviewerWidget, SIGNAL(sendFocusToEditor()), this, SLOT(getFocusToEditor()));
	connect(pdfviewerWidget, SIGNAL(sendPaperSize(const QString&)), this, SLOT(setPrintPaperSize(const QString&)));
	StackedViewers->addWidget(pdfviewerWidget);
	//pdfviewerWidget->raise();
	pdfviewerWidget->show();
	pdfviewerWidget->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
	if ( (pdflatex_command.contains("synctex=1")) || (latex_command.contains("synctex=1")) ) pdfviewerWidget->jumpToPdfFromSource(getName(),currentline);
	}
      return;
      }
   else
    {
    if (pdfviewerWindow)
      {
      pdfviewerWindow->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
      pdfviewerWindow->raise();
      pdfviewerWindow->show();
      if ( (pdflatex_command.contains("synctex=1")) || (latex_command.contains("synctex=1")) ) pdfviewerWindow->jumpToPdfFromSource(getName(),currentline);
      }
    else
      {
  //    pdfviewerWindow=new PdfViewer(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command, this);
      pdfviewerWindow=new PdfViewer(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command,psize,0);
      connect(pdfviewerWindow, SIGNAL(openDocAtLine(const QString&, int, bool)), this, SLOT(fileOpenAndGoto(const QString&, int, bool)));
      connect(pdfviewerWindow, SIGNAL(sendFocusToEditor()), this, SLOT(getFocusToEditor()));
      connect(pdfviewerWindow, SIGNAL(sendPaperSize(const QString&)), this, SLOT(setPrintPaperSize(const QString&)));
      pdfviewerWindow->raise();
      pdfviewerWindow->show();
      if ( (pdflatex_command.contains("synctex=1")) || (latex_command.contains("synctex=1")) ) pdfviewerWindow->jumpToPdfFromSource(getName(),currentline);
      }
    return;     
    }
  }
else if (comd==asymptote_command)
  {
  QDir currentdir(fi.absolutePath());
  foreach (QFileInfo asyFileInfo, currentdir.entryInfoList(QStringList(basename+"*.asy"),QDir::Files | QDir::Readable, QDir::Name | QDir::IgnoreCase)) 
    {
    QString asyName = asyFileInfo.absolutePath()+"/"+asyFileInfo.completeBaseName();
    AsyFile(asyName);
    }
  return;
  }
proc = new QProcess( this );
proc->setWorkingDirectory(fi.absolutePath());
proc->setProperty("command",commandline);

//****
#ifdef Q_WS_MACX
#if (QT_VERSION >= 0x0406)
QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
env.insert("PATH", env.value("PATH") + ":/usr/bin:/usr/sbin:/sbin:/usr/local/bin:/usr/texbin:/sw/bin");
proc->setProcessEnvironment(env);
#endif
#endif
#ifdef Q_WS_WIN
#if (QT_VERSION >= 0x0406)
QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
QFileInfo figs(ghostscript_command);
env.insert("PATH", env.value("PATH") + ":"+figs.absolutePath());
proc->setProcessEnvironment(env);
#endif
#endif
//****
connect(proc, SIGNAL(readyReadStandardError()),this, SLOT(readFromStderr()));
//connect(proc, SIGNAL(readyReadStandardOutput()),this, SLOT(readFromStdoutput()));
if (checkViewerInstance && ((comd==viewdvi_command) || (comd==viewps_command) || (comd==viewpdf_command)))
  {
  connect(proc, SIGNAL(finished(int)),this, SLOT(SlotEndViewerProcess(int)));
  if (singleviewerinstance)
    {
    if (listViewerCommands.contains(commandline)) return;
    else listViewerCommands.append(commandline);
    }
  else
    {
    listViewerCommands.clear();
    }    
  }
else 
{
  if (((comd!=viewdvi_command) && (comd!=viewps_command) && (comd!=viewpdf_command))) disableToolsActions();
  connect(proc, SIGNAL(finished(int)),this, SLOT(SlotEndProcess(int)));
}
OutputTextEdit->clear();
OutputTableWidget->hide();
//OutputTextEdit->insertLine(commandline+"\n");
proc->start(commandline);
if (!proc->waitForStarted(1000)) 
	{
	ERRPROCESS=true;
	OutputTextEdit->insertLine("Error : could not start the command\n");
	checkViewerInstance=false;
	return;
	}
else OutputTextEdit->insertLine("Process started\n");
FINPROCESS=false;
STOPPROCESS=false;
if (waitendprocess)
	{
	StopAct->setEnabled(true);
	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	while (!FINPROCESS) 
		{
		qApp->instance()->processEvents(QEventLoop::AllEvents);
		if (STOPPROCESS && proc && proc->state()==QProcess::Running) 
		  {
		  proc->kill();
		  FINPROCESS=true;
		  ERRPROCESS=true;
		  }
		}
	QApplication::restoreOverrideCursor();
	enableToolsActions();
	StopAct->setEnabled(false);
	}
}

void Texmaker::readFromStderr()
{
QByteArray result=proc->readAllStandardError();
QString t=QString(result);
t=t.simplified();
if (!t.isEmpty()) OutputTextEdit->insertLine(t+"\n");
}

void Texmaker::stopProcess()
{
STOPPROCESS=true;  
}
// void Texmaker::readFromStdoutput()
// {
// QByteArray result=proc->readAllStandardOutput ();
// OutputTextEdit->insertLine(QString(result)+"\n");
// }

void Texmaker::SlotEndProcess(int err)
{
FINPROCESS=true;
enableToolsActions();
QString result=((err) ? "Process exited with error(s)" : "Process exited normally");
if (err) {ERRPROCESS=true;checkViewerInstance=false;}
OutputTextEdit->insertLine(result);
//stat2->setText(QString(" %1 ").arg(tr("Ready")));
}

void Texmaker::SlotEndViewerProcess(int err)
{
QString commandline = static_cast<QProcess*>(sender())->property("command").toString();
if ((singleviewerinstance) && (listViewerCommands.contains(commandline))) listViewerCommands.removeAll(commandline);
FINPROCESS=true;
enableToolsActions();
QString result=((err) ? "Process exited with error(s)" : "Process exited normally");
if (err) {ERRPROCESS=true;checkViewerInstance=false;}
OutputTextEdit->insertLine(result);
//stat2->setText(QString(" %1 ").arg(tr("Ready")));
}

void Texmaker::QuickBuild()
{
//stat2->setText(QString(" %1 ").arg(tr("Quick Build")));
ERRPROCESS=false;
checkViewerInstance=true;
switch (quickmode)
 {
  case 1:
    {
    //stat2->setText(QString(" %1 ").arg("Latex"));
    RunCommand(latex_command,true);
    if (ERRPROCESS && !LogExists()) 
        {
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	checkViewerInstance=false;
	return;
	}
    LoadLog();
    if (showoutputview) ViewLog();
    if (NoLatexErrors()) 
    	{
	//stat2->setText(QString(" %1 ").arg("Dvips"));
    	if (!ERRPROCESS) RunCommand(dvips_command,true);
        else {checkViewerInstance=false;return;}
	if (!ERRPROCESS) ViewPS();
        else {checkViewerInstance=false;return;}
	}
    else {NextError();}
    }break;
  case 2:
    {
    //stat2->setText(QString(" %1 ").arg("Latex"));
    RunCommand(latex_command,true);
    if (ERRPROCESS && !LogExists()) 
        {
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	checkViewerInstance=false;
	return;
	}
    LoadLog();
    if (showoutputview) ViewLog();
    if (NoLatexErrors()) 
    	{
	if (!ERRPROCESS) ViewDvi();
        else {checkViewerInstance=false;return;}
	}
    else {NextError();}
    }break;
 case 3:
    {
    //stat2->setText(QString(" %1 ").arg("Pdf Latex"));
    RunCommand(pdflatex_command,true);
    if (ERRPROCESS && !LogExists()) 
        {
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	checkViewerInstance=false;
	return;
	}
    LoadLog();
    if (showoutputview) ViewLog();
    if (NoLatexErrors()) 
    	{
	if (!ERRPROCESS) ViewPDF();
        else {checkViewerInstance=false;return;}
	}
    else {NextError();}
    }break;
 case 4:
    {
    //stat2->setText(QString(" %1 ").arg("Latex"));
    RunCommand(latex_command,true);
    if (ERRPROCESS && !LogExists()) 
        {
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	checkViewerInstance=false;
	return;
	}
    LoadLog();
    if (showoutputview) ViewLog();
    if (NoLatexErrors()) 
    	{
	//stat2->setText(QString(" %1 ").arg("Dvi to Pdf"));
	if (!ERRPROCESS) RunCommand(dvipdf_command,true);
        else {checkViewerInstance=false;return;}
	if (!ERRPROCESS) ViewPDF();
        else {checkViewerInstance=false;return;}
	}
    else {NextError();}
    }break;
 case 5:
    {
    //stat2->setText(QString(" %1 ").arg("Latex"));
    RunCommand(latex_command,true);
    if (ERRPROCESS && !LogExists()) 
        {
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	checkViewerInstance=false;
	return;
	}
    LoadLog();
    if (showoutputview) ViewLog();
    if (NoLatexErrors()) 
    	{
	//stat2->setText(QString(" %1 ").arg("Dvips"));
	if (!ERRPROCESS) RunCommand(dvips_command,true);
        else {checkViewerInstance=false;return;}
	//stat2->setText(QString(" %1 ").arg("Ps to Pdf"));
	if (!ERRPROCESS) RunCommand(ps2pdf_command,true);
        else {checkViewerInstance=false;return;}
	if (!ERRPROCESS) ViewPDF();
	}
    else {NextError();}
    }break;
 case 6:
    {
    QStringList commandList=userquick_command.split("|");
    for (int i = 0; i < commandList.size(); ++i) 
	{
	if ((!ERRPROCESS)&&(!commandList.at(i).isEmpty())) RunCommand(commandList.at(i),true);
        else {checkViewerInstance=false;return;}
	}
    }break;
  case 7:
    {
    //stat2->setText(QString(" %1 ").arg("Latex"));
    RunCommand(latex_command,true);
    if (ERRPROCESS && !LogExists()) 
        {
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	checkViewerInstance=false;
	return;
	}
    LoadLog();
    if (showoutputview) ViewLog();
    if (NoLatexErrors()) 
    	{
	//stat2->setText(QString(" %1 ").arg("Asymptote"));
    	if (!ERRPROCESS) RunCommand(asymptote_command,true);
        else {checkViewerInstance=false;return;}
	if (!ERRPROCESS) 
	    {
	    //stat2->setText(QString(" %1 ").arg("Latex"));
	    RunCommand(latex_command,true);
	    if (ERRPROCESS && !LogExists()) 
		{
		QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
		checkViewerInstance=false;
		return;
		}
	    LoadLog();
	    if (showoutputview) ViewLog();
	    if (NoLatexErrors()) 
		{
		//stat2->setText(QString(" %1 ").arg("Dvips"));
		if (!ERRPROCESS) RunCommand(dvips_command,true);
		else {checkViewerInstance=false;return;}
		if (!ERRPROCESS) ViewPS();
		else {checkViewerInstance=false;return;}
		}
	    else {NextError();}
	    }
        else return;
	}
    else {NextError();}
    }break;
  case 8:
    {
    //stat2->setText(QString(" %1 ").arg("Pdf Latex"));
    RunCommand(pdflatex_command,true);
    if (ERRPROCESS && !LogExists()) 
        {
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	checkViewerInstance=false;
	return;
	}
    LoadLog();
    if (showoutputview) ViewLog();
    if (NoLatexErrors()) 
    	{
	//stat2->setText(QString(" %1 ").arg("Asymptote"));
    	if (!ERRPROCESS) RunCommand(asymptote_command,true);
        else {checkViewerInstance=false;return;}
	if (!ERRPROCESS) 
	    {
	    //stat2->setText(QString(" %1 ").arg("Pdf Latex"));
	    RunCommand(pdflatex_command,true);
	    if (ERRPROCESS && !LogExists()) 
		{
		QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
		checkViewerInstance=false;
		return;
		}
	    LoadLog();
	    if (showoutputview) ViewLog();
	    if (NoLatexErrors()) 
		{
		if (!ERRPROCESS) ViewPDF();
		else {checkViewerInstance=false;return;}
		}
	    else {NextError();}
	    }
        else return;
	}
    else {NextError();}
    }break;
 case 9:
    {
    //stat2->setText(QString(" %1 ").arg("LatexMk"));
    RunCommand(latexmk_command,true);
    if (ERRPROCESS && !LogExists()) 
        {
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	checkViewerInstance=false;
	return;
	}
    LoadLog();
    if (showoutputview) ViewLog();
    if (NoLatexErrors()) 
    	{
	if (!ERRPROCESS) ViewPDF();
        else {checkViewerInstance=false;return;}
	}
    else {NextError();}
    }break;
 }
checkViewerInstance=false;
if (NoLatexErrors() && showoutputview) ViewLog();
}

void Texmaker::Latex()
{
//stat2->setText(QString(" %1 ").arg("Latex"));
RunCommand(latex_command,false);
}

void Texmaker::ViewDvi()
{
//stat2->setText(QString(" %1 ").arg(tr("View Dvi file")));
RunCommand(viewdvi_command,false);
}

void Texmaker::DviToPS()
{
//stat2->setText(QString(" %1 ").arg("Dvips"));
RunCommand(dvips_command,false);
}

void Texmaker::ViewPS()
{
//stat2->setText(QString(" %1 ").arg(tr("View PS file")));
RunCommand(viewps_command,false);
}

void Texmaker::PDFLatex()
{
//stat2->setText(QString(" %1 ").arg("Pdf Latex"));
RunCommand(pdflatex_command,false);
}

void Texmaker::ViewPDF()
{
//stat2->setText(QString(" %1 ").arg(tr("View Pdf file")));
RunCommand(viewpdf_command,false);
}

void Texmaker::MakeBib()
{
//stat2->setText(QString(" %1 ").arg("Bibtex"));
RunCommand(bibtex_command,false);
}

void Texmaker::MakeIndex()
{
//stat2->setText(QString(" %1 ").arg("Make index"));
RunCommand(makeindex_command,false);
}

void Texmaker::PStoPDF()
{
//stat2->setText(QString(" %1 ").arg("Ps -> Pdf"));
RunCommand(ps2pdf_command,false);
}

void Texmaker::DVItoPDF()
{
//stat2->setText(QString(" %1 ").arg("Dvi -> Pdf"));
RunCommand(dvipdf_command,false);
}

void Texmaker::MetaPost()
{
//stat2->setText(QString(" %1 ").arg("Mpost"));
QString finame=getName();
QFileInfo fi(finame);
RunCommand(metapost_command+fi.completeBaseName()+"."+fi.suffix(),false);
}

void Texmaker::CleanAll()
{
QString finame,f;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
if ((singlemode && !currentEditorView()) || finame.startsWith("untitled") || finame=="")
	{
	QMessageBox::warning( this,tr("Error"),tr("Can't detect the file name"));
	return;
	}
fileSave();
QFileInfo fi(finame);
QString name=fi.absoluteFilePath();
QString ext=fi.suffix();
QString basename=name.left(name.length()-ext.length()-1);
QStringList extension=QString(".log,.aux,.dvi,.lof,.lot,.bit,.idx,.glo,.bbl,.ilg,.toc,.ind,.out,.synctex.gz,.blg").split(",");
int query =QMessageBox::warning(this, "Texmaker", tr("Delete the output files generated by LaTeX ?\n(.log,.aux,.dvi,.lof,.lot,.bit,.idx,.glo,.bbl,.ilg,.toc,.ind,.out,.synctex.gz,.blg)"),tr("Delete Files"), tr("Cancel") );
if (query==0)
	{
	//stat2->setText(QString(" %1 ").arg(tr("Clean")));
	for ( QStringList::Iterator it = extension.begin(); it != extension.end(); ++it ) 
		{
		f=basename+*it;
		if (QFile::exists(f))
			{
			QFile file(f);
			file.open( QIODevice::ReadOnly );
			file.remove();
			}
		}
	
	}
}

void Texmaker::Asymptote()
{
//stat2->setText(QString(" %1 ").arg("Asymptote"));
RunCommand(asymptote_command,false);
}

void Texmaker::AsyFile(QString asyfile)
{
QString commandline=asymptote_command;
QFileInfo fi(asyfile);
commandline.replace("%","\""+asyfile+"\"");
proc = new QProcess( this );
proc->setWorkingDirectory(fi.absolutePath());
proc->setProperty("command",commandline);

//****
#ifdef Q_WS_MACX
#if (QT_VERSION >= 0x0406)
QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
env.insert("PATH", env.value("PATH") + ":/usr/bin:/usr/sbin:/sbin:/usr/local/bin:/usr/texbin:/sw/bin");
proc->setProcessEnvironment(env);
#endif
#endif
//****
connect(proc, SIGNAL(readyReadStandardError()),this, SLOT(readFromStderr()));
//connect(proc, SIGNAL(readyReadStandardOutput()),this, SLOT(readFromStdoutput()));
connect(proc, SIGNAL(finished(int)),this, SLOT(SlotEndProcess(int)));
OutputTextEdit->clear();
OutputTableWidget->hide();
//OutputTextEdit->insertLine(commandline+"\n");
proc->start(commandline);
if (!proc->waitForStarted(1000)) 
	{
	ERRPROCESS=true;
	OutputTextEdit->insertLine("Error : could not start the command\n");
	checkViewerInstance=false;
	return;
	}
else OutputTextEdit->insertLine("Process started\n");
FINPROCESS=false;
QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
while (!FINPROCESS) 
	{
	qApp->instance()->processEvents(QEventLoop::ExcludeUserInputEvents);
	}
QApplication::restoreOverrideCursor();
}

void Texmaker::LatexMk()
{
//stat2->setText(QString(" %1 ").arg("LatexMk"));
RunCommand(latexmk_command,false);
}

void Texmaker::UserTool1()
{
QStringList commandList=UserToolCommand[0].split("|");
ERRPROCESS=false;
for (int i = 0; i < commandList.size(); ++i) 
	{
	if ((!ERRPROCESS)&&(!commandList.at(i).isEmpty())) RunCommand(commandList.at(i),true);
        else return;
	}
}

void Texmaker::UserTool2()
{
QStringList commandList=UserToolCommand[1].split("|");
ERRPROCESS=false;
for (int i = 0; i < commandList.size(); ++i) 
	{
	if ((!ERRPROCESS)&&(!commandList.at(i).isEmpty())) RunCommand(commandList.at(i),true);
        else return;
	}
}

void Texmaker::UserTool3()
{
QStringList commandList=UserToolCommand[2].split("|");
ERRPROCESS=false;
for (int i = 0; i < commandList.size(); ++i) 
	{
	if ((!ERRPROCESS)&&(!commandList.at(i).isEmpty())) RunCommand(commandList.at(i),true);
        else return;
	}
}

void Texmaker::UserTool4()
{
QStringList commandList=UserToolCommand[3].split("|");
ERRPROCESS=false;
for (int i = 0; i < commandList.size(); ++i) 
	{
	if ((!ERRPROCESS)&&(!commandList.at(i).isEmpty())) RunCommand(commandList.at(i),true);
        else return;
	}
}

void Texmaker::UserTool5()
{
QStringList commandList=UserToolCommand[4].split("|");
ERRPROCESS=false;
for (int i = 0; i < commandList.size(); ++i) 
	{
	if ((!ERRPROCESS)&&(!commandList.at(i).isEmpty())) RunCommand(commandList.at(i),true);
        else return;
	}
}

void Texmaker::EditUserTool()
{
QAction *Act;
UserToolDialog *utDlg = new UserToolDialog(this,tr("Edit User &Commands"));
for ( int i = 0; i <= 4; i++ )
	{
	utDlg->Name[i]=UserToolName[i];
	utDlg->Tool[i]=UserToolCommand[i];
	}
utDlg->init();
if ( utDlg->exec() )
	{
	for ( int i = 0; i <= 4; i++ )
		{
		UserToolName[i]=utDlg->Name[i];
		UserToolCommand[i]=utDlg->Tool[i];
		}
	user12Menu->clear();
	Act = new QAction("1: "+UserToolName[0], this);
	Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F1);
	connect(Act, SIGNAL(triggered()), this, SLOT(UserTool1()));
	user12Menu->addAction(Act);
	Act = new QAction("2: "+UserToolName[1], this);
	Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F2);
	connect(Act, SIGNAL(triggered()), this, SLOT(UserTool2()));
	user12Menu->addAction(Act);
	Act = new QAction("3: "+UserToolName[2], this);
	Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F3);
	connect(Act, SIGNAL(triggered()), this, SLOT(UserTool3()));
	user12Menu->addAction(Act);
	Act = new QAction("4: "+UserToolName[3], this);
	Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F4);
	connect(Act, SIGNAL(triggered()), this, SLOT(UserTool4()));
	user12Menu->addAction(Act);
	Act = new QAction("5: "+UserToolName[4], this);
	Act->setShortcut(Qt::SHIFT+Qt::ALT+Qt::Key_F5);
	connect(Act, SIGNAL(triggered()), this, SLOT(UserTool5()));
	user12Menu->addAction(Act);
	user12Menu->addSeparator();
	Act = new QAction(tr("Edit User &Commands"), this);
	connect(Act, SIGNAL(triggered()), this, SLOT(EditUserTool()));
	user12Menu->addAction(Act);
	}
}

void Texmaker::doCompile()
{
switch (comboCompil->currentIndex())
	{
	case 0:
		{
QuickBuild();
		}break;
	case 1:
		{
Latex();
		}break;
	case 2:
		{
DviToPS();
		}break;
	case 3:
		{
PDFLatex();
		}break;
	case 4:
		{
MakeBib();
		}break;
	case 5:
		{
MakeIndex();
		}break;
	case 6:
		{
MakeIndex();
		}break;
	case 7:
		{
PStoPDF();
		}break;
	case 8:
		{
DVItoPDF();
		}break;
	case 9:
		{
Asymptote();
		}break;
	case 10:
		{
LatexMk();
		}break;
	case 11:
		{
UserTool1();
		}break;
	case 12:
		{
UserTool2();
		}break;
	case 13:
		{
UserTool3();
		}break;
	case 14:
		{
UserTool4();
		}break;
	case 15:
		{
UserTool5();
		}break;
	}
}

void Texmaker::doView()
{
switch (comboView->currentIndex())
	{
	case 0:
		{
ViewDvi();
		}break;
	case 1:
		{
ViewPS();
		}break;
	case 2:
		{
ViewPDF();
		}break;
	}
}

void Texmaker::jumpToPdfline(int line)
{
if (!builtinpdfview) return;
QString finame;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
if ((singlemode && !currentEditorView()) || finame.startsWith("untitled") || finame=="")
	{
	QMessageBox::warning( this,tr("Error"),tr("Can't detect the file name"));
	return;
	}
fileSave();
QFileInfo fi(finame);
if (!fi.exists()) return;
QString basename=fi.completeBaseName();
if (largescreen)
    {
    if (pdfviewerWidget)
      {
      if (pdfviewerWidget->pdf_file!=fi.absolutePath()+"/"+basename+".pdf") pdfviewerWidget->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
      StackedViewers->setCurrentWidget(pdfviewerWidget);
      //pdfviewerWidget->raise();
      pdfviewerWidget->show();
      if ( (pdflatex_command.contains("synctex=1")) || (latex_command.contains("synctex=1")) ) pdfviewerWidget->jumpToPdfFromSource(finame,line);
      pdfviewerWidget->getFocus();
      }
    else
      {
  //    pdfviewerWidget=new PdfViewer(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command, this);
      pdfviewerWidget=new PdfViewerWidget(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command,psize,StackedViewers);
      connect(pdfviewerWidget, SIGNAL(openDocAtLine(const QString&, int, bool)), this, SLOT(fileOpenAndGoto(const QString&, int, bool)));
      connect(pdfviewerWidget, SIGNAL(sendFocusToEditor()), this, SLOT(getFocusToEditor()));
      connect(pdfviewerWidget, SIGNAL(sendPaperSize(const QString&)), this, SLOT(setPrintPaperSize(const QString&)));
      StackedViewers->addWidget(pdfviewerWidget);
      //pdfviewerWidget->raise();
      pdfviewerWidget->show();
      pdfviewerWidget->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
      if ( (pdflatex_command.contains("synctex=1")) || (latex_command.contains("synctex=1")) ) pdfviewerWidget->jumpToPdfFromSource(finame,line);
      pdfviewerWidget->getFocus();
      }
    }
else
    {
    if (pdfviewerWindow)
      {
      if (pdfviewerWindow->pdf_file!=fi.absolutePath()+"/"+basename+".pdf") pdfviewerWindow->openFile(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command);
      pdfviewerWindow->raise();
      pdfviewerWindow->show();
      if ( (pdflatex_command.contains("synctex=1")) || (latex_command.contains("synctex=1")) ) pdfviewerWindow->jumpToPdfFromSource(finame,line);
      }
    else
      {
  //    pdfviewerWindow=new PdfViewer(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command, this);
      pdfviewerWindow=new PdfViewer(fi.absolutePath()+"/"+basename+".pdf",viewpdf_command,ghostscript_command,psize,0);
      connect(pdfviewerWindow, SIGNAL(openDocAtLine(const QString&, int, bool)), this, SLOT(fileOpenAndGoto(const QString&, int, bool)));
      connect(pdfviewerWindow, SIGNAL(sendFocusToEditor()), this, SLOT(getFocusToEditor()));
      connect(pdfviewerWindow, SIGNAL(sendPaperSize(const QString&)), this, SLOT(setPrintPaperSize(const QString&)));
      pdfviewerWindow->raise();
      pdfviewerWindow->show();
      if ( (pdflatex_command.contains("synctex=1")) || (latex_command.contains("synctex=1")) ) pdfviewerWindow->jumpToPdfFromSource(finame,line);
      }  
    }
}


//////////////// MESSAGES - LOG FILE///////////////////////
bool Texmaker::LogExists()
{
QString finame;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
if ((singlemode && !currentEditorView()) ||finame.startsWith("untitled") || finame=="")
	{
	return false;
	}
QFileInfo fi(finame);
QString name=fi.absoluteFilePath();
QString ext=fi.suffix();
QString basename=name.left(name.length()-ext.length()-1);
QString logname=basename+".log";
QFileInfo fic(logname);
if (fic.exists() && fic.isReadable()) return true;
else return false;
}

void Texmaker::LoadLog()
{
OutputTextEdit->clear();
logpresent=false;
QString finame;
if (singlemode) {finame=getName();}
else {finame=MasterName;}
if ((singlemode && !currentEditorView()) ||finame.startsWith("untitled") || finame=="")
	{
	QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
	ERRPROCESS=true;
	return;
	}
QFileInfo fi(finame);
QString name=fi.absoluteFilePath();
QString ext=fi.suffix();
QString basename=name.left(name.length()-ext.length()-1);
QString logname=basename+".log";
QString line;
QFileInfo fic(logname);
if (fic.exists() && fic.isReadable() )
	{
	OutputTextEdit->insertLine("LOG FILE :");
	QFile f(logname);
	if ( f.open(QIODevice::ReadOnly) )
		{
		QTextStream t( &f );
//		OutputTextEdit->setPlainText( t.readAll() );
		while ( !t.atEnd() )
			{
			line=t.readLine();
			line=line.simplified();
			if (!line.isEmpty()) OutputTextEdit->append(line);
			}
		}
		f.close();
	LatexError();
	logpresent=true;
	}
else {QMessageBox::warning( this,tr("Error"),tr("Log File not found !"));}
}
void Texmaker::ViewLog()
{
LoadLog();
if (!showoutputview) ShowOutputView(true);
}

void Texmaker::ClickedOnOutput(int l)
{
if ( !currentEditorView() ) return;
currentEditorView()->editor->gotoLine(l);
}

void Texmaker::ClickedOnLogLine(QTableWidgetItem *item)
{
if ( !currentEditorView() ) return;
if ( !item ) return;
QString content=item->text();
int row=OutputTableWidget->row(item);
int col=OutputTableWidget->column(item);
if (col!=3) content=OutputTableWidget->item(row,3)->text();
QString file=OutputTableWidget->item(row,1)->text();

int Start, End;
bool ok;
QString s;
QString line="";
//// l. ///
s = content;
Start=End=0;
Start=s.indexOf(QRegExp("l.[0-9]"), End);
if (Start!=-1)
	{
	Start=Start+2;
	s=s.mid(Start,s.length());
	
	if (End!=-1)
	line=s.mid(0,End);
	else
	line=s.mid(0,s.length());
	};
//// line ///
s = content;
Start=End=0;
Start=s.indexOf(QRegExp("line [0-9]"), End);
if (Start!=-1)
	{
	Start=Start+5;
	s=s.mid(Start,s.length());
	End=s.indexOf(QRegExp("[ a-zA-Z.\\-]"),0);
	if (End!=-1)
	line=s.mid(0,End);
	else
	line=s.mid(0,s.length());
	};
//// lines ///
s = content;
Start=End=0;
Start=s.indexOf(QRegExp("lines [0-9]"), End);
if (Start!=-1)
	{
	Start=Start+6;
	s=s.mid(Start,s.length());
	End=s.indexOf(QRegExp("[ a-zA-Z.\\-]"),0);
	if (End!=-1)
	line=s.mid(0,End);
	else
	line=s.mid(0,s.length());
	};
int l=line.toInt(&ok,10)-1;
if (ok)
	{
	if (file.isEmpty()) currentEditorView()->editor->gotoLine(l);
	else 
	  {
	  QFileInfo fi(file);
	  if (fi.exists() && fi.isReadable() ) fileOpenAndGoto(file,l+1,true);
	  else 
	    {
	    QFileInfo fic(getName());
	    file=fic.absolutePath()+"/"+file;
	    fileOpenAndGoto(file,l+1,true);
	    }
	  }
	}
QString ll=item->data(Qt::UserRole).toString();
int logline=ll.toInt(&ok,10)-1;
OutputTextEdit->setCursorPosition(logline , 0);
}

////////////////////////// ERRORS /////////////////////////////
void Texmaker::LatexError()
{
errorFileList.clear();
errorTypeList.clear();
errorLineList.clear();
errorMessageList.clear();
errorLogList.clear();
onlyErrorList.clear();
errorIndex=-1;

QString mot,suivant,lettre,expression,warning,latexerror,badbox,name;
QStringList pile,filestack;
filestack.clear();
pile.clear();

int j;

int ligne=0;
int par=1;
int errorpar=1;

QRegExp rxWarning1("Warning: (.*) on.*line *(\\d+)");
QRegExp rxWarning2("Warning: (.*)");
QRegExp rxLatexError("(! )*(LaTeX Error:)* *(.*)\\.l\\.(\\d+) *(.*)");
QRegExp rxLineError("l\\.(\\d+)");
QRegExp rxBadBox("at (line|lines) ([0-9]+)");
QRegExp rxFile("(.*(\\.tex|\\.sty|\\.cls))");


QTextBlock tb = OutputTextEdit->document()->begin();
while (tb.isValid())
 	{
	errorpar=par;
	expression=tb.text();
	j=0;
	pile.clear();
	while (j<expression.length())
		{
		lettre=expression.mid(j,1);
		if (lettre=="(" || lettre==")") 
			{
			pile.prepend(lettre);
			j+=1;
			}
		else
			{
			mot="";
			while (j<expression.length() && (expression.mid(j,1)!="(" && expression.mid(j,1)!=")")) 
				{
				mot+=expression.mid(j,1);
				j+=1;
				}
			pile.prepend(mot);
			}
		}
	while (pile.count()>0)
		{
		mot=pile.last();
		pile.removeLast();
		if (mot=="(" && pile.count()>0)
			{
			suivant=pile.last();
			pile.removeLast();
			if (rxFile.indexIn(suivant) != -1) name=rxFile.cap(1);
			else name="";
			filestack.append(name.remove("./"));
			}
		else if (mot==")" && filestack.count()>0) filestack.removeLast();
		}
	if (expression.contains("Warning"))
		{
		warning=expression.trimmed();
		while (tb.isValid() && !expression.contains(QRegExp("\\.$")))
			{
			par++;
			tb=tb.next();
			if (tb.isValid())
				{
				expression=tb.text();
				warning+=expression.trimmed();
				}
			}

		if ( rxWarning1.indexIn(warning) != -1 )
			{
			if (!filestack.isEmpty()) errorFileList.append(filestack.last());
			else errorFileList.append("");
			errorTypeList.append("Warning");
			errorLineList.append(rxWarning1.cap(2));
			errorMessageList.append(rxWarning1.cap(1).replace("*",""));
			errorLogList.append(QString::number(errorpar));
			}
		else if ( rxWarning2.indexIn(warning) != -1 )
			{
			if (!filestack.isEmpty()) errorFileList.append(filestack.last());
			else errorFileList.append("");
			errorTypeList.append("Warning");
			errorLineList.append("1");
			errorMessageList.append(rxWarning2.cap(1).replace("*",""));
			errorLogList.append(QString::number(errorpar));
			}
		else 
			{
			if (!filestack.isEmpty()) errorFileList.append(filestack.last());
			else errorFileList.append("");
			errorTypeList.append("Warning");
			errorLineList.append("1");
			errorMessageList.append(warning.replace("*",""));
			errorLogList.append(QString::number(errorpar));
 			}
		errorpar=par;
		}
//	else if (expression.contains(QRegExp("^!")))
	else if (expression.contains(QRegExp("^! (.*)")))
		{
		latexerror=expression.trimmed();
		while (tb.isValid() && !expression.contains(rxLineError))
			{
			par++;
			tb=tb.next();
			if (tb.isValid())
				{
				expression=tb.text();
				latexerror+=expression.trimmed();
				}
			}
// 		if ( rxLatexError.indexIn(latexerror) != -1 )
// 			{
// 			errorFileList.append(filestack.last());
// 			errorTypeList.append("Error");
// 			errorLineList.append(rxLatexError.cap(1));
// 			errorMessageList.append(rxLatexError.cap(3)+" :"+rxLatexError.cap(5));
// 			}
		if ( rxLineError.indexIn(latexerror) != -1 )
			{
			if (!filestack.isEmpty()) errorFileList.append(filestack.last());
			else errorFileList.append("");
			errorTypeList.append("Error");
			errorLineList.append(rxLineError.cap(1));
			errorMessageList.append(latexerror.remove(rxLineError).replace("*",""));
			errorLogList.append(QString::number(errorpar));
			}
		else 
			{
			if (!filestack.isEmpty()) errorFileList.append(filestack.last());
			else errorFileList.append("");
			errorTypeList.append("Error");
			errorLineList.append("1");
			errorMessageList.append(latexerror.replace("*",""));
			errorLogList.append(QString::number(errorpar));
			}
		errorpar=par;
		}
	else if (expression.contains(QRegExp("^(Over|Under)(full \\\\[hv]box .*)")))
		{
		badbox=expression.trimmed();
/*		while (tb.isValid() && !expression.contains(QRegExp("(.*) at line")))
			{
			par++;
			tb=tb.next();
			if (tb.isValid())
				{
				expression=tb.text();
				badbox+=expression.trimmed();
				}
			}*/
		if ( rxBadBox.indexIn(badbox) != -1 )
			{
			if (!filestack.isEmpty()) errorFileList.append(filestack.last());
			else errorFileList.append("");
			errorTypeList.append("Badbox");
			errorLineList.append(rxBadBox.cap(2));
			errorMessageList.append(badbox.replace("*",""));
			errorLogList.append(QString::number(errorpar));
			}
		else
			{
			if (!filestack.isEmpty()) errorFileList.append(filestack.last());
			else errorFileList.append("");
			errorTypeList.append("Badbox");
			errorLineList.append("1");
			errorMessageList.append(badbox.replace("*",""));
			errorLogList.append(QString::number(errorpar));
			}
		errorpar=par;
		}
	if (tb.isValid()) 
		{
		par++;
		tb = tb.next();
		}
	}
DisplayLatexError();
}

void Texmaker::DisplayLatexError()
{
OutputTableWidget->clearContents();
QFontMetrics fm(qApp->font());
int rowheight=fm.lineSpacing()+4;
int maxwidth=0;
int maxwidthline=0;
int row=0;
if (errorFileList.count()>0)
	{
	OutputTableWidget->setRowCount(errorFileList.count());
	for ( int i = 0; i <errorFileList.count(); i++ )
		{
		if (errorTypeList.at(i)=="Error")
			{
			QTableWidgetItem *ItemFile = new QTableWidgetItem(errorFileList.at(i));
			ItemFile->setData(Qt::UserRole,errorLogList.at(i));
			ItemFile->setForeground(QBrush(QColor(Qt::red)));
			ItemFile->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			OutputTableWidget->setItem(row,1, ItemFile);
			QTableWidgetItem *ItemType = new QTableWidgetItem(errorTypeList.at(i));
			ItemType->setData(Qt::UserRole,errorLogList.at(i));
			ItemType->setForeground(QBrush(QColor(Qt::red)));
			ItemType->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			OutputTableWidget->setItem(row,2, ItemType);
			QTableWidgetItem *ItemLine = new QTableWidgetItem("line "+errorLineList.at(i));
			if (fm.width("line  "+errorLineList.at(i))>maxwidthline) maxwidthline=fm.width("line  "+errorLineList.at(i));
			ItemLine->setData(Qt::UserRole,errorLogList.at(i));
			ItemLine->setForeground(QBrush(QColor(Qt::red)));
			ItemLine->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			ItemLine->setToolTip(tr("Click to jump to the line"));
			OutputTableWidget->setItem(row,3, ItemLine);
			QTableWidgetItem *ItemMessage = new QTableWidgetItem(errorMessageList.at(i));
			if (fm.width(errorMessageList.at(i))>maxwidth) maxwidth=fm.width(errorMessageList.at(i));
			ItemMessage->setData(Qt::UserRole,errorLogList.at(i));
			ItemMessage->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			ItemMessage->setForeground(QBrush(QColor(Qt::red)));
			OutputTableWidget->setItem(row,4, ItemMessage);

			onlyErrorList.append(i);
			row++;
			}
		}
	for ( int i = 0; i <errorFileList.count(); i++ )
		{
		if (errorTypeList.at(i)!="Error")
			{
			QTableWidgetItem *ItemFile = new QTableWidgetItem(errorFileList.at(i));
			ItemFile->setData(Qt::UserRole,errorLogList.at(i));
			ItemFile->setForeground(QBrush(QColor(Qt::blue)));
			ItemFile->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			OutputTableWidget->setItem(row,1, ItemFile);
			QTableWidgetItem *ItemType = new QTableWidgetItem(errorTypeList.at(i));
			ItemType->setData(Qt::UserRole,errorLogList.at(i));
			ItemType->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			ItemType->setForeground(QBrush(QColor(Qt::blue)));
			OutputTableWidget->setItem(row,2, ItemType);
			QTableWidgetItem *ItemLine = new QTableWidgetItem("line "+errorLineList.at(i));
			if (fm.width("line  "+errorLineList.at(i))>maxwidthline) maxwidthline=fm.width("line  "+errorLineList.at(i));
			ItemLine->setData(Qt::UserRole,errorLogList.at(i));
			ItemLine->setForeground(QBrush(QColor(Qt::blue)));
			ItemLine->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			ItemLine->setToolTip(tr("Click to jump to the line"));
			OutputTableWidget->setItem(row,3, ItemLine);
			QTableWidgetItem *ItemMessage = new QTableWidgetItem(errorMessageList.at(i));
			if (fm.width(errorMessageList.at(i))>maxwidth) maxwidth=fm.width(errorMessageList.at(i));
			ItemMessage->setData(Qt::UserRole,errorLogList.at(i));
			ItemMessage->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
			ItemMessage->setForeground(QBrush(QColor(Qt::blue)));
			OutputTableWidget->setItem(row,4, ItemMessage);
			row++;
			}
		}
	OutputTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	OutputTableWidget->horizontalHeader()->resizeSection(3,maxwidthline+10);
	//OutputTableWidget->horizontalHeader()->resizeSection(4,maxwidth);
	//OutputTableWidget->setColumnWidth(4,maxwidth);
	//OutputTableWidget->setColumnWidth(3,maxwidthline);
	for ( int i = 0; i<OutputTableWidget->rowCount(); ++i )
		{
		OutputTableWidget->setRowHeight(i,rowheight);
		}
	//OutputTableWidget->resizeColumnsToContents();
//	OutputTableWidget->resizeRowsToContents();
	}
else
	{
	OutputTableWidget->setRowCount(1);
	OutputTableWidget->setRowHeight(0,rowheight);
	OutputTableWidget->clearContents();
	}
OutputTableWidget->show();
OutputTextEdit->setCursorPosition(0 , 0);
}

bool Texmaker::NoLatexErrors()
{
return onlyErrorList.isEmpty();
}

void Texmaker::NextError()
{
int line=0;
QTableWidgetItem *Item;
if (!logpresent) {LoadLog();}
ViewLog();
if (logpresent && !onlyErrorList.isEmpty())
  	{
	if (errorIndex<onlyErrorList.size()-1) errorIndex=errorIndex+1;
	if (errorIndex<0) errorIndex=0;
	if (errorIndex>=onlyErrorList.count()) errorIndex=onlyErrorList.count()-1;
	if (singlemode)// && onlyErrorList.at(errorIndex)!=0) 
		{
		line=errorLineList.at(onlyErrorList.at(errorIndex)).toInt();
		for ( int i = 0; i <errorFileList.count(); i++ )
			{
			Item = new QTableWidgetItem(" ");
			OutputTableWidget->setItem(i,0, Item);
			}
		Item = new QTableWidgetItem(">");
		OutputTableWidget->setItem(errorIndex,0, Item);
		OutputTableWidget->scrollToItem(Item,QAbstractItemView::PositionAtCenter);
		ClickedOnOutput(line-1);
		int logline=errorLogList.at(onlyErrorList.at(errorIndex)).toInt()-1;
		OutputTextEdit->setCursorPosition(logline , 0);
		}
  	}
if (logpresent && onlyErrorList.isEmpty())
	{
	QMessageBox::information( this,"Texmaker",tr("No LaTeX errors detected !"));
	OutputTextEdit->setCursorPosition(0 , 0);
	}
}

void Texmaker::PreviousError()
{
int line=0;
QTableWidgetItem *Item;
if (!logpresent) {LoadLog();}
ViewLog();
if (logpresent && !onlyErrorList.isEmpty())
  	{
	if (errorIndex>0) errorIndex=errorIndex-1;
	if (errorIndex<0) errorIndex=0;
	if (errorIndex>=onlyErrorList.count()) errorIndex=onlyErrorList.count()-1;
	if (singlemode)// && onlyErrorList.at(errorIndex)!=0) 
		{
		line=errorLineList.at(onlyErrorList.at(errorIndex)).toInt();
		for ( int i = 0; i <errorFileList.count(); i++ )
			{
			Item = new QTableWidgetItem(" ");
			OutputTableWidget->setItem(i,0, Item);
			}
		Item = new QTableWidgetItem(">");
		OutputTableWidget->setItem(errorIndex,0, Item);
		OutputTableWidget->scrollToItem(Item,QAbstractItemView::PositionAtCenter);
		ClickedOnOutput(line-1);
		int logline=errorLogList.at(onlyErrorList.at(errorIndex)).toInt()-1;
		OutputTextEdit->setCursorPosition(logline , 0);
		}
  	}
if (logpresent && onlyErrorList.isEmpty())
	{
	QMessageBox::information( this,"Texmaker",tr("No LaTeX errors detected !"));
	OutputTextEdit->setCursorPosition(0 , 0);
	}
}

//////////////// HELP /////////////////
void Texmaker::LatexHelp()
{
#if defined( Q_WS_X11 )

#ifdef USB_VERSION
QString docfile=QCoreApplication::applicationDirPath() + "/latexhelp.html";
#else
QString docfile=PREFIX"/share/texmaker/latexhelp.html";
#endif

#endif
#if defined( Q_WS_MACX )
QString docfile=QCoreApplication::applicationDirPath() + "/../Resources/latexhelp.html";
#endif
#if defined(Q_WS_WIN)
QString docfile=QCoreApplication::applicationDirPath() + "/latexhelp.html";
#endif
QFileInfo fic(docfile);
if (fic.exists() && fic.isReadable() )
	{
        if (browserWindow)
          {
          browserWindow->close();
          }
	browserWindow=new Browser("file:///"+docfile, 0);
	browserWindow->raise();
	browserWindow->show();
//	QDesktopServices::openUrl("file:///"+docfile);
	}
else { QMessageBox::warning( this,tr("Error"),tr("File not found"));}
}

void Texmaker::UserManualHelp()
{
QString locale = QString(QLocale::system().name()).left(2);
if ( locale.length() < 2 || locale!="fr" ) locale = "en";
#if defined( Q_WS_X11 )

#ifdef USB_VERSION
QString docfile=QCoreApplication::applicationDirPath() + "/usermanual_"+locale+".html";
#else
QString docfile=PREFIX"/share/texmaker/usermanual_"+locale+".html";
#endif

#endif
#if defined( Q_WS_MACX )
QString docfile=QCoreApplication::applicationDirPath() + "/../Resources/usermanual_"+locale+".html";
#endif
#if defined(Q_WS_WIN)
QString docfile=QCoreApplication::applicationDirPath() + "/usermanual_"+locale+".html";
#endif
QFileInfo fic(docfile);
if (fic.exists() && fic.isReadable() )
	{
        if (browserWindow)
          {
          browserWindow->close();
          }
	browserWindow=new Browser("file:///"+docfile, 0);
	browserWindow->raise();
	browserWindow->show();
//	QDesktopServices::openUrl("file:///"+docfile);
	}
else { QMessageBox::warning( this,tr("Error"),tr("File not found"));}
}

void Texmaker::HelpAbout()
{
AboutDialog *abDlg = new AboutDialog(this);
abDlg->exec();
}
////////////// OPTIONS //////////////////////////////////////
void Texmaker::GeneralOptions()
{
ConfigDialog *confDlg = new ConfigDialog(this);

confDlg->ui.lineEditLatex->setText(latex_command);
confDlg->ui.lineEditPdflatex->setText(pdflatex_command);
confDlg->ui.lineEditDvips->setText(dvips_command);
confDlg->ui.lineEditDviviewer->setText(viewdvi_command);
confDlg->ui.lineEditPsviewer->setText(viewps_command);
confDlg->ui.lineEditDvipdfm->setText(dvipdf_command);
confDlg->ui.lineEditPs2pdf->setText(ps2pdf_command);
confDlg->ui.lineEditBibtex->setText(bibtex_command);
confDlg->ui.lineEditMakeindex->setText(makeindex_command);
confDlg->ui.lineEditPdfviewer->setText(viewpdf_command);
if (builtinpdfview) confDlg->ui.radioButtonInternalPdfViewer->setChecked(true);
else confDlg->ui.radioButtonExternalPdfViewer->setChecked(true);
confDlg->ui.lineEditMetapost->setText(metapost_command);
confDlg->ui.lineEditGhostscript->setText(ghostscript_command);
confDlg->ui.lineEditAsymptote->setText(asymptote_command);
confDlg->ui.lineEditLatexmk->setText(latexmk_command);
if (singleviewerinstance) confDlg->ui.checkBoxSingleInstanceViewer->setChecked(true);

confDlg->ui.comboBoxFont->lineEdit()->setText(EditorFont.family() );
confDlg->ui.comboBoxEncoding->setCurrentIndex(confDlg->ui.comboBoxEncoding->findText(input_encoding, Qt::MatchExactly));

confDlg->ui.spinBoxSize->setValue(EditorFont.pointSize() );
if (wordwrap) {confDlg->ui.checkBoxWordwrap->setChecked(true);}
else {confDlg->ui.checkBoxWordwrap->setChecked(false);}
if (showline) {confDlg->ui.checkBoxLinenumber->setChecked(true);}
else {confDlg->ui.checkBoxLinenumber->setChecked(false);}
if (completion) {confDlg->ui.checkBoxCompletion->setChecked(true);}
else {confDlg->ui.checkBoxCompletion->setChecked(false);}
if (inlinespellcheck) {confDlg->ui.checkBoxInlineSpell->setChecked(true);}
else {confDlg->ui.checkBoxInlineSpell->setChecked(false);}

confDlg->ui.lineEditAspellCommand->setText(spell_dic);

confDlg->ui.pushButtonColorMath->setPalette(QPalette(colorMath));
confDlg->ui.pushButtonColorMath->setAutoFillBackground(true);
confDlg->ui.pushButtonColorCommand->setPalette(QPalette(colorCommand));
confDlg->ui.pushButtonColorCommand->setAutoFillBackground(true);
confDlg->ui.pushButtonColorKeyword->setPalette(QPalette(colorKeyword));
confDlg->ui.pushButtonColorKeyword->setAutoFillBackground(true);

if (quickmode==1) {confDlg->ui.radioButton1->setChecked(true); confDlg->ui.lineEditUserquick->setEnabled(false);confDlg->ui.pushButtonWizard->setEnabled(false);}
if (quickmode==2) {confDlg->ui.radioButton2->setChecked(true); confDlg->ui.lineEditUserquick->setEnabled(false);confDlg->ui.pushButtonWizard->setEnabled(false);}
if (quickmode==3) {confDlg->ui.radioButton3->setChecked(true); confDlg->ui.lineEditUserquick->setEnabled(false);confDlg->ui.pushButtonWizard->setEnabled(false);}
if (quickmode==4)  {confDlg->ui.radioButton4->setChecked(true); confDlg->ui.lineEditUserquick->setEnabled(false);confDlg->ui.pushButtonWizard->setEnabled(false);}
if (quickmode==5)  {confDlg->ui.radioButton5->setChecked(true); confDlg->ui.lineEditUserquick->setEnabled(false);confDlg->ui.pushButtonWizard->setEnabled(false);}
if (quickmode==6)  {confDlg->ui.radioButton6->setChecked(true); confDlg->ui.lineEditUserquick->setEnabled(true);confDlg->ui.pushButtonWizard->setEnabled(true);}
if (quickmode==7)  {confDlg->ui.radioButton7->setChecked(true); confDlg->ui.lineEditUserquick->setEnabled(false);confDlg->ui.pushButtonWizard->setEnabled(false);}
if (quickmode==8)  {confDlg->ui.radioButton8->setChecked(true); confDlg->ui.lineEditUserquick->setEnabled(false);confDlg->ui.pushButtonWizard->setEnabled(false);}
if (quickmode==9)  {confDlg->ui.radioButton8->setChecked(true); confDlg->ui.lineEditUserquick->setEnabled(false);confDlg->ui.pushButtonWizard->setEnabled(false);}
confDlg->ui.lineEditUserquick->setText(userquick_command);

int row=0;
KeysMap::Iterator its, iter;
QString d,f;
for( its = shortcuts.begin(); its != shortcuts.end(); ++its )
	{
	d=its.key().section("/",0,0);
	for( iter = actionstext.begin(); iter != actionstext.end(); ++iter )
		{
		f=iter.key().section("/",0,0);
		if (d==f)
			{
			QTableWidgetItem *newItem = new QTableWidgetItem(iter.value());
			//QTableWidgetItem *newItem = new QTableWidgetItem(*actionstext.find(its.key()));
			newItem->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
			newItem->setData(Qt::UserRole, its.key());
			confDlg->ui.shorttableWidget->setRowCount(row+1);
			confDlg->ui.shorttableWidget->setItem(row, 0, newItem);
			QTableWidgetItem *newItembis = new QTableWidgetItem(its.value());
			//QTableWidgetItem *newItem = new QTableWidgetItem(*actionstext.find(its.key()));
			newItembis->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
			newItembis->setData(Qt::UserRole,QString("key"));
			confDlg->ui.shorttableWidget->setItem(row, 1,newItembis);
			row++;
			}
		}
	}
confDlg->ui.shorttableWidget->horizontalHeader()->resizeSection( 0, 250 );
confDlg->ui.shorttableWidget->verticalHeader()->hide();

if (confDlg->exec())
	{
	listViewerCommands.clear();
	for(int row=0; row<confDlg->ui.shorttableWidget->rowCount(); row++ )
	{
		QString itemtext = confDlg->ui.shorttableWidget->item(row, 0)->text();
		QString itemshortcut = confDlg->ui.shorttableWidget->item(row, 1)->text();
		QString itemdata=confDlg->ui.shorttableWidget->item(row, 0)->data(Qt::UserRole).toString();
		shortcuts.remove(itemdata);
		shortcuts.insert(itemdata,itemshortcut);
	}
	ModifyShortcuts();

	if (confDlg->ui.radioButton1->isChecked()) quickmode=1;
	if (confDlg->ui.radioButton2->isChecked()) quickmode=2;
	if (confDlg->ui.radioButton3->isChecked()) quickmode=3;
	if (confDlg->ui.radioButton4->isChecked()) quickmode=4;
	if (confDlg->ui.radioButton5->isChecked()) quickmode=5;
	if (confDlg->ui.radioButton6->isChecked()) quickmode=6;
	if (confDlg->ui.radioButton7->isChecked()) quickmode=7;
	if (confDlg->ui.radioButton8->isChecked()) quickmode=8;
	if (confDlg->ui.radioButton9->isChecked()) quickmode=9;
	userquick_command=confDlg->ui.lineEditUserquick->text();
	
	latex_command=confDlg->ui.lineEditLatex->text();
	pdflatex_command=confDlg->ui.lineEditPdflatex->text();
	dvips_command=confDlg->ui.lineEditDvips->text();
	viewdvi_command=confDlg->ui.lineEditDviviewer->text();
	viewps_command=confDlg->ui.lineEditPsviewer->text();
	dvipdf_command=confDlg->ui.lineEditDvipdfm->text();
	ps2pdf_command=confDlg->ui.lineEditPs2pdf->text();
	bibtex_command=confDlg->ui.lineEditBibtex->text();
	makeindex_command=confDlg->ui.lineEditMakeindex->text();
	viewpdf_command=confDlg->ui.lineEditPdfviewer->text();
	metapost_command=confDlg->ui.lineEditMetapost->text();
	if (metapost_command.right(1)!=" ") metapost_command+=" ";
	ghostscript_command=confDlg->ui.lineEditGhostscript->text();
	asymptote_command=confDlg->ui.lineEditAsymptote->text();
	latexmk_command=confDlg->ui.lineEditLatexmk->text();
	builtinpdfview=confDlg->ui.radioButtonInternalPdfViewer->isChecked();
	if (largescreen && builtinpdfview) 
	  {
	  StackedViewers->show();
	  ViewPdfPanelAct->setEnabled(true);
	  togglePdfButton->show();
	  ShowPdfView(false);
	  }
	else 
	  {
	  if (pdfviewerWidget) {StackedViewers->removeWidget(pdfviewerWidget);delete(pdfviewerWidget);}
	  StackedViewers->hide();
	  ViewPdfPanelAct->setEnabled(false);
	  togglePdfButton->hide();
	  }
	singleviewerinstance=confDlg->ui.checkBoxSingleInstanceViewer->isChecked();
	
	QString fam=confDlg->ui.comboBoxFont->lineEdit()->text();
	int si=confDlg->ui.spinBoxSize->value();
	QFont F(fam,si);
	EditorFont=F;
	
	input_encoding=confDlg->ui.comboBoxEncoding->currentText();
	stat3->setText(QString(" %1 ").arg(input_encoding));
	
	wordwrap=confDlg->ui.checkBoxWordwrap->isChecked();

	completion=confDlg->ui.checkBoxCompletion->isChecked();
	showline=confDlg->ui.checkBoxLinenumber->isChecked();
	inlinespellcheck=confDlg->ui.checkBoxInlineSpell->isChecked();
	spell_dic=confDlg->ui.lineEditAspellCommand->text();
	if (spellChecker) delete spellChecker;
	if (spelldicExist()) 
	      {
	      QString dic=spell_dic.left(spell_dic.length()-4);
	      spellChecker = new Hunspell(dic.toLatin1()+".aff",dic.toLatin1()+".dic");
	      }
	else spellChecker=0;
	colorMath=confDlg->ui.pushButtonColorMath->palette().background().color();
	colorCommand=confDlg->ui.pushButtonColorCommand->palette().background().color();
	colorKeyword=confDlg->ui.pushButtonColorKeyword->palette().background().color();
	QTextCodec* codec = QTextCodec::codecForName(input_encoding.toLatin1());
	if(!codec) codec = QTextCodec::codecForLocale();
	if (currentEditorView())
		{
		LatexEditorView *temp = new LatexEditorView( EditorView,EditorFont,showline,colorMath,colorCommand,colorKeyword,inlinespellcheck,spell_ignored_words,spellChecker);
		temp=currentEditorView();
		FilesMap::Iterator it;
		for( it = filenames.begin(); it != filenames.end(); ++it )
			{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
			EditorView->setCurrentIndex(EditorView->indexOf(it.key()));
			bool  MODIFIED =currentEditorView()->editor->document()->isModified();
			QString tmp =currentEditorView()->editor->toPlainText();
			disconnect(currentEditorView()->editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(NewDocumentStatus(bool)));
			disconnect(currentEditorView()->editor, SIGNAL(spellme()), this, SLOT(editSpell()));
			disconnect(currentEditorView()->editor, SIGNAL(tooltiptab()), this, SLOT(editTipTab()));
			disconnect(currentEditorView()->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
			disconnect(currentEditorView()->editor, SIGNAL(requestpdf(int)),this, SLOT(jumpToPdfline(int)));
			disconnect(currentEditorView()->editor->document(), SIGNAL(undoAvailable(bool)),UndoAct, SLOT(setEnabled(bool)));
			disconnect(currentEditorView()->editor->document(), SIGNAL(redoAvailable(bool)),RedoAct, SLOT(setEnabled(bool)));
			disconnect(currentEditorView()->editor, SIGNAL(copyAvailable(bool)), CutAct, SLOT(setEnabled(bool)));
			disconnect(currentEditorView()->editor, SIGNAL(copyAvailable(bool)), CopyAct, SLOT(setEnabled(bool)));
			disconnect(currentEditorView()->editor, SIGNAL(blockCountChanged(int)), this, SLOT(refreshAllFromCursor(int)));
			disconnect(currentEditorView()->editor, SIGNAL(requestGotoStructure(int)),this, SLOT(jumpToStructure(int)));
			currentEditorView()->editor->setSpellChecker(spellChecker);
			currentEditorView()->editor->highlighter->setSpellChecker(spellChecker);
			currentEditorView()->editor->activateInlineSpell(inlinespellcheck);
			currentEditorView()->editor->highlighter->activateInlineSpell(inlinespellcheck);
			if (wordwrap) {currentEditorView()->editor->setWordWrapMode(QTextOption::WordWrap);}
			else {currentEditorView()->editor->setWordWrapMode(QTextOption::NoWrap);}
			if (completion) currentEditorView()->editor->setCompleter(completer);
			else currentEditorView()->editor->setCompleter(0);
			currentEditorView()->changeSettings(EditorFont,showline);
			currentEditorView()->editor->highlighter->setColors(colorMath,colorCommand,colorKeyword);
			QTextStream ts( &tmp,QIODevice::ReadOnly );
			ts.setCodec(codec);
			currentEditorView()->editor->setPlainText( ts.readAll() );
			currentEditorView()->editor->setLastSavedTime(QDateTime::currentDateTime());
			if( MODIFIED ) currentEditorView()->editor->document()->setModified(TRUE );
			else currentEditorView()->editor->document()->setModified( FALSE );
			connect(currentEditorView()->editor->document(), SIGNAL(modificationChanged(bool)), this, SLOT(NewDocumentStatus(bool)));
			connect(currentEditorView()->editor, SIGNAL(spellme()), this, SLOT(editSpell()));
			connect(currentEditorView()->editor, SIGNAL(tooltiptab()), this, SLOT(editTipTab()));
			connect(currentEditorView()->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
			connect(currentEditorView()->editor, SIGNAL(requestpdf(int)),this, SLOT(jumpToPdfline(int)));
			connect(currentEditorView()->editor->document(), SIGNAL(undoAvailable(bool)),UndoAct, SLOT(setEnabled(bool)));
			connect(currentEditorView()->editor->document(), SIGNAL(redoAvailable(bool)),RedoAct, SLOT(setEnabled(bool)));
			connect(currentEditorView()->editor, SIGNAL(copyAvailable(bool)), CutAct, SLOT(setEnabled(bool)));
			connect(currentEditorView()->editor, SIGNAL(copyAvailable(bool)), CopyAct, SLOT(setEnabled(bool)));
			currentEditorView()->editor->setLastNumLines(currentEditorView()->editor->numoflines());
			connect(currentEditorView()->editor, SIGNAL(blockCountChanged(int)), this, SLOT(refreshAllFromCursor(int)));
			connect(currentEditorView()->editor, SIGNAL(requestGotoStructure(int)),this, SLOT(jumpToStructure(int)));
			UpdateStructure();
			UpdateBibliography();
			QApplication::restoreOverrideCursor();
			}
		EditorView->setCurrentIndex(EditorView->indexOf(temp));
		UpdateCaption();
		OutputTextEdit->clear();
		OutputTableWidget->hide();
		//OutputTextEdit->insertLine("Editor settings apply only to new loaded document.");
		currentEditorView()->editor->setFocus();
		}
	}
}

void Texmaker::ToggleMode()
{
//QAction *action = qobject_cast<QAction *>(sender());
if (!singlemode)
	{
	ToggleAct->setText(tr("Define Current Document as 'Master Document'"));
	OutputTextEdit->clear();
	OutputTableWidget->hide();
	logpresent=false;
	singlemode=true;
	listbibfiles.clear();
	listchildfiles.clear();
	stat1->setText(QString(" %1 ").arg(tr("Normal Mode")));
	ToggleDocAct->setEnabled(false);
	return;
	}
if (singlemode && currentEditorView())  
	{
	MasterName=getName();
	if (MasterName.startsWith("untitled") || MasterName=="")
		{
		QMessageBox::warning( this,tr("Error"),tr("Could not start the command."));
		return;
		}
	QString shortName = MasterName;
	int pos;
	while ( (pos = (int)shortName.indexOf('/')) != -1 ) shortName.remove(0,pos+1);
	ToggleAct->setText(tr("Normal Mode (current master document :")+shortName+")");
	singlemode=false;
	stat1->setText(QString(" %1 ").arg(tr("Master Document :")+shortName));
	ToggleDocAct->setEnabled(true);
	return;
	}
}

void Texmaker::onOtherInstanceMessage(const QString &msg)  // Added slot for messages to the single instance
{
QStringList argv = msg.split("#!#");
int argc = argv.size();
for ( int i = 1; i < argc; ++i )
	{
	QString arg = argv[ i ];
	if ( arg[0] != '-' )    load( arg );
	if ( arg == "-master" ) ToggleMode();
	if (( arg == "-line" ) && (i<argc-1))  setLine( argv[ ++i ] );
	}
//A bad (but applicable) trick for activating Texmaker MainWindow //add by S. R. Alavizadeh
//setWindowState(Qt::WindowMinimized);
//setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
//show();
}
////////////////// VIEW ////////////////

void Texmaker::gotoNextDocument()
{
if ( EditorView->count() < 2 ) return;
int cPage = EditorView->currentIndex() + 1;
if ( cPage >= EditorView->count() ) EditorView->setCurrentIndex( 0 );
else EditorView->setCurrentIndex( cPage );
}

void Texmaker::gotoPrevDocument()
{
if ( EditorView->count() < 2 ) return;
int cPage = EditorView->currentIndex() - 1;
if ( cPage < 0 ) EditorView->setCurrentIndex( EditorView->count() - 1 );
else EditorView->setCurrentIndex( cPage );
}

void Texmaker::SetInterfaceFont()
{
#if defined( Q_WS_X11 )
X11FontDialog *xfdlg = new X11FontDialog(this);
int ft=xfdlg->ui.comboBoxFont->findText (x11fontfamily , Qt::MatchExactly);
xfdlg->ui.comboBoxFont->setCurrentIndex(ft);
xfdlg->ui.spinBoxSize->setValue(x11fontsize);
if (xfdlg->exec())
	{
	x11fontfamily=xfdlg->ui.comboBoxFont->currentText();
	x11fontsize=xfdlg->ui.spinBoxSize->value();
	QFont x11Font (x11fontfamily,x11fontsize);
	QApplication::setFont(x11Font);
	}
#endif
}

void Texmaker::gotoBookmark1()
{
if ( !currentEditorView() ) return;
int l=currentEditorView()->editor->UserBookmark[0];
if (l>0) currentEditorView()->editor->gotoLine(l-1);
}

void Texmaker::gotoBookmark2()
{
if ( !currentEditorView() ) return;
int l=currentEditorView()->editor->UserBookmark[1];
if (l>0) currentEditorView()->editor->gotoLine(l-1);
}

void Texmaker::gotoBookmark3()
{
if ( !currentEditorView() ) return;
int l=currentEditorView()->editor->UserBookmark[2];
if (l>0) currentEditorView()->editor->gotoLine(l-1);
}

//*********************************
void Texmaker::dragEnterEvent(QDragEnterEvent *event)
{
if (event->mimeData()->hasFormat("text/uri-list")) event->acceptProposedAction();
}

void Texmaker::dropEvent(QDropEvent *event)
{
#if defined(Q_WS_WIN)
QRegExp rx("file:(/+)(.*\\.(?:tex|bib|sty|cls|mp))");
#else
QRegExp rx("file:(//)(.*\\.(?:tex|bib|sty|cls|mp))");
#endif
QList<QUrl> uris=event->mimeData()->urls();
QString uri;
for (int i = 0; i < uris.size(); ++i)
	{
	uri=uris.at(i).toString();
	if (rx.exactMatch(uri)) {load(rx.cap(2));}
	}
event->acceptProposedAction();
}

void Texmaker::changeEvent(QEvent *e)
{
QMainWindow::changeEvent(e);
if (e->type() == QEvent::ActivationChange) 
  {
  if (isActiveWindow()) emit windowActivated();
  } 
}
//***********************************
void Texmaker::SetMostUsedSymbols()
{
for ( int i = 0; i <=11; ++i ) symbolMostused[i]=-1;
QList<int> list_num, list_score;
list_num.clear();
list_score.clear();
for (int i=0; i <412 ; i++)
	{
	list_num.append(i);
	list_score.append(symbolScore[i]);
	}
int max;
for (int i = 0; i < 412; i++)
	{
	max=i;
	for (int j = i+1; j < 412; j++) 
		{
		if (list_score.at(j)>list_score.at(max)) max=j;
		}
	if (max!=i) 
		{
		list_num.swap(i,max);
		list_score.swap(i,max);
		}
	}
for ( int i = 0; i <=11; ++i ) 
	{
	if (list_score.at(i)>0) symbolMostused[i]=list_num.at(i);
	}
MostUsedListWidget->SetUserPage(symbolMostused);
}

void Texmaker::InsertFavoriteSymbols()
{
QString actData;
QAction *action = qobject_cast<QAction *>(sender());
if (action)
	{
	actData=action->data().toString();
	QRegExp rxnumber(";([0-9]+)");
	int number=-1;
	if (!actData.isEmpty())
		{
		if ( rxnumber.indexIn(actData) != -1) number=rxnumber.cap(1).toInt();
		if (!favoriteSymbolList.contains(number)) favoriteSymbolList.append(number);
		FavoriteListWidget->SetFavoritePage(favoriteSymbolList);
		}
	}
}

void Texmaker::RemoveFavoriteSymbols()
{
QString actData;
QAction *action = qobject_cast<QAction *>(sender());
if (action)
	{
	actData=action->data().toString();
	QRegExp rxnumber(";([0-9]+)");
	int number=-1;
	if (!actData.isEmpty())
		{
		if ( rxnumber.indexIn(actData) != -1) number=rxnumber.cap(1).toInt();
		if (favoriteSymbolList.contains(number)) favoriteSymbolList.removeOne(number);
		FavoriteListWidget->SetFavoritePage(favoriteSymbolList);
		}
	}
}



void Texmaker::ModifyShortcuts()
{
KeysMap::Iterator its;
QString d,f,s;
QList<QAction *> listaction;
listaction << latex1Menu->actions();
listaction << latex11Menu->actions();
listaction << latex12Menu->actions();
listaction << latex13Menu->actions();
listaction << latex14Menu->actions();
listaction << latex15Menu->actions();
listaction << latex16Menu->actions();
listaction << latex17Menu->actions();
listaction << math1Menu->actions();
listaction << math11Menu->actions();
listaction << math12Menu->actions();
listaction << math13Menu->actions();
listaction << math14Menu->actions();
QListIterator<QAction*> iterator(listaction);
actionstext.clear();
while ( iterator.hasNext() )
	{
	QAction *action=iterator.next();
        if (action && (!action->menu())  && (!action->data().toString().isEmpty())) 
		{
		actionstext.insert(action->data().toString(),action->text());
		d=action->data().toString().section("/",0,0);
		for( its = shortcuts.begin(); its != shortcuts.end(); ++its )
			{
			f=its.key().section("/",0,0);
			s=its.value();
			if (f==d && s!="none" && !s.isEmpty()) action->setShortcut(QKeySequence(s));
			}
// 		its=shortcuts.find(action->data().toString());
// 		if (its!=shortcuts.end()) 
// 			{
// 			s=*its;
// 			if (s!="none" && !s.isEmpty()) action->setShortcut(s);
// 			}
		//QString s=*shortcuts.find(action->data().toString());
		//if (s!="none" && !s.isEmpty()) action->setShortcut(s);
		}
	}
}

void Texmaker::updateCompleter()
{
QTextCodec *codec = QTextCodec::codecForName("UTF-8");
QAbstractItemModel *model;
QFile tagsfile(":/completion/completion.txt");

#if defined( Q_WS_X11 )//add by S. R. Alavizadeh

#ifdef USB_VERSION
QFile userTagsfile(QCoreApplication::applicationDirPath()+"/completion.txt");
#else
QFile userTagsfile(PREFIX"/share/texmaker/completion.txt");
#endif

#endif
#if defined( Q_WS_MACX )
QFile userTagsfile(QCoreApplication::applicationDirPath() + "/../Resources/completion.txt");
#endif
#if defined(Q_WS_WIN)
QFile userTagsfile(QCoreApplication::applicationDirPath()+"/completion.txt");
#endif
if (!tagsfile.open(QFile::ReadOnly)) model=new QStringListModel(completer);
QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
QStringList words;
QString line;
QTextStream tscompleter(&tagsfile);
tscompleter.setCodec(codec);
while (!tscompleter.atEnd()) 
	{
	line = tscompleter.readLine();
	if (!line.isEmpty()) words.append(line.remove("\n"));
	}
	

if (userTagsfile.open(QFile::ReadOnly))
    {
    QTextStream usertscompleter(&userTagsfile);
    usertscompleter.setCodec(codec);
    while (!usertscompleter.atEnd()) 
	    {
	    line = usertscompleter.readLine();
	    if (!line.isEmpty()) words.append(line.remove("\n"));
	    }
    }
for (int i=0; i<bibitem.count();++i) 
	{
	words.append("\\cite{"+bibitem.at(i)+"}");
	words.append("\\footcite{"+bibitem.at(i)+"}");
	}
for (int i=0; i<labelitem.count();++i) 
	{
	words.append("\\ref{"+labelitem.at(i)+"}");
	words.append("\\pageref{"+labelitem.at(i)+"}");
	}
for (int i=0; i<userCompletionList.count();++i) 
	{
	words.append(userCompletionList.at(i));
	}
words.removeDuplicates();
words.sort();

QApplication::restoreOverrideCursor();
model=new QStringListModel(words, completer);
completer->setModel(model);
completer->setModelSorting(QCompleter::CaseSensitivelySortedModel);
completer->setCaseSensitivity(Qt::CaseSensitive);
completer->setWrapAround(false);
}

bool Texmaker::spelldicExist()
{
QString affdic=spell_dic.left(spell_dic.length()-4)+".aff";
QFileInfo fidic(spell_dic);
QFileInfo fiaff(affdic);
return (fidic.exists() && fidic.isReadable() && fiaff.exists() && fiaff.isReadable());
}

void Texmaker::updateTranslation()
{
QAction *action = qobject_cast<QAction *>(sender());
QString lang=action->text();
TexmakerApp::instance()->language=lang;
QMessageBox::information( this,"Texmaker",tr("The language setting will take effect after restarting the application."));
}

void Texmaker::updateAppearance()
{
QAction *action = qobject_cast<QAction *>(sender());
QString style=action->text();
modern_style=(action->text()=="Modern");
QMessageBox::information( this,"Texmaker",tr("The appearance setting will take effect after restarting the application."));
}

void Texmaker::disableToolsActions()
{
QList<QAction *> listaction;
listaction << toolMenu->actions();
listaction << user12Menu->actions();
//listaction << optionsMenu->actions();
listaction << runToolBar->actions();
QListIterator<QAction*> iterator(listaction);
while ( iterator.hasNext() )
	{
	QAction *action=iterator.next();
	action->setEnabled(false);
	}
QTimer::singleShot(5000,this, SLOT(enableToolsActions()));
}

void Texmaker::enableToolsActions()
{
QList<QAction *> listaction;
listaction << toolMenu->actions();
listaction << user12Menu->actions();
//listaction << optionsMenu->actions();
listaction << runToolBar->actions();
QListIterator<QAction*> iterator(listaction);
while ( iterator.hasNext() )
	{
	QAction *action=iterator.next();
	action->setEnabled(true);
	}
}

void Texmaker::clipboardDataChanged()
{
if ( !currentEditorView() ) return;
PasteAct->setEnabled(!QApplication::clipboard()->text().isEmpty());
}

void Texmaker::refreshAll()
{
disconnect(currentEditorView()->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
currentEditorView()->editor->highlighter->rehighlight();
UpdateStructure();
connect(currentEditorView()->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
}

void Texmaker::refreshAllFromCursor(int newnumlines)
{
/*disconnect(currentEditorView()->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));
int oldnumlines=currentEditorView()->editor->getLastNumLines();
int delta=newnumlines-oldnumlines;
QTextBlock p = currentEditorView()->editor->textCursor().block();
int currentline=p.blockNumber();
int l;
QList<int> listofmodifiedlines;
const QList<LatexEditor::StructItem>& structure = currentEditorView()->editor->getStructItems();
for (int j = 0; j < structure.count(); j++)
  {
  l=structure.at(j).line;
  if (l>=currentline) 
    {
    listofmodifiedlines.append(l+delta);
    }
   }
int i=currentline;
while (p.isValid())
  {
  if (listofmodifiedlines.contains(i))
    {
    currentEditorView()->editor->highlighter->rehighlightBlock(p); 
    }
  i++;
  p = p.next();
  }
refreshRange();
//UpdateStructure();
connect(currentEditorView()->editor, SIGNAL(requestUpdateStructure()), this, SLOT(UpdateStructure()));*/
refreshRange();
currentEditorView()->editor->setLastNumLines(newnumlines);
}

void Texmaker::refreshRange()
{
currentEditorView()->editor->foldableLines.clear();
int endpreamble = currentEditorView()->editor->searchLine("\\begin{document}");
if (endpreamble>1) currentEditorView()->editor->foldableLines.insert(0,endpreamble-1);
int nb=StructureTreeWidget->topLevelItemCount();
if (nb>0)
    {
    for (int i = 0; i < nb; i++) 
	{
	ParseTree(StructureTreeWidget->topLevelItem(i));
	}
    }
}

void Texmaker::jumpToStructure(int line)
{
const QList<LatexEditor::StructItem>& structure = currentEditorView()->editor->getStructItems();
int index=-1;
for (int j = 0; j < structure.count(); j++)
    {
    if (structure.at(j).cursor.block().blockNumber()==line) {index=j; break;}
    } 
if (index<0) return;
QList<QTreeWidgetItem *> fItems=StructureTreeWidget->findItems (structure.at(index).item,Qt::MatchRecursive,0);

if (fItems.size()>0 ) 
  {
  for (int i = 0;  i< fItems.size(); i++)
    {
    if ((fItems.at(i)) && (fItems.at(i)->text(1)==QString::number(index))) 
      {
      StructureTreeWidget->scrollToItem(fItems.at(i),QAbstractItemView::EnsureVisible);
      StructureTreeWidget->setCurrentItem(fItems.at(i));
      }
    }
  }
}

void Texmaker::mainWindowActivated()
{
QTimer::singleShot(0, this, SLOT(checkModifiedOutsideAll()));
}


void Texmaker::ToggleStructurePanel()
{
ShowStructView(true);
}

void Texmaker::ToggleLogPanel()
{
ShowOutputView(true);
}

void Texmaker::TogglePdfPanel()
{
ShowPdfView(true);
}

void Texmaker::ShowStructView(bool change)
{
if (change) showstructview=!showstructview;
if (showstructview)
   {
   LeftPanelFrameBis->show();
   }
else
   {
   LeftPanelFrameBis->hide();
   }
ViewStructurePanelAct->setChecked(showstructview);
toggleStructureButton->setEnabled(showstructview);
}

void Texmaker::ShowOutputView(bool change)
{
if (change) showoutputview=!showoutputview;
if (showoutputview)
    {
    Outputframe->show();
    }
else
   {
   Outputframe->hide();
   }
ViewLogPanelAct->setChecked(showoutputview);
toggleLogButton->setEnabled(showoutputview);
}

void Texmaker::ShowPdfView(bool change)
{
if (change) showpdfview=!showpdfview;
if (showpdfview)
    {
    StackedViewers->show();
    }
else
   {
   StackedViewers->hide();
   }
ViewPdfPanelAct->setChecked(showpdfview);
togglePdfButton->setEnabled(showpdfview);
}

void Texmaker::ToggleFullScreen() 
{
if(FullScreenAct->isChecked()) 
  {
  windowstate=saveState(0);
  setWindowState(Qt::WindowFullScreen);
  restoreState(fullscreenstate,1);
  }
else 
  {
  fullscreenstate=saveState(1);
  setWindowState(Qt::WindowNoState);
  restoreState(windowstate,0);
  }
}

void Texmaker::EditUserCompletion() 
{
UserCompletionDialog *ucDlg = new UserCompletionDialog(this,userCompletionList);
QTextCodec *codec = QTextCodec::codecForName("UTF-8");
QFile tagsfile(":/completion/completion.txt");
if (tagsfile.open(QFile::ReadOnly))
    {
    QTextStream tscompleter(&tagsfile);
    tscompleter.setCodec(codec);
    ucDlg->ui.plainTextEdit->setPlainText( tscompleter.readAll() );
    }
if ( ucDlg->exec() )
	{
	userCompletionList=ucDlg->userlist;
	updateCompleter();
	}
}

