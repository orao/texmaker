/***************************************************************************
 *   copyright       : (C) 2003-2010 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
//#include <QtWebKit>
#include <QWebView>


class Browser : public QMainWindow
{
    Q_OBJECT
public:
    Browser( const QString home, bool showToolBar=true, QWidget* parent = 0);
    ~Browser();

protected slots:
    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);
private slots:
  void Index();
  void Print();
  void Find();
private:
    QWebView *view;
    int progress;
    QLineEdit *searchLineEdit;
    QPushButton *findButton;
};

#endif

