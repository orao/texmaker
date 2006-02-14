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

#ifndef TEXMAKER_H
#define TEXMAKER_H

#include <QMainWindow>
#include <QDockWidget>
#include <QMenuBar>
#include <QToolBar>
#include <QListWidget>
#include <QToolBox>
#include <QTabWidget>
#include <QTreeWidget>
#include <QLabel>
#include <QAction>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QPointer>
#include <QProcess>
#include <QPushButton>

#include "latexeditorview.h"
#include "symbollistwidget.h"
#include "metapostlistwidget.h"
#include "logeditor.h"
#include "gotolinedialog.h"
#include "replacedialog.h"
#include "finddialog.h"
#include "structdialog.h"
#include "filechooser.h"
#include "tabdialog.h"
#include "arraydialog.h"
#include "tabbingdialog.h"
#include "letterdialog.h"
#include "quickdocumentdialog.h"
#include "usermenudialog.h"
#include "usertooldialog.h"
#include "helpwidget.h"
#include "refdialog.h"
#include "configdialog.h"
#include "aboutdialog.h"

#if defined( Q_WS_X11 ) || defined( Q_WS_MACX )
#include "webpublishdialog.h"
#endif

#if defined( Q_WS_X11 )
#include "x11fontdialog.h"
#endif

typedef  QMap<LatexEditorView*, QString> FilesMap;
typedef  QString Userlist[10];
typedef  QString UserCd[5];

class Texmaker : public QMainWindow
{
    Q_OBJECT

public:
    Texmaker(QWidget *parent = 0, Qt::WFlags flags = 0);

QString getName();
QFont EditorFont;

public slots:
void load( const QString &f );
void setLine( const QString &line );

private:
void setupMenus();
void setupToolBars();
void createStatusBar();
bool FileAlreadyOpen(QString f);
void closeEvent(QCloseEvent *e);

FilesMap filenames;
//gui
QDockWidget *OutputView, *StructureView;
QTabWidget *EditorView;
LogEditor* OutputTextEdit;
QToolBox *StructureToolbox;
MetapostListWidget *MpListWidget;
SymbolListWidget *RelationListWidget, *ArrowListWidget, *MiscellaneousListWidget, *DelimitersListWidget, *GreekListWidget;
QTreeWidget *StructureTreeWidget;
//menu-toolbar
QMenu *fileMenu, *recentMenu, *editMenu, *toolMenu;
QMenu *latex1Menu, *latex11Menu, *latex12Menu, *latex13Menu, *latex14Menu, *latex15Menu, *latex16Menu ;
QMenu *math1Menu, *math11Menu, *math12Menu, *math13Menu;
QMenu *wizardMenu;
QMenu *bibMenu;
QMenu *user1Menu, *user11Menu, *user12Menu;
QMenu *viewMenu;
QMenu *optionsMenu;
QMenu *helpMenu;

QToolBar *fileToolBar, *editToolBar, *runToolBar, *formatToolBar, *mathToolBar;
QAction *recentFileActs[5];

QLabel *stat1, *stat2;
QPushButton *pb1, *pb2, *pb3;
QString MasterName;
bool logpresent;
QStringList recentFilesList;
//settings
int split1_right, split1_left, split2_top, split2_bottom, quickmode;
bool singlemode, wordwrap, parenmatch, showline, showoutputview, showstructview, ams_packages, makeidx_package;
QString document_class, typeface_size, paper_size, document_encoding, author;
QString latex_command, viewdvi_command, dvips_command, dvipdf_command, metapost_command;
QString viewps_command, ps2pdf_command, makeindex_command, bibtex_command, pdflatex_command, viewpdf_command, userquick_command;
QString lastDocument, input_encoding;
QString struct_level1, struct_level2, struct_level3, struct_level4, struct_level5;
QStringList userClassList, userPaperList, userEncodingList, userOptionsList;
QStringList structlist, labelitem, structitem;
Userlist UserMenuName, UserMenuTag;
UserCd UserToolName, UserToolCommand;
//dialogs
QPointer<FindDialog> findDialog;
QPointer<ReplaceDialog> replaceDialog;
QPointer<GotoLineDialog> gotoLineDialog;
QPointer<HelpWidget> help_widget;
StructDialog *stDlg;
FileChooser *sfDlg;
TabDialog *quickDlg;
ArrayDialog *arrayDlg;
TabbingDialog *tabDlg;
LetterDialog *ltDlg;
QuickDocumentDialog *startDlg;
UserMenuDialog *umDlg;
UserToolDialog *utDlg;
RefDialog *refDlg;
ConfigDialog *confDlg;
#if defined( Q_WS_X11 ) || defined( Q_WS_MACX )
WebPublishDialog *ttwpDlg;
#endif
AboutDialog *abDlg;
//tools
QProcess *proc;
bool FINPROCESS, ERRPROCESS;
//latex errors
QStringList errorList;
int errorIndex;
//X11
#if defined( Q_WS_X11 )
QString x11style;
QString x11fontfamily;
int x11fontsize;
X11FontDialog *xfdlg;
#endif
private slots:
LatexEditorView *currentEditorView() const;
void fileNew();
void fileOpen();
void fileSave();
void fileSaveAll();
void fileSaveAs();
void fileClose();
void fileCloseAll();
void fileExit();
void fileOpenRecent();
void AddRecentFile(const QString &f);
void UpdateRecentFile();

void editUndo();
void editRedo();
void editCut();
void editCopy();
void editPaste();
void editSelectAll();
void editFind();
void editFindNext();
void editReplace();
void editGotoLine();
void editComment();
void editUncomment();
void editIndent();

void ReadSettings();
void SaveSettings();

void NewDocumentStatus(bool m);
void UpdateCaption();

void UpdateStructure();
void ShowStructure();
void ClickedOnStructure(QTreeWidgetItem *item,int);

void InsertTag(QString Entity, int dx, int dy);
void InsertSymbol(QTableWidgetItem *item);
void InsertMetaPost(QListWidgetItem *item);
void InsertFromAction();
void InsertWithSelectionFromAction();
void InsertWithSelectionFromString(const QString& text);
void InsertFromString(const QString& text);
void InsertBib();
void InsertStruct();
void InsertStructFromString(const QString& text);
void InsertImage();
void InsertInclude();
void InsertInput();

void InsertBib1();
void InsertBib2();
void InsertBib3();
void InsertBib4();
void InsertBib5();
void InsertBib6();
void InsertBib7();
void InsertBib8();
void InsertBib9();
void InsertBib10();
void InsertBib11();
void InsertBib12();
void InsertBib13();
void CleanBib();

void InsertUserTag1();
void InsertUserTag2();
void InsertUserTag3();
void InsertUserTag4();
void InsertUserTag5();
void InsertUserTag6();
void InsertUserTag7();
void InsertUserTag8();
void InsertUserTag9();
void InsertUserTag10();
void EditUserMenu();

void SectionCommand(const QString& text);
void OtherCommand(const QString& text);
void InsertRef();
void InsertPageRef();
void SizeCommand(const QString& text);
void LeftDelimiter(const QString& text);
void RightDelimiter(const QString& text);

void QuickTabular();
void QuickArray();
void QuickTabbing();
void QuickLetter();
void QuickDocument();

void RunCommand(QString comd,bool waitendprocess);
void readFromStderr();
//void readFromStdoutput();
void SlotEndProcess(int err);
void QuickBuild();
void Latex();
void ViewDvi();
void DviToPS();
void ViewPS();
void PDFLatex();
void ViewPDF();
void CleanAll();
void MakeBib();
void MakeIndex();
void PStoPDF();
void DVItoPDF();
void MetaPost();
void UserTool1();
void UserTool2();
void UserTool3();
void UserTool4();
void UserTool5();
void EditUserTool();

void WebPublish();

void ViewLog();
void ClickedOnOutput(int l);
void QuickLatexError();
void LatexError();
void NextError();
void PreviousError();

void LatexHelp();
void UserManualHelp();
void HelpAbout();

void GeneralOptions();

void gotoNextDocument();
void gotoPrevDocument();
void ToggleMode();

void SetInterfaceFont();

void gotoBookmark1();
void gotoBookmark2();
void gotoBookmark3();
};

#endif
