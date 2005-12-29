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

#include "chemicaldataobject.h"
#include "isotope.h"

#include <kdebug.h>

class IsotopeParser::Private
{
public:
	Private()
	: currentDataObject(0),
	currentUnit(ChemicalDataObject::noUnit),
	currentErrorValue(QVariant()),
	currentElementSymbol(QString()),
	currentIsotope(0),
	inIsotope(false),
	inAtomicNumber(false),
	inExactMass(false),
	inAlphaPercentage(false),
	inAlphaDecay(false),
	inBetaplusPercentage(false),
	inBetaplusDecay(false),
	inBetaminusPercentage(false),
	inBetaminusDecay(false),
	inECPercentage(false),
	inECDecay(false),
	inAbundance(false)
	{
	}

	ChemicalDataObject *currentDataObject;
	ChemicalDataObject::BlueObeliskUnit currentUnit;
	QVariant currentErrorValue;
	QString currentElementSymbol;
	Isotope* currentIsotope;
	
	QList<Isotope*> isotopes;
	
	bool inIsotope;
	bool inAtomicNumber;
	bool inExactMass;
	bool inAlphaPercentage;
	bool inAlphaDecay;
	bool inBetaplusPercentage;
	bool inBetaplusDecay;
	bool inBetaminusPercentage;
	bool inBetaminusDecay;
	bool inECPercentage;
	bool inECDecay;
	bool inAbundance;
};

IsotopeParser::IsotopeParser()
	: QXmlDefaultHandler(), d( new Private )
{
}

IsotopeParser::~IsotopeParser()
{
	delete d;
}

bool IsotopeParser::startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs)
{
	if (localName == "isotope") 
	{
		d->currentIsotope = new Isotope();
		d->inIsotope = true;
		
		//now save the symbol of the current element
		for (int i = 0; i < attrs.length(); ++i) 
		{
			if ( attrs.localName( i ) == "elementType" )
				d->currentElementSymbol = attrs.value( i );
		}
	} else if (d->inIsotope && localName == "scalar")
	{
		for (int i = 0; i < attrs.length(); ++i) 
		{
			if ( attrs.localName( i ) == "errorValue" )
			{
				d->currentErrorValue = QVariant( attrs.value( i ) );
				continue;
			}
			
			if (attrs.value(i) == "bo:atomicNumber")
				d->inAtomicNumber = true;
			else if (attrs.value(i) == "bo:alphapercentage")
				d->inAlphaPercentage = true;
			else if (attrs.value(i) == "bo:alphadecay")
				d->inAlphaDecay = true;
			else if (attrs.value(i) == "bo:betapluspercentage")
				d->inBetaplusPercentage = true;
			else if (attrs.value(i) == "bo:betaplusdecay")
				d->inBetaplusDecay = true;
			else if (attrs.value(i) == "bo:betaminuspercentage")
				d->inBetaminusPercentage = true;
			else if (attrs.value(i) == "bo:betaminusdecay")
				d->inBetaminusDecay = true;
			else if (attrs.value(i) == "bo:ecpercentage")
				d->inECPercentage = true;
			else if (attrs.value(i) == "bo:ecdecay")
				d->inECDecay = true;
			else if (attrs.value(i) == "bo:exactMass")
				d->inExactMass = true;
		}
	} else if (d->inIsotope && localName == "bo:relativeAbundance") {
		kdDebug() << "bo:relativeAbundance" << endl;
		d->inAbundance = true;
	}
	return true;
}

bool IsotopeParser::endElement( const QString&, const QString& localName, const QString& )
{
	if ( localName == "isotope" )
	{
		d->currentIsotope->addData( new ChemicalDataObject( QVariant( d->currentElementSymbol ), ChemicalDataObject::symbol ) );
		d->isotopes.append(d->currentIsotope);
		
		d->currentIsotope = 0;
		d->currentDataObject = 0;
		d->inIsotope = false;
	}
	else if ( localName == "scalar" )
	{
		if ( d->currentDataObject->type() == ChemicalDataObject::exactMass ){
			d->currentDataObject->setErrorValue( d->currentErrorValue );
		}
	}

	return true;
}

bool IsotopeParser::characters(const QString &ch)
{
	d->currentDataObject = new ChemicalDataObject();
	ChemicalDataObject::BlueObelisk type;
	QVariant value;

	if (d->inExactMass){
		value = ch.toDouble();
		type = ChemicalDataObject::exactMass; 
		d->inExactMass = false;
	}
	else if (d->inAtomicNumber) {
		value = ch.toInt();
		type = ChemicalDataObject::atomicNumber; 
		d->inAtomicNumber = false;
	}
	else if (d->inAlphaPercentage) {
		value = ch.toDouble();
		type = ChemicalDataObject::alphapercentage; 
		d->inAtomicNumber = false;
	}
	else if (d->inAlphaDecay) {
		value = ch.toDouble();
		type = ChemicalDataObject::alphadecay; 
		d->inAtomicNumber = false;
	}
	else if (d->inBetaplusPercentage) {
		value = ch.toDouble();
		type = ChemicalDataObject::betapluspercentage; 
		d->inAtomicNumber = false;
	}
	else if (d->inBetaplusDecay) {
		value = ch.toDouble();
		type = ChemicalDataObject::betaplusdecay; 
		d->inAtomicNumber = false;
	}
	else if (d->inBetaminusPercentage) {
		value = ch.toDouble();
		type = ChemicalDataObject::betaminuspercentage; 
		d->inAtomicNumber = false;
	}
	else if (d->inBetaminusDecay) {
		value = ch.toDouble();
		type = ChemicalDataObject::betaminusdecay; 
		d->inAtomicNumber = false;
	}
	else if (d->inECPercentage) {
		value = ch.toDouble();
		type = ChemicalDataObject::ecpercentage; 
		d->inAtomicNumber = false;
	}
	else if (d->inECDecay) {
		value = ch.toDouble();
		type = ChemicalDataObject::ecdecay; 
		d->inAtomicNumber = false;
	}
	else if (d->inAbundance){
		value = ch;
		type = ChemicalDataObject::relativeAbundance;
		d->inAbundance = false;
	}
	else//it is a non known value. Do not create a wrong object but return
		return true;

	d->currentDataObject->setData( value );
	d->currentDataObject->setType( type );

	if ( d->currentIsotope )
		d->currentIsotope->addData( d->currentDataObject );

	return true;
}

QList<Isotope*> IsotopeParser::getIsotopes()
{
	return d->isotopes;
}
