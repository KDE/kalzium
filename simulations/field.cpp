
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

QList<Stone*> Field::tokensOnPosition( const QPoint& point )
{
	QList<Stone*> list;
	foreach( Stone * s , m_stones )
	{
		if ( s->position() == point )
			list.append( s );
	}

	return list;
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

int Field::neighboursNum( Stone* stone, bool direct )
{
	QPoint point( stone->position() );
	
	QList<Stone*> Stones;
	QList<Stone*> n = neighbours( point, direct );

	qDebug("Neighbours: %d", n.count() );
	
	foreach( Stone * s , n )
		if ( stone->player() == s->player() )
			Stones << s;

	return Stones.count();
}

int Field::neighboursTeam( Stone* stone, bool direct )
{
	QPoint point( stone->position() );
	
	QList<Stone*> TeamStones;
	
	QList<Stone*> n = neighbours( point, direct );
	
	foreach( Stone * s , n )
		if ( stone->player() == s->player() )
			TeamStones << s;
	
	return TeamStones.count();
}

void Field::exchangeStones( const QPoint& point, bool direct )
{
	Stone* stone = stoneAtPosition( point );
	
	QList<Stone*> n = neighbours( point, direct );
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

QPoint Field::freeNeighbourCell( const QPoint& point )
{
	return point;
}

QList<Stone*> Field::neighbours( const QPoint& point, bool direct )
{
	//FIXME I am not sure this is the best way... of course it works! But
	//isn't there a better way to filter out null-pointers? append(Ptr) 
	//doesn't check for null-pointers it seems...
	//I would love to have something like QList<>::strip() which removes
	//all null-pointers.
	QList<Stone*> stones;

	//save the x and y value
	const int x = point.x();
	const int y = point.y();

	if ( m_fieldtype == Field::HEX )
	{
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
