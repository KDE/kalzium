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

QList<Element*> ElementParser::loadAllElements( const QDomDocument& dataDocument )
{
	QList<Element*> elementList;
	QStringList elementSymbols = loadElementSymbols(dataDocument);

	foreach(QString symbol, elementSymbols)
	{
		Element *e = loadElement( symbol, dataDocument );
		if ( e )
			elementList.append( e );
	}

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





////////////
//

ElementSaxParser::ElementSaxParser()
: QXmlDefaultHandler(), currentElement_(0), inElement_(false), inName_(false)
{
}

bool ElementSaxParser::startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs)
{
	if (localName == "elementType") {
		currentElement_ = new Element();
		inElement_ = true;
	} else if (inElement_ && localName == "scalar") {
		for (int i = 0; i < attrs.length(); ++i) {
			if (attrs.value(i) == "bo:name")
				inName_ = true;
		}
	}
	return true;
}

bool ElementSaxParser::endElement(const QString&, const QString &localName, const QString&, const QXmlAttributes&)
{
	if (localName == "elementType") {
		inElement_ = false;
		elements_.append(currentElement_);
		currentElement_ = 0;
	}
	return true;
}

bool ElementSaxParser::characters(const QString &ch)
{
	if (inName_) {
		kdDebug() << "nimi: " << ch << endl;
		currentElement_->setName(ch);
		inName_ = false;
	}
	return true;
}

QList<Element*> ElementSaxParser::getElements()
{
	return elements_;
}
