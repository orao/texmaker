Name: texmaker
Summary: LaTeX editor
Version: 3.0.2
Release: xm1
License: GPL
Group: Applications/Publishing
Source: texmaker-%{version}.tar.bz2
Packager: Pascal Brachet
Url: http://www.xm1math.net/texmaker/
#BuildRequires: libqt4-devel >= 4.6.1
#BuildRequires: libpoppler-qt4-devel
%description
Texmaker is a program, that integrates many tools needed to develop 
documents with LaTeX, in just one application.

%prep
%setup 

%build
export QTDIR=/usr/local/qt4static
PATH=$QTDIR/bin:$PATH
LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
export QTDIR PATH LD_LIBRARY_PATH
qmake -unix texmaker.pro
make INSTALL_ROOT=$RPM_BUILD_ROOT

%install
make INSTALL_ROOT=$RPM_BUILD_ROOT install

%files 
/usr/bin/texmaker
/usr/share/applications/texmaker.desktop
/usr/share/pixmaps/texmaker.png
/usr/share/texmaker/doc1.png
/usr/share/texmaker/doc10.png
/usr/share/texmaker/doc11.png
/usr/share/texmaker/doc12.png
/usr/share/texmaker/doc13.png
/usr/share/texmaker/doc14.png
/usr/share/texmaker/doc15.png
/usr/share/texmaker/doc16.png
/usr/share/texmaker/doc17.png
/usr/share/texmaker/doc18.png
/usr/share/texmaker/doc19.png
/usr/share/texmaker/doc2.png
/usr/share/texmaker/doc3.png
/usr/share/texmaker/doc4.png
/usr/share/texmaker/doc5.png
/usr/share/texmaker/doc6.png
/usr/share/texmaker/doc7.png
/usr/share/texmaker/doc8.png
/usr/share/texmaker/doc9.png
/usr/share/texmaker/doc6bis.png
/usr/share/texmaker/doc20.png
/usr/share/texmaker/doc21.png
/usr/share/texmaker/doc22.png
/usr/share/texmaker/latexhelp.html
/usr/share/texmaker/usermanual_en.html
/usr/share/texmaker/usermanual_fr.html
/usr/share/texmaker/AUTHORS
/usr/share/texmaker/COPYING
/usr/share/texmaker/CHANGELOG.txt
/usr/share/texmaker/texmaker16x16.png
/usr/share/texmaker/texmaker22x22.png
/usr/share/texmaker/texmaker32x32.png
/usr/share/texmaker/texmaker48x48.png
/usr/share/texmaker/texmaker64x64.png
/usr/share/texmaker/texmaker128x128.png
/usr/share/texmaker/texmaker.svg
/usr/share/texmaker/qt_cs.qm
/usr/share/texmaker/qt_de.qm
/usr/share/texmaker/qt_es.qm
/usr/share/texmaker/qt_fr.qm
/usr/share/texmaker/qt_pt.qm
/usr/share/texmaker/qt_ru.qm
/usr/share/texmaker/qt_zh_CN.qm
/usr/share/texmaker/qt_zh_TW.qm
/usr/share/texmaker/texmaker_ca.qm
/usr/share/texmaker/texmaker_de.qm
/usr/share/texmaker/texmaker_es.qm
/usr/share/texmaker/texmaker_fa.qm
/usr/share/texmaker/texmaker_fr.qm
/usr/share/texmaker/texmaker_gl.qm
/usr/share/texmaker/texmaker_it.qm
/usr/share/texmaker/texmaker_pt_BR.qm
/usr/share/texmaker/texmaker_ru.qm
/usr/share/texmaker/texmaker_zh_CN.qm
/usr/share/texmaker/texmaker_zh_TW.qm
/usr/share/texmaker/texmaker_cs.qm
/usr/share/texmaker/texmaker_nl.qm
/usr/share/texmaker/nl_NL.aff
/usr/share/texmaker/nl_NL.dic
/usr/share/texmaker/de_DE.aff
/usr/share/texmaker/de_DE.dic
/usr/share/texmaker/en_GB.aff
/usr/share/texmaker/en_GB.dic
/usr/share/texmaker/es_ES.aff
/usr/share/texmaker/es_ES.dic
/usr/share/texmaker/fr_FR.aff
/usr/share/texmaker/fr_FR.dic
/usr/share/texmaker/it_IT.aff
/usr/share/texmaker/it_IT.dic
/usr/share/texmaker/it_IT_README.txt
/usr/share/texmaker/README_de_DE_frami.txt
/usr/share/texmaker/README_DIC_fr_FR.txt
/usr/share/texmaker/README_es_ES.txt
