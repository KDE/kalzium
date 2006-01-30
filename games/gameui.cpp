/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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
#include "gameui.h"

GameWidget::GameWidget( QWidget * parent )
	: QWidget( parent )
{

}

void GameWidget::paintEvent( QPaintEvent * /*e*/ )
{
	QPainter p( this );

	if ( !m_field ) return;

	int square = width()  /  6;
	
	for ( int x = 0; x < 6 ; ++x )
	{
		for ( int y = 0; y < 6 ; ++y )
		{
			Stone* s = m_field->stoneAtPosition( QPoint( x,y ) );
			if ( s->player() == Stone::White )
				p.drawText(  square * x, square*y, "W" );
			else
				p.drawText(  square * x, square*y, "B" );
		}
	}
}

#include "gameui.moc"
