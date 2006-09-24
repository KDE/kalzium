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

double Spectrum::minPeak()
{
	double value = m_peaklist.first()->wavelength;
	
	foreach( peak * p, m_peaklist )
	{
		if ( value > p->wavelength )
			value = p->wavelength;
	}
	return value;
}

double Spectrum::maxPeak()
{
	double value = m_peaklist.first()->wavelength;
	
	foreach( peak * p, m_peaklist )
	{
		if ( value < p->wavelength )
			value = p->wavelength;
	}
	
	return value;
}


Spectrum* Spectrum::adjustToWavelength( double min, double max )
{
	Spectrum *spec = new Spectrum();

	foreach( peak * p , m_peaklist )
	{
		if ( p->wavelength >= min || p->wavelength <= max )
			spec->addPeak( p );
	}

	return spec;
}

void Spectrum::adjustIntensities()
{
	int maxInt = 0;
	//find the highest intensity
	foreach ( Spectrum::peak * p, m_peaklist)
	{
		if ( p->intensity > maxInt )
			maxInt = p->intensity;
	}

	//check if an adjustment is needed or not
	if ( maxInt == 1000 ) return;

	const double max = ( double ) maxInt;

	//now adjust the intensities.
	foreach ( Spectrum::peak * p, m_peaklist)
	{
		double newInt = max*1000/p->intensity;

		p->intensity = ( int ) round( newInt );
	}
}

QList<double> Spectrum::wavelengths( double min, double max )
{
	QList<double> list;
	
	foreach( peak * p , m_peaklist )
	{
		if ( p->wavelength >= min || p->wavelength <= max )
			list.append( p->wavelength );
	}

	return list;
}


