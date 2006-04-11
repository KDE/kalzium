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
#include <QPolygonF>

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
//X 	const int w = width() / x_size;
//X 	const int h = height() / y_size;

	m_dirty = true;
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
	
	QPixmap icon;
	QRectF target;
			
	QPixmap one ( ":/images/one.png" );
	QPixmap two ( ":/images/two.png" );

	foreach( Stone * stone, m_field->stones() ) 
	{
		const int x = stone->position().x();
		const int y = stone->position().y();
		
		if ( stone->player() == Stone::First )
		{
			icon = one;
			p->setBrush( b_white );
		}
		else
		{
			icon = two;
			p->setBrush( b_black );
		}

		switch ( m_design )
		{
			case CIRCLE:
//X 				p->drawEllipse( x*s+2, y*s+2, s-4, s-4 );
				target = QRectF( x*s+2, y*s+2, s-4, s-4 );
				p->drawPixmap(  target.topLeft(), icon );
				break;
			case SQUARE:
//X 				p->drawRect( x*s, y*s, s, s );
				target = QRectF( x*s+2, y*s+2, s-4, s-4 );
				p->drawPixmap(  target.topLeft(), icon );
				break;
			case DENSITY:
				//FIXME implement
				p->drawRect( x*s, y*s, s, s );
				break;
		}
	}
}

void SimulationfieldWidget::paintHex( QPainter * p, int s )
{
	const int x_size = m_field->xSize();
	const int y_size = m_field->ySize();

	s *= 0.75;

	//set the size of one field
	const int w = width() / x_size;
	const int h = height() / y_size;
	if ( m_dirty )
	{
		delete m_pix;
		m_pix = new QPixmap( x_size * s + 1, y_size * s + 1 );

		p->begin( m_pix );
		//draw the big rect
		p->fillRect( m_pix->rect(), QBrush( Qt::white ) );

		p->setBrush( QBrush( Qt::black ) );
		p->setPen( Qt::white );

		//now draw the hex'es
		for ( int i = 0; i <= x_size ; ++i )
		{
			for ( int e = 0; e <= y_size ; ++e )
			{
				double x1;
				double y1;

				int x = i*s - i*s*1/4;
				int y = e*s;

				if ( i%2 )
				{//for odd cols: make y half a cell lower!
					y += s*1/2;
				}

				QPolygonF points;

				points << QPointF( x + s * 0.25, y );
				points << QPointF( x + s * 0.75, y );
				points << QPointF( x + s, y + s * 0.5 );
				points << QPointF( x + s * 0.75, y + s );
				points << QPointF( x + s * 0.25, y + s );
				points << QPointF( x, y + s * 0.5 );

				p->drawPolygon( points );
			}
		}
		p->end();

		m_dirty = false;
	}

	p->begin( m_pix );
	p->setRenderHint( QPainter::Antialiasing, true );

	QBrush b_white( Qt::yellow, Qt::SolidPattern );
	QBrush b_black( Qt::red, Qt::SolidPattern );

	foreach( Stone * stone, m_field->stones() ) 
	{
		int a = stone->position().x();
		int b = stone->position().y();

		int x = a*s - a*s*1/4;
		int y = b*s;

		if ( a%2 )
		{//for odd cols: make y half a cell lower!
			y += s*1/2;
		}

		qDebug("Position a: %d",a);
		qDebug("Position b: %d",b);
		qDebug("Position x: %d",x);
		qDebug("Position y: %d",y);

		if ( stone->player() == Stone::First )
			p->setBrush( b_white );
		else
			p->setBrush( b_black );

		p->drawEllipse( x*s+2, y*s+2, s-4, s-4 );
	}
}

#include "simulationfield.moc"
