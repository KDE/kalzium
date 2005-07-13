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

#include <kglobal.h>
#include <kstandarddirs.h>

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

	QString spectrumpix = KGlobal::dirs()->findResourceDir("data", "kalzium/data/" );
	spectrumpix += "kalzium/data/spektrum.png";

	ref_image = QImage( spectrumpix );

	m_stretch = 1;
}

SpectrumWidget::~SpectrumWidget(){}

void SpectrumWidget::paintEvent( QPaintEvent * /*e*/ )
{
	QPainter p;
	p.begin( this );
	p.fillRect( 0, 0, width(), height(), paletteBackgroundColor() ); 
	p.drawRect( 0,0, width(), height() );
//X 	QImage i = ref_image;
//X 	QPixmap px;
//X 	px.convertFromImage( i.smoothScale( width(), height() ) );
//X 	m_stretch = ( width() * 1.0 ) / ref_image.width();
//X 	bitBlt( this, 0, 0, &px );
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
	//
	//(0,166,172)  is for 500 nm
	//(99,182,82)  is for 550 nm
	//(255,231,49) is for 600 nm
	//(246,144,49) is for 650 nm
	

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
			temp = 55;
		else
			temp = 0;
		
		p->setPen(linecolor( *it ));
		p->drawLine( x,0,x, m_realHeight+10+temp );
		p->save();
		p->translate(x, m_realHeight+10+15+temp);
		p->rotate(-90);
		p->drawText(0, 0, QString::number( *it ));
		p->restore();

		i++;
	}
}

void SpectrumWidget::wavelengthToRGB( double wavelength, int& r, int& g, int& b )
{
	double blue, green, red, factor;

	if ( wavelength > 380 && wavelength < 439 )
	{
		red = -( wavelength-440 ) / ( 440-380 );
		green = 0.0;
		blue = 1.0;
		kdDebug() << "RGB on wavelength 1" << wavelength << ": " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	
	}
	if ( wavelength > 440 && wavelength < 489 )
	{
		red = 0.0;
		green = -( wavelength-440 ) / ( 490-440 );
		blue = 1.0;
		kdDebug() << "RGB on wavelength 2" << wavelength << ": " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength > 490 && wavelength < 509 )
	{
		red = 0.0;
		green = 1.0;
		blue = -( wavelength-510 ) / ( 510-490 );
		kdDebug() << "RGB on wavelength 3" << wavelength << ": " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength > 510 && wavelength < 579 )
	{
		red = -( wavelength-510 ) / ( 580-510 );
		green = 1.0;
		blue = 0.0;
		kdDebug() << "RGB on wavelength 4" << wavelength << ": " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength > 580 && wavelength < 644 )
	{
		red = 1.0;
		green = -( wavelength-645 ) / ( 645-580 );
		blue = 0.0;
		kdDebug() << "RGB on wavelength 5" << wavelength << ": " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength > 645 && wavelength < 780 )
	{
		red = 1.0;
		green = 0.0;
		blue = 0.0;
		kdDebug() << "RGB on wavelength 6" << wavelength << ": " << red << " :: " << green << " :: " << blue << " Factor: " << factor << endl;
	}
	if ( wavelength > 380 && wavelength < 419 )
		factor = 0.3 + 0.7*( wavelength - 380 ) / ( 420 - 380 );
	else if ( wavelength > 420 && wavelength < 700 )
		factor = 1.0;
	else if ( wavelength > 701 && wavelength < 780 )
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
/*
	double var = ( endValue-startValue );
	double coeff = ( value - startValue )/var;

	return coeff * m_realWidth;
*/
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

#include "spectrum.moc"
