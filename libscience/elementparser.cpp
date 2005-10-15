/***************************************************************************
copyright            : (C) 2005 by Carsten Niehaus
email                : cniehaus@kde.org
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "elementparser.h"
#include "element.h"

#include <qdom.h>
#include <QList>
#include <QFile>
#include <QStringList>

#include <kdebug.h>
#include <kurl.h>

QList<Element*> ElementParser::loadAllElements( const QDomDocument& dataDocument2 )
{
	//only for testing!
	QDomDocument dataDocument = dataDocument2;
	
	QList<Element*> elementList;

	KURL url;
	url.setPath("/home/carsten/svn/trunk/KDE/kdeedu/libkdeedu/libscience/" );
	url.setFileName( "elements.xml" );
	QFile layoutFile( url.path() );
	
	if (!layoutFile.open(IO_ReadOnly))
	{
		kdDebug() << "layoutfile IO-error" << endl;
	}

	// Check if the document is well-formed
	if (!dataDocument.setContent(&layoutFile))
	{
		kdDebug() << "wrong xml" << endl;
		layoutFile.close();
	}
	layoutFile.close();

	QStringList elementSymbols = loadElementSymbols(dataDocument);

	foreach(QString symbol, elementSymbols)
	{
		Element *e = loadElement( symbol, dataDocument );
		if ( e )
			elementList.append( e );
	}

	kdDebug() << elementList.count() << endl;

	return elementList;
}

QStringList ElementParser::loadElementSymbols( const QDomDocument& dataDocument )
{
	QStringList symbolList;

	//xml-reading
	QDomNodeList elementNodes = dataDocument.elementsByTagName( "elementType" );
	
	const uint count = elementNodes.count();

	for ( uint i = 0; i < count; ++i )
	{
		QString symbol = elementNodes.item( i ).toElement().attribute("id");
		symbolList.append( symbol );

		//for debugging!
		loadElement( symbol, dataDocument );
	}

	return symbolList;
}

Element* ElementParser::loadElement( const QString& symbol, const QDomDocument& dataDocument )
{
	QDomNodeList elementNodes = dataDocument.elementsByTagName( "elementType" );
	const uint count = elementNodes.count();

	for ( uint i = 0; i < count; ++i )
	{
		QDomElement currentElement = elementNodes.item( i ).toElement();
		QString currentSymbol = currentElement.attribute("id");

		if ( currentSymbol == symbol )
			return loadElement( currentElement );
	}

	//the element was not found...
	return 0;
}

Element* ElementParser::loadElement( const QDomElement& element )
{
	Element *e = new Element();

	QString symbol = element.attribute( "id" );
	
	QDomNodeList scalarList = element.elementsByTagName( "scalar" );

	e->setSymbol( symbol );

	kdDebug() << "scalarList::count of element " << e->symbol() << ": " << scalarList.count() << endl;
	
	return e;
}
