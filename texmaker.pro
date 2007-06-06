TEMPLATE	= app
LANGUAGE	= C++
TARGET	 = texmaker
CONFIG	+= qt warn_off release
###############################
HEADERS	+= texmaker.h \
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
	refdialog.h \
	configdialog.h \
	aboutdialog.h \
	webpublishdialog.h \
	spellerdialog.h \
	pstrickslistwidget.h
SOURCES	+= main.cpp \
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
	refdialog.cpp \
	configdialog.cpp \
	aboutdialog.cpp \
	webpublishdialog.cpp \
	spellerdialog.cpp \
	pstrickslistwidget.cpp
RESOURCES += texmaker.qrc
FORMS   += finddialog.ui\
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
TRANSLATIONS += texmaker_fr.ts \
	texmaker_zh.ts \
	texmaker_ru.ts \
	texmaker_de.ts
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
	utilities/texmaker_ru.qm \
	utilities/texmaker_zh.qm \
	utilities/texmaker_de.qm \
	utilities/up.gif \
	utilities/up_d.gif \
	utilities/usermanual_en.html \
	utilities/usermanual_fr.html \
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
	utilities/texmaker.svg \
	utilities/xpdf_texmaker_sh.txt
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
CONFIG += link_prl x86 ppc
QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.4u.sdk
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
	utilities/texmaker_ru.qm \
	utilities/texmaker_zh.qm \
	utilities/texmaker_de.qm \
	utilities/up.gif \
	utilities/up_d.gif \
	utilities/usermanual_en.html \
	utilities/usermanual_fr.html \
	utilities/AUTHORS \
	utilities/COPYING \
	utilities/CHANGELOG.txt
QMAKE_BUNDLE_DATA += utilities
INSTALLS += utilities
ICON = texmaker.icns
QMAKE_INFO_PLIST =Info.plist
}