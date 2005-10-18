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
	inExactMass_( false ),
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
			if (attrs.value(i) == "bo:exactMass")
				inExactMass_ = true;
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
//X 		if ( currentElement_ )
//X 			kdDebug() << "Number of ChemicalDataObject: " << currentElement_->dataList.count() << endl;
		currentElement_ = 0;
		inElement_ = false;
	}
	return true;
}

bool ElementSaxParser::characters(const QString &ch)
{
	ChemicalDataObject *dataobject = new ChemicalDataObject();
	ChemicalDataObject::BlueObelisk type;
	QVariant value;

	if (inName_) {
		value = ch;
		type = ChemicalDataObject::name; 
		inName_ = false;
	}
	else if ( inMass_ ){
		value = ch.toDouble();
		type = ChemicalDataObject::mass; 
		inMass_ = false;
	}
	else if ( inExactMass_ ){
		value = ch.toDouble();
		type = ChemicalDataObject::exactMass; 
		inExactMass_ = false;
	}
	else if (inSymbol_) {
		value = ch;
		type = ChemicalDataObject::symbol; 
		inSymbol_ = false;
	}
	else if (inAtomicNumber_) {
		value = ch.toInt();
		type = ChemicalDataObject::atomicNumber; 
		inAtomicNumber_ = false;
	}

	dataobject->setData( value );
	dataobject->setType( type );

//X 	kdDebug() << dataobject->valueAsString() << endl;

	if ( currentElement_ )
		currentElement_->addData( dataobject );

	return true;
}

QList<Element*> ElementSaxParser::getElements()
{
	return elements_;
}
