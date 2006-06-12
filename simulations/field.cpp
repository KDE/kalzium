
/***************************************************************************
 *   Copyright (C) 2005,2006 by Carsten Niehaus,    cniehaus@kde.org       *
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

#include <stdlib.h>
#include <time.h>

#include "field.h"
#include "stone.h"

///Field
Field::Field( FIELDTYPE t )
{
	//FIXME Is this if-check needed? t has a default-value in the constructor...
	if ( t != Field::SQUARE )            
		m_fieldtype = t;				 
	else 								
		m_fieldtype = Field::SQUARE;

	m_size_x = m_size_y = 6;
}

Field::~Field()
{}

Stone* Field::stoneAtPosition( const QPoint& pos )
{
	foreach( Stone* s, m_stones )
	{
		if ( s->position() == pos )
			return s;
	}

	return 0;
}

void Field::clear()
{
	m_stones.clear();
}

void Field::removeStone( Stone * stone )
{
	//FIXME Is this really the preferred way to handle this? Shouldn't it
	//be easier to remove a stone from the list?
	QList<Stone*> newList;
	
	foreach( Stone * st , m_stones )
	{
		if (stone != st)
			newList.append( st );
	}

	m_stones = newList;
}

QList<Stone*> Field::neighboursTeam( Stone* stone, bool direct )
{
	QList<Stone*> TeamStones;
	
	QList<Stone*> n = neighbours( stone, direct );
	
	foreach( Stone * s , n )
		if ( stone->player() == s->player() )
			TeamStones << s;
	
	return TeamStones;
}

void Field::exchangeStones( const QPoint& point, bool direct )
{
	Stone* stone = stoneAtPosition( point );
	
	QList<Stone*> n = neighbours( stone, direct );
	QList<Stone*> otherTeamStones;
	
	foreach( Stone * s , n )
		if ( stone->player() != s->player() )
			otherTeamStones << s;
	
	if ( otherTeamStones.count() < 1 )//well, there is nothing to exchange...
	{
		return;
	}

	//the stone to exchange...
	Stone *chosenStone = 0;
	
	if ( otherTeamStones.count() == 1 ) {
		chosenStone = otherTeamStones[ 0 ];//take the first (and only) Stone
	}
	else{
		const int choice = ( int ) ( rand()%otherTeamStones.count() );

		chosenStone = otherTeamStones[ choice ];
	}

	chosenStone->swap();
	stone->swap();
}

QPoint Field::randomFreeNeighbourCell( const QPoint& point, bool direct )
{
	srand (  time( NULL ) );
	QList<QPoint> points = freeNeighbourCell( point, direct );
	
	if ( points.count() == 0 )
		return point;//no free cells found!

	const int x = ( int ) ( rand()%points.count() );

	return points.at( x );
}

QList<QPoint> Field::freeNeighbourCell( const QPoint& point, bool direct )
{
	QList<QPoint> points;
	
	const int x = point.x();
	const int y = point.y();

	if ( m_fieldtype == Field::HEX )
	{
		//FIXME
		//have to think about it
	}
	else if ( m_fieldtype == Field::SQUARE )
	{
		Stone* stone1 = stoneAtPosition( QPoint(x+1, y ) );
		Stone* stone2 = stoneAtPosition( QPoint(x-1, y ) );
		Stone* stone3 = stoneAtPosition( QPoint(x,   y+1 ) );
		Stone* stone4 = stoneAtPosition( QPoint(x,   y-1 ) );

		if ( !stone1 )
			points << QPoint( x+1,y );
		if ( !stone2 )
			points << QPoint( x-1,y );
		if ( !stone3 )
			points << QPoint( x,y+1 );
		if ( !stone4 )
			points << QPoint( x,y-1 );
		
		if ( !direct )
		{//count the diagonal points as well!
			Stone* d1 = stoneAtPosition( QPoint(x+1, y+1 ) );
			Stone* d2 = stoneAtPosition( QPoint(x+1, y-1 ) );
			Stone* d3 = stoneAtPosition( QPoint(x-1, y+1 ) );
			Stone* d4 = stoneAtPosition( QPoint(x-1, y-1 ) );
		
			if ( !d1 )
				points << QPoint(x+1, y+1 ) ;
			if ( !d2 )
				points << QPoint(x+1, y-1 );
			if ( !d3 )
				points << QPoint(x-1, y+1 );
			if ( !d4 )
				points << QPoint(x-1, y-1 );
		}
	}

//X 	qDebug( "Position x: %d", point.x() );
//X 	qDebug( "Position y: %d", point.y() );
//X 	qDebug( "number of free cells: %d", points.count() );
//X 	
	return points;
}

QList<Stone*> Field::neighbours( Stone* stone, bool direct )
{
	//FIXME I am not sure this is the best way... of course it works! But
	//isn't there a better way to filter out null-pointers? append(Ptr) 
	//doesn't check for null-pointers it seems...
	//I would love to have something like QList<>::strip() which removes
	//all null-pointers.
	QList<Stone*> stones;

	//save the x and y value
	const int x = stone->position().x();
	const int y = stone->position().y();

	if ( m_fieldtype == Field::HEX )
	{
		//FIXME
		//have to think about it
	}
	else if ( m_fieldtype == Field::SQUARE )
	{
		Stone* stone1 = stoneAtPosition( QPoint(x+1, y ) );
		Stone* stone2 = stoneAtPosition( QPoint(x-1, y ) );
		Stone* stone3 = stoneAtPosition( QPoint(x,   y+1 ) );
		Stone* stone4 = stoneAtPosition( QPoint(x,   y-1 ) );

		if ( stone1 )
			stones << stone1;
		if ( stone2 )
			stones << stone2;
		if ( stone3 )
			stones << stone3;
		if ( stone4 )
			stones << stone4;
		if ( !direct )
		{//count the diagonal stones as well!
			Stone* d1 = stoneAtPosition( QPoint(x+1, y+1 ) );
			Stone* d2 = stoneAtPosition( QPoint(x+1, y-1 ) );
			Stone* d3 = stoneAtPosition( QPoint(x-1, y+1 ) );
			Stone* d4 = stoneAtPosition( QPoint(x-1, y-1 ) );
		
			if ( d1 )
				stones << d1;
			if ( d2 )
				stones << d2;
			if ( d3 )
				stones << d3;
			if ( d4 )
				stones << d4;
		}
	}

	return stones;
}

Stone* Field::randomStone( Stone::PLAYER player )
{
	Stone *stone = 0;
	QList<Stone*> stones;
	srand (  time( NULL ) );

	foreach( Stone*s, m_stones )
	{
		if ( s->player() == player )
			stones << s;
	}
	const int x = ( int ) ( rand()%stones.count() );
	return stones.at( x );
}

void Field::debugOutput()
{
	QString debug = QString();

	qDebug( "# %d", allStones( Stone::First ).count() );
	return;

	for ( int y = 0; y < ySize() ; ++y )
	{
		debug += "\n";
		for ( int x = 0 ; x < xSize() ; ++x )
		{
			QPoint p( x,y );
 			Stone *s = stoneAtPosition( p );
			if ( !s )
			{
				debug += " ";	
				continue;
			}
			
			if ( s->player() == Stone::First )
				debug += "1";
			else if ( s->player() == Stone::Second )
				debug += "2";
			else if ( s->player() == Stone::Third )
				debug += "3";
			else if ( s->player() == Stone::Fourth )
				debug += "4";
		}
	}
	qDebug( "%s", qPrintable( debug ) );
}

QList<Stone*> Field::allStones( Stone::PLAYER player )
{
	QList<Stone*> stones;

	foreach( Stone * s, m_stones )
		if ( s->player() == player )
			stones << s;

	return stones;
}
