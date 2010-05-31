/***************************************************************************
 *   Copyright (C) 2005, 2006 by Carsten Niehaus                                 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/
#include "spectrumwidget.h"
#include "spectrum.h"
#include "kalziumutils.h"

#include <config-kalzium.h>

#include <element.h>

#include <kdebug.h>
#include <klocale.h>
#include <math.h>

#include <QKeyEvent>
#include <QSizePolicy>
#include <QPainter>
#include <QPixmap>

#include <qglobal.h>
#if defined(HAVE_IEEEFP_H)
#include <ieeefp.h>
#endif

SpectrumWidget::SpectrumWidget( QWidget *parent )
  : QWidget( parent )
{
	startValue = 0;
	endValue = 0;

	m_LMBPointCurrent.setX( -1 );
	m_LMBPointPress.setX( -1 );

	m_realHeight = 200;

	Gamma = 0.8;
	IntensityMax = 255;

	setType( EmissionSpectrum );

	setMinimumSize( 400, 230 );
	setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	setAttribute( Qt::WA_OpaquePaintEvent, true );
	setContextMenuPolicy( Qt::PreventContextMenu );
}

void SpectrumWidget::paintEvent( QPaintEvent * /*e*/ )
{
	if ( !m_spectrum )
		 return;

	m_pixmap = QPixmap( width(), height() );
	m_pixmap.fill( this, width(), height() );

	QPainter p;
	p.begin( &m_pixmap );
	p.fillRect( 0, 0, width(), m_realHeight, Qt::black ); 

	paintBands( &p );
	
	drawTickmarks( &p );

	if ( m_LMBPointPress.x() != -1 && m_LMBPointCurrent.x() != -1 )
		drawZoomLine( &p );

	p.end();

	QPainter p2(this);
	p2.drawPixmap(0, 0, m_pixmap);
}

void SpectrumWidget::drawZoomLine( QPainter* p )
{
	p->setPen( Qt::white );
	p->drawLine( m_LMBPointPress.x(), m_LMBPointPress.y(), m_LMBPointCurrent.x(), m_LMBPointPress.y() );
	p->drawLine( m_LMBPointCurrent.x(), m_LMBPointPress.y()+10, m_LMBPointCurrent.x(), m_LMBPointPress.y()-10 );
	p->drawLine( m_LMBPointPress.x(), m_LMBPointPress.y()+10, m_LMBPointPress.x(), m_LMBPointPress.y()-10 );
}

void SpectrumWidget::paintBands( QPainter* p )
{
    if ( m_type == AbsorptionSpectrum )
    {
        for ( double va = startValue; va <= endValue ; va += 0.1 )
        {
            int x = xPos( va );
            p->setPen( linecolor( va ) );
            p->drawLine( x,0,x, m_realHeight );
        }

        p->setPen( Qt::black );
    }

    int i = 0;
    int x = 0;
    int temp = 0;	

    foreach ( Spectrum::peak * peak , m_spectrum->peaklist() )
    {
        if ( peak->wavelength < startValue || peak->wavelength > endValue )
            continue;

        x = xPos( peak->wavelength );

        temp = 0;  

        switch ( m_type )
        {
            case EmissionSpectrum:
                p->setPen( linecolor( peak->wavelength ) );
                p->drawLine( x,0,x, m_realHeight-1 );

                p->setPen( Qt::black );
                p->drawLine( x,m_realHeight,x, m_realHeight );
                break;

            case AbsorptionSpectrum:
                p->setPen( Qt::black );
                p->drawLine( x,0,x, m_realHeight-1 );
                break;
        }

        i++;
    }
}

QColor SpectrumWidget::linecolor( double spectrum )
{
        int r,g,b;
        wavelengthToRGB( spectrum, r,g,b );

        QColor c( r,g,b );
        return c;
}


void SpectrumWidget::wavelengthToRGB( double wavelength, int& r, int& g, int& b )
{
    double blue = 0.0, green = 0.0, red = 0.0, factor = 0.0;

    int wavelength_ = ( int ) floor( wavelength );
    if ( wavelength_ < 380 || wavelength_ > 780 )
    {
        //make everything white
        r = g = b = 255;
        return;
    }
    else if ( wavelength_ > 380 && wavelength_ < 439 )
    {
        red = -( wavelength-440 ) / ( 440-380 );
        green = 0.0;
        blue = 1.0;
    }
    else if ( wavelength_ > 440 && wavelength_ < 489 )
    {
        red = 0.0;
        green = ( wavelength-440 ) / ( 490-440 );
        blue = 1.0;
    }
    else if ( wavelength_ > 490 && wavelength_ < 509 )
    {
        red = 0.0;
        green = 1.0;
        blue = -( wavelength-510 ) / ( 510-490 );
    }
    else if ( wavelength_ > 510 && wavelength_ < 579 )
    {
        red = ( wavelength-510 ) / ( 580-510 );
        green = 1.0;
        blue = 0.0;
    }
    else if ( wavelength_ > 580 && wavelength_ < 644 )
    {
        red = 1.0;
        green = -( wavelength-645 ) / ( 645-580 );
        blue = 0.0;
    }
    else if ( wavelength_ > 645 && wavelength_ < 780 )
    {
        red = 1.0;
        green = 0.0;
        blue = 0.0;
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
		return qRound( IntensityMax * pow( color*factor, Gamma ));
}

void SpectrumWidget::drawTickmarks( QPainter* p )
{
    //the size of the text on the tickmarks
    const int space = 20;

    //the distance between the tickmarks in pixel
    const int d = 10;

    //the total number of tickmarks to draw (small and long)
    const int numberOfTickmarks = ( int )floor( ( double )(width()/d) );

    double pos = 0.0;

    for ( int i = 0; i < numberOfTickmarks; i++  )
    {
        if( i%5 == 0 )
        {//long tickmarks plus text
            p->drawLine( i*d, m_realHeight, i*d, m_realHeight+10 );	
            if ( i%10 == 0 && 
                    i*d > space && 
                    i*d < width()-space )
            {
                pos = ( double ) ( i*d )/width();
                p->fillRect( i*d-space, m_realHeight+12, 2*space, 15, Qt::white );
                p->drawText( i*d-space, m_realHeight+12, 2*space, 15, Qt::AlignCenter, QString::number( KalziumUtils::strippedValue( Wavelength( pos ) ) ) );
            }
        }
        else {//small tickmarks
            p->drawLine( i*d, m_realHeight, i*d, m_realHeight+5 );
        }
    }
}

void SpectrumWidget::keyPressEvent( QKeyEvent *e )
{
    switch ( e->key() )
    {
        case Qt::Key_Plus:
            slotZoomIn();
            break;
        case Qt::Key_Minus:
            slotZoomOut();
            break;
    }
}

void SpectrumWidget::slotZoomOut()
{
    kDebug() << "SpectrumWidget::slotZoomOut() "<< startValue << ":: "<< endValue;

    double diff = endValue - startValue;

    double offset = diff * 0.05;

    endValue = endValue + offset;
    startValue = startValue - offset;

    //check for invalid values
    if ( startValue < 0.0 )
        startValue = 0.0;

    if ( endValue > 10000.0 )
        endValue = 40000.0;

    setBorders( startValue, endValue );
}

void SpectrumWidget::setBorders( double left, double right )
{
    kDebug() << "setBorders    " << left << ".."<< right;

    startValue = left;
    endValue = right;

    //round the startValue down and the endValue up
    emit bordersChanged( int(startValue+0.5), int(endValue+0.5) );

    update();
}

void SpectrumWidget::slotZoomIn()
{
    kDebug() << "SpectrumWidget::slotZoomIn() "<< startValue << ":: "<< endValue;

    double diff = endValue - startValue;

    double offset = diff * 0.05;

    endValue = endValue - offset;
    startValue = startValue + offset;

    setBorders( startValue, endValue );
}

void SpectrumWidget::mouseMoveEvent( QMouseEvent *e )
{
    m_LMBPointCurrent = e->pos();
    update();
}

void SpectrumWidget::mousePressEvent(  QMouseEvent *e )
{
    if (  e->button() == Qt::LeftButton ) {
        m_LMBPointPress = e->pos();
    }
    if (  e->button() == Qt::RightButton ) {
        restart();
    }
    
    findPeakFromMouseposition( Wavelength( ( double )e->pos().x()/width() ) );
}

void SpectrumWidget::findPeakFromMouseposition( double wavelength )
{
    kDebug() << "SpectrumWidget::findPeakFromMouseposition()";
    Spectrum::peak *peak = NULL;

    //find the difference in percent (1.0 is 100%, 0.1 is 10%)
    double dif = 0.0;

    bool foundWavelength = false;

    //find the highest intensity
    foreach( Spectrum::peak *currentPeak, m_spectrum->peaklist() )
    {
        double thisdif = currentPeak->wavelength / wavelength;

        if ( thisdif < 0.9 || thisdif > 1.1 )
            continue;

        if ( thisdif > 1.0 ){//convert for example 1.3 to 0.7
            thisdif = thisdif-1;
            thisdif = 1-thisdif;
        }

        if ( thisdif > dif )
        {
            dif = thisdif;
            peak = currentPeak;
            foundWavelength = true;
        }
    }

    if ( foundWavelength )
        emit peakSelected(peak);
}

void SpectrumWidget::mouseReleaseEvent(  QMouseEvent *e )
{
	if (  e->button() == Qt::LeftButton )
	{
		int left = (int)Wavelength( ( double )m_LMBPointPress.x()/width() );
		int right = (int)Wavelength( ( double )e->pos().x()/width() );

		if ( left == right )
			return;

		if ( left > right )
			setBorders( right, left );
		else
			setBorders( left, right );
	}

	m_LMBPointPress.setX( -1 );
	m_LMBPointCurrent.setX( -1 );
}

void SpectrumWidget::restart()
{
    //set the minimum and maximum peak to the min/max wavelenght
    //plus/minus ten. This makes then always visible
    setBorders(m_spectrum->minPeak()-10.0, m_spectrum->maxPeak()+10.0);
}

#include "spectrumwidget.moc"

