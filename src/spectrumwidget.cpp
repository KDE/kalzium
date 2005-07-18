/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *   
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
#include "spectrumwidget.h"
#include "spectrum.h"

SpectrumWidget::SpectrumWidget( QWidget* parent, const char* name )
	: QWidget( parent,name )
{
	kdDebug() << "SpectrumWidget::SpectrumWidget()" << endl;

	m_realWidth = 360;
	m_realHeight = 200;

	setType( EmissionSpectrum );
}

SpectrumWidget::~SpectrumWidget(){}

void SpectrumWidget::paintEvent( QPaintEvent * /*e*/ )
{
	if ( !m_spectrum ) return;
	QPainter p;
	p.begin( this );
	p.fillRect( 0, 0, width(), m_realHeight, Qt::black ); 

	m_spectrum->setWidth ( width() );
	
	switch ( m_type )
	{
		case EmissionSpectrum:	
			drawEmmissionSpectrum(  &p );
			break;
		case AbsorptionSpectrum:
			drawAbsorptionSpectrum( &p );
			break;
	}
	drawTickmarks( &p );
}

void SpectrumWidget::drawAbsorptionSpectrum( QPainter *p )
{
	kdDebug() << "SpectrumWidget::drawAbsorptionSpectrum()" << endl;

	m_spectrum->paintBands ( p, startValue, endValue, false );
}

void SpectrumWidget::drawEmmissionSpectrum( QPainter *p )
{
	kdDebug() << "SpectrumWidget::drawEmmissionSpectrum()" << endl;

	m_spectrum->paintBands ( p, startValue, endValue, true );
		
//To test the widget uncomment this code.
//X 	for ( double va = startValue; va <= endValue ; va += 0.7 )
//X 	{
//X 		int x = xPos( va );
//X 		p->setPen(linecolor( va ));
//X 		p->drawLine( x,0,x, m_realHeight+10 );
//X 	}
}

void SpectrumWidget::drawTickmarks( QPainter* p )
{
	kdDebug() << "SpectrumWidget::drawTickmarks()" << endl;
	const int space = 13;
	
/*	for ( int i = 0; i < width() ; i+=10 )
	{
		p->drawLine( i,m_realHeight,i, m_realHeight+5 );
	}
	for ( int i = 50; i < width() ; i+=50 )
	{
		double pos = ( double )i/width();

		int wave = ( int )Wavelength( pos );
		
		p->drawLine( i,m_realHeight,i, m_realHeight+10 );
		p->fillRect( i-space, m_realHeight+12, 2*space, 15, Qt::white );
		p->drawText( i-space, m_realHeight+12, 2*space, 15, Qt::AlignCenter, QString::number( wave ) );
	}
*/
	int start = (int)startValue % 10;
	double dist =  width()/(endValue-startValue) * 10;	//distance between the tickles in px

	int count = ( int )startValue - start + 10;
	start *= width()/(endValue-startValue);

	for ( int i = start; i < width(); i += dist )
	{
		if(count%50 == 0 )
		{
			//int wave = ( int )Wavelength( count );

			p->drawLine( i, m_realHeight, i, m_realHeight+10 );	
			p->fillRect( i-space, m_realHeight+12, 2*space, 15, Qt::white );
			p->drawText( i-space, m_realHeight+12, 2*space, 15, Qt::AlignCenter, QString::number( count ) );		
		}
		else
		{
			p->drawLine( i, m_realHeight, i, m_realHeight+5 );
		}
		count += 10;
	}
}

#include "spectrumwidget.moc"

