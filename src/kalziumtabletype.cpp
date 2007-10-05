/***************************************************************************
 *   Copyright (C) 2005, 2006     by Pino Toscano, toscano.pino@tiscali.it *
 *   Copyright (C) 2006, 2007     by Carsten Niehaus, cniehaus@kde.org     *
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
#include <kdebug.h>

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
	6, 6, 6, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,    //Element 71 (Lr)
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	7, 7, 7, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
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

static const int posYD[40] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};

static const int posXD[40] = {
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
	1, 2, 3, 4, 5, 6, 7, 8, 9, 10
};

///DZ is short for "Deutscher Zentralausschuss"
static const int posXDZ[117] = {
    1, 2,
    2, 2,
         3, 4, 5, 6, 7, 8,
    1, 2,
         3, 4, 5, 6, 7, 8,
    1, 2,
                           9,10,11,12,13,14,15,16,17,18,      
         3, 4, 5, 6, 7, 8, 
    1, 2,
                           9,10,11,12,13,14,15,16,17,18,      
         3 ,4 ,5 ,6 ,7 ,8 ,
    1, 2,
                                                              19,20,21,22,23,24,25,26,27,28,29,30,31,32,
                           9,10,11,12,13,14,15,16,17,18,      
         3 ,4 ,5 ,6 ,7 ,8 ,
    1, 2,
                                                              19,20,21,22,23,24,25,26,27,28,29,30,31,32,
                           9,10,11,12,13,14,15,16,17,18,      
         3 ,4 ,5 ,6  

};

///DZ is short for "Deutscher Zentralausschuss"
static const int posYDZ[117] = {
    1, 1,
    2, 2,
         3, 3, 3, 3, 3, 3,
    4, 4,
         5, 5, 5, 5, 5, 5,
    6, 6,
                          7, 7, 7, 7, 7, 7, 7, 7, 7, 7,      
         8, 8, 8, 8, 8, 8, 
    9, 9,
                          10,10,10,10,10,10,10,10,10,10,
         11,11,11,11,11,11,
    12,12,
                                                              13,13,13,13,13,13,13,13,13,13,13,13,13,13,    
                          14,14,14,14,14,14,14,14,14,14,
         15,15,15,15,15,15,
    16,16,
                                                              17,17,17,17,17,17,17,17,17,17,17,17,17,17,    
                          18,18,18,18,18,18,18,18,18,18,
         19,19,19,19 
};


KalziumTableTypeFactory::KalziumTableTypeFactory()
{
    m_tables << KalziumClassicTableType::instance();
    m_tables << KalziumShortTableType::instance();
    m_tables << KalziumDTableType::instance();
    m_tables << KalziumDZTableType::instance();
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

int KalziumTableType::previousOf( int element ) const
{
    return element > 1 ? element - 1 : -1;
}

int KalziumTableType::nextOf( int element ) const
{
    return element < KalziumDataObject::instance()->numberOfElements() ? element + 1 : -1;
}

int KalziumTableType::firstElement() const
{
    return 1;
}

QList<int> KalziumTableType::elementList() const
{
    return m_elementList;
}

QRect KalziumTableType::numerationRect( const int num, KalziumNumerationType *nt ) const
{
    if ( !nt ) return QRect();

    int c = nt->items().count();
    if ( ( num < 0 ) || ( num >= c ) )
        return QRect();

    return QRect( num * ELEMENTSIZE, 0, ELEMENTSIZE, ELEMENTSIZE );
}

QPoint KalziumTableType::elementUnderMouse( const QPoint& coords ) const
{
    int X = coords.x() / ELEMENTSIZE;
    int Y = coords.y() - ELEMENTSIZE;

    X += 1;
    Y = Y / ELEMENTSIZE + 1;

    return QPoint( X, Y );
}

//////////// Classis //////////////////////////////////////////////////////////////////////

KalziumClassicTableType* KalziumClassicTableType::instance()
{
    static KalziumClassicTableType kctt;
    return &kctt;
}

    KalziumClassicTableType::KalziumClassicTableType()
: KalziumTableType()
{
    const int numElements = KalziumDataObject::instance()->numberOfElements();
    for (int i = 1 ; i < numElements ; i++) 
        m_elementList.append(i);
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
    return QSize( ELEMENTSIZE * 18 + 1, ELEMENTSIZE * 11 );
}

int KalziumClassicTableType::elementAtCoords( const QPoint& coords ) const
{
    const QPoint ourcoord = elementUnderMouse( coords );

    int x = 0;
    int y = 0;
    foreach (int counter, m_elementList )
    {
        x = posXRegular[counter-1];
        y = posYRegular[counter-1];
        if ( ( ourcoord.x() == x ) && ( ourcoord.y() == y ) ) {
            return counter;
        }
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

    return QRect( x, y, ELEMENTSIZE, ELEMENTSIZE );
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
    const QPoint ourcoord = elementUnderMouse( coords );

    int x = 0;
    int y = 0;

    foreach (int counter, m_elementList )
    {
        int realNum = translateToShort( counter );
        x = posXShort[realNum-1];
        y = posYShort[realNum-1];

        if ( ( ourcoord.x() == x ) && ( ourcoord.y() == y ) ){
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

    if ( realElementNumber == 0 ) {
        kDebug() << "This element doesn't exist in this table...";
        return QRect( 0,0,0,0 );
    } else {
    }

    // x coord
    int x = ( posXShort[realElementNumber-1] - 1 ) * ELEMENTSIZE;
    // y coord
    int y = ( posYShort[realElementNumber-1] ) * ELEMENTSIZE;


    //    kDebug() << "Element " << realElementNumber << " (" << numelem << ") sits on " << posXD[numelem-1] << " :: " << posYD[numelem-1];

    return QRect( x, y, ELEMENTSIZE, ELEMENTSIZE );
}

int KalziumShortTableType::previousOf( int element ) const
{
    int index = m_elementList.indexOf( element );
    return index > 1 ? m_elementList.at( index - 1 ) : -1;
}

int KalziumShortTableType::nextOf( int element ) const
{
    int index = m_elementList.indexOf( element );
    return index != -1 && ( index < m_elementList.count() - 1 ) ? m_elementList.at( index + 1 ) : -1;
}


//////////////////////////// D-Block /////////////////////////////////////////

KalziumDTableType* KalziumDTableType::instance()
{
    static KalziumDTableType kctt;
    return &kctt;
}

    KalziumDTableType::KalziumDTableType()
: KalziumTableType()
{
    for (int i = 21 ; i < 31 ; i++) 
        m_elementList.append(i);

    for (int i = 39 ; i < 49 ; i++) 
        m_elementList.append(i);

    m_elementList.append(57);//Adding La

    for (int i = 72 ; i < 81 ; i++) 
        m_elementList.append(i);

    m_elementList.append(89);//Adding Ac

    for (int i = 104 ; i < 113 ; i++) 
        m_elementList.append(i);
}

QByteArray KalziumDTableType::name() const
{
    return "D";
}

QString KalziumDTableType::description() const
{
    return i18n( "Transition Elements" );
}

int KalziumDTableType::firstElement() const
{
    //The d-tables starts with Scandium, element 21
    return 21;
}


QSize KalziumDTableType::size() const
{
    return QSize( ELEMENTSIZE * 10 + 1, ELEMENTSIZE * 4 );
}

int KalziumDTableType::elementAtCoords( const QPoint& coords ) const
{
    const QPoint ourcoord = elementUnderMouse( coords );

    int x = 0;
    int y = 0;

    foreach (int counter, m_elementList )
    {
        x = posXD[counter-1];
        y = posYD[counter-1];
        if ( ( ourcoord.x() == x ) && ( ourcoord.y() == y ) ){
            return counter;
        }
    }

    // not found
    return 0;
}

int KalziumDTableType::translateToD(int num)
{
    if (num < 21 ) return 0;

    //1 becomes 21, 10 becomes 30
    if ( num < 31 ) 
        return num-20;

    //11 becomes 39, 20 becomes 48
    if ( num > 38 && num < 49 ) 
        return num-28;

    //21 becomes 57 (Lanthan)
    if ( num == 57 ) 
        return 21;

    //22 becomes 72, 30 becomes 80
    if ( num > 71 && num < 81 ) 
        return num-50;

    //31 becomes 89 (Ac)
    if ( num == 89 ) 
        return 31;

    //32 becomes 104, 40 becomes 112
    if ( num > 103 && num < 113 ) 
        return num-72;

    return 0;
}

QRect KalziumDTableType::elementRect( const int numelem ) const
{
    int realElementNumber = translateToD(numelem);

    if ( realElementNumber == 0 ) {
        kDebug() << "This element doesn't exist in this table...";
        return QRect( 0,0,0,0 );
    } else {
    }

    // x coord
    int x = ( posXD[realElementNumber-1] - 1 ) * ELEMENTSIZE;
    // y coord
    int y = ( posYD[realElementNumber-1] ) * ELEMENTSIZE;

    //    kDebug() << "Element " << realElementNumber << " (" << numelem << ") sits on " << posXD[numelem-1] << " :: " << posYD[numelem-1];

    return QRect( x, y, ELEMENTSIZE, ELEMENTSIZE );
}

int KalziumDTableType::previousOf( int element ) const
{
    int index = m_elementList.indexOf( element );
    return index > 1 ? m_elementList.at( index - 1 ) : -1;
}

int KalziumDTableType::nextOf( int element ) const
{
    int index = m_elementList.indexOf( element );
    return index != -1 && ( index < m_elementList.count() - 1 ) ? m_elementList.at( index + 1 ) : -1;
}

//////////// DZ ////////////////////////////////////////////////////////////////////////////

KalziumDZTableType* KalziumDZTableType::instance()
{
    static KalziumDZTableType kctt;
    return &kctt;
}

    KalziumDZTableType::KalziumDZTableType()
: KalziumTableType()
{
    const int numElements = KalziumDataObject::instance()->numberOfElements();
    for (int i = 1 ; i < numElements ; i++) 
        m_elementList.append(i);
}

QByteArray KalziumDZTableType::name() const
{
    return "DZ";
}

QString KalziumDZTableType::description() const
{
    return i18n( "DZ Periodic Table" );
}

QSize KalziumDZTableType::size() const
{
    return QSize( ELEMENTSIZE * 32 + 1, ELEMENTSIZE * 20 + 1 );
}

int KalziumDZTableType::elementAtCoords( const QPoint& coords ) const
{
    const QPoint ourcoord = elementUnderMouse( coords );

    int x = 0;
    int y = 0;
    foreach (int counter, m_elementList )
    {
        x = posXDZ[counter-1];
        y = posYDZ[counter-1];
        if ( ( ourcoord.x() == x ) && ( ourcoord.y() == y ) ) {
            return counter;
        }
    }

    // not found
    return 0;
}

QRect KalziumDZTableType::elementRect( const int numelem ) const
{
    kDebug() << "KalziumDZTableType::elementRect(), num: " << numelem;

    // x coord
    int x = ( posXDZ[numelem-1] - 1 ) * ELEMENTSIZE;
    // y coord
    int y = ( posYDZ[numelem-1] ) * ELEMENTSIZE;

    return QRect( x, y, ELEMENTSIZE, ELEMENTSIZE );
}

