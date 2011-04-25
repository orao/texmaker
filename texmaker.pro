TEMPLATE	= app
LANGUAGE	= C++
TARGET	 = texmaker
QT += network \
      xml \
      webkit
CONFIG	+= qt warn_off release
TEXMAKERVERSION=3.0.2
DEFINES += TEXMAKERVERSION=\\\"$${TEXMAKERVERSION}\\\"
DEFINES += HAVE_SPLASH
###############################
HEADERS	+= texmaker.h \
	texmakerapp.h \
	minisplitter.h \
	playerbutton.h \
	symbollistwidget.h \
	icondelegate.h \
	latexeditor.h \
	latexhighlighter.h \
	latexeditorview.h \
	linenumberwidget.h \
	logeditor.h \
	loghighlighter.h \
	findwidget.h \
	gotolinedialog.h \
	replacedialog.h \
	structdialog.h \
	filechooser.h \
	graphicfilechooser.h \
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
	spellerdialog.h \
	xmltagslistwidget.h \
	blockdata.h \
	manhattanstyle.h \
	stylehelper.h \
	styleanimator.h \
	keysequencedialog.h \
	paperdialog.h \
	browser.h \
	pdfviewerwidget.h \
	pdfviewer.h \
	pdfdocumentwidget.h \
	pdfscrollarea.h \
	userquickdialog.h \
	encodingdialog.h \
	usercompletiondialog.h \
	synctex_parser.h \
	synctex_parser_utils.h \
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
	hunspell/replist.hxx \
	hunspell/w_char.hxx \
	singleapp/qtlocalpeer.h \
	singleapp/qtlockedfile.h \
	singleapp/qtsingleapplication.h \
	singleapp/qtsinglecoreapplication.h \
	encodingprober/CharDistribution.h \
	encodingprober/ChineseGroupProber.h \
	encodingprober/ctype_test_p.h \
	encodingprober/JapaneseGroupProber.h \
	encodingprober/JpCntx.h \
	encodingprober/nsBig5Prober.h \
	encodingprober/nsCharSetProber.h \
	encodingprober/nsCodingStateMachine.h \
	encodingprober/nsEscCharsetProber.h \
	encodingprober/nsEUCJPProber.h \
	encodingprober/nsEUCKRProber.h \
	encodingprober/nsEUCTWProber.h \
	encodingprober/nsGB2312Prober.h \
	encodingprober/nsHebrewProber.h \
	encodingprober/nsLatin1Prober.h \
	encodingprober/nsMBCSGroupProber.h \
	encodingprober/nsPkgInt.h \
	encodingprober/nsSBCharSetProber.h \
	encodingprober/nsSBCSGroupProber.h \
	encodingprober/nsSJISProber.h \
	encodingprober/nsUniversalDetector.h \
	encodingprober/qencodingprober.h \
	encodingprober/UnicodeGroupProber.h 
SOURCES	+= main.cpp \
	texmakerapp.cpp \
	texmaker.cpp \
	minisplitter.cpp \
	playerbutton.cpp \
	symbollistwidget.cpp \
	icondelegate.cpp \
	latexeditor.cpp \
	latexhighlighter.cpp \
	latexeditorview.cpp \
	linenumberwidget.cpp \
	logeditor.cpp \
	loghighlighter.cpp \
	findwidget.cpp \
	gotolinedialog.cpp \
	replacedialog.cpp \
	structdialog.cpp \
	filechooser.cpp \
	graphicfilechooser.cpp \
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
	spellerdialog.cpp \
	xmltagslistwidget.cpp \
	blockdata.cpp \
	manhattanstyle.cpp \
	stylehelper.cpp \
	styleanimator.cpp \
	keysequencedialog.cpp \
	paperdialog.cpp \
	browser.cpp \
	pdfviewerwidget.cpp \
	pdfviewer.cpp \
	pdfdocumentwidget.cpp \
	pdfscrollarea.cpp \
	userquickdialog.cpp \
	encodingdialog.cpp \
	usercompletiondialog.cpp \
	synctex_parser.c \
	synctex_parser_utils.c \
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
	hunspell/replist.cxx \
	hunspell/hunzip.cxx \
	singleapp/qtlocalpeer.cpp \
	singleapp/qtlockedfile.cpp \
	singleapp/qtsingleapplication.cpp \
	singleapp/qtsinglecoreapplication.cpp \
	encodingprober/CharDistribution.cpp \
	encodingprober/ChineseGroupProber.cpp \
	encodingprober/JapaneseGroupProber.cpp \
	encodingprober/JpCntx.cpp \
	encodingprober/LangBulgarianModel.cpp \
	encodingprober/LangCyrillicModel.cpp \
	encodingprober/LangGreekModel.cpp \
	encodingprober/LangHebrewModel.cpp \
	encodingprober/LangHungarianModel.cpp \
	encodingprober/LangThaiModel.cpp \
	encodingprober/nsBig5Prober.cpp \
	encodingprober/nsCharSetProber.cpp \
	encodingprober/nsEscCharsetProber.cpp \
	encodingprober/nsEscSM.cpp \
	encodingprober/nsEUCJPProber.cpp \
	encodingprober/nsEUCKRProber.cpp \
	encodingprober/nsEUCTWProber.cpp \
	encodingprober/nsGB2312Prober.cpp \
	encodingprober/nsHebrewProber.cpp \
	encodingprober/nsLatin1Prober.cpp \
	encodingprober/nsMBCSGroupProber.cpp \
	encodingprober/nsMBCSSM.cpp \
	encodingprober/nsSBCharSetProber.cpp \
	encodingprober/nsSBCSGroupProber.cpp \
	encodingprober/nsSJISProber.cpp \
	encodingprober/nsUniversalDetector.cpp \
	encodingprober/qencodingprober.cpp \
	encodingprober/UnicodeGroupProber.cpp 
RESOURCES += texmaker.qrc
FORMS   += findwidget.ui\
	gotolinedialog.ui \
	replacedialog.ui \
	structdialog.ui \
	filechooser.ui \
	graphicfilechooser.ui \
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
	configdialog.ui \
	spellerdialog.ui \
	keysequencedialog.ui \
	paperdialog.ui \
	userquickdialog.ui \
	encodingdialog.ui \
	usercompletiondialog.ui
TRANSLATIONS += texmaker_fr.ts \
	texmaker_de.ts \
	texmaker_es.ts \
	texmaker_fa.ts \
	texmaker_gl.ts \
	texmaker_it.ts \
	texmaker_ru.ts \
	texmaker_zh_CN.ts \
	texmaker_zh_TW.ts \
	texmaker_cs.ts \
	texmaker_pt_BR.ts \
	texmaker_nl.ts  \
	texmaker_vi_VN.ts \
	texmaker_da.ts
################################
unix:!macx {
UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj
isEmpty( PREFIX ) {
    PREFIX=/usr
}
isEmpty( DESKTOPDIR ) {
    DESKTOPDIR=/usr/share/applications
}
isEmpty( ICONDIR ) {
    ICONDIR=/usr/share/pixmaps
}

INCLUDEPATH  += /usr/include/poppler/qt4
LIBS         += -L/usr/lib -lpoppler-qt4
LIBS         += -L/usr/lib -lz
DEFINES += PREFIX=\\\"$${PREFIX}\\\"
target.path = $${PREFIX}/bin

#DEFINES += DEBIAN_SPELLDIR

INSTALLS = target
HEADERS	+= x11fontdialog.h 
SOURCES	+= x11fontdialog.cpp \
	  singleapp/qtlockedfile_unix.cpp
FORMS += x11fontdialog.ui

utilities.path = $${PREFIX}/share/texmaker

utilities.files = doc/doc1.png \
	doc/doc10.png \
	doc/doc11.png \
	doc/doc12.png \
	doc/doc13.png \
	doc/doc14.png \
	doc/doc15.png \
	doc/doc16.png \
	doc/doc17.png \
	doc/doc18.png \
	doc/doc19.png \
	doc/doc2.png \
	doc/doc3.png \
	doc/doc4.png \
	doc/doc5.png \
	doc/doc6.png \
	doc/doc7.png \
	doc/doc8.png \
	doc/doc9.png \
	doc/doc6bis.png \
	doc/doc20.png \
	doc/doc21.png \
	doc/doc22.png \
	doc/latexhelp.html \
	doc/usermanual_en.html \
	doc/usermanual_fr.html \
	utilities/AUTHORS \
	utilities/COPYING \
	utilities/CHANGELOG.txt \
	utilities/texmaker16x16.png \
	utilities/texmaker22x22.png \
	utilities/texmaker32x32.png \
	utilities/texmaker48x48.png \
	utilities/texmaker64x64.png \
	utilities/texmaker128x128.png \
	utilities/texmaker.svg \
	locale/qt_cs.qm \
	locale/qt_de.qm \
	locale/qt_es.qm \
	locale/qt_fr.qm \
	locale/qt_pt.qm \
	locale/qt_ru.qm \
	locale/qt_zh_CN.qm \
	locale/qt_zh_TW.qm \
	locale/texmaker_ca.qm \
	locale/texmaker_de.qm \
	locale/texmaker_es.qm \
	locale/texmaker_fa.qm \
	locale/texmaker_fr.qm \
	locale/texmaker_gl.qm \
	locale/texmaker_it.qm \
	locale/texmaker_pt_BR.qm \
	locale/texmaker_ru.qm \
	locale/texmaker_zh_CN.qm \
	locale/texmaker_zh_TW.qm \
	locale/texmaker_cs.qm \
	locale/texmaker_nl.qm \
	dictionaries/nl_NL.aff \
	dictionaries/nl_NL.dic \
	dictionaries/de_DE.aff \
	dictionaries/de_DE.dic \
	dictionaries/en_GB.aff \
	dictionaries/en_GB.dic \
	dictionaries/es_ES.aff \
	dictionaries/es_ES.dic \
	dictionaries/fr_FR.aff \
	dictionaries/fr_FR.dic \
	dictionaries/it_IT.aff \
	dictionaries/it_IT.dic \
	dictionaries/it_IT_README.txt \
	dictionaries/README_de_DE_frami.txt \
	dictionaries/README_DIC_fr_FR.txt \
	dictionaries/README_es_ES.txt 
INSTALLS += utilities

desktop.path = $${DESKTOPDIR}

desktop.files = utilities/texmaker.desktop
INSTALLS += desktop

icon.path = $${ICONDIR}

icon.files = utilities/texmaker.png
INSTALLS += icon
}
################################
win32 {
INCLUDEPATH += C:\QtSDK\mingw\include
INCLUDEPATH += C:\poppler
LIBS         += -LC:\poppler -lpoppler-qt4
RC_FILE = win.rc

#DEFINES += USB_VERSION

SOURCES	+= singleapp/qtlockedfile_win.cpp

target.path = texmakerwin32
#target.path = texmakerwin32usb

INSTALLS = target

utilities.path = texmakerwin32
#utilities.path = texmakerwin32usb

utilities.files =doc/doc1.png \
	doc/doc10.png \
	doc/doc11.png \
	doc/doc12.png \
	doc/doc13.png \
	doc/doc14.png \
	doc/doc15.png \
	doc/doc16.png \
	doc/doc17.png \
	doc/doc18.png \
	doc/doc19.png \
	doc/doc2.png \
	doc/doc3.png \
	doc/doc4.png \
	doc/doc5.png \
	doc/doc6.png \
	doc/doc7.png \
	doc/doc8.png \
	doc/doc9.png \
	doc/doc6bis.png \
	doc/doc20.png \
	doc/doc21.png \
	doc/doc22.png \
	doc/latexhelp.html \
	doc/usermanual_en.html \
	doc/usermanual_fr.html \
	utilities/AUTHORS \
	utilities/COPYING \
	utilities/CHANGELOG.txt \
	locale/qt_cs.qm \
	locale/qt_de.qm \
	locale/qt_es.qm \
	locale/qt_fr.qm \
	locale/qt_pt.qm \
	locale/qt_ru.qm \
	locale/qt_zh_CN.qm \
	locale/qt_zh_TW.qm \
	locale/texmaker_ca.qm \
	locale/texmaker_de.qm \
	locale/texmaker_es.qm \
	locale/texmaker_fa.qm \
	locale/texmaker_fr.qm \
	locale/texmaker_gl.qm \
	locale/texmaker_it.qm \
	locale/texmaker_pt_BR.qm \
	locale/texmaker_ru.qm \
	locale/texmaker_zh_CN.qm \
	locale/texmaker_zh_TW.qm \
	locale/texmaker_cs.qm \
	locale/texmaker_nl.qm \
	dictionaries/nl_NL.aff \
	dictionaries/nl_NL.dic \
	dictionaries/de_DE.aff \
	dictionaries/de_DE.dic \
	dictionaries/en_GB.aff \
	dictionaries/en_GB.dic \
	dictionaries/es_ES.aff \
	dictionaries/es_ES.dic \
	dictionaries/fr_FR.aff \
	dictionaries/fr_FR.dic \
	dictionaries/it_IT.aff \
	dictionaries/it_IT.dic 
INSTALLS += utilities

others.path = texmakerwin32
#others.path = texmakerwin32usb

others.files = texmaker.ico \
		C:\QtSDK\Desktop\Qt\4.7.2\mingw\bin\mingwm10.dll \
		C:\QtSDK\Desktop\Qt\4.7.2\mingw\bin\libgcc_s_dw2-1.dll \
		C:\QtSDK\Desktop\Qt\4.7.2\mingw\bin\QtCore4.dll \
		C:\QtSDK\Desktop\Qt\4.7.2\mingw\bin\QtGui4.dll \
		C:\QtSDK\Desktop\Qt\4.7.2\mingw\bin\QtWebKit4.dll \
		C:\QtSDK\Desktop\Qt\4.7.2\mingw\bin\QtXml4.dll \
		C:\QtSDK\Desktop\Qt\4.7.2\mingw\bin\QtXmlPatterns4.dll \
		C:\QtSDK\Desktop\Qt\4.7.2\mingw\bin\phonon4.dll \
		C:\QtSDK\Desktop\Qt\4.7.2\mingw\bin\QtNetwork4.dll 
INSTALLS += others
}
###############################
macx {
UI_DIR = .ui
MOC_DIR = .moc
OBJECTS_DIR = .obj

INCLUDEPATH  += /usr/local/include/poppler/qt4
LIBS         += -L/usr/local/lib -lpoppler-qt4

##tiger 32
#CONFIG += link_prl x86
#QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk
#QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.5
#target.path = TexmakerMacosx32

##tiger snow 64
CONFIG += link_prl x86_64
QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.6.sdk
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.6
target.path = TexmakerMacosx64

#target.path = /Applications
INSTALLS = target
SOURCES	+= singleapp/qtlockedfile_unix.cpp
utilities.path = Contents/Resources
utilities.files = utilities/qt_menu.nib \
	doc/doc1.png \
	doc/doc10.png \
	doc/doc11.png \
	doc/doc12.png \
	doc/doc13.png \
	doc/doc14.png \
	doc/doc15.png \
	doc/doc16.png \
	doc/doc17.png \
	doc/doc18.png \
	doc/doc19.png \
	doc/doc2.png \
	doc/doc3.png \
	doc/doc4.png \
	doc/doc5.png \
	doc/doc6.png \
	doc/doc7.png \
	doc/doc8.png \
	doc/doc9.png \
	doc/doc6bis.png \
	doc/doc20.png \
	doc/doc21.png \
	doc/doc22.png \
	doc/latexhelp.html \
	doc/usermanual_en.html \
	doc/usermanual_fr.html \
	utilities/AUTHORS \
	utilities/COPYING \
	utilities/CHANGELOG.txt \
	locale/qt_cs.qm \
	locale/qt_de.qm \
	locale/qt_es.qm \
	locale/qt_fr.qm \
	locale/qt_pt.qm \
	locale/qt_ru.qm \
	locale/qt_zh_CN.qm \
	locale/qt_zh_TW.qm \
	locale/texmaker_ca.qm \
	locale/texmaker_de.qm \
	locale/texmaker_es.qm \
	locale/texmaker_fa.qm \
	locale/texmaker_fr.qm \
	locale/texmaker_gl.qm \
	locale/texmaker_it.qm \
	locale/texmaker_pt_BR.qm \
	locale/texmaker_ru.qm \
	locale/texmaker_zh_CN.qm \
	locale/texmaker_zh_TW.qm \
	locale/texmaker_cs.qm \
	locale/texmaker_nl.qm \
	dictionaries/nl_NL.aff \
	dictionaries/nl_NL.dic \
	dictionaries/de_DE.aff \
	dictionaries/de_DE.dic \
	dictionaries/en_GB.aff \
	dictionaries/en_GB.dic \
	dictionaries/es_ES.aff \
	dictionaries/es_ES.dic \
	dictionaries/fr_FR.aff \
	dictionaries/fr_FR.dic \
	dictionaries/it_IT.aff \
	dictionaries/it_IT.dic \
	dictionaries/it_IT_README.txt \
	dictionaries/README_de_DE_frami.txt \
	dictionaries/README_DIC_fr_FR.txt \
	dictionaries/README_es_ES.txt 
QMAKE_BUNDLE_DATA += utilities
INSTALLS += utilities
ICON = texmaker.icns
QMAKE_INFO_PLIST =Info.plist
}
