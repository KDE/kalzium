#ifndef ISOTOPE_H
#define ISOTOPE_H
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include <qstring.h>

/**
 * @author Carsten Niehaus
 * @author Jörg Buchwald
 *
 * This class represents an Isotope with all its properties
 */
class Isotope
{
	public:
		Isotope( int neutrons, 
				int protones, 
				double percentage, 
				double weight, 
				double halflife, 
				QString format, 
				double alphadecay, 
				double betaplusdecay, 
				double betaminusdecay, 
				double ecdecay, 
				double alphapercentage, 
				double betapluspercentage, 
				double betaminuspercentage, 
				double ecpercentage, 
				QString spin, 
				QString magmoment);

		bool seconds() const{
			if ( m_format == "seconds" )
				return true;
			else
				return false;
		}

		/**
		 * @return the halflife period of the isotope it if has one. The format
		 * is defined by seconds()
		 */
		double halflife() const{
			return m_halflife;
		}
		
		double percentage() const{
			return m_percentage;
		}

		/**
		 * @return the number of neutrons the isotope has
		 */
		int neutrons() const{
			return m_neutrons;
		}

		/**
		 * @return the number of protones the isotope has
		 */
		int protones() const{
			return m_protones;
		}

		/**
		 * @return the number of nucleons of ths isotope
		 */
		int nucleons() const{
			return m_protones+m_neutrons;
		}

		/**
		 * the weight of the isotope
		 */
		double weight() const{
			return m_weight;
		}
		
		/**
		 * @return the energy of isotope's alpha decay 
		 */
		double alphadecay() const{
			return m_alphadecay;
		}
		
		/**
		 * @return the energy of isotope's beta plus decay
		 */
		double betaplusdecay() const{
			return m_betaplusdecay;
		}
		
		/**
		 * @return the energy of isotope's beta-minus decay
		 */
		double betaminusdecay() const{
			return m_betaminusdecay;
		}
		
		/**
		 * @return the energy of isotope's EC- decay
		 */
		double ecdecay() const{
			return m_ecdecay;
		}
	
		QString spin() const{
			return m_spin;
		}
		
		QString magmoment() const{
			return m_magmoment;
		}

		/**
		 * @return the halflife as a QString. The format will be
		 * appended, for example "seconds" or "years" depending 
		 * on the timeframe
		 */
		QString halflifeAsString();

		/**
		 * @return the percentage of the betaminus decay
		 */
		double betaminuspercentage() const{
			return m_betaminuspercentage;
		}
		
		/**
		 * @return the percentage of the betaplus decay
		 */
		double betapluspercentage() const{
			return m_betapluspercentage;
		}
		
		/**
		 * @return the percentage of the alpha decay
		 */
		double alphapercentage() const{
			return m_alphapercentage;
		}
		
		/**
		 * @return the percentage of the EC decay
		 */
		double ecpercentage() const{
			return m_ecpercentage;
		}

	private:
		/**
		 * it is either "years" or "seconds". Usually we use seconds. But some
		 * isotopes have half-lifes of billion of years. This simply
		 * doesn't fit into a unsigned int or double
		 */
		QString m_format;

		/**
		 * the weight of the isotope
		 */
		double m_weight;

		/**
		 * the half-life of an isotope, usually in seconds
		 * @see m_format
		 */
		double m_halflife;
		
		/**
		 * If 95.2% of the isotopes are of this type, this
		 * variable will have the value 95.2
		 */
		double m_percentage;

		/**
		 * the number of neutrons
		 */
		int m_neutrons;

		/**
		 * the number of protones
		 */
		int m_protones;
	
		///Specify if the decay is the energy
		double  m_alphadecay;
		///Specify if the decay is the energy
		double  m_betaplusdecay;
		///Specify if the decay is the energy
		double  m_betaminusdecay;
		///Specify if the decay is the energy
		double  m_ecdecay;
		
		
		/**
		*spin and parity
		*/
		QString m_spin;
		
		/**
		* magnetic moment
		*/
		QString m_magmoment;

		/**
		 * the percentage with which the istope decays as alpha-rays
		 */
		double m_alphapercentage;
		
		/**
		 * the percentage with which the istope decays as beta-plus 
		 */
		double m_betapluspercentage;
		
		/**
		 * the percentage with which the istope decays as beta-minus
		 */
		double m_betaminuspercentage;
		/**
		 * the percentage with which the istope decays as EC
		 */
		double m_ecpercentage;
};

#endif // ISOTOPE_H
