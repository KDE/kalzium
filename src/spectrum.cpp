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

#include <qlayout.h>
#include <qlabel.h>

#include <kglobal.h>
#include <kstandarddirs.h>
#include <klocale.h>

#include <math.h>

const double Gamma = 0.80;
const int IntensityMax = 255;

SpectrumWidget::SpectrumWidget( QWidget* parent, const char* name )
	: QWidget( parent,name )
{
	//For the colorcalculation
	Gamma = 0.8;
	IntensityMax = 255,
	
	startValue = 450;
	endValue = 700;

	m_realWidth = 360;
	m_realHeight = 200;

	m_stretch = 1;
}

SpectrumWidget::~SpectrumWidget(){}

void SpectrumWidget::paintEvent( QPaintEvent * /*e*/ )
{
	QPainter p;
	p.begin( this );
	p.fillRect( 0, 0, width(), m_realHeight, Qt::black ); 
	drawLines(  &p );
}

void SpectrumWidget::drawLines( QPainter *p )
{
	//the spectrum goes from about 780nm to about 400nm
	//700 is a dark red 
	//660 yellow
	//580 green
	//500 light blue
	//400 dark blue

	int i = 0;
	for ( QValueList<double>::Iterator it = m_spectra.begin();
			it != m_spectra.end();
			++it )
	{
		if ( (*it) < startValue || ( *it ) > endValue )
			continue;

		int x = xPos( *it );
		
		int temp = 0; // every second item will have a little offset
		if ( i%2 )
			temp = 35;
		else
			temp = 0;
		
		p->setPen(linecolor( *it ));
		p->drawLine( x,0,x, m_realHeight );
		p->setPen( Qt::black );
		p->drawLine( x,m_realHeight,x, m_realHeight+10+temp );
		p->save();
		p->translate(x, m_realHeight+10+15+temp);
		p->rotate(-90);
		p->setPen( Qt::black );
		QString text = QString::number( *it );
		p->drawText(0, 0, text);
		p->restore();

		i++;
	}
		
//To test the widget uncomment this code.
//X 	for ( double va = startValue; va <= endValue ; va += 0.7 )
//X 	{
//X 		int x = xPos( va );
//X 		p->setPen(linecolor( va ));
//X 		p->drawLine( x,0,x, m_realHeight+10 );
//X 	}
}

void SpectrumWidget::wavelengthToRGB( double wavelength, int& r, int& g, int& b )
{
	double blue = 0.0, green = 0.0, red = 0.0, factor = 0.0;

	int wavelength_ = floor( wavelength );

	kdDebug() << wavelength << " :: " << wavelength_ << endl;

	if ( wavelength_ > 380 && wavelength_ < 439 )
	{
		red = -( wavelength-440 ) / ( 440-380 );
		green = 0.0;
		blue = 1.0;
//		kdDebug() << "RGB on wavelength " << wavelength << " (1): " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	
	}
	if ( wavelength_ > 440 && wavelength_ < 489 )
	{
		red = 0.0;
		green = ( wavelength-440 ) / ( 490-440 );
		blue = 1.0;
//		kdDebug() << "RGB on wavelength " << wavelength << " (2): " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength_ > 490 && wavelength_ < 509 )
	{
		red = 0.0;
		green = 1.0;
		blue = -( wavelength-510 ) / ( 510-490 );
//		kdDebug() << "RGB on wavelength " << wavelength << " (3): " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength_ > 510 && wavelength_ < 579 )
	{
		red = ( wavelength-510 ) / ( 580-510 );
		green = 1.0;
		blue = 0.0;
//		kdDebug() << "RGB on wavelength " << wavelength << " (4): " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength_ > 580 && wavelength_ < 644 )
	{
		red = 1.0;
		green = -( wavelength-645 ) / ( 645-580 );
		blue = 0.0;
//		kdDebug() << "RGB on wavelength " << wavelength << "(5): " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength_ > 645 && wavelength_ < 780 )
	{
		red = 1.0;
		green = 0.0;
		blue = 0.0;
//		kdDebug() << "RGB on wavelength " << wavelength << " (6): " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength_ > 380 && wavelength_ < 419 )
		factor = 0.3 + 0.7*( wavelength - 380 ) / ( 420 - 380 );
	else if ( wavelength_ > 420 && wavelength_ < 700 )
		factor = 1.0;
	else if ( wavelength_ > 701 && wavelength_ < 780 )
		factor = 0.3 + 0.7*( 780 - wavelength ) / ( 780 - 700 );
	else
		factor = 0.0;

	r = Adjust( red, factor );
	g = Adjust( green, factor );
	b = Adjust( blue, factor );
}

int SpectrumWidget::Adjust( double color, double factor )
{
	if ( color == 0.0 )
		return 0;
	else
		return round( IntensityMax * pow( color*factor, Gamma ) );
}

int SpectrumWidget::xPos( double value )
{
	int proportion = width() * ( value - startValue ) / ( endValue - startValue );
	return proportion;
}

QColor SpectrumWidget::linecolor( double spectrum )
{
	int r,g,b;
	wavelengthToRGB( spectrum, r,g,b );
	
	QColor c( r,g,b );
	return c;
}

SpectrumView::SpectrumView( QWidget *parent, const char* name )
	: QWidget( parent, name )
{
	QVBoxLayout *spectrumLayout = new QVBoxLayout( this );
	m_spectrum = new SpectrumWidget( this, "spectrum" );
	spectrumLayout->addWidget( m_spectrum );

	QHBoxLayout *hbox = new QHBoxLayout( this );
	m_spinbox_left = new QSpinBox( 380, 779, 1, this );
	m_spinbox_right = new QSpinBox( 381, 780, 1, this );
	m_spinbox_right->setValue( 700 );
	m_spinbox_left->setValue( 400 );
	
	connect( m_spinbox_right, SIGNAL( valueChanged( int ) ), m_spectrum, SLOT( setRightBorder( int ) ) );
	connect( m_spinbox_left, SIGNAL( valueChanged( int ) ), m_spectrum, SLOT( setLeftBorder( int ) ) );
	
	hbox->addWidget( new QLabel( i18n( "Minimumvalue" ), this ) );
	hbox->addWidget( m_spinbox_left );
	hbox->addWidget( new QLabel( i18n( "Maximumvalue" ), this ) );
	hbox->addWidget( m_spinbox_right );

	spectrumLayout->addLayout( hbox );
}

#include "spectrum.moc"
