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

SpectrumWidget::SpectrumWidget( QWidget* parent, const char* name )
	: QWidget( parent,name )
{
	startValue = 450;
	endValue = 760;

	m_realWidth = 360;
	m_realHeight = 200;

	ref_image = QImage( "/home/carsten/cvs/trunk/kdeedu/kalzium/src/data/spectrum.png" );
}

SpectrumWidget::~SpectrumWidget(){}

void SpectrumWidget::paintEvent( QPaintEvent * /*e*/ )
{
	QPainter p;
	p.begin( this );
	p.fillRect( 0, 0, width(), height(), paletteBackgroundColor() ); 
	p.drawRect( 0,0, width(), height() );
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
	
	for(int h = 450; h < 750 ; ++h)
	{
		p->setPen(linecolor(h) );
		p->drawLine(h-450,0,h-450,m_realHeight );
	}
		
	p->setPen(Qt::black);

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
			temp = 15;
		else
			temp = 0;
		
		p->drawLine( x,0,x, m_realHeight+10+temp );
		p->drawText( x,m_realHeight+10+15+temp, QString::number( *it ));

		i++;
	}
}

int SpectrumWidget::xPos( double value )
{
	double var = ( endValue-startValue );
	double coeff = ( value - startValue )/var;

	return coeff * m_realWidth;
}

QColor SpectrumWidget::linecolor( double spectrum )
{
//The picture has 575 pixel
//

	int max = endValue;
	int min = startValue;

	//Example: Freq is 600nm. 760-450 = 360.
	//760 - 600 = 160.   360/160 = 2.25
	int proportion = ( max-min )/( max - spectrum );

	QRgb rgb = ref_image.pixel(proportion, 2 );

	kdDebug() << "RGB: " << rgb << " Prop " << proportion << endl;
	
	QColor c( rgb );
	return c;
}

#include "spectrum.moc"
