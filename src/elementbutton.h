/***************************************************************************

      elementbutton.h  -  description
                             -------------------
    begin                : Die Dez  28 15:59:34 CET 2002
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


#ifndef ELEMENTBUTTON_H
#define ELEMENTBUTTON_H

#include <qbutton.h>

class ElementButton : public QButton
{
  Q_OBJECT

  public:
    ElementButton ( QWidget *parent, const char* name=0 );
  protected:
    void  drawButton( QPainter * );
    virtual void  drawButtonLabel( QPainter * ) = 0;
};

#endif

