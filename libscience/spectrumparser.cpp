/***************************************************************************
copyright            : (C) 2005, 2008 by Carsten Niehaus
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
#include <QDebug> 

class SpectrumParser ::Private
{
    public:
        Private()
            : currentSpectrum(0),
            inMetadata_(false),
            inSpectrum_(false),
            inSpectrumList_(false),
            inPeakList_(false),
            inPeak_(false),
            inXValue_(false),
            inYValue_(false)
    {}

        ~Private()
        {
            delete currentSpectrum;
            delete currentPeak;
        }

        Spectrum * currentSpectrum;
        Spectrum::peak * currentPeak;

        bool inMetadata_;
        bool inSpectrum_;
        bool inSpectrumList_;
        bool inPeakList_;
        bool inPeak_;
        bool inXValue_;
        bool inYValue_;

        QList<Spectrum*> spectra;
};

SpectrumParser::SpectrumParser()
	: QXmlDefaultHandler(), d( new Private )
{
}

bool SpectrumParser::startElement(const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs)
{
	if (localName == "spectrum") 
	{
		d->currentSpectrum = new Spectrum();
		d->inSpectrum_ = true;
		
		//now save the element of the current spectrum
		for (int i = 0; i < attrs.length(); ++i) 
		{
                    if ( attrs.localName( i ) == "id" ) {
                        currentElementID = attrs.value( i );
                    }
		}
	} else if (d->inSpectrum_ && localName == "peakList") {
		d->inPeakList_ = true;
	}
	else if (d->inSpectrum_ && d->inPeakList_ && localName == "peak") {
		for (int i = 0; i < attrs.length(); ++i) 
		{
			if (attrs.value(i) == "xValue")
				d->inXValue_ = true;
			else if (attrs.value(i) == "yValue")
				d->inYValue_ = true;
		}
	}
	return true;
}

bool SpectrumParser::endElement( const QString&, const QString& localName, const QString& )
{
	if ( localName == "spectrum" )
	{
            int num = currentElementID.mid(1).toInt();
            qDebug() << "num is " << num;
            d->currentSpectrum->setParentElementNumber( num );

            d->spectra.append( d->currentSpectrum );

            d->currentSpectrum = 0;
//X 		d->inSpectrum_ = false;
	}

	return true;
}

bool SpectrumParser::characters(const QString &ch)
{
	ChemicalDataObject currentDataObject_;
	ChemicalDataObject::BlueObelisk type;
	QVariant value;

	if ( d->inXValue_ ){
		value = ch.toDouble();
		type = ChemicalDataObject::exactMass; 
		d->inXValue_ = false;
	}
	else if (d->inYValue_) {
		value = ch.toInt();
		type = ChemicalDataObject::atomicNumber; 
		d->inYValue_ = false;
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
	return d->spectra;
}
