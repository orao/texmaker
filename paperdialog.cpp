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

#include "paperdialog.h"


PaperDialog::PaperDialog(QWidget *parent, const char *name)
    :QDialog( parent)
{
setWindowTitle("Texmaker");
setModal(true);
ui.setupUi(this);
ui.comboBoxPaper->addItem( "a1" );
ui.comboBoxPaper->addItem( "a2" );
ui.comboBoxPaper->addItem( "a3" );
ui.comboBoxPaper->addItem( "a4" );
ui.comboBoxPaper->addItem( "a5" );
ui.comboBoxPaper->addItem( "a6" );
ui.comboBoxPaper->addItem( "a7" );
ui.comboBoxPaper->addItem( "a8" );
ui.comboBoxPaper->addItem( "11x17" );
ui.comboBoxPaper->addItem( "ledger" );
ui.comboBoxPaper->addItem( "legal" );
ui.comboBoxPaper->addItem( "letter" );
ui.comboBoxPaper->addItem( "lettersmall" );
}

PaperDialog::~PaperDialog(){
}

