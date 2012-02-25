/***************************************************************************
 *   copyright       : (C) 2006-2009 by Pascal Brachet                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef PDFCHECKER_H
#define PDFCHECKER_H

#include <QObject>
#include <QString>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "goo/GooString.h"
#include "goo/gmem.h"

class PdfChecker : public QObject
{
    Q_OBJECT
    public:
        PdfChecker(QString infn="",QString outfn="", int page=1, QString atdpath="", QString atdlang="");
        ~PdfChecker();
private :
	QString pdf_file, html_file, text_file, atd_path, atd_lang;
	int page_selected;
	GooString *QStringToGoo(const QString &s) 
	  {
	  int len = s.length();
	  char *cstring = (char *)gmallocn(s.length(), sizeof(char));
	  for (int i = 0; i < len; ++i)
	      cstring[i] = s.at(i).unicode();
	  GooString *ret = new GooString(cstring, len);
	  gfree(cstring);
	  return ret;
	  }
public slots:
bool createPage();
int extractText();
};
#endif
