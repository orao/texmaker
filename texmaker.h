/***************************************************************************
 *   copyright       : (C) 2003-2011 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *   addons by Luis Silvestre ; Tom Hoffmann                               *
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
#include <QStackedWidget>
#include <QTabWidget>
#include <QTreeWidget>
#include <QListWidget>
#include <QLabel>
#include <QAction>
#include <QActionGroup>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QTableWidgetItem>
#include <QPointer>
#include <QProcess>
#include <QPushButton>
#include <QColor>
#include <QCompleter>
#include <QTextTable>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTranslator>
#include <QToolButton>

#include "latexeditorview.h"
#include "minisplitter.h"
#include "playerbutton.h"
#include "symbollistwidget.h"
#include "xmltagslistwidget.h"
#include "logeditor.h"
#include "gotolinedialog.h"
#include "replacedialog.h"
#include "hunspell/hunspell.hxx"
#include "browser.h"
#include "pdfviewerwidget.h"
#include "pdfviewer.h"
#include "encodingprober/qencodingprober.h"



typedef  QMap<LatexEditorView*, QString> FilesMap;
typedef  QMap<QString,QString> KeysMap;
typedef  QString Userlist[10];
typedef  QString UserCd[5];
typedef int SymbolList[412];

class Texmaker : public QMainWindow
{
    Q_OBJECT

public:
    Texmaker(QWidget *parent = 0, Qt::WFlags flags = 0);
    ~Texmaker();
QString getName();
QFont EditorFont;
QByteArray windowstate,splitter1state,splitter2state,fullscreenstate;

public slots:
void load( const QString &f );
void setLine( const QString &line );
void ToggleMode();
void onOtherInstanceMessage(const QString &);  // For messages for the single instance

private:
void setupMenus();
void setupToolBars();
void createStatusBar();
bool FileAlreadyOpen(QString f);
void closeEvent(QCloseEvent *e);
int LevelItem(const QTreeWidgetItem *item);
int LineItem(const QTreeWidgetItem *item);
bool currentfileSaved();
bool isCurrentModifiedOutside();

int untitled_id;

FilesMap filenames;
KeysMap shortcuts, actionstext;
//gui
QFrame *LeftPanelFrameBis, *Outputframe;
MiniSplitter *splitter1, *splitter2 ;
PlayerButton *toggleStructureButton, *toggleLogButton, *togglePdfButton; 
bool embedinternalpdf;

QStackedWidget *EditorView;
QStackedWidget *StackedViewers;
LogEditor* OutputTextEdit;

QStackedWidget *LeftPanelStackedWidget;
XmlTagsListWidget *MpListWidget, *PsListWidget, *leftrightWidget, *tikzWidget, *asyWidget;
SymbolListWidget *RelationListWidget, *ArrowListWidget, *MiscellaneousListWidget, *DelimitersListWidget, *GreekListWidget, *MostUsedListWidget, *FavoriteListWidget;
QTreeWidget *StructureTreeWidget;
QVBoxLayout *OutputLayoutV, *CentralLayoutBis,*LeftPanelLayoutBis,;
QHBoxLayout *OutputLayoutH, *LeftPanelLayout, *CentralLayout;
QTableWidget *OutputTableWidget;
//menu-toolbar
QMenu *fileMenu, *recentMenu, *editMenu, *toolMenu;
QMenu *latex1Menu, *latex11Menu, *latex12Menu, *latex13Menu, *latex14Menu, *latex15Menu, *latex16Menu, *latex17Menu ;
QMenu *math1Menu, *math11Menu, *math12Menu, *math13Menu, *math14Menu;
QMenu *wizardMenu;
QMenu *bibMenu;
QMenu *user1Menu, *user11Menu, *user12Menu;
QMenu *viewMenu;
QMenu *optionsMenu, *translationMenu, *appearanceMenu;
QMenu *helpMenu;

QToolBar *fileToolBar, *editToolBar, *runToolBar, *formatToolBar, *logToolBar, *LeftPanelToolBar,*LeftPanelToolBarBis, *centralToolBar, *centralToolBarBis;
QAction *recentFileActs[10], *ToggleAct, *StopAct, *UndoAct, *RedoAct, *SaveAct, *CutAct, *CopyAct,*PasteAct, *ToggleDocAct, *ViewStructurePanelAct, *ViewLogPanelAct, *ViewPdfPanelAct, *FullScreenAct ;
QComboBox *comboCompil, *comboView, *comboFiles;
QLabel *stat1, *stat3, *titleLeftPanel;
QPushButton *pb1, *pb2, *pb3;
QString MasterName;
bool logpresent;
QStringList recentFilesList, sessionFilesList;
//settings
int split1_right, split1_left, split2_top, split2_bottom, quickmode;
bool singlemode, wordwrap, parenmatch, showline, showoutputview, showstructview, showpdfview, ams_packages, makeidx_package, completion, inlinespellcheck, modern_style, new_gui, builtinpdfview, singleviewerinstance ;
QString document_class, typeface_size, paper_size, document_encoding, author;
QString latex_command, viewdvi_command, dvips_command, dvipdf_command, metapost_command, psize;
QString viewps_command, ps2pdf_command, makeindex_command, bibtex_command, pdflatex_command, viewpdf_command, userquick_command, ghostscript_command, asymptote_command, latexmk_command;
QString spell_dic, spell_ignored_words;
QString lastDocument, input_encoding, lastChild;
QString struct_level1, struct_level2, struct_level3, struct_level4, struct_level5;
QStringList userClassList, userPaperList, userEncodingList, userOptionsList, userCompletionList;
QStringList labelitem, bibitem, listbibfiles, listchildfiles;
Userlist UserMenuName, UserMenuTag;
UserCd UserToolName, UserToolCommand;
//dialogs
QPointer<ReplaceDialog> replaceDialog;
QPointer<GotoLineDialog> gotoLineDialog;
QPointer<Browser> browserWindow;
QPointer<PdfViewerWidget> pdfviewerWidget;
QPointer<PdfViewer> pdfviewerWindow;

//tools
QProcess *proc;
bool FINPROCESS, ERRPROCESS, STOPPROCESS;
QStringList listViewerCommands;
bool checkViewerInstance;
//latex errors
QStringList errorFileList, errorTypeList, errorLineList, errorMessageList, errorLogList;
QList<int> onlyErrorList;
int errorIndex, runIndex, viewIndex;

//X11
#if defined( Q_WS_X11 )
QString x11style;
QString x11fontfamily;
int x11fontsize;
#endif
SymbolList symbolScore;
usercodelist symbolMostused;
QList<int> favoriteSymbolList;

QColor colorMath, colorCommand, colorKeyword;

QCompleter *completer;
Hunspell * spellChecker;
bool spelldicExist();
QStringList translationList;
QActionGroup *translationGroup, *appearanceGroup;

private slots:
LatexEditorView *currentEditorView() const;
void fileNew();
void fileNewFromFile();
void fileOpen();
void checkModifiedOutsideAll();
void fileSave();
void fileSaveAll();
void fileSaveAs();
void fileRestoreSession();
void fileClose();
void fileCloseRequested( int index);
void fileCloseAll();
void fileExit();
void fileOpenRecent();
void AddRecentFile(const QString &f);
void UpdateRecentFile();
void filePrint();
void fileOpenAndGoto(const QString &f, int line, bool focus);
void getFocusToEditor();
void fileReload();
void listSelectionActivated(int index);
void ComboFilesInsert(const QString & file);
void ToggleMasterCurrent();

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
void editUnindent();
void editSpell();
void editTipTab();

void ReadSettings();
void SaveSettings();
void setPrintPaperSize(const QString &p);

void NewDocumentStatus(bool m);
void UpdateCaption();

void UpdateStructure();
void UpdateChildsLabels(QStringList listfiles);
void UpdateBibliography();
void ParseTree(QTreeWidgetItem *item);
void ItemToRange(QTreeWidgetItem *item);

void ShowStructure();
void ShowRelation(); //RelationListWidget
void ShowArrow(); //ArrowListWidget
void ShowMisc(); //MiscellaneousListWidget
void ShowDelim(); //DelimitersListWidget
void ShowGreek(); //GreekListWidget
void ShowMostUsed(); //MostUsedListWidget
void ShowFavorite(); //FavoriteListWidget
void ShowPstricks(); //PsListWidget
void ShowLeftRight(); //leftrightWidget
void ShowMplist(); //MpListWidget
void ShowTikz(); //tikzWidget
void ShowAsy(); //asyWidget
void ClickedOnStructure(QTreeWidgetItem *item,int);

void InsertTag(QString Entity, int dx, int dy);
void InsertSymbol(QTableWidgetItem *item);
void InsertXmlTag(QListWidgetItem *item);
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

void InsertUserTag(QString Entity);
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
void InsertCite();
void InsertRef();
void InsertPageRef();
void SizeCommand(const QString& text);

void QuickTabular();
void QuickArray();
void QuickTabbing();
void QuickLetter();
void QuickDocument();

void RunCommand(QString comd,bool waitendprocess);
void readFromStderr();
void stopProcess();
//void readFromStdoutput();
void SlotEndProcess(int err);
void SlotEndViewerProcess(int err);
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
void Asymptote();
void LatexMk();
void AsyFile(QString asyfile);
void UserTool1();
void UserTool2();
void UserTool3();
void UserTool4();
void UserTool5();
void EditUserTool();
void doCompile();
void doView();
void jumpToPdfline(int line);

void LoadLog();
void ViewLog();
void ClickedOnOutput(int l);
void ClickedOnLogLine(QTableWidgetItem *item);
void LatexError();
void DisplayLatexError();
void NextError();
void PreviousError();
bool NoLatexErrors();
bool LogExists();

/////
void LatexHelp();
void UserManualHelp();
void HelpAbout();

void GeneralOptions();

void gotoNextDocument();
void gotoPrevDocument();

void SetInterfaceFont();

void gotoBookmark1();
void gotoBookmark2();
void gotoBookmark3();

void SetMostUsedSymbols();
void InsertFavoriteSymbols();
void RemoveFavoriteSymbols();


void ModifyShortcuts();

void updateCompleter();
void updateTranslation();
void updateAppearance();

void disableToolsActions();
void enableToolsActions();

void clipboardDataChanged();
void refreshAll();
void refreshAllFromCursor(int newnumlines);
void refreshRange();
void jumpToStructure(int line);
void mainWindowActivated();

void ToggleStructurePanel();
void ToggleLogPanel();
void TogglePdfPanel();
void ShowStructView(bool change);
void ShowOutputView(bool change);
void ShowPdfView(bool change);
void ToggleFullScreen(); 
void EditUserCompletion();
protected:
void dragEnterEvent(QDragEnterEvent *event);
void dropEvent(QDropEvent *event);
virtual void changeEvent(QEvent *e);

signals:
void windowActivated();
    
};

#endif
