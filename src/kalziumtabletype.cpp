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

KalziumTableTypeFactory::KalziumTableTypeFactory()
{
}

KalziumTableTypeFactory* KalziumTableTypeFactory::instance()
{
    static KalziumTableTypeFactory kttf;
    return &kttf;
}

KalziumTableType* KalziumTableTypeFactory::build( int id ) const
{

     KalziumTableType * KalziumTable = new KalziumTableType(pseTables::instance()->getTabletype( id ));
     return KalziumTable;
}

KalziumTableType* KalziumTableTypeFactory::build( const QByteArray& id ) const
{
   KalziumTableType * KalziumTable = new KalziumTableType(pseTables::instance()->getTabletype( QString(id) ));
   return KalziumTable;
}

QStringList KalziumTableTypeFactory::tables() const
{
    return pseTables::instance()->tables() ;
}

KalziumTableType::KalziumTableType(pseTable *tableType)
{
  m_table = tableType;
}

KalziumTableType::~KalziumTableType()
{
}

int KalziumTableType::previousOf( int element ) const
{
    int index = m_table->elements().indexOf( element );
    return index > 1 ? m_table->elements().at( index - 1 ) : -1;
}

int KalziumTableType::nextOf( int element ) const
{
    int index = m_table->elements().indexOf( element );
    return index != -1 && ( index < m_table->elements().count() - 1 ) ? m_table->elements().at( index + 1 ) : -1;
}

int KalziumTableType::firstElement() const
{
    return m_table->elements().first();
}

QList<int> KalziumTableType::elementList() const
{
    return m_table->elements();
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

int KalziumTableType::elementAtCoords( const QPoint& coords ) const
{
    const QPoint ourcoord = elementUnderMouse( coords );

    int x = 0;
    int y = 0;
    foreach (int counter, m_table->elements() )
    {
        x = m_table->elementCoords( counter ).x();
        y = m_table->elementCoords( counter ).y();
        if ( ( ourcoord.x() == x ) && ( ourcoord.y() == y ) ) {
            return counter;
        }
    }

    // not found
    return 0;
}

QRect KalziumTableType::elementRect( const int numelem ) const
{
    // x coord
    int x = ( m_table->elementCoords( numelem ).x() - 1 ) * ELEMENTSIZE;
    // y coord
    int y = (m_table->elementCoords( numelem ).y() ) * ELEMENTSIZE;

    return QRect( x, y, ELEMENTSIZE, ELEMENTSIZE );
}

QSize KalziumTableType::size() const
{
    return QSize( ELEMENTSIZE * m_table->coordsMax().x() + 1, ELEMENTSIZE * (m_table->coordsMax().y() + 1) + 1 );
}

QByteArray KalziumTableType::name() const
{
    return m_table->name().toAscii();
}

QString KalziumTableType::description() const
{
    return m_table->description();
}

#include "kalziumtabletype.moc"