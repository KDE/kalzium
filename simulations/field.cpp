
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
	
	Stone* stone1 = stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 ) Stones << stone1;
	if ( stone2 ) Stones << stone2;
	if ( stone3 ) Stones << stone3;
	if ( stone4 ) Stones << stone4;

	return Stones.count();
}

int Field::neighboursTeam( Stone* stone, bool direct )
{
	QPoint point( stone->position() );
	
	QList<Stone*> TeamStones;
	
	Stone* stone1 = stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 && stone1->player() == stone->player() )
		TeamStones << stone1;
	if ( stone2 && stone2->player() == stone->player()  )
		TeamStones << stone2;
	if ( stone3 && stone3->player() == stone->player()  )
		TeamStones << stone3;
	if ( stone4 && stone4->player() == stone->player()  )
		TeamStones << stone4;
	
	return TeamStones.count();
}

void Field::exchangeStones( const QPoint& point, bool direct )
{
	Stone* stone = stoneAtPosition( point );
	
	QList<Stone*> otherTeamStones;
	
	Stone* stone1 = stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 && stone1->player() != stone->player() )
		otherTeamStones << stone1;
	if ( stone2 && stone2->player() != stone->player()  )
		otherTeamStones << stone2;
	if ( stone3 && stone3->player() != stone->player()  )
		otherTeamStones << stone3;
	if ( stone4 && stone4->player() != stone->player()  )
		otherTeamStones << stone4;
	
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
