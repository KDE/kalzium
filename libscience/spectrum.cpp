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
	double value = ( *m_bandlist.begin() )->wavelength;
	QList<band*>::const_iterator it = m_bandlist.begin();
	const QList<band*>::const_iterator itEnd = m_bandlist.end();
	for (;it!=itEnd;++it)
	{
		if ( value > ( *it )->wavelength )
			value = ( *it )->wavelength;
	}
	return value;
}

double Spectrum::maxBand()
{
	double value = ( *m_bandlist.begin() )->wavelength;
	QList<band*>::const_iterator it = m_bandlist.begin();
	const QList<band*>::const_iterator itEnd = m_bandlist.end();
	for (;it!=itEnd;++it)
	{
		if ( value < ( *it )->wavelength )
			value = ( *it )->wavelength;
	}
	return value;
}


Spectrum* Spectrum::adjustToWavelength( double min, double max )
{
	Spectrum *spec = new Spectrum();

	QList<band*>::const_iterator it = m_bandlist.begin();
	const QList<band*>::const_iterator itEnd = m_bandlist.end();

	for ( ; it != itEnd; ++it )
	{
		if ( ( *it )->wavelength < min || ( *it )->wavelength > max )
			continue;

		spec->addBand( *it );
	}

	spec->adjustMinMax();

	return spec;
}

void Spectrum::adjustIntensities()
{
	int maxInt = 0;
	QList<band*>::Iterator it = m_bandlist.begin();
	const QList<band*>::Iterator itEnd = m_bandlist.end();

	//find the highest intensity
	for ( ; it != itEnd; ++it )
	{
		if ( ( *it )->intensity > maxInt )
			maxInt = ( *it )->intensity;
	}

	//check if an adjustment is needed or not
	if ( maxInt == 1000 ) return;

	double max = ( double ) maxInt;

	//now adjust the intensities.
	it = m_bandlist.begin();
	for ( ; it != itEnd; ++it )
	{
		double curInt = ( ( double )( *it )->intensity );
		
		double newInt = max*1000/curInt;
		( *it )->intensity = ( int ) round( newInt );
	}
}

QList<double> Spectrum::wavelengths( double min, double max )
{
	QList<double> list;
	
	QList<band*>::const_iterator it = m_bandlist.begin();
	const QList<band*>::const_iterator itEnd = m_bandlist.end();

	for ( ; it != itEnd; ++it )
	{
		if ( ( *it )->wavelength < min || ( *it )->wavelength > max )
			continue;

		list.append( ( *it )->wavelength );
	}

	return list;
}


