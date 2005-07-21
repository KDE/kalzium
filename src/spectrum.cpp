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
#include "exporter.h"

#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <kglobal.h>
#include <kstandarddirs.h>
#include <kfiledialog.h>
#include <kmessagebox.h>
#include <klocale.h>
#include <kdebug.h>

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
		( *it ).intensity = ( int ) round( newInt );
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

QString Spectrum::BandsAsHtml()
{
	QString html = "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><html><head><title>Chemical data</title>i<body>";

	html.append( "<table>" );
	
	QValueList<band>::const_iterator it = m_bandlist.begin();
	const QValueList<band>::const_iterator itEnd = m_bandlist.end();
	for (;it!=itEnd;++it)
	{
		html.append( "<tr>" );
		html.append( i18n( "<td>Wavelength: %1 nm</td>" ).arg( ( *it ).wavelength ) );
		html.append( i18n( "<td>Intensity: %1</td>" ).arg( ( *it ).intensity ) );
		html.append( i18n( "<td>Probability: %1 10<sup>8</sup>s<sup>-1</sup></td>" ).arg( ( *it ).aki ) );
		html.append( i18n( "<td>Energy 1: %1</td>" ).arg( ( *it ).energy1 ) );
		html.append( i18n( "<td>Energy 2: %1</td>" ).arg( ( *it ).energy2 ) );
		html.append( i18n( "<td>Electronconfiguration 1: %1</td>" ).arg( ( *it ).electronconfig1 ) );
		html.append( i18n( "<td>Electronconfiguration 2: %1</td>" ).arg( ( *it ).electronconfig2 ) );
		html.append( i18n( "<td>Term 1: %1</td>" ).arg( ( *it ).term1 ) );
		html.append( i18n( "<td>Term 2: %1</td>" ).arg( ( *it ).term2 ) );
		html.append( i18n( "<td>J 1: %1</td>" ).arg( ( *it ).J1 ) );
		html.append( i18n( "<td>J 2: %1</td>" ).arg( ( *it ).J2 ) );
		html.append( "</tr>\n");
	}
	
	html.append( "</table>" );

	html.append( "</body></html>" );
	return html;
}

SpectrumView::SpectrumView( Spectrum *spec, QWidget *parent, const char* name )
	: QWidget( parent, name )
{
	QVBoxLayout *spectrumLayout = new QVBoxLayout( this, 0, -1, "spectrumLayout" );

	m_spectrum = spec->adjustToWavelength( 100.0, 1000.0 );
	m_spectrum->adjustMinMax();
	
	m_spectrumWidget = new SpectrumWidget( this, "spectrum" );
	m_spectrumWidget->setSpectrum( m_spectrum );
	
	//m_spectrumWidget->setBorders( m_spectrum->min(), m_spectrum->max() );
	
	spectrumLayout->addWidget( m_spectrumWidget );

	QHBoxLayout *hbox = new QHBoxLayout( this );
//X 	m_spinbox_left = new QSpinBox( m_spectrum->min(), m_spectrum->max()-1, 1, this );
//X 	m_spinbox_right = new QSpinBox( m_spectrum->min()+1, m_spectrum->max(), 1, this );i
	m_spinbox_left = new QSpinBox( 100, 1000, 1, this );
	m_spinbox_right = new QSpinBox( 100, 1000, 1, this );
//X 	m_spinbox_left->setValue(  ( double ) m_spectrum->min() );
//X 	m_spinbox_right->setValue( ( double ) m_spectrum->max() );
	m_button_save = new QPushButton( i18n("Save spectrum as PNG"), this, "button_save" );
	
	connect( m_spinbox_right, SIGNAL( valueChanged( int ) ), m_spectrumWidget, SLOT( setRightBorder( int ) ) );
	connect( m_spinbox_left, SIGNAL( valueChanged( int ) ), m_spectrumWidget, SLOT( setLeftBorder( int ) ) );
	connect( m_spectrumWidget, SIGNAL( bordersChanged( int, int ) ), this, SLOT( slotBordersChanged( int, int ) ) );	
	connect( m_button_save, SIGNAL( pressed() ), this, SLOT( slotSave() ) ); 

	m_spectrumbox = new KComboBox( this, "combobox" );
	m_spectrumbox->insertItem( "Emission Spectrum" );
	m_spectrumbox->insertItem( "Absorption Spectrum" );
	connect( m_spectrumbox, SIGNAL( activated( int ) ), m_spectrumWidget, SLOT( slotActivateSpectrum( int ) ) );
	
	m_spinbox_left->setValue( 100 );
	m_spinbox_right->setValue( 1000 );
	m_spectrumWidget->setBorders( 100, 1000 );

	hbox->addWidget( new QLabel( i18n( "Minimumvalue" ), this ) );
	hbox->addWidget( m_spinbox_left );
	hbox->addWidget( new QLabel( i18n( "Maximumvalue" ), this ) );
	hbox->addWidget( m_spinbox_right );
	hbox->addWidget( m_button_save );
	hbox->addWidget( m_spectrumbox );

	spectrumLayout->addLayout( hbox );
}

void SpectrumView::slotBordersChanged( int left, int right )
{
	m_spinbox_left->setValue( left );
	m_spinbox_right->setValue( right );
}

void SpectrumView::slotSave()
{
	QString fileName = KFileDialog::getSaveFileName( QString::null, "*.png", this, i18n( "Save Spectrum" ) ); 
	if( !fileName.isEmpty() )
	{
    		Exporter* exporter = new Exporter();
		if ( !exporter->saveAsPNG( *m_spectrumWidget, fileName ) )
			KMessageBox::error( this, i18n( "The spectrum could not be saved"), i18n( "PNG could not be saved") );
		delete exporter;
	}	
}

#include "spectrum.moc"
