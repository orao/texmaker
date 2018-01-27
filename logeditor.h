/***************************************************************************
 *   copyright       : (C) 2003-2007 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef LOGEDITOR_H
#define LOGEDITOR_H

#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>


#include "loghighlighter.h"

class LogEditor : public QTextEdit  {
   Q_OBJECT
public: 
	LogEditor(QWidget *parent);
	~LogEditor();

public slots:
   void insertLine(QString l);
   void setCursorPosition(int parag, int index);
protected:
    virtual void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *event);
signals:
    void clickonline(int l);
private:
  LogHighlighter *highlighter;
};

#endif
