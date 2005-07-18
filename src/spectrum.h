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
#include <kcombobox.h>

#include "element.h"

class SpectrumWidget;

/**
 * @author Carsten Niehaus
 *
 * This class represents an spectrum with all its properties
 */
class Spectrum
{
	public:
		Spectrum(){
			kdDebug() <<"Spectrum::Spectrum" << endl;

			m_min = minBand();
			m_max = maxBand();

			//For the colorcalculation
        		Gamma = 0.8;
        		IntensityMax = 255,

        		m_realWidth = 360;
        		m_realHeight = 200;
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

		double min() const{
			return m_min;
		}
		
		double max() const{
			return m_max;
		}

		void setWidth( int width ){
			m_width = width;
		}

		void paintBands( QPainter* p, double startValue, double endValue, bool emissionSpectrum );

		/**
		 * @returns the color of a line
		 * @param spectrum the value of the spectrum
		 */
		QColor linecolor( double spectrum );


	private:
		/**
		 * @return the smallest wavelength
		 */
		double minBand();
		
		/**
		 * @return the biggest wavelength
		 */
		double maxBand();

		/**
		 * @return the postion in the widget of a band 
		 * with the wavelength @p wavelength
		 * 
		 * @param wavelength the wavelength for which the position is needed
		 */
		inline int xPos( double wavelength, double startValue, double endValue ){
			return ( int ) ( m_width * ( wavelength - startValue ) / ( endValue - startValue ) );
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

		int m_realWidth;
		int m_realHeight;

		/**
		 * @return the adjusted value of the @p color. The
		 * correction depends on @p factor which has been
		 * figured out emperically
		 */
		int Adjust( double color, double factor );

		QValueList<band> m_bandlist;

		double m_max, m_min;

		int m_width;
};

class SpectrumView : public QWidget
{
	Q_OBJECT

	public: 
		SpectrumView( Spectrum *spec, QWidget* parent, const char* name );

	private:
		SpectrumWidget *m_spectrumWidget;

		Spectrum* m_spectrum;

		QSpinBox *m_spinbox_left, *m_spinbox_right;

		KComboBox *m_spectrumbox;
};

#endif // SPECTRUM_H

