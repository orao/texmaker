/***************************************************************************
 *   copyright       : (C) 2006 by Pascal Brachet                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "playerbutton.h"


PlayerButton::PlayerButton( QWidget *parent )
    : QLabel( parent )
{
state = true;
}


void PlayerButton::setImages( QString name )
{
this->pixEnabled.load(":/images/"+name+"_enabled.png");
this->pixHover.load(":/images/"+name+"_enabled.png");
this->pixDisabled.load(":/images/"+name+"_disabled.png");
this->setPixmap(this->pixEnabled);
}


void PlayerButton::setEnabled( bool enabled )
{
state = enabled;
this->setPixmap( enabled ? this->pixEnabled : this->pixDisabled );
}


void PlayerButton::mouseReleaseEvent( QMouseEvent * e )
{
emit clicked();
}


void PlayerButton::enterEvent( QEvent * e )
{
//if (state) this->setPixmap( pixHover );
}


void PlayerButton::leaveEvent( QEvent *e )
{
//setEnabled( state );
}

