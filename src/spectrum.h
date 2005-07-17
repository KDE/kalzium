#ifndef SPECTRUM_H
#define SPECTRUM_H
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
#include <qwidget.h>
#include <qcolor.h>
#include <qpainter.h>
#include <qimage.h>
#include <qstring.h>
#include <qvaluelist.h>
#include <qspinbox.h>

#include <kimageeffect.h>
#include <kdebug.h>
#include <kpixmapeffect.h>
#include <kcombobox.h>

#include "element.h"

/**
 * @author Carsten Niehaus
 *
 * This class represents an spectrum with all its properties
 */
class Spectrum
{
	public:
		Spectrum(){
			m_min = minBand();
			m_max = maxBand();
		};
		~Spectrum(){};

		/**
		 * a band is one line in the spectrum of an element
		 */
		struct band
		{
			double wavelength;
			double aki;
			double energy1;
			double energy2;
			int intensity;
			QString electronconfig1;
			QString electronconfig2;
			QString term1;
			QString term2;
			QString J1;
			QString J2;
		};

		void addBand( band b ){
			m_bandlist.append( b );
		}

	private:
		/**
		 * @return the smallest wavelength
		 */
		double minBand();
		
		/**
		 * @return the biggest wavelength
		 */
		double maxBand();

		QValueList<band> m_bandlist;

		double m_max, m_min;
};

/**
 * @author Carsten Niehaus
 */
class SpectrumWidget : public QWidget
{
	Q_OBJECT

	public:
		SpectrumWidget( QWidget *parent, const char* name = 0 );
		~SpectrumWidget();

		void setSpectrum( Spectrum* spec ){
			m_spectrum = spec;
		}

		/**
		 * This limits the width of the spectrum in terms of
		 * wavelength. For example you can set it to only
		 * show the area between 500 and 550 nm
		 *
		 * @param left the left border
		 * @param right the right border
		 */
		void setBorders( double left, double right ){
			startValue = left;
			endValue = right;
		}
		
		/**
		 * find the nearest band. The returnvalue is the number
		 * of pixel the next band is away
		 */
		int findNearestBand( QValueList<double>::iterator it );

		/**
		 * there are several possible types.
		 */
		enum SpectrumType
		{
			EmissionSpectrum = 0,
			AbsorptionSpectrum
		};

		/**
		 * sets the type of the spectrum to @p t
		 * @param t the type of the spectrum
		 */
		void setType( SpectrumType t ){
			m_type = t;
		}

		/**
		 * @return the currently active type
		 * of the spectrum
		 */
		SpectrumType spectrumType() const{
			return m_type;
		}
	
	private:
		QValueList<double> m_spectra;

		SpectrumType m_type;

		/**
		 * @return the adjusted value of the @p color. The
		 * correction depends on @p factor which has been
		 * figured out emperically
		 */
		int Adjust( double color, double factor );

		Spectrum *m_spectrum;

		/**
		 * @param the position on a 0 to 1-scale in the widget. 0.5 mean 
		 * that you want the wavelength in the middle of the widget,
		 * 0.25 mean at one quarter of the width of the widget
		 *
		 * @param position the position on a 0 to 1 scale.
		 * @return the Wavelength on @p position
		 */
		inline double Wavelength( double position ){
			return startValue + ( ( endValue-startValue ) *  position );
		}

		/**
		 * This method changes the three values @p r @p g and @p b to the 
		 * correct values
		 * param wavelength the wavelength for which the color is searched
		 * param r red
		 * param g green 
		 * param b blue
		 */
		void wavelengthToRGB( double wavelength, int& r, int& g, int& b );

		double Gamma;
		int IntensityMax;

		/**
		 * draws the spectra-lines
		 */
		void drawAbsorptionSpectrum( QPainter *p );
		
		/**
		 * draws the spectra-lines
		 */
		void drawEmmissionSpectrum( QPainter *p );

		/**
		 * Draw the scale
		 */
		void drawTickmarks( QPainter *p );

		/**
		 * @return the postion in the widget of a band 
		 * with the wavelength @p wavelength
		 * 
		 * @param wavelength the wavelength for which the position is needed
		 */
		inline int xPos( double wavelength ){
			return ( int ) ( width() * ( wavelength - startValue ) / ( endValue - startValue ) );
		}

		/**
		 * @returns the color of a line
		 * @param spectrum the value of the spectrum
		 */
		QColor linecolor( double spectrum );
	
		double startValue;
		double endValue;

		int m_realWidth;
		int m_realHeight;

	public slots:
		/**
		 * set the the minimumvalue to @p value
		 */
		void setRightBorder( int value ){
			endValue = value;
			if ( endValue < startValue )
				startValue = endValue-1;
			update();
		}
		
		/**
		 * set the the maximumvalue to @p value
		 */
		void setLeftBorder( int value ){
			startValue = value;
			if ( startValue > endValue )
				endValue = startValue+1;
			update();
		}

		/**
		 * activates the spectrum of the type @p spectrumtype
		 */
		void slotActivateSpectrum( int spectrumtype ){
			m_type = ( SpectrumType )spectrumtype;
			update();
		}
	
	protected:
		virtual void paintEvent( QPaintEvent *e );
};

class SpectrumView : public QWidget
{
	Q_OBJECT

	public: 
		SpectrumView( QWidget* parent, const char* name );

		void setSpectra( Spectrum* spec ){
			m_spectrum = spec;
		}

	private:
		SpectrumWidget *m_spectrumWidget;

		Spectrum* m_spectrum;

		QSpinBox *m_spinbox_left, *m_spinbox_right;

		KComboBox *m_spectrumbox;
};

#endif // SPECTRUM_H

