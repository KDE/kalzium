/***************************************************************************
      
      elementbutton.cpp  -  description
                             -------------------
    begin                : Die Dez  28 16:04:56 CET 2002
    copyright            : (C) 2002, 2003 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "elementbutton.h"
#include "elementbutton.moc"

#include <qpainter.h>

ElementButton::ElementButton ( QWidget *parent, const char* name)
  : QButton (parent, name)
{
}

void ElementButton::drawButton( QPainter *paint )
{ 
  paint->drawRect(0,0, width(), height());
  drawButtonLabel ( paint );
}
