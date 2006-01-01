/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
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
#include <spectrumparser.h>
#include <isotope.h>
#include <spectrum.h>

#include <QFile>

#include <klocale.h>
#include <kdebug.h>
#include <klocale.h>
#include <kurl.h>
#include <kstandarddirs.h>
#include <kapplication.h>

KalziumDataObject* KalziumDataObject::instance()
{
	static KalziumDataObject kdo;
	return &kdo;
}

KalziumDataObject::KalziumDataObject()
{
	ElementSaxParser * parser = new ElementSaxParser();

	QFile xmlFile( locate( "data", "libkdeedu/data/elements.xml" ) );
	QXmlInputSource source(&xmlFile);
	QXmlSimpleReader reader;
	
	reader.setContentHandler(parser);
	reader.parse(source);

	ElementList = parser->getElements();

	// cache it
	m_numOfElements = ElementList.count();
}

KalziumDataObject::~KalziumDataObject()
{
}

Element* KalziumDataObject::element( int number )
{
	// checking that we are requesting a valid element
	if ( ( number <= 0 ) || ( number > m_numOfElements ) )
		return 0;
	return ElementList[ number-1 ];
}

