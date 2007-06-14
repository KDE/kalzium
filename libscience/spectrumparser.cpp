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
#include "spectrumparser.h"
#include "spectrum.h"

#include <qdom.h>
#include <QList>
#include <QFile>

#include <kdebug.h>

SpectrumParser::SpectrumParser()
	: QXmlDefaultHandler(), 
	currentSpectrum_(0), 
	currentPeak_( 0 )
{
}

bool SpectrumParser::startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs)
{
	if (localName == "spectrum") 
	{
		currentSpectrum_ = new Spectrum();
		inSpectrum_ = true;
		
//X 		//now save the element of the current spectrum
//X 		for (int i = 0; i < attrs.length(); ++i) 
//X 		{
//X 			if ( attrs.localName( i ) == "elementType" )
//X 				currentElementSymbol_ = attrs.value( i );
//X 		}
	} else if (inSpectrum_ && localName == "peakList") {
		inPeakList_ = true;
	}
	else if (inSpectrum_ && inPeakList_ && localName == "peak") {
		for (int i = 0; i < attrs.length(); ++i) 
		{
			if (attrs.value(i) == "xValue")
				inXValue_ = true;
			else if (attrs.value(i) == "bo:exactMass")
				inYValue_ = true;
		}
	}
	return true;
}

bool SpectrumParser::endElement( const QString&, const QString& localName, const QString& )
{
	if ( localName == "isotope" )
	{
//X 		currentSpectrum_->addData( new ChemicalDataObject( QVariant( currentElementSymbol_ ), ChemicalDataObject::symbol ) );
//X 		isotopes_.append(currentSpectrum_);
//X 		
//X 		currentSpectrum_ = 0;
//X 		inSpectrum_ = false;
	}
	else if ( localName == "scalar" )
	{
//X 		if ( currentDataObject_.type() == ChemicalDataObject::exactMass ){
//X 			currentDataObject_.setErrorValue( currentErrorValue_ );
//X 		}
	}

	return true;
}

bool SpectrumParser::characters(const QString &ch)
{
	ChemicalDataObject currentDataObject_;
	ChemicalDataObject::BlueObelisk type;
	QVariant value;

	if ( inXValue_ ){
		value = ch.toDouble();
		type = ChemicalDataObject::exactMass; 
		inXValue_ = false;
	}
	else if (inYValue_) {
		value = ch.toInt();
		type = ChemicalDataObject::atomicNumber; 
		inYValue_ = false;
	}
	else//it is a non known value. Do not create a wrong object but return
		return true;

	currentDataObject_.setData( value );
	currentDataObject_.setType( type );

//X 	if ( currentSpectrum_ )
//X 		currentSpectrum_->addData( currentDataObject_ );

	return true;
}

QList<Spectrum*> SpectrumParser::getSpectrums()
{
	return spectra_;
}
