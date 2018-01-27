TEMPLATE	= app
LANGUAGE	= C++
TARGET	 = texmaker
QT += network
CONFIG	+= qt warn_off release
###############################
HEADERS	+= texmaker.h \
	dsingleapplication.h \
	symbollistwidget.h \
	icondelegate.h \
	latexeditor.h \
	latexhighlighter.h \
	latexeditorview.h \
	linenumberwidget.h \
	logeditor.h \
	loghighlighter.h \
	metapostlistwidget.h \
	findwidget.h \
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
	refdialog.h \
	configdialog.h \
	aboutdialog.h \
	webpublishdialog.h \
	spellerdialog.h \
	pstrickslistwidget.h \
	blockdata.h \
	parenmatcher.h \
	hunspell/affentry.hxx \
	hunspell/affixmgr.hxx \
	hunspell/atypes.hxx \
	hunspell/baseaffix.hxx \
	hunspell/csutil.hxx \
	hunspell/dictmgr.hxx \
	hunspell/hashmgr.hxx \
	hunspell/htypes.hxx \
	hunspell/hunspell.hxx \
	hunspell/hunspell.h \
	hunspell/langnum.hxx \
	hunspell/license.hunspell \
	hunspell/phonet.hxx \
	hunspell/suggestmgr.hxx \
	hunspell/license.myspell \
	hunspell/filemgr.hxx \
	hunspell/hunzip.hxx \
	hunspell/w_char.hxx
SOURCES	+= main.cpp \
	dsingleapplication.cpp \
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
	findwidget.cpp \
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
	refdialog.cpp \
	configdialog.cpp \
	aboutdialog.cpp \
	webpublishdialog.cpp \
	spellerdialog.cpp \
	pstrickslistwidget.cpp \
	parenmatcher.cpp \
	hunspell/affentry.cxx \
	hunspell/affixmgr.cxx \
	hunspell/csutil.cxx \
	hunspell/dictmgr.cxx \
	hunspell/hashmgr.cxx \
	hunspell/hunspell.cxx \
	hunspell/phonet.cxx \
	hunspell/suggestmgr.cxx \
	hunspell/utf_info.cxx \
	hunspell/filemgr.cxx \
	hunspell/hunzip.cxx
RESOURCES += texmaker.qrc
FORMS   += findwidget.ui\
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
	aboutdialog.ui \
	webpublishdialog.ui \
	configdialog.ui \
	spellerdialog.ui
TRANSLATIONS += texmaker_fr.ts
################################
unix:!macx {
UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj
isEmpty( PREFIX ) {
    PREFIX=/usr
}
DEFINES += PREFIX=\\\"$${PREFIX}\\\"
target.path = $${PREFIX}/bin
INSTALLS = target
HEADERS	+= x11fontdialog.h
SOURCES	+= x11fontdialog.cpp
FORMS += x11fontdialog.ui
utilities.path = $${PREFIX}/share/texmaker
utilities.files = utilities/blank.png \
	utilities/doc1.png \
	utilities/doc10.png \
	utilities/doc11.png \
	utilities/doc12.png \
	utilities/doc13.png \
	utilities/doc14.png \
	utilities/doc15.png \
	utilities/doc16.png \
	utilities/doc17.png \
	utilities/doc18.png \
	utilities/doc19.png \
	utilities/doc2.png \
	utilities/doc20.png \
	utilities/doc3.png \
	utilities/doc4.png \
	utilities/doc5.png \
	utilities/doc6.png \
	utilities/doc7.png \
	utilities/doc8.png \
	utilities/doc9.png \
	utilities/latexhelp.html \
	utilities/next.gif \
	utilities/next_d.gif \
	utilities/prev.gif \
	utilities/prev_d.gif \
	utilities/psheader.txt \
	utilities/style.css \
	utilities/texmaker_fr.qm \
	utilities/texmaker_de.qm \
	utilities/texmaker_zh.qm \
	utilities/qt_fr.qm \
	utilities/qt_de.qm \
	utilities/qt_zh.qm \
	utilities/up.gif \
	utilities/up_d.gif \
	utilities/usermanual_en.html \
	utilities/usermanual_fr.html \
	utilities/en_GB.aff \
	utilities/en_GB.dic \
	utilities/fr_FR.aff \
	utilities/fr_FR.dic \
	utilities/AUTHORS \
	utilities/COPYING \
	utilities/CHANGELOG.txt \
	utilities/texmaker16x16.png \
	utilities/texmaker22x22.png \
	utilities/texmaker32x32.png \
	utilities/texmaker48x48.png \
	utilities/texmaker64x64.png \
	utilities/texmaker128x128.png \
	utilities/texmaker.desktop \
	utilities/texmaker.svg
INSTALLS += utilities
}
################################
win32 {
INCLUDEPATH += C:\MinGW\include
RC_FILE = win.rc
}
###############################
macx {
UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj
##universal tiger
CONFIG += link_prl x86 ppc
QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.4
target.path = /Applications
INSTALLS = target
utilities.path = Contents/Resources
utilities.files = utilities/blank.png \
	utilities/doc1.png \
	utilities/doc10.png \
	utilities/doc11.png \
	utilities/doc12.png \
	utilities/doc13.png \
	utilities/doc14.png \
	utilities/doc15.png \
	utilities/doc16.png \
	utilities/doc17.png \
	utilities/doc18.png \
	utilities/doc19.png \
	utilities/doc2.png \
	utilities/doc20.png \
	utilities/doc3.png \
	utilities/doc4.png \
	utilities/doc5.png \
	utilities/doc6.png \
	utilities/doc7.png \
	utilities/doc8.png \
	utilities/doc9.png \
	utilities/latexhelp.html \
	utilities/next.gif \
	utilities/next_d.gif \
	utilities/prev.gif \
	utilities/prev_d.gif \
	utilities/psheader.txt \
	utilities/style.css \
	utilities/texmaker_fr.qm \
	utilities/texmaker_de.qm \
	utilities/texmaker_zh.qm \
	utilities/qt_fr.qm \
	utilities/qt_de.qm \
	utilities/qt_zh.qm \
	utilities/up.gif \
	utilities/up_d.gif \
	utilities/usermanual_en.html \
	utilities/usermanual_fr.html \
	utilities/en_GB.aff \
	utilities/en_GB.dic \
	utilities/fr_FR.aff \
	utilities/fr_FR.dic \
	utilities/AUTHORS \
	utilities/COPYING \
	utilities/CHANGELOG.txt
QMAKE_BUNDLE_DATA += utilities
INSTALLS += utilities
ICON = texmaker.icns
QMAKE_INFO_PLIST =Info.plist
}
