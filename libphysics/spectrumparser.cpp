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

#include <QList>
#include <qbytearray.h>
#include <qdom.h>
#include <qfile.h>
#include <qregexp.h>
#include <qtextstream.h>
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
//	QTextStream stream(  m_file );

	QList<Spectrum::band*> blist = spectrum->bandlist();
		
	QDomElement docelem = doc.createElement(  "spectrum" );
	doc.appendChild( docelem );

	foreach( Spectrum::band* band, blist )
	{
		QDomElement bandelem = doc.createElement(  "band" );
		bandelem.setAttribute(  "wavelength", band->wavelength );
		bandelem.setAttribute(  "intensity", band->intensity );
		bandelem.setAttribute(  "aki", band->aki );
		bandelem.setAttribute(  "energy1", band->energy1 );
		bandelem.setAttribute(  "energy2", band->energy2 );
		bandelem.setAttribute(  "term1", band->term1 );
		bandelem.setAttribute(  "term2", band->term2 );
		bandelem.setAttribute(  "electronconfig1", band->electronconfig1 );
		bandelem.setAttribute(  "electronconfig2", band->electronconfig2 );
		bandelem.setAttribute(  "J1", band->J1 );
		bandelem.setAttribute(  "J2", band->J2 );
		docelem.appendChild( bandelem );
	}



	kdDebug() << "Text: " << doc.toString() << endl;
}
