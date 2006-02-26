/***************************************************************************
 *   Copyright (C) 2006      by Carsten Niehaus,    cniehaus@kde.org       *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/
#include "simulationfield.h"
#include "field.h"
#include "stone.h"

#include <QBrush>
#include <QPainter>
#include <QPixmap>

SimulationfieldWidget::SimulationfieldWidget( QWidget * parent )
	: QFrame( parent ), m_field( 0 ), m_pix( 0 ), m_dirty( true ),
	m_design( SimulationfieldWidget::CIRCLE )
{
}

void SimulationfieldWidget::paintEvent( QPaintEvent * /*e*/ )
{
	if ( !m_field ) return;

	const int x_size = m_field->xSize();
	const int y_size = m_field->ySize();

	//set the size of one field
	const int w = width() / x_size;
	const int h = height() / y_size;

	int s = qMin( h, w );

	QPainter p;

	if ( m_field->fieldtype() == Field::SQUARE )
		paintSquares( &p, s );
	else
		paintHex( &p, s );

	p.end();

	QPixmap tmp = *m_pix;

	p.begin( this );
	p.drawPixmap( 0, 0, tmp );
	p.end();
}

void SimulationfieldWidget::resizeEvent( QResizeEvent * )
{
	m_dirty = true;
}

void SimulationfieldWidget::paintSquares( QPainter * p, int s )
{
	const int x_size = m_field->xSize();
	const int y_size = m_field->ySize();

	//set the size of one field
	const int w = width() / x_size;
	const int h = height() / y_size;
	if ( m_dirty )
	{
		delete m_pix;
		m_pix = new QPixmap( x_size * s + 1, y_size * s + 1 );
		
		p->begin( m_pix );
		p->fillRect( m_pix->rect(), QBrush( Qt::yellow ) );
		for ( int i = 0; i <= x_size ; ++i )
			p->drawLine( i * s, 0, i * s, m_pix->height() );
		for ( int i = 0; i <= y_size ; ++i )
			p->drawLine( 0, i * s, m_pix->width(), i * s );
		p->end();

		m_dirty = false;
	}

	p->begin( m_pix );
	p->setRenderHint( QPainter::Antialiasing, true );

	QBrush b_white( Qt::white, Qt::SolidPattern );
	QBrush b_black( Qt::black, Qt::SolidPattern );

	foreach( Stone * stone, m_field->stones() ) 
	{
		const int x = stone->position().x();
		const int y = stone->position().y();

		if ( stone->player() == Stone::First )
			p->setBrush( b_white );
		else
			p->setBrush( b_black );

		switch ( m_design )
		{
			case CIRCLE:
				p->drawEllipse( x*s+2, y*s+2, s-4, s-4 );
				break;
			case SQUARE:
				p->drawRect( x*s, y*s, s, s );
				break;
			case DENSITY:
				break;
		}
	}
}

void SimulationfieldWidget::paintHex( QPainter * p, int s )
{
	const int x_size = m_field->xSize();
	const int y_size = m_field->ySize();

	//set the size of one field
	const int w = width() / x_size;
	const int h = height() / y_size;
	if ( m_dirty )
	{
		delete m_pix;
		m_pix = new QPixmap( x_size * s + 1, y_size * s + 1 );
		
		p->begin( m_pix );
		p->fillRect( m_pix->rect(), QBrush( Qt::yellow ) );
		p->end();

		m_dirty = false;
	}

	p->begin( m_pix );
	p->setRenderHint( QPainter::Antialiasing, true );

	QBrush b_white( Qt::white, Qt::SolidPattern );
	QBrush b_black( Qt::black, Qt::SolidPattern );

	foreach( Stone * stone, m_field->stones() ) 
	{
		if ( stone->player() == Stone::First )
			p->setBrush( b_white );
		else
			p->setBrush( b_black );

		//FIXME here we need to check if the design is CIRCLE, SQUARE and so n
		//but until all the drawing code is not working in general this is not needed
		
		//x and y are the base-coordinates of the hex (the top left corner)
		const int x = stone->position().x() * s;
		const int y = stone->position().y() * s;

		double x1 = 0;
		double y1 = 0;
			
		QPointF points[ 6 ];

		for ( int i = 0; i < 6; ++i )
		{
			if ( i == 0 )
			{
				x1 = x + s * 1/3;
				y1 = y;
			}
			else if ( i == 1 )
			{
				x1 = x + s*2/3;
				y1 = y;
			}
			else if ( i == 2 )
			{
				x1 = x + s;
				y1 = y + s * 1/2;
			}
			else if ( i == 3 )
			{
				x1 = x + s * 2/3;
				y1 = y + s;
			}
			else if ( i == 4 )
			{
				x1 = x + s * 1/3;
				y1 = y + s;
			}
			else if ( i == 5 )
			{
				x1 = x;
				y1 = y + s * 1/2;
			}
			QPointF p( x1, y1 );
			points[ i ] = p;
		}

		p->drawPolygon( points, 6 );
	}
}

#include "moc_simulationfield.cpp"
