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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "spectrum.h"
#include "spectrumwidget.h"

#include <qlayout.h>
#include <qlabel.h>

#include <kglobal.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kdebug.h>

#include <math.h>

double Spectrum::minBand()
{
	double value = ( *m_bandlist.begin() ).wavelength;
	QValueList<band>::Iterator it = m_bandlist.begin();
	const QValueList<band>::Iterator itEnd = m_bandlist.end();
	for (;it!=itEnd;++it)
	{
		kdDebug() << "value: " << ( *it ).wavelength << " Current min-value: " << value << endl;
		if ( value > ( *it ).wavelength )
			value = ( *it ).wavelength;
	}
	return value;
}

double Spectrum::maxBand()
{
	kdDebug() << "Spectrum::maxBand()" << endl;

	double value = ( *m_bandlist.begin() ).wavelength;
	QValueList<band>::Iterator it = m_bandlist.begin();
	const QValueList<band>::Iterator itEnd = m_bandlist.end();
	for (;it!=itEnd;++it)
	{
		kdDebug() << "value: " << ( *it ).wavelength << " Current max-value: " << value << endl;
		if ( value < ( *it ).wavelength )
			value = ( *it ).wavelength;
	}
	return value;
}


Spectrum* Spectrum::adjustToWavelength( double min, double max )
{
	Spectrum *spec = new Spectrum();

	QValueList<band>::Iterator it = m_bandlist.begin();
	const QValueList<band>::Iterator itEnd = m_bandlist.end();

	for ( ; it != itEnd; ++it )
	{
		kdDebug( ) << "WL: " << ( *it ).wavelength << endl;
		if ( ( *it ).wavelength < min || ( *it ).wavelength > max )
			continue;

		spec->addBand( *it );
	}

	spec->adjustMinMax();

	return spec;
}

void Spectrum::adjustIntensities()
{
	kdDebug() << "Spectrum::adjustIntensities()" << endl;

	int maxInt = 0;
	QValueList<band>::Iterator it = m_bandlist.begin();
	const QValueList<band>::Iterator itEnd = m_bandlist.end();

	//find the highest intensity
	for ( ; it != itEnd; ++it )
	{
		if ( ( *it ).intensity > maxInt )
			maxInt = ( *it ).intensity;
	}

	kdDebug() << "maxInt" << maxInt << endl;

	//check if an adjustment is needed or not
	if ( maxInt == 1000 ) return;

	double max = ( double ) maxInt;

	//now adjust the intensities.
	it = m_bandlist.begin();
	for ( ; it != itEnd; ++it )
	{
		double curInt = ( ( double )( *it ).intensity );
		
		double newInt = max*1000/curInt;
		( *it ).intensity = ( int ) round( newInt );
	}
}

QValueList<double> Spectrum::wavelengths( double min, double max )
{
	QValueList<double> list;
	
	QValueList<band>::Iterator it = m_bandlist.begin();
	const QValueList<band>::Iterator itEnd = m_bandlist.end();

	for ( ; it != itEnd; ++it )
	{
		if ( ( *it ).wavelength < min || ( *it ).wavelength > max )
			continue;

		list.append( ( *it ).wavelength );
	}

	return list;
}

SpectrumView::SpectrumView( Spectrum *spec, QWidget *parent, const char* name )
	: QWidget( parent, name )
{
	kdDebug()<<"SpectrumView::SpectrumView()" << endl;

	//make sure only visible light will be plotted
	m_spectrum = spec->adjustToWavelength( 380.0, 780.0 );
	m_spectrum->adjustMinMax();
	
	QVBoxLayout *spectrumLayout = new QVBoxLayout( this, 0, -1, "spectrumLayout" );
	m_spectrumWidget = new SpectrumWidget( this, "spectrum" );
	m_spectrumWidget->setSpectrum( m_spectrum );
	kdDebug() << m_spectrum->min() << " max: " << m_spectrum->max() << endl;
	m_spectrumWidget->setBorders( m_spectrum->min(), m_spectrum->max() );
	
	spectrumLayout->addWidget( m_spectrumWidget );

	QHBoxLayout *hbox = new QHBoxLayout( this );
	m_spinbox_left = new QSpinBox( 380, 779, 1, this );
	m_spinbox_right = new QSpinBox( 381, 780, 1, this );
	m_spinbox_right->setValue( 700 );
	m_spinbox_left->setValue( 400 );
	
	connect( m_spinbox_right, SIGNAL( valueChanged( int ) ), m_spectrumWidget, SLOT( setRightBorder( int ) ) );
	connect( m_spinbox_left, SIGNAL( valueChanged( int ) ), m_spectrumWidget, SLOT( setLeftBorder( int ) ) );

	m_spectrumbox = new KComboBox( this, "combobox" );
	m_spectrumbox->insertItem( "Emission Spectrum" );
	m_spectrumbox->insertItem( "Absorption Spectrum" );
	connect( m_spectrumbox, SIGNAL( activated( int ) ), m_spectrumWidget, SLOT( slotActivateSpectrum( int ) ) );
	
	
	hbox->addWidget( new QLabel( i18n( "Minimumvalue" ), this ) );
	hbox->addWidget( m_spinbox_left );
	hbox->addWidget( new QLabel( i18n( "Maximumvalue" ), this ) );
	hbox->addWidget( m_spinbox_right );
	hbox->addWidget( m_spectrumbox );

	spectrumLayout->addLayout( hbox );
}

#include "spectrum.moc"
