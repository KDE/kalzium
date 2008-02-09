/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007 by Carsten Niehaus                     *
 *   cniehaus@kde.org                                                      *
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

#include "kalziumdataobject.h"

#include <elementparser.h>
#include <isotope.h>
#include <isotopeparser.h>

#include <QFile>
#include <QVariant>
#include <QSvgRenderer>
#include <QPainter>

#include <klocale.h>
#include <kdebug.h>
#include <kurl.h>
#include <kstandarddirs.h>

KalziumDataObject* KalziumDataObject::instance()
{
	static KalziumDataObject kdo;
	return &kdo;
}

KalziumDataObject::KalziumDataObject()
	: m_search( 0 )
{
	// reading elements
	ElementSaxParser * parser = new ElementSaxParser();

	QFile xmlFile( KStandardDirs::locate( "data", "libkdeedu/data/elements.xml" ) );
	QXmlInputSource source(&xmlFile);
	QXmlSimpleReader reader;
	
	reader.setContentHandler(parser);
	reader.parse(source);

	ElementList = parser->getElements();

	//we don't need parser anymore, let's free its memory
	delete parser;

	// reading isotopes
	IsotopeParser * isoparser = new IsotopeParser();

	QFile xmlIsoFile( KStandardDirs::locate( "data", "libkdeedu/data/isotopes.xml" ) );
	QXmlInputSource isosource(&xmlIsoFile);
	QXmlSimpleReader isoreader;
	
	isoreader.setContentHandler(isoparser);
	isoreader.parse(isosource);

	QList<Isotope*> isotopes = isoparser->getIsotopes();

	//we don't need isoparser anymore, let's free its memory
	delete isoparser;

	foreach( Isotope *iso, isotopes )
	{
		int num = iso->parentElementNumber();
		if ( m_isotopes.contains( num ) )
		{
			m_isotopes[num].append( iso );
		}
		else
		{
			QList<Isotope*> newlist;
			newlist.append( iso );
			m_isotopes.insert( num, newlist );
		}
	}
	
	// cache it
	m_numOfElements = ElementList.count();
            
	
	for ( int i = 0 ; i < m_numOfElements ; i++ )
        {
            //FIXME in case we ever get more than one theme we need
            //a settings-dialog where we can select the different iconsets...
            QString setname = "school";

            QPixmap pix( 40, 40 );
            pix.fill(Qt::transparent);

            QString pathname = KGlobal::dirs()->findResourceDir( "appdata", "data/iconsets/" ) + "data/iconsets/";

            QString filename = pathname + setname + '/' + QString::number( i+1 ) + ".svg";

            QSvgRenderer svgrenderer;

            QPainter p( &pix );
            if ( QFile::exists(filename) && svgrenderer.load(filename) ) {
                svgrenderer.render( &p );
                p.end();

                PixmapList << pix;
            }
            else {
                Element *e =  ElementList.at(i);
                QString esymbol = e->dataAsString( ChemicalDataObject::symbol );
                p.drawText(0,0,40,40, Qt::AlignCenter | Qt::TextWordWrap, esymbol );
                p.end();

                PixmapList << pix;
            }
        }

}

KalziumDataObject::~KalziumDataObject()
{
    //Delete all elements
    qDeleteAll(ElementList);

    //Delete all isotopes
    QHashIterator<int, QList<Isotope*> > i(m_isotopes);
    while (i.hasNext()) {
        i.next();
        qDeleteAll( i.value());
    }
}

Element* KalziumDataObject::element( int number )
{
	// checking that we are requesting a valid element
	if ( ( number <= 0 ) || ( number > m_numOfElements ) )
		return 0;
	return ElementList[ number-1 ];
}

QPixmap KalziumDataObject::pixmap( int number )
{
	// checking that we are requesting a valid element
	if ( ( number <= 0 ) || ( number > m_numOfElements ) )
		return 0;
	return PixmapList[ number-1 ];
}

QList<Isotope*> KalziumDataObject::isotopes( Element * element )
{
	return isotopes( element->dataAsVariant( ChemicalDataObject::atomicNumber ).toInt() );
}

QList<Isotope*> KalziumDataObject::isotopes( int number )
{
	return m_isotopes.contains( number ) ? m_isotopes.value( number ) : QList<Isotope*>();
}

void KalziumDataObject::setSearch( Search *srch )
{
	m_search = srch;
}

Search* KalziumDataObject::search() const
{
	return m_search;
}
