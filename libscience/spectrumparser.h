#ifndef SPECTRUMPARSER_H
#define SPECTRUMPARSER_H
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

#include <libkdeedu_science_export.h>

#include <QFile>
#include <qxml.h>

#include "spectrum.h"

#include "chemicaldataobject.h"

class Spectrum;

/**
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class EDUSCIENCE_EXPORT SpectrumParser : public QXmlDefaultHandler
{
	public:
		/**
		 * Constructor
		 */
		SpectrumParser();
		bool startElement( const QString&, const QString &localName, const QString&, const QXmlAttributes &attrs );

		bool endElement( const QString& namespaceURI, const QString &localName, const QString& qName );
		
		bool characters(const QString &ch);

		QList<Spectrum*> getSpectrums();

	private:
		ChemicalDataObject *currentDataObject_;
		ChemicalDataObject::BlueObeliskUnit currentUnit_;

		QVariant currentErrorValue_;

		Spectrum* currentSpectrum_;
		Spectrum::peak* currentPeak_;
		
		QList<Spectrum*> spectra_;

		bool inMetadata_;
		
		bool inSpectrum_;
		bool inSpectrumList_;

		bool inPeakList_;
		bool inPeak_;
		
		bool inXValue_;
		bool inYValue_;
};
#endif // SPECTRUMPARSER_H

