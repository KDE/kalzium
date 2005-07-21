/***************************************************************************
 *   Copyright (C) 2005 by Martin Pfeiffer                                 *
 *   hubipete@gmx.net                                                      *
 *									   *		
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "elementbox.h"

#include "element.h"

ElementBox::ElementBox( QWidget *parent, const char* name ) : QWidget( parent, name )
{
	setBackgroundMode( NoBackground ); // widget has no background

	setCoordiantes();

	m_richText = 0;
}

void ElementBox::paintEvent( QPaintEvent* e )
{
	QPainter p( this );
	p->drawRoundRect( 0, 0, width(), height() ); 

	delete m_richText;

	m_richText = new QSimpleRichText( "<qt><h1>" + m_element->symbol() + "</h1><p>"
                                                    + m_underline + "</p><p>"
                                                    +   +"</p></qt>", font() );
}

void ElementBox::mouseMoveEvent( QMouseEvent *e )
{
}

void ElementBox::mousePressEvent( QMouseEvent *e )
{
}

void ElementBox::mouseReleaseEvent( QMouseEvent *e )
{
}

void ElementBox::focusInEvent( QFocusEvent* e )
{

}

void ElementBox::focusOutEvent( QFocusEvent* e )
{
}

void ElementBox::setCoordinates()
{
	// move to the correct coordinates... 
	//move();
}



#include "elementbox.moc"

