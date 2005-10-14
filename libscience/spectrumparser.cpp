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
#include "element.h"

#include <qdom.h>
#include <QList>
#include <QFile>
#include <QTextStream>
#include <QString>

#include <kdebug.h>

SpectrumParser::SpectrumParser(const QString& filename)
{
//	m_file = QFile(filename);
}

SpectrumParser::SpectrumParser(const QFile& file)
{
//	m_file = file;
}

void SpectrumParser::saveSpectrum( Spectrum *spectrum )
{
	if ( !spectrum ) return;

	QDomDocument doc( "SpectrumDocument" );
	
	QFile docfile( "/home/carsten/test.xml" );

	QList<Spectrum::band*> blist = spectrum->bandlist();
		
	QDomElement cmlElement = doc.createElement(  "cml" );
	QDomElement spectrumListElement = doc.createElement(  "spectrumList" );
	QDomElement spectrumElement = doc.createElement(  "spectrum" );
	QDomElement metadataListElement = doc.createElement(  "metadataList" );
	
	//metadata
//X 	QDomElement metadataElement = doc.createElement(  "metadata" );
//X 	if ( spectrum )
//X 	{
//X 		metadataElement.setAttribute("content",spectrum->parentElement()->elname() );
//X 		metadataElement.setAttribute("name","ElementName");
//X 	}
	
	QDomElement metadataElement2 = doc.createElement(  "metadata" );
	metadataElement2.setAttribute("content", "National Institute of Standards and Technology" );
	metadataElement2.setAttribute("name","OWNER");
	
	QDomElement peakListElement = doc.createElement(  "peakList" );

	doc.appendChild( cmlElement );
	cmlElement.appendChild( spectrumListElement );
	spectrumListElement.appendChild( spectrumElement );
	spectrumElement.appendChild( metadataListElement );
//X 	if ( spectrum ) 
//X 		metadataListElement.appendChild( metadataElement );
	metadataListElement.appendChild( metadataElement2 );
	spectrumElement.appendChild( peakListElement );

	foreach( Spectrum::band* band, blist )
	{
		QDomElement peakElement = doc.createElement(  "peak" );
		peakElement.setAttribute( "id" , "" );
		peakElement.setAttribute(  "xUnits", "nm" );
		peakElement.setAttribute(  "xValue", band->wavelength );
		peakElement.setAttribute(  "yUnits", "Intensity relative to 1000" );
		peakElement.setAttribute(  "yValue", band->intensity );

		//now the scalar values
		QDomElement scalar = doc.createElement( "scalar" );
		QDomElement scalar2 = doc.createElement( "scalar" );
		QDomElement scalar3 = doc.createElement( "scalar" );
		QDomElement scalar4 = doc.createElement( "scalar" );
		QDomElement scalar5 = doc.createElement( "scalar" );
		QDomElement scalar6 = doc.createElement( "scalar" );
		QDomElement scalar7 = doc.createElement( "scalar" );
		QDomElement scalar8 = doc.createElement( "scalar" );
		QDomElement scalar9 = doc.createElement( "scalar" );

		scalar.setAttribute( "title","aki" );
		scalar2.setAttribute( "title","energy1" );
		scalar3.setAttribute( "title","energy2" );
		scalar4.setAttribute( "title","term1" );
		scalar5.setAttribute( "title","term2" );
		scalar6.setAttribute( "title","electronconfig1" );
		scalar7.setAttribute( "title","electronconfig2" );
		scalar8.setAttribute( "title","J1" );
		scalar9.setAttribute( "title","J2" );

		scalar.setNodeValue( "test123" );//QString::number( band->aki ) );

//X 		peakElement.setAttribute(  "aki", band->aki );
//X 		peakElement.setAttribute(  "energy1", band->energy1 );
//X 		peakElement.setAttribute(  "energy2", band->energy2 );
//X 		peakElement.setAttribute(  "term1", band->term1 );
//X 		peakElement.setAttribute(  "term2", band->term2 );
//X 		peakElement.setAttribute(  "electronconfig1", band->electronconfig1 );
//X 		peakElement.setAttribute(  "electronconfig2", band->electronconfig2 );
//X 		peakElement.setAttribute(  "J1", band->J1 );
//X 		peakElement.setAttribute(  "J2", band->J2 );


		peakElement.appendChild( scalar );
		peakElement.appendChild( scalar2 );
		peakElement.appendChild( scalar3 );
		peakElement.appendChild( scalar4 );
		peakElement.appendChild( scalar5 );
		peakElement.appendChild( scalar6 );
		peakElement.appendChild( scalar7 );
		peakElement.appendChild( scalar8 );
		peakElement.appendChild( scalar9 );

		peakListElement.appendChild( peakElement );
	}

	kdDebug() << "Text: " << doc.toString() << endl;
}
