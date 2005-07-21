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
class QPushButton;

/**
 * @author Carsten Niehaus
 *
 * This class represents an spectrum with all its properties
 */
class Spectrum
{
	public:
		/**
		 * public ctor
		 */
		Spectrum( Element* parent ){
			m_parentElement = parent;
		};
		
		/**
		 * public dtor
		 */
		~Spectrum(){};

		/**
		 * a band is one line in the spectrum of an element
		 */
		struct band
		{
			///in nm)
			double wavelength;

			///Transition Probabilities
			///10^8s-1  (1000000000 per second)
			double aki;

			///per centimeter
			double energy1;

			///per centimeter
			double energy2;

			///relative. The highest is per definition 1000
			int intensity;

			QString electronconfig1;
			QString electronconfig2;
			QString term1;
			QString term2;
			QString J1;
			QString J2;
		};

		/**
		 * adds the band @p b to the internal
		 * lists of bands
		 */
		void addBand( band b ){
			m_bandlist.append( b );
		}

		/**
		 * @param min the lowest allowed wavalength in nanometer
		 * @param max the highest allowed wavalength in nanometer
		 * 
		 * @returns a spectrum with the wavelength in the range
		 * of @p min to @p max. The intensities are readjusted
		 * so that the biggest intensity is again 1000 and the 
		 * others are adopted.
		 */
		Spectrum* adjustToWavelength( double min, double max );

		/**
		 * sets the highest intensity to 1000 and adjusts the
		 * others
		 */
		void adjustIntensities();

		/**
		 * @param min the lowest allowed wavalength in nanometer
		 * @param max the highest allowed wavalength in nanometer
		 * 
		 * @return the wavelength in a QValueList<double>
		 */
		QValueList<double> wavelengths( double min, double max );

		/**
		 * @return the smallest wavelength
		 */
		double min() const{
			return m_min;
		}
		
		/**
		 * @return the highest wavelength
		 */
		double max() const{
			return m_max;
		}

		QValueList<band>* bandlist(){
			return &m_bandlist;
		}
	
		/**
		 * cache the values of the biggest and
		 * smallest wavelenght
		 */
		void adjustMinMax(){
			m_min = minBand();
			m_max = maxBand();
		}

		/**
		 * @return the data of a spectrum as html-code
		 */
		QString BandsAsHtml();

		/**
		 * @return the parent element of this spectrum
		 */
		Element* parentElement()
		{ return m_parentElement; }

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
		 * the internal dataset
		 */
		QValueList<band> m_bandlist;

		/**
		 * the cached values of the highest and lowest wavelength
		 */
		double m_max, m_min;

		Element* m_parentElement;
};

/**
 * @author Carsten Niehaus
 */
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

		QPushButton *m_button_save;

	public slots:
		void slotBordersChanged( int, int );
		void slotSave();
};

#endif // SPECTRUM_H

