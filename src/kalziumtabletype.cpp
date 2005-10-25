/***************************************************************************
 *   Copyright (C) 2005      by Pino Toscano, toscano.pino@tiscali.it      *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "kalziumtabletype.h"

#include "kalziumdataobject.h"
#include "kalziumnumerationtype.h"

#include <klocale.h>

#define ELEMENTSIZE 40

static const int posXRegular[117] = {
	1,                                                         18,
	1, 2,                                  13, 14, 15, 16, 17, 18,
	1, 2,                                  13, 14, 15, 16, 17, 18,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, //Element 54 (Xe)
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,     //Element 71 (Lr)
	         4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,     //Element 71 (Lr)
	         4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
};

static const int posYRegular[117] = {
	1,                                                         1,
	2, 2,                                  2,  2,  2,  2,  2,  2,
	3, 3,                                  3,  3,  3,  3,  3,  3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  4,  4,  4,  4,  4,  4,  4,  4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  5,  5,  5,  5,  5,  5,  5,  5, //Element 54 (Xe)
	6, 6, 6, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,    //Element 71 (Lr)
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

KalziumTableTypeFactory::KalziumTableTypeFactory()
{
	m_tables << KalziumClassicTableType::instance();
}

KalziumTableTypeFactory* KalziumTableTypeFactory::instance()
{
	static KalziumTableTypeFactory kttf;
	return &kttf;
}

KalziumTableType* KalziumTableTypeFactory::build( int id ) const
{
	if ( ( id < 0 ) || ( id >= m_tables.count() ) )
		return 0;

	return m_tables.at( id );
}

KalziumTableType* KalziumTableTypeFactory::build( const QByteArray& id ) const
{
	for ( int i = 0; i < m_tables.count(); i++ )
	{
		if ( m_tables.at( i )->name() == id )
			return m_tables.at( i );
	}
	
	// not found
	return 0;
}

QStringList KalziumTableTypeFactory::tables() const
{
	QStringList l;
	for ( int i = 0; i < m_tables.count(); i++ )
	{
		l << m_tables.at( i )->description();
	}
	return l;
}


KalziumTableType* KalziumTableType::instance()
{
	return 0;
}

KalziumTableType::KalziumTableType()
{
}

KalziumTableType::~KalziumTableType()
{
}

KalziumClassicTableType* KalziumClassicTableType::instance()
{
	static KalziumClassicTableType kctt;
	return &kctt;
}

KalziumClassicTableType::KalziumClassicTableType()
  : KalziumTableType()
{
}

QByteArray KalziumClassicTableType::name() const
{
	return "Classic";
}

QString KalziumClassicTableType::description() const
{
	return i18n( "Classic Periodic Table" );
}

QSize KalziumClassicTableType::size() const
{
	return QSize( ELEMENTSIZE * 18 + 1, ELEMENTSIZE * 10 + 30 );
}

int KalziumClassicTableType::elementAtCoords( const QPoint& coords ) const
{
	QPoint ourcoord = elementUnderMouse( coords );

	int x = 0;
	int y = 0;
	for ( int counter = 1; counter <= KalziumDataObject::instance()->numberOfElements(); counter++ )
	{
		x = posXRegular[counter-1];
		y = posYRegular[counter-1];
		if ( ( ourcoord.x() == x ) && ( ourcoord.y() == y ) )
			return counter;
	}

	// not found
	return 0;
}

QRect KalziumClassicTableType::elementRect( const int numelem ) const
{
	// x coord
	int x = ( posXRegular[numelem-1] - 1 ) * ELEMENTSIZE;
	// y coord
	int y = ( posYRegular[numelem-1] ) * ELEMENTSIZE;

	// mind the small gap over rare earth!
	// 57=Lanthanum, 72=Hafnium, 89=Actinium & 104=Rutherfordium (i.e., if
	// n_number is in rare earth's block)
	if ( ( numelem > 57 && numelem < 72 ) || ( numelem > 89 && numelem < 104 ) )
		y += ELEMENTSIZE / 3;

	return QRect( x, y, ELEMENTSIZE, ELEMENTSIZE );
}

QRect KalziumClassicTableType::legendRect() const
{
	int legendLeft   = ELEMENTSIZE * 5 / 2;
	int legendTop    = ELEMENTSIZE * 4 / 5;
	int legendWidth  = ELEMENTSIZE * 9;
	int legendHeight = ELEMENTSIZE * 3;

	return QRect( legendLeft, legendTop, legendWidth, legendHeight );
}

QRect KalziumClassicTableType::numerationRect( const int num, KalziumNumerationType *nt ) const
{
	if ( !nt ) return QRect();

	int c = nt->items().count();
	if ( ( num < 0 ) || ( num >= c ) )
		return QRect();

	return QRect( num * ELEMENTSIZE, 0, ELEMENTSIZE, ELEMENTSIZE );
}

QPoint KalziumClassicTableType::elementUnderMouse( const QPoint& coords ) const
{
	int X = coords.x() / ELEMENTSIZE;
	int Y = coords.y() - ELEMENTSIZE;

	// mind the gap over rare earth!
	if ( Y >= ( ELEMENTSIZE * 7 ) && Y < ( ELEMENTSIZE * 7 + ELEMENTSIZE / 3 + 1 ) )
		return QPoint();

	if ( Y > ( ELEMENTSIZE * 7 ) )
		Y -= ELEMENTSIZE / 3;

	X += 1;
	Y = Y / ELEMENTSIZE + 1;

	return QPoint( X, Y );
}
