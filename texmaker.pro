TEMPLATE	= app
LANGUAGE	= C++
TARGET	 = texmaker

QT_VERSION=$$[QT_VERSION]
contains( QT_VERSION, "^5.*" ) {
QT += xml webkitwidgets network widgets printsupport concurrent
} else {
QT += xml webkit network
}

CONFIG	+= qt warn_off release
TEXMAKERVERSION=4.1
DEFINES += TEXMAKERVERSION=\\\"$${TEXMAKERVERSION}\\\"
DEFINES += HAVE_SPLASH



unix:!macx {
contains( QT_VERSION, "^5.*" ) {
CONFIG		+= link_pkgconfig
PKGCONFIG 	= poppler-qt5
DEFINES += POPPLER24
} else {
CONFIG		+= link_pkgconfig
PKGCONFIG 	= poppler-qt4
DETECTEDPOPPLER=$$system(pkg-config --modversion poppler)
contains( DETECTEDPOPPLER, "^0.18.*" ){
DEFINES += OLDPOPPLER
}
}
}

###############################
HEADERS	+= texmaker.h \
	geticon.h \
	documentview.h \
	pageitem.h \
	presentationview.h \
	texmakerapp.h \
	minisplitter.h \
	playerbutton.h \
	symbollistwidget.h \
	icondelegate.h \
	latexeditor.h \
	latexhighlighter.h \
	latexeditorview.h \
	linenumberwidget.h \
	lightlatexeditor.h \
	lightlatexhighlighter.h \
	lightlinenumberwidget.h \
	sourceview.h \
	logeditor.h \
	loghighlighter.h \
	findwidget.h \
	gotolinewidget.h \
	lightfindwidget.h \
	lightgotolinewidget.h \
	replacewidget.h \
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
	browser.h \
	pdfviewerwidget.h \
	pdfviewer.h \
	userquickdialog.h \
	encodingdialog.h \
	usercompletiondialog.h \
	texdocdialog.h \
	textblockselection.h \
	scandialog.h \
	exportdialog.h \
	synctex_parser.h \
	synctex_parser_utils.h \
	usertagslistwidget.h \
	addtagdialog.h \
	versiondialog.h \
	unicodedialog.h \
	unicodeview.h \
	quickbeamerdialog.h \
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
	geticon.cpp \
	texmakerapp.cpp \
	texmaker.cpp \
	documentview.cpp \
	pageitem.cpp \
	presentationview.cpp \
	minisplitter.cpp \
	playerbutton.cpp \
	symbollistwidget.cpp \
	icondelegate.cpp \
	latexeditor.cpp \
	latexhighlighter.cpp \
	latexeditorview.cpp \
	linenumberwidget.cpp \
	lightlatexeditor.cpp \
	lightlatexhighlighter.cpp \
	lightlinenumberwidget.cpp \
	sourceview.cpp \
	logeditor.cpp \
	loghighlighter.cpp \
	findwidget.cpp \
	gotolinewidget.cpp \
	lightfindwidget.cpp \
	lightgotolinewidget.cpp \
	replacewidget.cpp \
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
	browser.cpp \
	pdfviewerwidget.cpp \
	pdfviewer.cpp \
	userquickdialog.cpp \
	encodingdialog.cpp \
	usercompletiondialog.cpp \
	texdocdialog.cpp \
	scandialog.cpp \
	exportdialog.cpp \
	synctex_parser.c \
	synctex_parser_utils.c \
	usertagslistwidget.cpp \
	addtagdialog.cpp \
	versiondialog.cpp \
	unicodedialog.cpp \
	unicodeview.cpp \
	quickbeamerdialog.cpp \
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
	gotolinewidget.ui \
	replacewidget.ui \
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
	userquickdialog.ui \
	encodingdialog.ui \
	usercompletiondialog.ui \
	texdocdialog.ui \
	scandialog.ui \
	exportdialog.ui \
	addtagdialog.ui \
	versiondialog.ui \
	unicodedialog.ui \
	quickbeamerdialog.ui 
TRANSLATIONS += trans/texmaker_fr.ts \
	trans/texmaker_de.ts \
	trans/texmaker_es.ts \
	trans/texmaker_gl.ts \
	trans/texmaker_it.ts \
	trans/texmaker_ru.ts \
	trans/texmaker_zh_CN.ts \
	trans/texmaker_zh_TW.ts \
	trans/texmaker_cs.ts \
	trans/texmaker_pt.ts \
	trans/texmaker_pt_BR.ts \
	trans/texmaker_nl.ts  \
	trans/texmaker_hu.ts  \
	trans/texmaker_fa.ts  \
	trans/texmaker_pl.ts  \
	trans/texmaker_vi_VN.ts \
	trans/texmaker_da.ts \
	trans/texmaker_ca.ts \
	trans/texmaker_sr.ts \
	trans/texmaker_el.ts \
	trans/texmaker_se.ts \
	trans/texmaker_ar.ts \
	trans/texmaker_lv.ts \
	trans/texmaker_uk.ts
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

DEFINES += PREFIX=\\\"$${PREFIX}\\\"

LIBS         += -lz


target.path = $${PREFIX}/bin
utilities.path = $${PREFIX}/share/texmaker
desktop.path = $${DESKTOPDIR}
icon.path = $${ICONDIR}

INSTALLS = target
HEADERS	+= x11fontdialog.h 
SOURCES	+= x11fontdialog.cpp \
	  singleapp/qtlockedfile_unix.cpp
FORMS += x11fontdialog.ui

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
	doc/doc10hu.png \
	doc/doc11hu.png \
	doc/doc12hu.png \
	doc/doc13hu.png \
	doc/doc14hu.png \
	doc/doc15hu.png \
	doc/doc16hu.png \
	doc/doc17hu.png \
	doc/doc1hu.png \
	doc/doc20hu.png \
	doc/doc21hu.png \
	doc/doc22hu.png \
	doc/doc2hu.png \
	doc/doc3hu.png \
	doc/doc4hu.png \
	doc/doc5hu.png \
	doc/doc6ahu.png \
	doc/doc6hu.png \
	doc/doc7hu.png \
	doc/doc8hu.png \
	doc/doc9hu.png \
	doc/latexhelp.html \
	doc/usermanual_en.html \
	doc/usermanual_fr.html \
	doc/usermanual_ru.html \
	doc/usermanual_hu.html \
	atd/atd.css \
	atd/csshttprequest.js \
	atd/jquery.atd.textarea.js \
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
	locale/qt_fa.qm \
	locale/qt_pl.qm \
	locale/qt_nl.qm \
	locale/texmaker_de.qm \
	locale/texmaker_es.qm \
	locale/texmaker_fr.qm \
	locale/texmaker_gl.qm \
	locale/texmaker_it.qm \
	locale/texmaker_pt.qm \
	locale/texmaker_pt_BR.qm \
	locale/texmaker_ru.qm \
	locale/texmaker_zh_CN.qm \
	locale/texmaker_zh_TW.qm \
	locale/texmaker_cs.qm \
	locale/texmaker_nl.qm \
	locale/texmaker_fa.qm \
	locale/texmaker_pl.qm \
	locale/texmaker_hu.qm \
	locale/texmaker_sr.qm \
	locale/texmaker_el.qm \
	locale/texmaker_se.qm \
	locale/texmaker_ar.qm \
	locale/texmaker_lv.qm \
	locale/texmaker_uk.qm \
	dictionaries/nl_NL.aff \
	dictionaries/nl_NL.dic \
	dictionaries/de_DE.aff \
	dictionaries/de_DE.dic \
	dictionaries/en_GB.aff \
	dictionaries/en_GB.dic \
	dictionaries/en_US.aff \
	dictionaries/en_US.dic \
	dictionaries/es_ES.aff \
	dictionaries/es_ES.dic \
	dictionaries/fr_FR.aff \
	dictionaries/fr_FR.dic \
	dictionaries/it_IT.aff \
	dictionaries/it_IT.dic \
	dictionaries/it_IT_README.txt \
	dictionaries/hu_HU.aff \
	dictionaries/hu_HU.dic \
	dictionaries/README_hu_HU.txt \
	dictionaries/cs_CZ.aff \
	dictionaries/cs_CZ.dic \
	dictionaries/pl_PL.aff \
	dictionaries/pl_PL.dic \
	dictionaries/README_pl.txt \
	dictionaries/README_cs_CZ.txt \
	dictionaries/README_de_DE_frami.txt \
	dictionaries/README_DIC_fr_FR.txt \
	dictionaries/README_es_ES.txt \
	dictionaries/README_en_US.txt
INSTALLS += utilities

desktop.files = utilities/texmaker.desktop
INSTALLS += desktop

icon.files = utilities/texmaker.png
INSTALLS += icon
}
