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
	endValue = 800;

	m_realWidth = 360;
	m_realHeight = 200;
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
	
	for(int h = 0; h < 300 ; ++h)
	{
		p->setPen(linecolor( h+450 ));
		int xvalue = m_realWidth/360;
		p->drawLine(xvalue*h,0,xvalue*h,m_realHeight );
	}

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
//	int Hcolor = 
	QColor c( ( int )spectrum-450,255,255, QColor::Hsv );
	return c;
}

#include "spectrum.moc"
