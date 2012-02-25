/***************************************************************************
 *   copyright       : (C) 2006-2012 by Pascal Brachet                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//========================================================================
//
// contains parts of pdftotext.cc
//
// Copyright 1997-2003 Glyph & Cog, LLC
//
// Modified for Debian by Hamish Moffatt, 22 May 2002.
//
//========================================================================

//========================================================================
//
// Modified under the Poppler project - http://poppler.freedesktop.org
//
// All changes made under the Poppler project to this file are licensed
// under GPL version 2 or later
//
// Copyright (C) 2006 Dominic Lachowicz <cinamod@hotmail.com>
// Copyright (C) 2007-2008, 2010, 2011 Albert Astals Cid <aacid@kde.org>
// Copyright (C) 2009 Jan Jockusch <jan@jockusch.de>
// Copyright (C) 2010 Hib Eris <hib@hiberis.nl>
// Copyright (C) 2010 Kenneth Berland <ken@hero.com>
// Copyright (C) 2011 Tom Gleason <tom@buildadam.com>
// Copyright (C) 2011 Steven Murdoch <Steven.Murdoch@cl.cam.ac.uk>
//
// To see a description of the changes please see the Changelog file that
// came with your tarball or type make ChangeLog if you are building from git
//
//========================================================================

#include "pdfchecker.h"
#include <QFile>
#include <QTextStream>

#include <poppler-config.h>


#include "GlobalParams.h"
#include "Object.h"
#include "Stream.h"
#include "Array.h"
#include "Dict.h"
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"
#include "PDFDoc.h"
#include "PDFDocFactory.h"
#include "TextOutputDev.h"
#include "CharTypes.h"
#include "UnicodeMap.h"
#include "PDFDocEncoding.h"
#include "Error.h"
#include <string>

static void printInfoString(FILE *f, Dict *infoDict, char *key,
			    char *text1, char *text2, UnicodeMap *uMap);
static void printInfoDate(FILE *f, Dict *infoDict, char *key, char *fmt);

static int firstPage = 1;
static int lastPage = 0;
static double resolution = 72.0;
static int x = 0;
static int y = 0;
static int w = 0;
static int h = 0;
static GBool bbox = gFalse;
static GBool physLayout = gTrue;
static GBool rawOrder = gFalse;
static GBool htmlMeta = gFalse;
static char textEncName[128] = "";
static char textEOL[16] = "";
static GBool noPageBreaks = gFalse;
static char ownerPassword[33] = "\001";
static char userPassword[33] = "\001";
static GBool quiet = gFalse;
static GBool printVersion = gFalse;
static GBool printHelp = gFalse;
static GBool printEnc = gFalse;



static std::string myStringReplace(const std::string &inString, const std::string &oldToken, const std::string &newToken) {
  std::string result = inString;
  size_t foundLoc;
  int advance = 0;
  do {
    foundLoc = result.find(oldToken, advance);
    if (foundLoc != std::string::npos){
      result.replace(foundLoc, oldToken.length(), newToken);
      advance = foundLoc + newToken.length();
    }
  } while (foundLoc != std::string::npos );
  return result;
}

static std::string myXmlTokenReplace(const char *inString){
  std::string myString(inString);
  myString = myStringReplace(myString, "&",  "&amp;" );
  myString = myStringReplace(myString, "'",  "&apos;" );
  myString = myStringReplace(myString, "\"", "&quot;" );
  myString = myStringReplace(myString, "<",  "&lt;" );
  myString = myStringReplace(myString, ">",  "&gt;" );
  return myString;
}

PdfChecker::PdfChecker(QString infn,QString outfn, int page,QString atdpath, QString atdlang)
{
pdf_file=infn;
html_file=outfn+".html";
text_file=outfn+".txt";
if (QFile::exists(text_file))
	{
	QFile fi_text(text_file);
	fi_text.remove();
	}
page_selected=page;
atd_path=atdpath;
atd_lang=atdlang;
}

PdfChecker::~PdfChecker()
{

}

bool PdfChecker::createPage()
{
bool result=false;
int ok=extractText();
if (ok!=0) return result;
QString pdf_text="";
QFile tf(text_file);
if (!tf.open( QIODevice::ReadOnly )) return result;
QTextStream ttf( &tf );
pdf_text=ttf.readAll();
tf.close();
QFile fi_html(html_file);
if (!fi_html.open(QIODevice::WriteOnly)) return result;
QTextStream out (&fi_html);
out << "<html>\n";
out << "<head>\n";
out << "<title>Texmaker</title>\n";
out << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
out << "<style>\n";
out << ".input\n";
out << "{\n";
out << "width:90%;\n";
out << "height:90%;\n";
out << "border: 1px solid black;\n";
out << "padding: 2px;\n";
out << "margin: 2px;\n";
out << "font-family: times;\n";
out << "font-size: 80%;\n";
out << "}\n";
out << "</style>\n";
out << "<script src=\"http://code.jquery.com/jquery-1.4.2.js\"></script>\n";
out << "<script src=\"";
out << atd_path;
out << "/jquery.atd.textarea.js\"></script>\n";
out << "<script src=\"";
out << atd_path;
out << "/csshttprequest.js\"></script>\n";
out << "<link rel=\"stylesheet\" type=\"text/css\" media=\"screen\" href=\"";
out << atd_path;
out << "/atd.css\" />\n";
out << "<script>\n";
out << "function check() {\n";
out << "AtD.rpc_css_lang = document.langform.langchoice.options[document.langform.langchoice.selectedIndex].text;\n";
//out << atd_lang;
//out << "';\n";
out << "AtD.checkTextAreaCrossAJAX('textInput', 'checkLink', 'Click to reset');\n";
out << " };\n";
out << "</script>\n";
out << "</head>\n";
out << "<body BGCOLOR=\"#ffffff\">\n";
out << "<textarea name=\"textInput\" id=\"textInput\" class=\"input\">\n";
out << pdf_text;
out << "</textarea>\n";
out << "<form name=\"langform\">\n";
out << "<p><a href=\"javascript:check()\" id=\"checkLink\">Click to check</a>&nbsp;&nbsp;\n";
out << "<SELECT NAME=\"langchoice\">\n";
bool done=false;
if (atd_lang=="fr") {out << "<OPTION selected>fr</OPTION>\n"; done=true;}
else out << "<OPTION>fr</OPTION>\n";
if (atd_lang=="de") {out << "<OPTION selected>de</OPTION>\n"; done=true;}
else out << "<OPTION>de</OPTION>\n";
if (atd_lang=="nl") {out << "<OPTION selected>nl</OPTION>\n"; done=true;}
else out << "<OPTION>nl</OPTION>\n";
if (atd_lang=="pl") {out << "<OPTION selected>pl</OPTION>\n"; done=true;}
else out << "<OPTION>pl</OPTION>\n";
if (atd_lang=="pt") {out << "<OPTION selected>pt</OPTION>\n"; done=true;}
else out << "<OPTION>pt</OPTION>\n";
if (atd_lang=="ru") {out << "<OPTION selected>ru</OPTION>\n"; done=true;}
else out << "<OPTION>ru</OPTION>\n";
if (atd_lang=="it") {out << "<OPTION selected>it</OPTION>\n"; done=true;}
else out << "<OPTION>it</OPTION>\n";
if (atd_lang=="es") {out << "<OPTION selected>es</OPTION>\n"; done=true;}
else out << "<OPTION>es</OPTION>\n";
if ((atd_lang=="en") || (!done)) out << "<OPTION selected>en</OPTION>\n";
else out << "<OPTION>en</OPTION>\n";
out << "</SELECT></p></form>\n";
out << "</body>\n";
out << "</html>\n";
fi_html.close();
return true;
}

int PdfChecker::extractText()
{
  PDFDoc *doc;
  GooString *fileName;
  GooString *textFileName;
  GooString *ownerPW, *userPW;
  TextOutputDev *textOut;
  FILE *f;
  UnicodeMap *uMap;
  Object info;
  GBool ok;
  char *p;
  int exitCode;

  exitCode = 99;

  // parse args

  if (bbox) {
    htmlMeta = gTrue;
  }

  // read config file
  globalParams = new GlobalParams();

  if (printEnc) {
    //printEncodings();
    delete globalParams;
    exitCode = 0;
    goto err0;
  }

  fileName = QStringToGoo(pdf_file);//new GooString(argv[1]);

  if (textEncName[0]) {
    globalParams->setTextEncoding(textEncName);
  }
  if (textEOL[0]) {
    if (!globalParams->setTextEOL(textEOL)) {
      fprintf(stderr, "Bad '-eol' value on command line\n");
    }
  }
  if (noPageBreaks) {
    globalParams->setTextPageBreaks(gFalse);
  }
  if (quiet) {
    globalParams->setErrQuiet(quiet);
  }

  // get mapping to output encoding
  if (!(uMap = globalParams->getTextEncoding())) {
    error(-1, "Couldn't get text encoding");
    delete fileName;
    goto err1;
  }

  // open PDF file
  if (ownerPassword[0] != '\001') {
    ownerPW = new GooString(ownerPassword);
  } else {
    ownerPW = NULL;
  }
  if (userPassword[0] != '\001') {
    userPW = new GooString(userPassword);
  } else {
    userPW = NULL;
  }

  if (fileName->cmp("-") == 0) {
      delete fileName;
      fileName = new GooString("fd://0");
  }

  doc = PDFDocFactory().createPDFDoc(*fileName, ownerPW, userPW);

  if (userPW) {
    delete userPW;
  }
  if (ownerPW) {
    delete ownerPW;
  }
  if (!doc->isOk()) {
    exitCode = 1;
    goto err2;
  }

#ifdef ENFORCE_PERMISSIONS
  // check for copy permission
  if (!doc->okToCopy()) {
    error(-1, "Copying of text from this document is not allowed.");
    exitCode = 3;
    goto err2;
  }
#endif
  firstPage=page_selected;
  lastPage=page_selected;
  // get page range
  if (firstPage < 1) {
    firstPage = 1;
  }
  if (lastPage < 1 || lastPage > doc->getNumPages()) {
    lastPage = doc->getNumPages();
  }
  
  textFileName=QStringToGoo(text_file);
  // write text file

    textOut = new TextOutputDev(textFileName->getCString(),
				physLayout, rawOrder, htmlMeta);
    if (textOut->isOk()) {
      if ((w==0) && (h==0) && (x==0) && (y==0)) {
	doc->displayPages(textOut, firstPage, lastPage, resolution, resolution, 0,
			  gTrue, gFalse, gFalse);
      } else {
	
	for (int page = firstPage; page <= lastPage; ++page) {
	  doc->displayPageSlice(textOut, page, resolution, resolution, 0,
			      gTrue, gFalse, gFalse, 
			      x, y, w, h);
	}
      }

    } else {
    delete textOut;
    exitCode = 2;
    goto err3;
    }
  delete textOut;

  exitCode = 0;

  // clean up
 err3:
  delete textFileName;
 err2:
  delete doc;
  delete fileName;
  uMap->decRefCnt();
 err1:
  delete globalParams;
 err0:

  // check for memory leaks
  Object::memCheck(stderr);
  gMemReport(stderr);

  return exitCode;
}

static void printInfoString(FILE *f, Dict *infoDict, char *key,
			    char *text1, char *text2, UnicodeMap *uMap) {
  Object obj;
  GooString *s1;
  GBool isUnicode;
  Unicode u;
  char buf[8];
  int i, n;

  if (infoDict->lookup(key, &obj)->isString()) {
    fputs(text1, f);
    s1 = obj.getString();
    if ((s1->getChar(0) & 0xff) == 0xfe &&
	(s1->getChar(1) & 0xff) == 0xff) {
      isUnicode = gTrue;
      i = 2;
    } else {
      isUnicode = gFalse;
      i = 0;
    }
    while (i < obj.getString()->getLength()) {
      if (isUnicode) {
	u = ((s1->getChar(i) & 0xff) << 8) |
	    (s1->getChar(i+1) & 0xff);
	i += 2;
      } else {
	u = pdfDocEncoding[s1->getChar(i) & 0xff];
	++i;
      }
      n = uMap->mapUnicode(u, buf, sizeof(buf));
      fwrite(buf, 1, n, f);
    }
    fputs(text2, f);
  }
  obj.free();
}

static void printInfoDate(FILE *f, Dict *infoDict, char *key, char *fmt) {
  Object obj;
  char *s;

  if (infoDict->lookup(key, &obj)->isString()) {
    s = obj.getString()->getCString();
    if (s[0] == 'D' && s[1] == ':') {
      s += 2;
    }
    fprintf(f, fmt, s);
  }
  obj.free();
}
