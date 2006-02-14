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

#include "symbollistwidget.h"
#include "icondelegate.h" 
#include <QFont>
#include <QStandardItemModel>
#include <QHeaderView>

SymbolListWidget :: SymbolListWidget(QWidget *parent, int page) : QTableWidget(parent)
{
setItemDelegate(new IconDelegate(this));
QString icon_name;
setShowGrid(true);
verticalHeader()->hide();
horizontalHeader()->hide();
setIconSize ( QSize(32,32 ));
setSelectionMode (QAbstractItemView::SingleSelection);
switch (page)
{
	case 0:
	{
		setColumnCount(4);
		setRowCount(57);
		setColumnWidth(0,40);
		setColumnWidth(1,40);
		setColumnWidth(2,40);
		setColumnWidth(3,40);
		for ( uint i = 0; i <= 56; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i,0,item);
		setRowHeight(i,40);
		}
		for ( uint i = 57; i <= 113; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-57,1,item);
		}
		for ( uint i = 114; i <= 170; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-114,2,item);
		}
		for ( uint i = 171; i <= 225; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-171,3,item);
		}
	}
	break;
	case 1:
	{
		setColumnCount(4);
		setRowCount(17);
		setColumnWidth(0,40);
		setColumnWidth(1,40);
		setColumnWidth(2,40);
		setColumnWidth(3,40);
		for ( uint i = 247; i <= 263; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-247,0,item);
		setRowHeight(i-247,40);
		}
		for ( uint i = 264; i <= 280; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-264,1,item);
		}
		for ( uint i = 281; i <= 297; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-281,2,item);
		}
		for ( uint i = 298; i <= 313; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-298,3,item);
		}
	}
	break;
	case 2:
	{
		setColumnCount(4);
		setRowCount(15);
		setColumnWidth(0,40);
		setColumnWidth(1,40);
		setColumnWidth(2,40);
		setColumnWidth(3,40);
		for ( uint i = 314; i <= 328; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-314,0,item);
		setRowHeight(i-314,40);
		}
		for ( uint i = 329; i <= 343; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-329,1,item);
		}
		for ( uint i = 344; i <= 358; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-344,2,item);
		}
		for ( uint i = 359; i <= 371; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-359,3,item);
		}
	}
	break;
	case 3:
	{
		setColumnCount(4);
		setRowCount(6);
		setColumnWidth(0,40);
		setColumnWidth(1,40);
		setColumnWidth(2,40);
		setColumnWidth(3,40);
		for ( uint i = 226; i <= 231; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-226,0,item);
		setRowHeight(i-226,40);
		}
		for ( uint i = 232; i <= 237; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-232,1,item);
		}
		for ( uint i = 238; i <= 243; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-238,2,item);
		}
		for ( uint i = 244; i <= 246; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+".png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-244,3,item);
		}
	}
	break;
	case 4:
	{
		setColumnCount(4);
		setRowCount(10);
		setColumnWidth(0,40);
		setColumnWidth(1,40);
		setColumnWidth(2,40);
		setColumnWidth(3,40);
		for ( uint i = 0; i <= 9; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+"greek.png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i+372]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i,0,item);
		setRowHeight(i,40);
		}
		for ( uint i = 10; i <= 19; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+"greek.png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i+372]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-10,1,item);
		}
		for ( uint i = 20; i <= 29; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+"greek.png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i+372]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-20,2,item);
		}
		for ( uint i = 30; i <= 39; ++i )
		{
		icon_name=":/symbols/img"+QString::number(i+1)+"greek.png";
		QTableWidgetItem* item= new QTableWidgetItem();
		item->setIcon(QIcon(icon_name));
		item->setText(code[i+372]);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		setItem(i-30,3,item);
		}
	}
	break;
}

}
