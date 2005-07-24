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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
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
		Isotope( int neutrons, double percentage, double weight, double halflife, const QString& format, bool alphadecay, bool betaplusdecay, bool betaminusdecay, double alphapercentage, double betaminuspercentage, double betapluspercentage, bool ecdecay, double decayenergy, const QString& spin, const QString& magmoment);

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
		 * the weight of the isotope
		 */
		double weight() const{
			return m_weight;
		}
		
		/**
		 * @return true if the isotopes decays in alpha rays
		 */
		bool alphadecay() const{
			return m_alphadecay;
		}
		
		/**
		 * @return true if the isotopes decays in beta plus rays
		 */
		bool betaplusdecay() const{
			return m_betaplusdecay;
		}
		
		/**
		 * @return true if the isotopes decays in beta-minus rays
		 */
		bool betaminusdecay() const{
			return m_betaminusdecay;
		}
		
		bool ecdecay() const{
			return m_ecdecay;
		}
		
		double decayenergy() const{
			return m_decayenergy;
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
		
		///Specify if the decay is of this kind
		bool  m_alphadecay;
		///Specify if the decay is of this kind
		bool  m_betaplusdecay;
		///Specify if the decay is of this kind
		bool  m_betaminusdecay;
		///Specify if the decay is of this kind
		bool  m_ecdecay;
		
		double m_decayenergy;
		
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
};

#endif // ISOTOPE_H
