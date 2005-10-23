/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/
#include "spectrum.h"

#include "element.h"

#include <klocale.h>

#include <math.h>

double Spectrum::minBand()
{
	double value = ( *m_bandlist.begin() ).wavelength;
	QValueList<band>::const_iterator it = m_bandlist.begin();
	const QValueList<band>::const_iterator itEnd = m_bandlist.end();
	for (;it!=itEnd;++it)
	{
		if ( value > ( *it ).wavelength )
			value = ( *it ).wavelength;
	}
	return value;
}

double Spectrum::maxBand()
{
	double value = ( *m_bandlist.begin() ).wavelength;
	QValueList<band>::const_iterator it = m_bandlist.begin();
	const QValueList<band>::const_iterator itEnd = m_bandlist.end();
	for (;it!=itEnd;++it)
	{
		if ( value < ( *it ).wavelength )
			value = ( *it ).wavelength;
	}
	return value;
}


Spectrum* Spectrum::adjustToWavelength( double min, double max )
{
	Spectrum *spec = new Spectrum();

	QValueList<band>::const_iterator it = m_bandlist.begin();
	const QValueList<band>::const_iterator itEnd = m_bandlist.end();

	for ( ; it != itEnd; ++it )
	{
		if ( ( *it ).wavelength < min || ( *it ).wavelength > max )
			continue;

		spec->addBand( *it );
	}

	spec->adjustMinMax();

	return spec;
}

void Spectrum::adjustIntensities()
{
	int maxInt = 0;
	QValueList<band>::Iterator it = m_bandlist.begin();
	const QValueList<band>::Iterator itEnd = m_bandlist.end();

	//find the highest intensity
	for ( ; it != itEnd; ++it )
	{
		if ( ( *it ).intensity > maxInt )
			maxInt = ( *it ).intensity;
	}

	//check if an adjustment is needed or not
	if ( maxInt == 1000 ) return;

	double max = ( double ) maxInt;

	//now adjust the intensities.
	it = m_bandlist.begin();
	for ( ; it != itEnd; ++it )
	{
		double curInt = ( ( double )( *it ).intensity );
		
		double newInt = max*1000/curInt;
		( *it ).intensity = qRound( newInt );
	}
}

QValueList<double> Spectrum::wavelengths( double min, double max )
{
	QValueList<double> list;
	
	QValueList<band>::const_iterator it = m_bandlist.begin();
	const QValueList<band>::const_iterator itEnd = m_bandlist.end();

	for ( ; it != itEnd; ++it )
	{
		if ( ( *it ).wavelength < min || ( *it ).wavelength > max )
			continue;

		list.append( ( *it ).wavelength );
	}

	return list;
}

QString Spectrum::bandsAsHtml()
{
	QString html = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><head><title>Chemical data</title>i<body>";

	html += "<table>";
	
	QValueList<band>::const_iterator it = m_bandlist.begin();
	const QValueList<band>::const_iterator itEnd = m_bandlist.end();
	for (;it!=itEnd;++it)
	{
		html += QString( "<tr>" )
		     + "<td>" + i18n( "Wavelength: %1 nm" ).arg( ( *it ).wavelength ) + "</td>"
		     + "<td>" + i18n( "Intensity: %1" ).arg( ( *it ).intensity ) + "</td>"
		     + "<td>" + i18n( "Probability: %1 10<sup>8</sup>s<sup>-1</sup>" ).arg( ( *it ).aki ) + "</td>"
		     + "<td>" + i18n( "Energy 1: %1" ).arg( ( *it ).energy1 ) + "</td>"
		     + "<td>" + i18n( "Energy 2: %1" ).arg( ( *it ).energy2 ) + "</td>"
		     + "<td>" + i18n( "Electron Configuration 1: %1" ).arg( ( *it ).electronconfig1 ) + "</td>"
		     + "<td>" + i18n( "Electron Configuration 2: %1" ).arg( ( *it ).electronconfig2 ) + "</td>"
		     + "<td>" + i18n( "Term 1: %1" ).arg( ( *it ).term1 ) + "</td>"
		     + "<td>" + i18n( "Term 2: %1" ).arg( ( *it ).term2 ) + "</td>"
		     + "<td>" + i18n( "J 1: %1" ).arg( ( *it ).J1 ) + "</td>"
		     + "<td>" + i18n( "J 2: %1" ).arg( ( *it ).J2 ) + "</td>"
		     + "</tr>\n";
	}

	html += "</table>";

	html += "</body></html>";
	return html;
}
