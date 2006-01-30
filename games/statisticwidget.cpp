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
#include "statisticwidget.h"

#include <QPainter>
#include <QBrush>

#include "games.h"
 
#include <klocale.h>
#include <kdebug.h>

StatisticWidget::StatisticWidget( QWidget * parent )
	: QWidget( parent )
{
	setMinimumSize( 100, 200 );
	m_game = 0;

	num = white = black = 0;
}

void StatisticWidget::paintEvent( QPaintEvent * /*e*/ )
{
	QPainter p;
	p.begin( this );
	p.drawText(10,10, i18n("Number of Moves: %1").arg( num ));
	p.drawText( 10,30, i18n( "Number of white tokens: %1" ).arg(white) );
	p.drawText( 10,50, i18n( "Number of black tokens: %1" ).arg(black) );
	p.drawRect( 0,0,width()-2,height()-2 );
	p.end();
}

void StatisticWidget::updateData()
{
	if ( !m_game ) return;
	
	num = m_game->numberOfMoves(); 

	Move * move = m_game->move( num );

	if ( !move ) return;
	
	white = move->numberOfStones( Stone::White );
	black = move->numberOfStones( Stone::Black );
	
	update();
}

#include "statisticwidget.moc"
