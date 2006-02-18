
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
Field::Field()
{
}

Field::~Field()
{}

Stone* Field::stoneAtPosition( const QPoint& pos )
{
	Stone* stone;

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
