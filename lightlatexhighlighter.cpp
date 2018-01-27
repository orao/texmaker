/***************************************************************************
 *   copyright       : (C) 2003-2009 by Pascal Brachet                     *
 *   http://www.xm1math.net/texmaker/                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <QtGui>

#include "lightlatexhighlighter.h"
#include "lightlatexeditor.h"
#include "blockdata.h"

LightLatexHighlighter::LightLatexHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
isGraphic=false;
ColorStandard = QColor("#FFFFFF");
ColorComment = QColor("#606060");
ColorMath = QColor(0x00,0x80, 0x00);
ColorCommand=QColor(0x80, 0x00, 0x00);
ColorKeyword=QColor(0x00, 0x00, 0xCC);
ColorVerbatim = QColor("#9A4D00");
ColorTodo=QColor("#FF0000");
ColorKeywordGraphic=QColor("#006699");
ColorNumberGraphic=QColor("#660066");
KeyWords= QString("section{,subsection{,subsubsection{,chapter{,part{,paragraph{,subparagraph{,section*{,subsection*{,subsubsection*{,chapter*{,part*{,paragraph*{,subparagraph*{,label{,includegraphics{,includegraphics[,includegraphics*{,includegraphics*[,include{,input{,begin{,end{").split(",");
KeyWordsGraphic=QString("and controls tension atleast curl if else while for do return break continue struct typedef new access import unravel from include quote static public private restricted this explicit true false null cycle newframe Braid FitResult Label Legend Segment TreeNode abscissa arc arrowhead binarytree binarytreeNode block bool bool3 bounds bqe circle conic coord coordsys cputime ellipse file filltype frame grid3 guide horner hsv hyperbola indexedTransform int inversion key light line linefit marginT marker mass object pair parabola path path3 pen picture point position projection real revolution scaleT scientific segment side slice splitface string surface tensionSpecifier ticklocate ticksgridT tickvalues transform transformation tree triangle trilinear triple vector vertex void \\psset \\psline \\pcline \\pspolygon \\psframe \\pscircle \\pswedge \\psarc \\psellipse \\psbezier \\parabola \\pscurve \\psdot \\psdots \\psgrid \\uput \\rput \\psplot \\parametricplot \\dataplot \\fileplot \\listplot \\psaxes \\draw \\fill \\filldraw \\pattern \\shade \\shadedraw \\clip \\path").split(" ");
}

LightLatexHighlighter::~LightLatexHighlighter(){
}

void LightLatexHighlighter::SetEditor(LightLatexEditor *ed)
{
editor=ed;
}

void LightLatexHighlighter::setColors(QList<QColor> colors)
{
ColorStandard=colors.at(0);
ColorComment=colors.at(1);
ColorMath=colors.at(2);
ColorCommand=colors.at(3);
ColorKeyword=colors.at(4);
ColorVerbatim=colors.at(5);
ColorTodo=colors.at(6);
ColorKeywordGraphic=colors.at(7);
ColorNumberGraphic=colors.at(8);
rehighlight();
}

void LightLatexHighlighter::highlightBlock(const QString &text)
{
QRegExp rxSweave("<<(.*)>>=");
QStringList types;
types << QLatin1String("article") << QLatin1String("book")
  << QLatin1String("booklet") << QLatin1String("inbook")
  << QLatin1String("incollection") << QLatin1String("inproceedings")
  << QLatin1String("manual") << QLatin1String("mastersthesis")
  << QLatin1String("misc") << QLatin1String("phdthesis")
  << QLatin1String("proceedings") << QLatin1String("techreport")
  << QLatin1String("unpublished") << QLatin1String("periodical")
  << QLatin1String("conference");
QRegExp rxBib("@("+types.join("|")+")\\s*\\{\\s*(.*),", Qt::CaseInsensitive);
rxBib.setMinimal(true);

const int StateStandard = 0;
const int StateComment = 1;
const int StateMath = 2;
const int StateCommand=3;
const int StateVerbatim =4;
const int StateVerbatimCommand =5;
const int StateSweave =6;
const int StateSweaveCommand =7;
const int StateGraphic =8;
const int StateGraphicCommand =9;
const int StateGraphicMath =10;
const int StateBib =11;
const int StateGraphicComment =12;

int i = 0;
int state = previousBlockState();
if (state<0) state=0;
QChar last, next ,ch,tmp, verbflag;
verbflag=' ';
QString buffer;


BlockData *blockData = new BlockData;
int leftPos = text.indexOf( '{' );
while ( leftPos != -1 ) 
  {
  ParenthesisInfo *info = new ParenthesisInfo;
  info->character = '{';
  info->position = leftPos;

  blockData->insertPar( info );
  leftPos = text.indexOf( '{', leftPos+1 );
  }

int rightPos = text.indexOf('}');
while ( rightPos != -1 ) 
  {
  ParenthesisInfo *info = new ParenthesisInfo;
  info->character = '}';
  info->position = rightPos;

  blockData->insertPar( info );
  rightPos = text.indexOf( '}', rightPos+1 );
  }
  
setCurrentBlockUserData(blockData);


/////////////////////

/////////////////
QRegExp rxverb("verb(.)");
QRegExp rxlst("lstinline(.)");
QTextCharFormat structFormat;
structFormat.setFontWeight(QFont::Bold);
structFormat.setForeground(ColorKeyword);
if (!isGraphic)
{
i=0;
blockData->code.clear(); 
blockData->misspelled.clear(); 
for (int j=0; j < text.length(); j++) {blockData->code.append(0);blockData->misspelled.append(false);}
while (i < text.length())
	{
        ch = text.at( i );
	buffer += ch;
	if ( i < text.length()-1 ) next = text.at( i+1 );

        switch (state) {
	
	case StateStandard: {
		tmp=text.at( i );
		if (tmp=='\\') {
			if (next=='[')
				{
				setFormat( i, 1,ColorMath );
				blockData->code[i]=1;
				state=StateMath;
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
			else
			{
			setFormat( i, 1,ColorCommand );
			blockData->code[i]=1;
			state=StateCommand;
			}
			buffer = QString::null;
		} else
		if (tmp=='$') {
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateMath;
			if (next=='$')
				{
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
			buffer = QString::null;
		} else
		if (tmp== '%' ){
			setFormat( i, 1,ColorComment);
			state=StateComment;
			blockData->code[i]=1;
			buffer = QString::null;
		} else
		if (tmp== '{' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
		} else
		if (tmp== '}' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
			if(buffer.indexOf("begin{verbatim}") != -1) {state=StateVerbatim;}
			if(buffer.indexOf("begin{lstlisting}") != -1) {state=StateVerbatim;}
			if(buffer.indexOf("begin{asy}") != -1) {state=StateGraphic;}
			if(buffer.indexOf("begin{tikzpicture}") != -1) {state=StateGraphic;}
			if(buffer.indexOf("begin{pspicture}") != -1) {state=StateGraphic;}
			if(buffer.indexOf("begin{pspicture*}") != -1) {state=StateGraphic;}
			buffer = QString::null;
		} else
		if (tmp== '<' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
		} else
		if (tmp== '>' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
		} else
		if (tmp== ',' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
			if(buffer.indexOf(rxBib) != -1) {state=StateBib;buffer = QString::null;}
		} else
		if (tmp== '=' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			if(buffer.indexOf(rxSweave) != -1) {state=StateSweave;buffer = QString::null;}
		} else
		if (tmp== '(' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
			buffer = QString::null;
		} else
		if (tmp== ')' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
			buffer = QString::null;
		} else
		if (isWordSeparator(tmp)){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			//buffer = QString::null;
		} else
		 {
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
			//buffer = QString::null;
		}
//	buffer = QString::null;
	} break;
	
	case StateComment: {
	setFormat( i, 1,ColorComment);
	blockData->code[i]=1;
	state=StateComment;
	buffer = QString::null;
	} break;

	case StateMath: {
		tmp=text.at( i );
		if (tmp== '$') {
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateStandard;
			if (next=='$')
				{
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
		} else if (tmp== '\\') {
			if (next==']')
				{
				setFormat( i, 1,ColorMath);
				blockData->code[i]=1;
				state=StateStandard;
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
			else if (next=='$')
				{
				setFormat( i, 1,ColorMath);
				blockData->code[i]=1;
				state=StateMath;
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath/*QColor("#838307")*/);
					blockData->code[i]=1;
					}
				}
			else
				{
				setFormat( i, 1,ColorMath);
				blockData->code[i]=1;
				state=StateMath;
				}
		} else
		if (tmp== '{' ){
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateMath;
		} else
		if (tmp== '}' ){
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateMath;
		} else
		 {
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateMath;
		}
	buffer = QString::null;
	} break;
	case StateGraphicMath: {
		tmp=text.at( i );
		if (tmp== '$') {
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphic;
			if (next=='$')
				{
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
		} else if (tmp== '\\') {
			if (next==']')
				{
				setFormat( i, 1,ColorMath);
				blockData->code[i]=1;
				state=StateGraphic;
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
			else
				{
				setFormat( i, 1,ColorMath);
				blockData->code[i]=1;
				state=StateGraphicMath;
				}
		} else
		if (tmp== '{' ){
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphicMath;
		} else
		if (tmp== '}' ){
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphicMath;
		} else
		 {
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphicMath;
		}
	buffer = QString::null;
	} break;
	case StateCommand:{
		tmp=text.at( i );
		if (rxverb.exactMatch(buffer))
				{
				 verbflag=rxverb.cap(1).at(0);
				 blockData->code[i]=1;
				 setFormat( i, 1,ColorVerbatim);
				 state=StateVerbatim;
		} else
		if (rxlst.exactMatch(buffer))
				{
				 verbflag=rxlst.cap(1).at(0);
				 blockData->code[i]=1;
				 setFormat( i, 1,ColorVerbatim);
				 state=StateVerbatim;
		} else
		if (tmp=='$') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorCommand);
				blockData->code[i]=1;
				state=StateStandard;
				}
			else
				{
				setFormat( i, 1,ColorMath);
				blockData->code[i]=1;
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
				blockData->code[i]=1;
				state=StateComment;
         			}
		} else
		if (tmp== ' ') {
         		setFormat( i, 1,ColorStandard);
         		state=StateStandard;
		} else
		if (tmp== '\t') {
         		setFormat( i, 1,ColorStandard);
         		state=StateStandard;
		}  else
		if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateStandard;
			if ( buffer.length() > 0 )
				{
				for ( QStringList::Iterator it = KeyWords.begin(); it != KeyWords.end(); ++it ) 
					{
        				if (( *it ).indexOf( buffer )!=-1) 
						{
						if (*it!="begin{" && *it!="end{") setFormat( i - buffer.length(), buffer.length(),structFormat);
						else setFormat( i - buffer.length(), buffer.length(),ColorKeyword);
						blockData->code[i]=1;
						}
					}
				}
		} else
		if (tmp=='\\' || tmp==',' || tmp==';' /*|| tmp=='\''*/ || tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
			blockData->code[i]=1;
			if (last=='\\')
			{
				setFormat( i, 1,ColorCommand);
				blockData->code[i]=1;
				state=StateStandard;
			}
			else
			{
				setFormat( i, 1,ColorCommand);
				blockData->code[i]=1;
				state=StateCommand;
			}
		} else
	     		{
         		setFormat( i, 1,ColorCommand);
			blockData->code[i]=1;
         		state=StateCommand;
		}
	} break;
       case StateVerbatim: {
               tmp=text.at( i );
		if (tmp==verbflag && verbflag!=' ')
		{
		setFormat( i, 1,ColorCommand);
		blockData->code[i]=1;
		state=StateStandard;  
		verbflag=' ';
		} else
		if (tmp=='\\') {
			if (next=='[')
				{
				setFormat( i, 1,ColorVerbatim );
				blockData->code[i]=1;
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorVerbatim);
					blockData->code[i]=1;
					}
				}
			else
			{
			setFormat( i, 1,ColorVerbatim );
			blockData->code[i]=1;
			state=StateVerbatimCommand;
			}
		} else
		if (tmp=='$') {
			setFormat( i, 1,ColorVerbatim);
			blockData->code[i]=1;
			if (next=='$')
				{
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorVerbatim);
					blockData->code[i]=1;
					}
				}
			buffer = QString::null;
		} else
		if (tmp== '%' ){
			setFormat( i, 1,ColorVerbatim);
			blockData->code[i]=1;
			buffer = QString::null;
		} else
		if (tmp== '{' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
		} else
		if (tmp== '}' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			state=StateVerbatim;
			int pos=buffer.indexOf("\\end{verbatim}");
			if( pos!= -1) 
			{
			    state=StateStandard;
			    setFormat(pos,4,ColorKeyword);
			    setFormat(pos+4,10,ColorStandard);
			}
			pos=buffer.indexOf("\\end{lstlisting}");
			if( pos!= -1) 
			{
			    state=StateStandard;
			    setFormat(pos,4,ColorKeyword);
			    setFormat(pos+4,12,ColorStandard);
			}
			buffer = QString::null;
		} else
		if (tmp== '(' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		if (tmp== ')' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		if (isWordSeparator(tmp)){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		 {
			setFormat( i, 1,ColorVerbatim);
			//buffer = QString::null;
		}
       } break;
	case StateVerbatimCommand:{
		tmp=text.at( i );
		if (tmp=='$') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateVerbatim;
				}
			else
				{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateVerbatim;
				}
		} else
		if (tmp=='%') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorVerbatim);
				state=StateVerbatim;
				}
			else
				{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateVerbatim;
         			}
		} else
		if (tmp== ' ') {
         		setFormat( i, 1,ColorVerbatim);
         		state=StateVerbatim;
		}  else
		if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			state=StateVerbatim;
		} else
		if (tmp=='\\' || tmp==',' || tmp==';' || /*tmp=='\'' ||*/ tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
			blockData->code[i]=1;
			if (last=='\\')
			{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateVerbatim;
			}
			else
			{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateVerbatim;
			}
		} else
	     		{
         		setFormat( i, 1,ColorVerbatim);
			blockData->code[i]=1;
         		state=StateVerbatimCommand;
		}
	} break;
       case StateGraphic: {
               tmp=text.at( i );
		if (tmp=='\\') {
			if (next=='[')
				{
				setFormat( i, 1,ColorVerbatim );
				blockData->code[i]=1;
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorVerbatim);
					blockData->code[i]=1;
					}
				}
			else
			{
			setFormat( i, 1,ColorVerbatim );
			blockData->code[i]=1;
			state=StateGraphicCommand;
			}
		} else
		if (tmp=='$') {
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphicMath;
			if (next=='$')
				{
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
			buffer = QString::null;
		} else
		if (tmp== '/' && next=='/'){
			setFormat( i, 1,ColorComment);
			state=StateGraphicComment;
			blockData->code[i]=1;
			buffer = QString::null;
		} else
		if (tmp== '%' ){
			setFormat( i, 1,ColorComment);
			state=StateGraphicComment;
			blockData->code[i]=1;
			buffer = QString::null;
		} else
		if (tmp== '{' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
		} else
		if (tmp== '}' ){
		  	blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			state=StateGraphic;
			int pos=buffer.indexOf("\\end{asy}");
			if( pos!= -1) 
			{
			    state=StateStandard;
			    setFormat(pos,4,ColorKeyword);
			    setFormat(pos+4,5,ColorStandard);
			}
			pos=buffer.indexOf("\\end{tikzpicture}");
			if( pos!= -1) 
			{
			    state=StateStandard;
			    setFormat(pos,4,ColorKeyword);
			    setFormat(pos+4,13,ColorStandard);
			}
			pos=buffer.indexOf("\\end{pspicture}");
			if( pos!= -1) 
			{
			    state=StateStandard;
			    setFormat(pos,4,ColorKeyword);
			    setFormat(pos+4,11,ColorStandard);
			}
			pos=buffer.indexOf("\\end{pspicture*}");
			if( pos!= -1) 
			{
			    state=StateStandard;
			    setFormat(pos,4,ColorKeyword);
			    setFormat(pos+4,12,ColorStandard);
			}
			buffer = QString::null;
		} else
		if (tmp== '(' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		if (tmp== ')' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		if (isWordSeparator(tmp) && tmp!='"' && tmp!='.'){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		 {
			setFormat( i, 1,ColorVerbatim);
			//buffer = QString::null;
		}
       } break;
       
	case StateGraphicComment: {
	setFormat( i, 1,ColorComment);
	blockData->code[i]=1;
	state=StateGraphicComment;
	buffer = QString::null;
	} break;
	
	case StateGraphicCommand:{
		tmp=text.at( i );
		if (tmp=='$') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateGraphic;
				}
			else
				{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateGraphic;
				}
		} else
		if (tmp=='%') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorVerbatim);
				state=StateGraphic;
				}
			else
				{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateGraphic;
         			}
		} else
		if (tmp== ' ') {
         		setFormat( i, 1,ColorVerbatim);
         		state=StateGraphic;
		}  else
		if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			state=StateGraphic;
		} else
		if (tmp=='\\' || tmp==',' || tmp==';' || /*tmp=='\'' ||*/ tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
			blockData->code[i]=1;
			if (last=='\\')
			{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateGraphic;
			}
			else
			{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateGraphic;
			}
		} else
	     		{
         		setFormat( i, 1,ColorVerbatim);
			//blockData->code[i]=1;
         		state=StateGraphicCommand;
		}
	} break;
/****************************/
       case StateBib: {
//	 qDebug() << "bib" << buffer << next;
               tmp=text.at( i );
/*		if (tmp== '\"' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			state=StateBibCommand;
		} else*/
		if (tmp== '}' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			if (next=='\n' || i == text.length()-1)
			  {
			  state=StateStandard;
			  buffer = QString::null;
			  }
		} else
		 {
			setFormat( i, 1,ColorVerbatim);
			state=StateBib;
			//buffer = QString::null;
		}
       } break;
/*       case StateBibCommand: {
	 	 qDebug() << "bibcommand" << buffer;
               tmp=text.at( i );
		if (tmp== '\"' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			state=StateBib;
		} else
		if (tmp== '}' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			if (next!=',' || i == text.length()-1)
			  {
			  state=StateBib;
			  }
			else 
			{
			 state=StateBibCommand; 
			}
		} else
		 {
			setFormat( i, 1,ColorVerbatim);
			state=StateBibCommand;
		}
       } break;*/
/***************************/
       case StateSweave: {
               tmp=text.at( i );
		if (tmp=='\\') {
			if (next=='[')
				{
				setFormat( i, 1,ColorVerbatim );
				blockData->code[i]=1;
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorVerbatim);
					blockData->code[i]=1;
					}
				}
			else
			{
			setFormat( i, 1,ColorVerbatim );
			blockData->code[i]=1;
			state=StateSweaveCommand;
			}
		} else
		if (tmp=='$') {
			setFormat( i, 1,ColorVerbatim);
			blockData->code[i]=1;
			if (next=='$')
				{
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorVerbatim);
					blockData->code[i]=1;
					}
				}
			buffer = QString::null;
		} else
		if (tmp== '%' ){
			setFormat( i, 1,ColorVerbatim);
			blockData->code[i]=1;
			buffer = QString::null;
		} else
		if (tmp== '{' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
		} else
		if (tmp== '}' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		if (tmp== '@' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			state=StateStandard;
			buffer = QString::null;
		} else
		if (tmp== '(' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		if (tmp== ')' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		if (isWordSeparator(tmp)){
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			buffer = QString::null;
		} else
		 {
			setFormat( i, 1,ColorVerbatim);
			//buffer = QString::null;
		}
       } break;
	case StateSweaveCommand:{
		tmp=text.at( i );
		if (tmp=='$') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateSweave;
				}
			else
				{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateSweave;
				}
		} else
		if (tmp=='%') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorVerbatim);
				state=StateSweave;
				}
			else
				{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateSweave;
         			}
		} else
		if (tmp== ' ') {
         		setFormat( i, 1,ColorVerbatim);
         		state=StateSweave;
		}  else
		if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
			blockData->code[i]=1;
			setFormat( i, 1,ColorVerbatim);
			state=StateSweave;
		} else
		if (tmp=='\\' || tmp==',' || tmp==';' /*|| tmp=='\''*/ || tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
			blockData->code[i]=1;
			if (last=='\\')
			{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateSweave;
			}
			else
			{
				setFormat( i, 1,ColorVerbatim);
				blockData->code[i]=1;
				state=StateSweave;
			}
		} else
	     		{
         		setFormat( i, 1,ColorVerbatim);
			blockData->code[i]=1;
         		state=StateSweaveCommand;
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
else if ( state == StateGraphicMath ) 
	{
	setCurrentBlockState(StateGraphicMath) ;
    	}
else if ( state == StateVerbatim ) 
       {
       setCurrentBlockState(StateVerbatim) ;
       }
else if ( state == StateVerbatimCommand ) 
       {
       setCurrentBlockState(StateVerbatimCommand) ;
       }
else if ( state == StateGraphicComment ) 
       {
       setCurrentBlockState(StateGraphic) ;
       }
else if ( state == StateGraphic ) 
       {
       setCurrentBlockState(StateGraphic) ;
       }
else if ( state == StateGraphicCommand ) 
       {
       setCurrentBlockState(StateGraphicCommand) ;
       }
else if ( state == StateSweave ) 
       {
       setCurrentBlockState(StateSweave) ;
       }
else if ( state == StateSweaveCommand ) 
       {
       setCurrentBlockState(StateSweaveCommand) ;
       }
else if ( state == StateBib ) 
       {
       setCurrentBlockState(StateBib) ;
       }
/*else if ( state == StateBibCommand ) 
       {
       setCurrentBlockState(StateBibCommand) ;
       } */
else 
	{
	setCurrentBlockState(StateStandard) ;
	state=StateStandard;
    	}

if (text.isEmpty()) return;
QBrush brushstandard(ColorStandard);
QBrush brushverbatim(ColorVerbatim);
QBrush brushcomment(ColorComment);
QTextCharFormat todoFormat;
todoFormat.setFontWeight(QFont::Bold);
todoFormat.setForeground(ColorTodo);
todoFormat.setBackground(Qt::yellow);
if (state == StateComment)
{
	i=0;
	while (i < text.length())
		{
		buffer = QString::null;
		ch = text.at( i );
		while (!isSpace(ch) || ch=='%')
		      {
		      buffer += ch;
		      i++;
		      if (i < text.length()) ch = text.at( i );
		      else break;
		      }
		if ((buffer.length() > 0) && (format(i - buffer.length()).foreground()==brushcomment)) 
		    {

        				if (buffer=="%TODO") 
						{
						setFormat( i - buffer.length()+1, buffer.length()-1,todoFormat);
						blockData->code[i]=1;
						}

		    }
		i++;
		}
}
if (state == StateGraphic || state == StateGraphicCommand)
{
	i=0;
	while (i < text.length())
		{
		buffer = QString::null;
		ch = text.at( i );
		while ((blockData->code[i]!=1 || ch=='\\') && (!isSpace(ch)))
		      {
		      buffer += ch;
		      i++;
		      if (i < text.length()) ch = text.at( i );
		      else break;
		      }
		if ((buffer.length() > 0) && (format(i - buffer.length()).foreground()==brushverbatim)) 
		    {
		    for ( QStringList::Iterator it = KeyWordsGraphic.begin(); it != KeyWordsGraphic.end(); ++it ) 
					{
        				if (*it==buffer ) 
						{
						setFormat( i - buffer.length(), buffer.length(),ColorKeywordGraphic);
						blockData->code[i]=1;
						}
					}
		    if (QRegExp("([0-9eE\\.\\-]+)").exactMatch(buffer)) setFormat( i - buffer.length(), buffer.length(),ColorNumberGraphic);
		    }
		i++;
		}
}
}
//****************************************************************
else
{
if (state<0) state=8;
i=0;
blockData->code.clear(); 
blockData->misspelled.clear(); 
for (int j=0; j < text.length(); j++) {blockData->code.append(0);blockData->misspelled.append(false);}
while (i < text.length())
	{
        ch = text.at( i );
	buffer += ch;
	if ( i < text.length()-1 ) next = text.at( i+1 );

        switch (state) {
	

	case StateGraphicMath: {
		tmp=text.at( i );
		if (tmp== '$') {
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphic;
			if (next=='$')
				{
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
		} else if (tmp== '\\') {
			if (next==']')
				{
				setFormat( i, 1,ColorMath);
				blockData->code[i]=1;
				state=StateGraphic;
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
			else
				{
				setFormat( i, 1,ColorMath);
				blockData->code[i]=1;
				state=StateGraphicMath;
				}
		} else
		if (tmp== '{' ){
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphicMath;
		} else
		if (tmp== '}' ){
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphicMath;
		} else
		 {
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphicMath;
		}
	buffer = QString::null;
	} break;

       case StateGraphic: {
               tmp=text.at( i );
		if (tmp=='\\') {
			if (next=='[')
				{
				setFormat( i, 1,ColorStandard );
				blockData->code[i]=1;
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorStandard);
					blockData->code[i]=1;
					}
				}
			else
			{
			setFormat( i, 1,ColorStandard );
			blockData->code[i]=1;
			state=StateGraphicCommand;
			}
		} else
		if (tmp=='$') {
			setFormat( i, 1,ColorMath);
			blockData->code[i]=1;
			state=StateGraphicMath;
			if (next=='$')
				{
				i++;
				if ( i < text.length())
					{
					setFormat( i, 1,ColorMath);
					blockData->code[i]=1;
					}
				}
			buffer = QString::null;
		} else
		if (tmp== '/' && next=='/'){
			setFormat( i, 1,ColorComment);
			state=StateGraphicComment;
			blockData->code[i]=1;
			buffer = QString::null;
		} else
		if (tmp== '%' ){
			setFormat( i, 1,ColorComment);
			state=StateGraphicComment;
			blockData->code[i]=1;
			buffer = QString::null;
		} else
		if (tmp== '{' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
		} else
		if (tmp== '}' ){
		  	blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateGraphic;
			buffer = QString::null;
		} else
		if (tmp== '(' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			buffer = QString::null;
		} else
		if (tmp== ')' ){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			buffer = QString::null;
		} else
		if (isWordSeparator(tmp) && tmp!='"' && tmp!='.'){
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			buffer = QString::null;
		} else
		 {
			setFormat( i, 1,ColorStandard);
			//buffer = QString::null;
		}
       } break;
       
	case StateGraphicComment: {
	setFormat( i, 1,ColorComment);
	blockData->code[i]=1;
	state=StateGraphicComment;
	buffer = QString::null;
	} break;
	
	case StateGraphicCommand:{
		tmp=text.at( i );
		if (tmp=='$') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorStandard);
				blockData->code[i]=1;
				state=StateGraphic;
				}
			else
				{
				setFormat( i, 1,ColorStandard);
				blockData->code[i]=1;
				state=StateGraphic;
				}
		} else
		if (tmp=='%') {
			if (last=='\\')
				{
				setFormat( i, 1,ColorStandard);
				state=StateGraphic;
				}
			else
				{
				setFormat( i, 1,ColorStandard);
				blockData->code[i]=1;
				state=StateGraphic;
         			}
		} else
		if (tmp== ' ') {
         		setFormat( i, 1,ColorStandard);
         		state=StateGraphic;
		}  else
		if (tmp=='(' || tmp=='[' || tmp=='{' || tmp==')' || tmp==']' || tmp=='}') {
			blockData->code[i]=1;
			setFormat( i, 1,ColorStandard);
			state=StateGraphic;
		} else
		if (tmp=='\\' || tmp==',' || tmp==';' || /*tmp=='\'' ||*/ tmp=='\"' || tmp=='`' || tmp=='^' || tmp=='~') {
			blockData->code[i]=1;
			if (last=='\\')
			{
				setFormat( i, 1,ColorStandard);
				blockData->code[i]=1;
				state=StateGraphic;
			}
			else
			{
				setFormat( i, 1,ColorStandard);
				blockData->code[i]=1;
				state=StateGraphic;
			}
		} else
	     		{
         		setFormat( i, 1,ColorStandard);
			//blockData->code[i]=1;
         		state=StateGraphicCommand;
		}
	} break;

	}
	last = ch;
	i++;
	}
if ( state == StateGraphicMath ) 
	{
	setCurrentBlockState(StateGraphicMath) ;
    	}
else if ( state == StateGraphicComment ) 
       {
       setCurrentBlockState(StateGraphic) ;
       }
else if ( state == StateGraphic ) 
       {
       setCurrentBlockState(StateGraphic) ;
       }
else if ( state == StateGraphicCommand ) 
       {
       setCurrentBlockState(StateGraphicCommand) ;
       }
else 
	{
	setCurrentBlockState(StateGraphic) ;
	state=StateGraphic;
    	}

if (text.isEmpty()) return;
if (state == StateGraphic || state == StateGraphicCommand)
{
	i=0;
	while (i < text.length())
		{
		buffer = QString::null;
		ch = text.at( i );
		while ((blockData->code[i]!=1 || ch=='\\') && (!isSpace(ch)))
		      {
		      buffer += ch;
		      i++;
		      if (i < text.length()) ch = text.at( i );
		      else break;
		      }
		if ((buffer.length() > 0) ) 
		    {
		    for ( QStringList::Iterator it = KeyWordsGraphic.begin(); it != KeyWordsGraphic.end(); ++it ) 
					{
        				if (*it==buffer ) 
						{
						setFormat( i - buffer.length(), buffer.length(),ColorKeywordGraphic);
						blockData->code[i]=1;
						}
					}
		    if (QRegExp("([0-9eE\\.\\-]+)").exactMatch(buffer)) setFormat( i - buffer.length(), buffer.length(),ColorNumberGraphic);
		    }
		i++;
		}
}
}
//*****************************************************************
}

////////////////////



bool LightLatexHighlighter::isWordSeparator(QChar c) const
{
    switch (c.toLatin1()) {
    case '.':
    case ',':
    case '?':
    case '!':
    case ':':
    case ';':
    case '-':
    case '<':
    case '>':
    case '[':
    case ']':
    case '(':
    case ')':
    case '{':
    case '}':
    case '=':
    case '/':
    case '+':
    case '%':
    case '&':
    case '^':
    case '`':
//    case '*':
//    case '\'':
    case '"':
    case '~':
        return true;
    default:
        return false;
    }
}

bool LightLatexHighlighter::isSpace(QChar c) const
{
    return c == QLatin1Char(' ')
        || c == QChar::Nbsp
        || c == QChar::LineSeparator
        || c == QLatin1Char('\t')
        ;
}

void LightLatexHighlighter::setModeGraphic(bool m)
{
isGraphic=m;
}

