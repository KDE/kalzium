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

#include <QPainter>

GamefieldWidget::GamefieldWidget( QWidget * parent )
	: QWidget ( parent )
{
	setMinimumSize( 100, 200 );
}

void GamefieldWidget::paintEvent( QPaintEvent * /*e*/ )
{
	//set the size of one field
	const int size_x = width() / m_field->xSize();
	const int size_y = width() / m_field->ySize();
	
	QPainter p;
	p.begin( this );

	p.drawLine( 0,0,height(), width() );
	p.drawRect( 0, 0, height(), width() );

	for ( int x = 0; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			p.drawRect( x*size_x, 
					y*size_y, 
					size_x,
					size_y );
		}
	}
	
	p.end();
}

#include "gamefieldwidget.moc"
