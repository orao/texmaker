#!/bin/sh
echo "Texmaker compilation :"
echo "----------------------------------------"
echo "Enter path to QT:"
read QTDIR
echo "Enter SYSTEM (1: UNIX ; 2: MACOSX) :"
read SYSTEM
PATH=$QTDIR/bin:$PATH
LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
DYLD_LIBRARY_PATH=$QTDIR/lib:$DYLD_LIBRARY_PATH
export QTDIR PATH LD_LIBRARY_PATH DYLD_LIBRARY_PATH
if [ "$SYSTEM" = 1 ] 
then
echo "Enter PREFIX (/usr , /usr/local or /opt) :"
read PREFIX
echo "Enter  DIRECTORY for the desktop file (/usr/share/applications) :"
read DESKTOPDIR
echo "Enter  DIRECTORY for the icon file (/usr/share/pixmaps) :"
read ICONDIR
$QTDIR/bin/qmake PREFIX=$PREFIX DESKTOPDIR=$DESKTOPDIR ICONDIR=$ICONDIR texmaker.pro
make
make install
echo "Compilation and installation done"
exit 0
fi

if [ "$SYSTEM" = 2 ] 
then
$QTDIR/bin/qmake -macx -spec macx-g++ texmaker.pro
make
make install
echo "Compilation and installation done"
exit 0
fi

exit 0

