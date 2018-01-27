/****************************************************************************
**
**   copyright       : (C) 2003-2010 by Pascal Brachet                     
**   http://www.xm1math.net/texmaker/                                      
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the documentation of Qt. It was originally
** published as part of Qt Quarterly.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
** Qt for Windows(R) Licensees
** As a special exception, Nokia, as the sole copyright holder for Qt
** Designer, grants users of the Qt/Eclipse Integration plug-in the
** right for the Qt/Eclipse Integration to link to functionality
** provided by Qt Designer and its related libraries.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/

#ifndef PDFVIEWER_H
#define PDFVIEWER_H

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QScrollArea>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QAction>
#include <QListWidgetItem>
#include <QProcess>
#include <QPointer>
#include <QKeyEvent>
#include "pdfdocumentwidget.h"
#include "pdfscrollarea.h"
#include "synctex_parser.h"


class PdfViewer : public QMainWindow
{
    Q_OBJECT
public:
    PdfViewer( const QString fileName, const QString externalCommand, QWidget* parent = 0, Qt::WFlags flags = 0);
    ~PdfViewer();
public slots:
void openFile(QString fn, QString ec);
void jumpToPdfFromSource(QString sourceFile,int source_line);

private:
QAction *upAct, *downAct, *fitWithAct, *fitPageAct, *zoominAct, *zoomoutAct;
QDockWidget *pagesWidget;
QListWidget *listpagesWidget;
PdfScrollArea  *scrollArea;
PdfDocumentWidget *pdfWidget;
QComboBox *scaleComboBox;
QLineEdit *searchLineEdit;
QPushButton *findButton, *externButton;
QVector<qreal> scaleFactors;
QString pdf_file,viewpdf_command;
int currentPage;
qreal currentScale;
bool fileLoaded;
QPointer<QProcess> proc;
QString lastFile;
int lastPage;
synctex_scanner_t scanner;

private slots:
void gotoPage(int page);
void updateCurrentPage(int index);
void scaleDocument(int index);
void searchDocument();
void pageUp();
void pageDown();
void wheelpageUp();
void wheelpageDown();
void fitWidth();
void fitPage();
void zoomIn();
void zoomOut();
void runExternalViewer();
void slotItemClicked(QListWidgetItem* item);
void jumpToEditor(int page, const QPointF& pos);

protected:
void keyPressEvent ( QKeyEvent * e );

signals:
void openDocAtLine(const QString&, int);
void sendFocusToEditor();
};

#endif

