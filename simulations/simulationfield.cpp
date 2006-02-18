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

#include <QPainter>
#include <QBrush>

SimulationfieldWidget::SimulationfieldWidget( QWidget * parent )
	: QFrame ( parent )
{
	m_field = 0;
	m_design = SimulationfieldWidget::CIRCLE;
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
	p.begin( this );
	p.setRenderHint( QPainter::Antialiasing, true );

	QBrush b_white( Qt::white, Qt::SolidPattern );
	QBrush b_black( Qt::black, Qt::SolidPattern );

	foreach( Stone * stone, m_field->stones() ) 
	{
		const int x = stone->position().x();
		const int y = stone->position().y();
		p.setBrush( QBrush( Qt::yellow, Qt::SolidPattern ) );

		p.drawRect( x*s, 
				y*s, 
				s,
				s );

		if ( stone)
		{
			if ( stone->player() == Stone::White )
				p.setBrush( b_white );
			else
				p.setBrush( b_black );

			switch ( m_design )
			{
				case CIRCLE:
					p.drawEllipse( x*s+2,
							y*s+2,
							s-4,
							s-4 );
					break;
				case SQUARE:
					p.drawRect( x*s,
							y*s,
							s,
							s );
					break;
				case DENSITY:
					break;
			}
		}
	}

	p.end();
}

#include "moc_simulationfield.cpp"
