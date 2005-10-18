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

ElementSaxParser::ElementSaxParser()
: QXmlDefaultHandler(), currentElement_(0), 
	inElement_(false), 
	inName_(false), 
	inMass_( false ),
	inAtomicNumber_(false), 
	inSymbol_( false )
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
			if (attrs.value(i) == "bo:mass")
				inMass_ = true;
			if (attrs.value(i) == "bo:atomicNumber")
				inAtomicNumber_ = true;
			if (attrs.value(i) == "bo:symbol")
				inSymbol_ = true;
		}
	}
	return true;
}
		
bool ElementSaxParser::endElement (  const QString & namespaceURI, const QString & localName, const QString & qName )
{
	if ( localName == "elementType" )
	{
		elements_.append(currentElement_);
		currentElement_ = 0;
		inElement_ = false;
	}
	return true;
}

bool ElementSaxParser::characters(const QString &ch)
{
	if (inName_) {
		currentElement_->setName(ch);
		inName_ = false;
	}
	if ( inMass_ ){
		currentElement_->setMass( ch.toDouble() );
		inMass_ = false;
	}
	if (inSymbol_) {
		currentElement_->setSymbol(ch);
		inSymbol_ = false;
	}
	if (inAtomicNumber_) {
		currentElement_->setNumber(ch.toInt());
		inAtomicNumber_ = false;
	}

	return true;
}

QList<Element*> ElementSaxParser::getElements()
{
	return elements_;
}
