/***************************************************************************
 *   Copyright (C) 2005, 2006      by Pino Toscano, toscano.pino@tiscali.it
 *   Copyright (C) 2006, 2007      by Carsten Niehaus, cniehaus@kde.org
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

static const int posXShort[44] = {
	1,                   8,//He
	1, 2, 3, 4, 5, 6, 7, 8,//Ne
	1, 2, 3, 4, 5, 6, 7, 8,//Ar
	1, 2, 3, 4, 5, 6, 7, 8,//Kr
	1, 2, 3, 4, 5, 6, 7, 8,//Xe
	1, 2, 3, 4, 5, 6, 7, 8,//Rn  
	1, 2                  //Fr and Ra 
};

static const int posYShort[44] = {
	1,                   1,//He
	2, 2, 2, 2, 2, 2, 2, 2,//Ne
	3, 3, 3, 3, 3, 3, 3, 3,//Ar
	4, 4, 4, 4, 4, 4, 4, 4,//Kr
	5, 5, 5, 5, 5, 5, 5, 5,//Xe
	6, 6, 6, 6, 6, 6, 6, 6,//Rn  
	7, 7                   //Fr and Ra
};

KalziumTableTypeFactory::KalziumTableTypeFactory()
{
	m_tables << KalziumClassicTableType::instance();
	m_tables << KalziumShortTableType::instance();
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
    for (int i = 1 ; i < 117 ; i++) {
        m_elementList.append(i);
    }
}

QByteArray KalziumClassicTableType::name() const
{
	return "Classic";
}

QList<int> KalziumClassicTableType::elementList() const
{
	return m_elementList;
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
	const QPoint ourcoord = elementUnderMouse( coords );

	int x = 0;
	int y = 0;
	const int numElements = KalziumDataObject::instance()->numberOfElements();
	for ( int counter = 1; counter <= numElements; counter++ )
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

//////////////////////////// SHORT /////////////////////////////////////////

KalziumShortTableType* KalziumShortTableType::instance()
{
	static KalziumShortTableType kctt;
	return &kctt;
}

KalziumShortTableType::KalziumShortTableType()
  : KalziumTableType()
{

    //I now need to append all the elements which are in the short table
    //The short table only shows elements in the s and p-block, that means
    //I am skipping all elements in the f and d-block.
    for (int i = 1 ; i < 21 ; i++) 
        m_elementList.append(i);
    for (int i = 31 ; i < 39 ; i++) 
        m_elementList.append(i);
    for (int i = 49 ; i < 57 ; i++) 
        m_elementList.append(i);
    for (int i = 81 ; i < 89 ; i++) 
        m_elementList.append(i);
}

QByteArray KalziumShortTableType::name() const
{
	return "Short";
}

QList<int> KalziumShortTableType::elementList() const
{
	return m_elementList;
}

QString KalziumShortTableType::description() const
{
	return i18n( "Short Periodic Table" );
}

QSize KalziumShortTableType::size() const
{
	return QSize( ELEMENTSIZE * 8 + 1, ELEMENTSIZE * 8 );
}

int KalziumShortTableType::elementAtCoords( const QPoint& coords ) const
{
    kDebug() << "KalziumShortTableType::elementAtCoords()" << endl;
	const QPoint ourcoord = elementUnderMouse( coords );

	int x = 0;
	int y = 0;
	const int numElements = KalziumDataObject::instance()->numberOfElements();
	for ( int counter = 1; counter <= numElements; counter++ )
	{
        int realElementNumber = translateToShort(counter);
		x = posXShort[realElementNumber-1];
		y = posYShort[realElementNumber-1];
		if ( ( ourcoord.x() == x ) && ( ourcoord.y() == y ) )
        {
            kDebug() << "KalziumShortTableType::elementAtCoords(). Element: " << counter << endl;
			return counter;
        }
	}

	// not found
	return 0;
}

int KalziumShortTableType::translateToShort(int num)
{
    //for the first 21 elements, nothing changes
    if ( num < 21 ) return num;

    //now 31 to 38
    if ( num < 49 && num > 30 )
        return num-10;

    //now 49 to 56
    if ( num < 57 && num > 48 )
        return num-20;
    
    //now 81 to 88
    if ( num < 89 && num > 80 )
        return num-44;

    return 0;
}

QRect KalziumShortTableType::elementRect( const int numelem ) const
{
    int realElementNumber = translateToShort(numelem);
	// x coord
	int x = ( posXShort[realElementNumber-1] - 1 ) * ELEMENTSIZE;
	// y coord
	int y = ( posYShort[realElementNumber-1] ) * ELEMENTSIZE;

kDebug() << "KalziumShortTableType::elementRect(). Number: " << numelem << " on " << x/40 << "::"
    << y/40 << endl;
	return QRect( x, y, ELEMENTSIZE, ELEMENTSIZE );
}

QRect KalziumShortTableType::legendRect() const
{
	int legendLeft   = ELEMENTSIZE * 5 / 2;
	int legendTop    = ELEMENTSIZE * 4 / 5;
	int legendWidth  = ELEMENTSIZE * 9;
	int legendHeight = ELEMENTSIZE * 3;

	return QRect( legendLeft, legendTop, legendWidth, legendHeight );
}

QRect KalziumShortTableType::numerationRect( const int num, KalziumNumerationType *nt ) const
{
	if ( !nt ) return QRect();

	int c = nt->items().count();
	if ( ( num < 0 ) || ( num >= c ) )
		return QRect();

	return QRect( num * ELEMENTSIZE, 0, ELEMENTSIZE, ELEMENTSIZE );
}

QPoint KalziumShortTableType::elementUnderMouse( const QPoint& coords ) const
{
	int X = coords.x() / ELEMENTSIZE;
	int Y = coords.y() - ELEMENTSIZE;
    
	return QPoint( X, Y );
}
