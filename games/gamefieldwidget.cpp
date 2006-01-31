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
#include "gamefieldwidget.h"
#include "field.h"
#include "stone.h"

#include <QPainter>
#include <QBrush>

#include <kdebug.h>

GamefieldWidget::GamefieldWidget( QWidget * parent )
	: QWidget ( parent )
{
	m_field = 0;
	setMinimumSize( 100, 200 );
}

void GamefieldWidget::paintEvent( QPaintEvent * /*e*/ )
{
	if ( !m_field ) return;
//X 	kdDebug() << "painting the field with ptr " << m_field << endl;

	const int x_size = m_field->xSize();
	const int y_size = m_field->ySize();

	//set the size of one field
	const int w = width() / x_size;
	const int h = height() / y_size;

	int s;
	w < h ? s = w : s = h;

	QPainter p;
	p.begin( this );
	p.setRenderHint( QPainter::Antialiasing, true );

	QBrush b_white( Qt::white, Qt::SolidPattern );
	QBrush b_black( Qt::black, Qt::SolidPattern );

	for ( int x = 0; x < x_size ; ++x )
	{
		for ( int y = 0; y < y_size ; ++y )
		{
 			Stone * stone = m_field->stoneAtPosition( QPoint( x,y ) );
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

				p.drawEllipse( x*s+2,
						y*s+2,
						s-4,
						s-4 );
			}
		}
	}

	p.end();
}

#include "gamefieldwidget.moc"
