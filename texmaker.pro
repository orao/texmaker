unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}

TEMPLATE	= app
LANGUAGE	= C++
TARGET	 = texmaker
CONFIG	+= qt warn_on release
##############################################
unix:!macx {
isEmpty( PREFIX ) {
    PREFIX=/usr
}
DEFINES += PREFIX=\"$${PREFIX}\"
target.path = $${PREFIX}/bin
INSTALLS = target
utilities.path = $${PREFIX}/share/texmaker
utilities.files = utilities/*
INSTALLS += utilities
HEADERS	= texmaker.h \
	symbollistwidget.h \
	icondelegate.h \
	latexeditor.h \
	latexhighlighter.h \
	latexeditorview.h \
	linenumberwidget.h \
	logeditor.h \
	loghighlighter.h \
	metapostlistwidget.h \
	finddialog.h \
	gotolinedialog.h \
	replacedialog.h \
	structdialog.h \
	filechooser.h \
	tabbingdialog.h \
	arraydialog.h \
	tabdialog.h \
	letterdialog.h \
	addoptiondialog.h \
	quickdocumentdialog.h \
	usermenudialog.h \
	usertooldialog.h \
	helpwidget.h \
	refdialog.h \
	configdialog.h \
	pageconfig.h \
	aboutdialog.h \
	webpublishdialog.h \
	x11fontdialog.h
SOURCES	= main.cpp \
	texmaker.cpp \
	symbollistwidget.cpp \
	icondelegate.cpp \
	latexeditor.cpp \
	latexhighlighter.cpp \
	latexeditorview.cpp \
	linenumberwidget.cpp \
	logeditor.cpp \
	loghighlighter.cpp \
	metapostlistwidget.cpp \
	finddialog.cpp \
	gotolinedialog.cpp \
	replacedialog.cpp \
	structdialog.cpp \
	filechooser.cpp \
	tabbingdialog.cpp \
	arraydialog.cpp \
	tabdialog.cpp \
	letterdialog.cpp \
	addoptiondialog.cpp \
	quickdocumentdialog.cpp \
	usermenudialog.cpp \
	usertooldialog.cpp \
	helpwidget.cpp \
	refdialog.cpp \
	configdialog.cpp \
	pageconfig.cpp \
	aboutdialog.cpp \
	webpublishdialog.cpp \
	x11fontdialog.cpp
RESOURCES     = texmaker.qrc
FORMS       = finddialog.ui\
	gotolinedialog.ui \
	replacedialog.ui \
	structdialog.ui \
	filechooser.ui \
	tabbingdialog.ui \
	arraydialog.ui \
	tabdialog.ui \
	letterdialog.ui \
	addoptiondialog.ui \
	quickdocumentdialog.ui \
	usermenudialog.ui \
	usertooldialog.ui \
	refdialog.ui \
	pageeditor.ui \
	pagequick.ui \
	pagetools.ui \
	aboutdialog.ui \
	webpublishdialog.ui \
	x11fontdialog.ui \
	configdialog.ui
TRANSLATIONS = texmaker_fr.ts
}
##############################################
macx {
CONFIG += link_prl
target.path = /Applications
INSTALLS = target
utilities.path = /Applications/texmaker.app/Contents/Resources
utilities.files = utilities/*
INSTALLS += utilities
HEADERS	= texmaker.h \
	symbollistwidget.h \
	icondelegate.h \
	latexeditor.h \
	latexhighlighter.h \
	latexeditorview.h \
	linenumberwidget.h \
	logeditor.h \
	loghighlighter.h \
	metapostlistwidget.h \
	finddialog.h \
	gotolinedialog.h \
	replacedialog.h \
	structdialog.h \
	filechooser.h \
	tabbingdialog.h \
	arraydialog.h \
	tabdialog.h \
	letterdialog.h \
	addoptiondialog.h \
	quickdocumentdialog.h \
	usermenudialog.h \
	usertooldialog.h \
	helpwidget.h \
	refdialog.h \
	configdialog.h \
	pageconfig.h \
	aboutdialog.h \
	webpublishdialog.h
SOURCES	= main.cpp \
	texmaker.cpp \
	symbollistwidget.cpp \
	icondelegate.cpp \
	latexeditor.cpp \
	latexhighlighter.cpp \
	latexeditorview.cpp \
	linenumberwidget.cpp \
	logeditor.cpp \
	loghighlighter.cpp \
	metapostlistwidget.cpp \
	finddialog.cpp \
	gotolinedialog.cpp \
	replacedialog.cpp \
	structdialog.cpp \
	filechooser.cpp \
	tabbingdialog.cpp \
	arraydialog.cpp \
	tabdialog.cpp \
	letterdialog.cpp \
	addoptiondialog.cpp \
	quickdocumentdialog.cpp \
	usermenudialog.cpp \
	usertooldialog.cpp \
	helpwidget.cpp \
	refdialog.cpp \
	configdialog.cpp \
	pageconfig.cpp \
	aboutdialog.cpp \
	webpublishdialog.cpp
RESOURCES     = texmaker.qrc
FORMS       = finddialog.ui\
	gotolinedialog.ui \
	replacedialog.ui \
	structdialog.ui \
	filechooser.ui \
	tabbingdialog.ui \
	arraydialog.ui \
	tabdialog.ui \
	letterdialog.ui \
	addoptiondialog.ui \
	quickdocumentdialog.ui \
	usermenudialog.ui \
	usertooldialog.ui \
	refdialog.ui \
	pageeditor.ui \
	pagequick.ui \
	pagetools.ui \
	aboutdialog.ui \
	webpublishdialog.ui \
	configdialog.ui
TRANSLATIONS = texmaker_fr.ts
ICON = texmaker.icns
}
##############################################
win32 {
INCLUDEPATH = C:\MinGW\include
HEADERS	= texmaker.h \
	symbollistwidget.h \
	icondelegate.h \
	latexeditor.h \
	latexhighlighter.h \
	latexeditorview.h \
	linenumberwidget.h \
	logeditor.h \
	loghighlighter.h \
	metapostlistwidget.h \
	finddialog.h \
	gotolinedialog.h \
	replacedialog.h \
	structdialog.h \
	filechooser.h \
	tabbingdialog.h \
	arraydialog.h \
	tabdialog.h \
	letterdialog.h \
	addoptiondialog.h \
	quickdocumentdialog.h \
	usermenudialog.h \
	usertooldialog.h \
	helpwidget.h \
	refdialog.h \
	configdialog.h \
	pageconfig.h \
	aboutdialog.h
SOURCES	= main.cpp \
	texmaker.cpp \
	symbollistwidget.cpp \
	icondelegate.cpp \
	latexeditor.cpp \
	latexhighlighter.cpp \
	latexeditorview.cpp \
	linenumberwidget.cpp \
	logeditor.cpp \
	loghighlighter.cpp \
	metapostlistwidget.cpp \
	finddialog.cpp \
	gotolinedialog.cpp \
	replacedialog.cpp \
	structdialog.cpp \
	filechooser.cpp \
	tabbingdialog.cpp \
	arraydialog.cpp \
	tabdialog.cpp \
	letterdialog.cpp \
	addoptiondialog.cpp \
	quickdocumentdialog.cpp \
	usermenudialog.cpp \
	usertooldialog.cpp \
	helpwidget.cpp \
	refdialog.cpp \
	configdialog.cpp \
	pageconfig.cpp \
	aboutdialog.cpp
RESOURCES     = texmaker.qrc
RC_FILE = win.rc
FORMS       = finddialog.ui\
	gotolinedialog.ui \
	replacedialog.ui \
	structdialog.ui \
	filechooser.ui \
	tabbingdialog.ui \
	arraydialog.ui \
	tabdialog.ui \
	letterdialog.ui \
	addoptiondialog.ui \
	quickdocumentdialog.ui \
	usermenudialog.ui \
	usertooldialog.ui \
	refdialog.ui \
	pageeditor.ui \
	pagequick.ui \
	pagetools.ui \
	aboutdialog.ui \
	configdialog.ui
TRANSLATIONS = texmaker_fr.ts
}