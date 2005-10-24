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
#include "isotopeparser.h"
#include "isotope.h"

#include <qdom.h>
#include <QList>
#include <QFile>
#include <QStringList>

#include <kdebug.h>

IsotopeParser::IsotopeParser()
	: QXmlDefaultHandler(), 
	currentIsotope_(0), 
	inAtomicNumber_(false),
	inExactMass_(false)
{
}

bool IsotopeParser::startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs)
{
	if (localName == "isotope") 
	{
		currentIsotope_ = new Isotope();
		inIsotope_ = true;
	} else if (inIsotope_ && localName == "scalar") 
	{
		for (int i = 0; i < attrs.length(); ++i) 
		{

			if ( attrs.localName( i ) == "errorValue" )
			{
				currentErrorValue_ = QVariant( attrs.value( i ) );
				continue;
			}
			
			if (attrs.value(i) == "bo:atomicNumber")
				inAtomicNumber_ = true;
			else if (attrs.value(i) == "bo:exactMass")
				inExactMass_ = true;
		}
	}
	return true;
}
		
bool IsotopeParser::endElement (  const QString & namespaceURI, const QString & localName, const QString & qName )
{
	if ( localName == "isotope" )
	{
		isotopes_.append(currentIsotope_);
		
		currentIsotope_ = 0;
		currentDataObject_ = 0;
		inIsotope_ = false;
	}
	else if ( localName == "scalar" )
	{
		if ( currentDataObject_->type() == ChemicalDataObject::exactMass ){
			currentDataObject_->setErrorValue( currentErrorValue_ );
		}
	}

	return true;
}

bool IsotopeParser::characters(const QString &ch)
{
	currentDataObject_ = new ChemicalDataObject();
	ChemicalDataObject::BlueObelisk type;
	QVariant value;

	if ( inExactMass_ ){
		value = ch.toDouble();
		type = ChemicalDataObject::exactMass; 
		inExactMass_ = false;
	}
	else if (inAtomicNumber_) {
		value = ch.toInt();
		type = ChemicalDataObject::atomicNumber; 
		inAtomicNumber_ = false;
	}
	else//it is a non known value. Do not create a wrong object but return
		return true;

	currentDataObject_->setData( value );
	currentDataObject_->setType( type );

	if ( currentIsotope_ )
		currentIsotope_->addData( currentDataObject_ );

	return true;
}

QList<Isotope*> IsotopeParser::getIsotopes()
{
	return isotopes_;
}
