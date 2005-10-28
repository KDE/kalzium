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
	inName_(false),
	inMass_(false),
	inExactMass_(false),
	inAtomicNumber_(false),
	inSymbol_(false),
	inIonization_(false),
	inElectronAffinity_(false),
	inElectronegativityPauling_(false),
	inRadiusCovalent_(false),
	inRadiusVDW_(false),
	inBoilingPoint_(false),
	inMeltingPoint_(false),
	inPeriodTableBlock_(false),
	inNameOrigin_(false),
	inDiscoveryDate_(false),
	inDiscoverers_(false),
	inPeriod_(false)
{
}

bool ElementSaxParser::startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs)
{
	if (localName == "elementType") 
	{
		currentElement_ = new Element();
		inElement_ = true;
	} else if (inElement_ && localName == "scalar") 
	{
		for (int i = 0; i < attrs.length(); ++i) 
		{
			if ( attrs.localName( i ) == "unit" )
			{
				currentUnit_ = ChemicalDataObject::unit( attrs.value( i ) );
				continue;
			}

			if (attrs.value(i) == "bo:atomicNumber")
				inAtomicNumber_ = true;
			else if (attrs.value(i) == "bo:symbol")
				inSymbol_ = true;
			else if (attrs.value(i) == "bo:name")
				inName_ = true;
			else if (attrs.value(i) == "bo:mass")
				inMass_ = true;
			else if (attrs.value(i) == "bo:exactMass")
				inExactMass_ = true;
			else if (attrs.value(i) == "bo:ionization")
				inIonization_ = true;
			else if (attrs.value(i) == "bo:electronAffinity")
				inElectronAffinity_ = true;
			else if (attrs.value(i) == "bo:electronegativityPauling")
				inElectronegativityPauling_ = true;
			else if (attrs.value(i) == "bo:radiusCovalent")
				inRadiusCovalent_ = true;
			else if (attrs.value(i) == "bo:radiusVDW")
				inRadiusVDW_ = true;
			else if (attrs.value(i) == "bo:meltingpoint")
				inMeltingPoint_ = true;
			else if (attrs.value(i) == "bo:boilingpoint")
				inBoilingPoint_ = true;
			else if (attrs.value(i) == "bo:periodTableBlock")
				inPeriodTableBlock_ = true;
			else if (attrs.value(i) == "bo:nameOrigin")
				inNameOrigin_ = true;
			else if (attrs.value(i) == "bo:discoveryDate")
				inDiscoveryDate_ = true;
			else if (attrs.value(i) == "bo:discoverers")
				inDiscoverers_ = true;
			else if (attrs.value(i) == "bo:period")
				inPeriod_ = true;
		}
	}
	return true;
}

bool ElementSaxParser::endElement( const QString &, const QString& localName, const QString& )
{
	if ( localName == "elementType" )
	{
		if ( currentElement_->dataAsString( ChemicalDataObject::symbol ) != "Xx" )
		elements_.append(currentElement_);
		
		currentElement_ = 0;
		currentDataObject_ = 0;
		inElement_ = false;
	}
	else if ( localName == "scalar" )
	{
		if ( currentUnit_ != ChemicalDataObject::noUnit )
			currentDataObject_->setUnit( currentUnit_ );

		currentUnit_ = ChemicalDataObject::noUnit;
	}
	return true;
}

bool ElementSaxParser::characters(const QString &ch)
{
	currentDataObject_ = new ChemicalDataObject();
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
	else if (inIonization_) {
		value = ch.toDouble();;
		type = ChemicalDataObject::ionization; 
		inIonization_ = false;
	}
	else if (inElectronAffinity_) {
		value = ch.toDouble();
		type = ChemicalDataObject::electronAffinity; 
		inElectronAffinity_ = false;
	}
	else if (inElectronegativityPauling_) {
		value = ch.toDouble();
		type = ChemicalDataObject::electronegativityPauling; 
		inElectronegativityPauling_ = false;
	}
	else if (inRadiusCovalent_) {
		value = ch.toDouble();
		type = ChemicalDataObject::radiusCovalent; 
		inRadiusCovalent_ = false;
	}
	else if (inRadiusVDW_) {
		value = ch.toDouble();
		type = ChemicalDataObject::radiusVDW; 
		inRadiusVDW_ = false;
	}
	else if (inMeltingPoint_) {
		value = ch.toDouble();
		type = ChemicalDataObject::meltingpoint; 
		inMeltingPoint_ = false;
	}
	else if (inBoilingPoint_) {
		value = ch.toDouble();
		type = ChemicalDataObject::boilingpoint; 
		inBoilingPoint_ = false;
	}
	else if (inPeriodTableBlock_) {
		value = ch;
		type = ChemicalDataObject::periodTableBlock; 
		inPeriodTableBlock_ = false;
	}
	else if (inNameOrigin_) {
		value = ch;
		type = ChemicalDataObject::nameOrigin; 
		inNameOrigin_ = false;
	}
	else if (inDiscoveryDate_) {
		value = ch.toInt();
		type = ChemicalDataObject::date;
		inDiscoveryDate_ = false;
	}
	else if (inDiscoverers_) {
		value = ch;
		type = ChemicalDataObject::discoverers;
		inDiscoverers_ = false;
	}
	else if (inPeriod_) {
		value = ch.toInt();
		type = ChemicalDataObject::period; 
		inPeriod_ = false;
	}
	else//it is a non known value. Do not create a wrong object but return
		return true;

	currentDataObject_->setData( value );
	currentDataObject_->setType( type );

	if ( currentElement_ )
		currentElement_->addData( currentDataObject_ );

	return true;
}

QList<Element*> ElementSaxParser::getElements()
{
	return elements_;
}
