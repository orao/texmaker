/***************************************************************************
 *   copyright       : (C) 2003-2005 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QtGui>

#include "latexhighlighter.h"

LatexHighlighter::LatexHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
	ColorStandard = QColor(0x00, 0x00, 0x00);
	ColorComment = QColor(0x83, 0x81, 0x83);
	ColorMath = QColor(0x00,0x80, 0x00);
	ColorCommand=QColor(0x80, 0x00, 0x00);
	ColorKeyword=QColor(0x00, 0x00, 0xCC);
	KeyWords= QString("section{,subsection{,subsubsection{,chapter{,part{,paragraph{,subparagraph{,section*{,subsection*{,subsubsection*{,chapter*{,part*{,paragraph*{,subparagraph*{,label{,includegraphics{,includegraphics[,includegraphics*{,includegraphics*[,include{,input{,begin{,end{").split(",");
}

void LatexHighlighter::highlightBlock(const QString &text)
{
int i = 0;
int state = previousBlockState();
if (state<0) state=0;
QChar last, next ,ch,tmp;
QString buffer;
const int StateStandard = 0;
const int StateComment = 1;
const int StateMath = 2;
const int StateCommand=3;

while (i < text.length())
	{
        ch = text.at( i );
	buffer += ch;
	if ( i < text.length()-1 ) next = text.at( i+1 );

        switch (state) {
	
	case StateStandard: {
		tmp=text.at( i );
		if (tmp=='\\') {
			setFormat( i, 1,ColorCommand );
			state=StateCommand;
		} else
		if (tmp=='$') {
			setFormat( i, 1,ColorMath);
			state=StateMath;
			if (next=='$')
				{
				i++;
				setFormat( i, 1,ColorMath);
				}
		} else
		if (tmp== '%' ){
			setFormat( i, 1,ColorComment);
			state=StateComment;
		} else
		 {
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
		}
	buffer = QString::null;
	} break;
	
	case StateComment: {
	setFormat( i, 1,ColorComment);
	state=StateComment;
	buffer = QString::null;
	} break;

	case StateMath: {
		tmp=text.at( i );
		if (tmp== '$') {
			setFormat( i, 1,ColorMath);
			state=StateStandard;
			if (next=='$')
				{
				i++;
				setFormat( i, 1,ColorMath);
				}
		} else
		 {
			setFormat( i, 1,ColorMath);
			state=StateMath;
		}
	buffer = QString::null;
	} break;

	case StateCommand:{
		tmp=text.at( i );
		if (tmp=='$') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorCommand);
				state=StateStandard;
				}
			else
				{
				setFormat( i, 1,ColorMath);
				state=StateMath;
				}
		} else
		if (tmp=='%') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorStandard);
				state=StateStandard;
				}
			else
				{
				setFormat( i, 1,ColorComment);
				state=StateComment;
         			}
		} else
		if (tmp== ' ') {
         		setFormat( i, 1,ColorStandard);
         		state=StateStandard;
		}  else
		if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
			if ( buffer.length() > 0 )
				{
				for ( QStringList::Iterator it = KeyWords.begin(); it != KeyWords.end(); ++it ) 
					{
        				if (( *it ).indexOf( buffer )!=-1) setFormat( i - buffer.length(), buffer.length(),ColorKeyword);
					}
				}
		} else
		if (tmp=='\\' || tmp==',' || tmp==';' || tmp=='\'' || tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
			if (last=='\\')
			{
				setFormat( i, 1,ColorCommand);
				state=StateStandard;
			}
			else
			{
				setFormat( i, 1,ColorCommand);
				state=StateCommand;
			}
		} else
	     		{
         		setFormat( i, 1,ColorCommand);
         		state=StateCommand;
		}
	} break;
	}
	last = ch;
	i++;
	}
if ( state == StateComment ) 
 	{
	setCurrentBlockState(StateStandard);
    	} 
else if ( state == StateMath ) 
	{
	setCurrentBlockState(StateMath) ;
    	} 
else 
	{
	setCurrentBlockState(StateStandard) ;
    	}
}
