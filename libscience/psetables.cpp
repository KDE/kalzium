/*********************************************************************************
 *   Copyright (C) 2005, 2006   by Pino Toscano, toscano.pino@tiscali.it         *
 *   Copyright (C) 2007         by Carste Niehaus, cniehaus@kde.org              *
 *   copyright (C) 2010         by Etienne Rebetez, etienne.rebetez@oberwallis.ch*
 *                                                                               *
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

#include "psetables.h"

#include <klocale.h>
#include <kdebug.h>


pseTables::pseTables()
{
    m_tables << pseRegularTable::init();
    m_tables << pseShortTable::init();
    m_tables << pseLongTable::init();
    m_tables << pseDTable::init();
    m_tables << pseDZTable::init();
}

pseTables::~pseTables()
{
//     foreach(psTable *i, m_tables) {
//         delete i;
//     }
}

pseTables *pseTables::instance()
{
    static pseTables tables;
    return &tables;
}

QStringList pseTables::tables() const
{
    QStringList l;
    for ( int i = 0; i < m_tables.count(); i++ )
    {
        l << m_tables.at( i )->description();
    }
    return l;
}

pseTable* pseTables::getTabletype(const int tableType)
{
    if ( ( tableType < 0 ) || ( tableType >= m_tables.count() ) ) {
        return 0;
    }

    return m_tables.at( tableType );
}

pseTable* pseTables::getTabletype(const QString tableName)
{
    for ( int i = 0; m_tables.count(); i++ ) {
        if (tableName == m_tables.at( i )->name() ) {
            return m_tables.at( i );
        }
    }
    return 0;
}

pseTable::pseTable()
{
}

pseTable::~pseTable()
{
}

pseTable *pseTable::init()
{
    return 0;
}

QString pseTable::name() const
{
    return m_name;
}

QString pseTable::description() const
{
    return m_description;
}

QList<int> pseTable::elements() const
{
    return m_elementList;
}

int pseTable::previousOf( int element ) const
{
    int index = m_elementList.indexOf( element );
    return index > 1 ? m_elementList.at( index - 1 ) : -1;
}

int pseTable::nextOf( int element ) const
{
    int index = m_elementList.indexOf( element );
    return index != -1 && ( index < m_elementList.count() - 1 ) ? m_elementList.at( index + 1 ) : -1;
}

int pseTable::firstElement() const
{
    return m_elementList.first();
}

int pseTable::lastElement() const
{
    return m_elementList.last();
}

QPoint pseTable::elementCoords(const int element)
{
    int elementIndex = m_elementList.indexOf( element );

    if ( elementIndex >= 0 && elementIndex < m_elementList.count() ) {
        return QPoint(m_posX.at( elementIndex ), m_posY.at( elementIndex ) );
    }
    return QPoint(0,0);
}

QPoint pseTable::coordsMax() const
{
    int x = 0, y = 0, i;

    for (i = 0; i < m_posX.count(); i++) {
        if (m_posX.at(i) > x) {
            x = m_posX.at(i) ;
        }
        if (m_posY.at(i) > y) {
            y = m_posY.at(i);
        }
    }
    return QPoint(x, y);
}

int pseTable::numeration( int xPos ) const
{
  if ( xPos >= 0 && xPos < m_xCoordsNumeration.count() ) {
     return m_xCoordsNumeration.at( xPos );
  }
  return -1;
}

/// Regular Table Data
pseRegularTable::pseRegularTable()
        : pseTable()
{
    m_name = "Classic";

    m_description = i18n( "Classic Periodic Table" );

    m_xCoordsNumeration <<
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18;

    m_posX <<
    1 <<                                                                                         18 <<
    1 << 2 <<                                                      13 << 14 << 15 << 16 << 17 << 18 <<
    1 << 2 <<                                                      13 << 14 << 15 << 16 << 17 << 18 <<
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 <<
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 << //Element 54 (Xe)
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 <<       //Element 58 (Ce) 71 (Lu)
    4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18 <<
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 <<       //Element 71 (Lr)
    4 << 5 << 6 << 7 << 8 << 9 << 10 << 11 << 12 << 13 << 14 << 15 << 16 << 17 << 18
    ;

    m_posY <<
    1 <<                                                                                         1 <<
    2 << 2 <<                                                      2 <<  2 <<  2 <<  2 <<  2 <<  2 <<
    3 << 3 <<                                                      3 <<  3 <<  3 <<  3 <<  3 <<  3 <<
    4 << 4 << 4 << 4 << 4 << 4 << 4 << 4 << 4 << 4 <<  4 <<  4 <<  4 <<  4 <<  4 <<  4 <<  4 <<  4 <<
    5 << 5 << 5 << 5 << 5 << 5 << 5 << 5 << 5 << 5 <<  5 <<  5 <<  5 <<  5 <<  5 <<  5 <<  5 <<  5 << //Element 54 (Xe)
    6 << 6 << 6 << 9 << 9 << 9 << 9 << 9 << 9 << 9 <<  9 <<  9 <<  9 <<  9 <<  9 <<  9 <<  9 <<          //Element 71 (Lr)
    6 << 6 << 6 << 6 << 6 << 6 << 6 << 6 << 6 << 6 <<  6 <<  6 <<  6 <<  6 <<  6 <<
    7 << 7 << 7 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 << 10 <<
    7 << 7 << 7 << 7 << 7 << 7 << 7 << 7 << 7 << 7 << 7 << 7 << 7 << 7 << 7
    ;

    // The classic PS has all Elements
    if (m_posX.count() == m_posY.count() ) {
        for (int i =  1; i <= m_posX.count(); i ++) {
            m_elementList.append(i);
        }
    }
}

pseRegularTable *pseRegularTable::init()
{
    static pseRegularTable thisTable;
    return &thisTable;
}

/// Long Table Data
pseLongTable::pseLongTable()
        : pseTable()
{
    m_name = "Long";

    m_description = i18n( "Long Periodic Table" );

    m_xCoordsNumeration <<
    1 << 2 <<-1 <<-1 <<-1 <<-1 <<-1 <<-1 <<-1 <<-1 <<-1 <<-1 <<-1 <<-1 <<-1 <<-1 << 3 << 4 << 5 << 6 << 7 << 8 << 9 <<10 <<11 <<12 <<13 <<14 <<15 <<16 <<17 <<18;

    m_posX <<
    1 <<                                                                                                                                                      32 <<
    1 << 2 <<                                                                                                                        27 <<28 <<29 <<30 <<31 <<32 <<
    1 << 2 <<                                                                                                                        27 <<28 <<29 <<30 <<31 <<32 <<
    1 << 2 <<                                                                      17 <<18 <<19 <<20 <<21 <<22 <<23 <<24 <<25 <<26 <<27 <<28 <<29 <<30 <<31 <<32 <<
    1 << 2 <<                                                                      17 <<18 <<19 <<20 <<21 <<22 <<23 <<24 <<25 <<26 <<27 <<28 <<29 <<30 <<31 <<32 <<
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 <<10 <<11 <<12 <<13 <<14 <<15 <<16 <<17 <<18 <<19 <<20 <<21 <<22 <<23 <<24 <<25 <<26 <<27 <<28 <<29 <<30 <<31 <<32 <<
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 <<10 <<11 <<12 <<13 <<14 <<15 <<16 <<17 <<18 <<19 <<20 <<21 <<22 <<23 <<24 <<25 <<26 <<27 <<28 <<29 <<30 <<31 <<32
    ;

    m_posY <<
    1 <<                                                                                                                                                      1 <<
    2 << 2 <<                                                                                                                        2  <<2  <<2  <<2  <<2  <<2 <<
    3 << 3 <<                                                                                                                        3  <<3  <<3  <<3  <<3  <<3 <<
    4 << 4 <<                                                                      4  <<4  <<4  <<4  <<4  <<4  <<4  <<4  <<4  <<4  <<4  <<4  <<4  <<4  <<4  <<4 <<
    5 << 5 <<                                                                      5  <<5  <<5  <<5  <<5  <<5  <<5  <<5  <<5  <<5  <<5  <<5  <<5  <<5  <<5  <<5 <<
    6 << 6 << 6 << 6 << 6 << 6 << 6 << 6 << 6 <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6  <<6 <<
    7 << 7 << 7 << 7 << 7 << 7 << 7 << 7 << 7 <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7  <<7
    ;

    // The long PS has all Elements
    if (m_posX.count() == m_posY.count() ) {
        for (int i =  1; i <= m_posX.count(); i ++) {
            m_elementList.append(i);
        }
    }
}

pseLongTable *pseLongTable::init()
{
    static pseLongTable thisTable;
    return &thisTable;
}

/// Short Table Data
pseShortTable::pseShortTable()
        : pseTable()
{
    m_name = "Short";

    m_description = i18n( "Short Periodic Table" );

    m_xCoordsNumeration <<
    1 << 2 <<13 <<14 <<15 <<16 <<17 <<18;

    m_posX <<
    1 <<                               8 <<//He
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 <<//Ne
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 <<//Ar
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 <<//Kr
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 <<//Xe
    1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 <<//Rn
    1 << 2                  //Fr and Ra
    ;

    m_posY <<
    1 <<                               1 <<//He
    2 << 2 << 2 << 2 << 2 << 2 << 2 << 2 <<//Ne
    3 << 3 << 3 << 3 << 3 << 3 << 3 << 3 <<//Ar
    4 << 4 << 4 << 4 << 4 << 4 << 4 << 4 <<//Kr
    5 << 5 << 5 << 5 << 5 << 5 << 5 << 5 <<//Xe
    6 << 6 << 6 << 6 << 6 << 6 << 6 << 6 <<//Rn
    7 << 7                   //Fr and Ra
    ;

    m_elementList <<
    1 <<                               2 <<//He
    3 << 4 << 5 << 6 << 7 << 8 << 9 << 10<<//Ne
    11<< 12<< 13<< 14<< 15<< 16<< 17<< 18<<//Ar
    19<< 20<< 31<< 32<< 33<< 34<< 35<< 36<<//Kr
    37<< 38<< 49<< 50<< 51<< 52<< 53<< 54<<//Xe
    55<< 56<< 81<< 82<< 83<< 84<< 85<< 86<<//Rn
    87<< 88                   //Fr and Ra
    ;
}

pseShortTable *pseShortTable::init()
{
    static pseShortTable thisTable;
    return &thisTable;
}

/// D-Group Table Data
pseDTable::pseDTable()
        : pseTable()
{
    m_name = "D";

    m_description = i18n( "Transition Elements" );

    m_xCoordsNumeration <<
    3<< 4<< 5<< 6<< 7<< 8<< 9<<10<<11<< 12;

    m_posX <<
    1<< 2<< 3<< 4<< 5<< 6<< 7<< 8<< 9<< 10<<
    1<< 2<< 3<< 4<< 5<< 6<< 7<< 8<< 9<< 10<<
    1<< 2<< 3<< 4<< 5<< 6<< 7<< 8<< 9<< 10<<
    1<< 2<< 3<< 4<< 5<< 6<< 7<< 8<< 9<< 10
    ;

    m_posY <<
    1<< 1<< 1<< 1<< 1<< 1<< 1<< 1<< 1<< 1<<
    2<< 2<< 2<< 2<< 2<< 2<< 2<< 2<< 2<< 2<<
    3<< 3<< 3<< 3<< 3<< 3<< 3<< 3<< 3<< 3<<
    4<< 4<< 4<< 4<< 4<< 4<< 4<< 4<< 4<< 4
    ;

    m_elementList <<
    21 << 22 << 23 << 24 << 25 << 26 << 27 << 28 << 29 << 30 <<
    39 << 40 << 41 << 42 << 43 << 44 << 45 << 46 << 47 << 48 <<
    57 << 72 << 73 << 74 << 75 << 76 << 77 << 78 << 79 << 80 <<
    89 << 104<< 105<< 106<< 107<< 108<< 109<< 110<< 111<< 112
    ;
}


pseDTable *pseDTable::init()
{
    static pseDTable thisTable;
    return &thisTable;
}

/// DZ Table Data
pseDZTable::pseDZTable()
: pseTable()
{
    m_name = "DZ";

    m_description = i18n( "DZ Periodic Table" );

    m_xCoordsNumeration <<
    1 << 2 <<13 <<14 <<15 <<16 <<17 <<18 << 3 << 4 << 5 << 6 << 7 << 8 << 9 <<10 <<11 <<12;

    m_posX <<
    1<< 2<<
    2<< 2<<
    3<< 4<< 5<< 6<< 7<< 8<<
    1<< 2<<
    3<< 4<< 5<< 6<< 7<< 8<<
    1<< 2<<
                           9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<
         3<< 4<< 5<< 6<< 7<< 8<<
    1<< 2<<
                           9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<
         3 <<4 <<5 <<6 <<7 <<8 <<
    1<< 2<<
                                                              19<<20<<21<<22<<23<<24<<25<<26<<27<<28<<29<<30<<31<<32<<
                           9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<
         3 <<4 <<5 <<6 <<7 <<8 <<
    1<< 2<<
                                                              19<<20<<21<<22<<23<<24<<25<<26<<27<<28<<29<<30<<31<<32<<
                           9<<10<<11<<12<<13<<14<<15<<16<<17<<18<<
         3 <<4 <<5 <<6 <<7 <<8
    ;

    m_posY <<
    1<< 1<<
    2<< 2<<
    3<< 3<< 3<< 3<< 3<< 3<<
    4<< 4<<
    5<< 5<< 5<< 5<< 5<< 5<<
    6<< 6<<
                          7<< 7<< 7<< 7<< 7<< 7<< 7<< 7<< 7<< 7<<
         8<< 8<< 8<< 8<< 8<< 8<<
    9<< 9<<
    10<<10<<10<<10<<10<<10<<10<<10<<10<<10<<
    11<<11<<11<<11<<11<<11<<
    12<<12<<

                                                              13<<13<<13<<13<<13<<13<<13<<13<<13<<13<<13<<13<<13<<13<<
                          14<<14<<14<<14<<14<<14<<14<<14<<14<<14<<
         15<<15<<15<<15<<15<<15<<
    16<<16<<
                                                              17<<17<<17<<17<<17<<17<<17<<17<<17<<17<<17<<17<<17<<17<<
                          18<<18<<18<<18<<18<<18<<18<<18<<18<<18<<
         19<<19<<19<<19<<19<<19
    ;

    // The DZ PS has all Elements
    if (m_posX.count() == m_posY.count() ) {
        for (int i =  1; i <= m_posX.count(); i ++) {
            m_elementList.append(i);
        }
    }

}

pseDZTable *pseDZTable::init()
{
    static pseDZTable thisTable;
    return &thisTable;
}
