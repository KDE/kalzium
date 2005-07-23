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

#include <qpainter.h>
#include <qsimplerichtext.h>
#include <qscrollview.h>

ElementBox::ElementBox( QWidget *parent, const char* name ) : QWidget( parent, name )
{
	m_rowCount = 18;
	m_colCount = 9;
	m_border = 25;
	m_lanthanoidDist = 50;

	setRowCol();

	m_richText = 0;

	setBackgroundMode( NoBackground ); // widget has no background

	connect( &hoverTimer, SIGNAL( timeout() ), this, SLOT( slotToolTip() ) );
	connect( &mouseOverTimer, SIGNAL( timeout() ),this, SLOT( slotMouseover() ) );
}

ElementBox::~ElementBox()
{
	delete m_richText;
}

void ElementBox::paintEvent( QPaintEvent* /*e*/ )
{
	QPainter p( this );
	p.drawRoundRect( 0, 0, width(), height() ); 

	delete m_richText;

	m_richText = new QSimpleRichText( "<qt><h1>" + m_element->symbol() + "</h1><p>", font() );
                                               //     + m_underline + "</p><p>"
                                               //     +   +"</p></qt>", font() );

	m_richText->setWidth( width() - 5 );
	m_richText->draw( &p, 5, 5, rect(), colorGroup() );
	
	setCoordinates();
	show();
}

void ElementBox::mouseMoveEvent( QMouseEvent* /*e*/ )
{
	emit mouseMove( m_element->number() );
}

void ElementBox::mousePressEvent( QMouseEvent* /*e*/ )
{
	emit mousePress( m_element->number() );
}

void ElementBox::focusInEvent( QFocusEvent* /*e*/ )
{
	//do some kind of highlighting
}

void ElementBox::focusOutEvent( QFocusEvent* /*e*/ )
{
	// remove highlighting
}

void ElementBox::setCoordinates()
{
	QScrollView *p = 0;
        if ( dynamic_cast<QScrollView*>( parent() ) )
                p = static_cast<QScrollView*>( parent() );

	// resize the box
	int width = ( p->contentsWidth() - 2 * m_border ) / m_colCount;
	int height = ( p->contentsHeight() - 2 * m_border ) / m_rowCount;

	resize( width, height );

	
	// move to correct position
	m_dynVertBorder = ( ( p->contentsHeight() - 2 * m_border ) % height ) / 2;
	m_dynHoriBorder = ( ( p->contentsWidth() - 2 * m_border ) % width ) / 2;

	if ( m_row < 8 )
		move( m_border + m_dynHoriBorder + width * m_col, m_border + m_dynVertBorder + height * m_row );
	else
		move( m_border + m_dynHoriBorder + width * m_col, 
					m_border + m_dynVertBorder + height * m_row + m_lanthanoidDist );
}

void ElementBox::setRowCol()
{	
	m_row = m_element->period();

	// exception for lanthanoids and actinoids
	if ( m_element->number() > 57 && m_element->number() < 72 )
		m_row = 8;
	else if ( m_element->number() > 89 && m_element->number() < 104 )
		m_row = 9;


 	if ( m_element->number() > 105 )	// elements without known orbital structure
		m_col = m_element->number() - 100;
	else if ( m_element->block() == "s" )
		m_col = m_element->group().toInt();
	else if ( m_element->block() == "p" )
		m_col = m_element->group().toInt() + 10;
	else if ( m_element->block() == "d" || m_element->number() == 57 || m_element->number() == 89 )
	{	
		QString tmp = m_element->orbits().section( " ", -2, -2 ); 
		m_col = tmp.section( "d", -1, -1 ).toInt();
	}
	else if ( m_element->block() == "f" && m_element->number() != 57 && m_element->number() != 89 )
	{
		QString tmp = m_element->orbits().section( " ", 1, 1 );
		m_col = tmp.section( "f", -1, -1 ) .toInt();	
	}	
}

#include "elementbox.moc"

