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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <QList>
#include <QString>

class Element;

/**
 * @author Carsten Niehaus
 *
 * This class represents an spectrum with all its properties
 */
class Spectrum
{
	public:
		/**
		 * This spectrum doesn't belong to any element
		 */
		Spectrum(){};
		
		/**
		 * public destructor
		 */
		~Spectrum(){};

		/**
		 * a peak is one line in the spectrum of an element
		 */
		class peak
		{
			public:
				peak(){
					wavelength = -1.0;
					aki = -1.0;
					energy1 = -1.0;
					energy2 = -1.0;
					intensity = -1;
				}

			///in nm
			double wavelength;

			///Transition Probabilities
			///10^8s-1  (1000000000 per second)
			double aki;

			///number of energylevels per centimeter
			double energy1;

			///number of energylevels per centimeter
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
		 * adds the peak @p b to the internal
		 * lists of peaks
		 */
		void addPeak( Spectrum::peak* b ){
			if ( b )
				m_peaklist.append( b );
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
		 * @return the wavelength in a QList<double>
		 */
		QList<double> wavelengths( double min, double max );

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

		/**
		 * @return the list of peaks of the spectrum
		 */
		QList<Spectrum::peak*> peaklist(){
			return m_peaklist;
		}
		
		QList<Spectrum::peak*> peakList(){
			return peaklist();
		}
	
		/**
		 * cache the values of the biggest and
		 * smallest wavelenght
		 */
		void adjustMinMax(){
			m_min = minPeak();
			m_max = maxPeak();
		}

		/**
		 * @return the parent element of this spectrum
		 */
		Element* parentElement() { 
			return m_parentElement; 
		}

	private:
		/**
		 * @return the smallest wavelength
		 */
		double minPeak();
		
		/**
		 * @return the bigest wavelength
		 */
		double maxPeak();

		/**
		 * the internal dataset
		 */
		QList<peak*> m_peaklist;

		/**
		 * the cached values of the highest and lowest wavelength
		 */
		double m_max, m_min;

		Element* m_parentElement;
};
#endif // SPECTRUM_H
