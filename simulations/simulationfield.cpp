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

	if ( m_dirty )
	{
		delete m_pix;
		m_pix = new QPixmap( x_size * s + 1, y_size * s + 1 );
		
		p.begin( m_pix );
		p.fillRect( m_pix->rect(), QBrush( Qt::yellow ) );
		for ( int i = 0; i <= x_size ; ++i )
			p.drawLine( i * s, 0, i * s, m_pix->height() );
		for ( int i = 0; i <= y_size ; ++i )
			p.drawLine( 0, i * s, m_pix->width(), i * s );
		p.end();

		m_dirty = false;
	}

	p.begin( m_pix );
	p.setRenderHint( QPainter::Antialiasing, true );

	QBrush b_white( Qt::white, Qt::SolidPattern );
	QBrush b_black( Qt::black, Qt::SolidPattern );

	foreach( Stone * stone, m_field->stones() ) 
	{
		const int x = stone->position().x();
		const int y = stone->position().y();

		if ( stone->player() == Stone::White )
			p.setBrush( b_white );
		else
			p.setBrush( b_black );

		switch ( m_design )
		{
			case CIRCLE:
				p.drawEllipse( x*s+2, y*s+2, s-4, s-4 );
				break;
			case SQUARE:
				p.drawRect( x*s, y*s, s, s );
				break;
			case DENSITY:
				break;
		}
	}

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

#include "moc_simulationfield.cpp"
