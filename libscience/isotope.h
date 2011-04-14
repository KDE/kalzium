#ifndef ISOTOPE_H
#define ISOTOPE_H
/***************************************************************************
 *   Copyright (C) 2005-2008 by Carsten Niehaus                            *
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

#include "chemicaldataobject.h"

#include "libkdeedu_science_export.h"

/**
 * @author Carsten Niehaus
 *
 * This class represents an Isotope with all its properties
 */
class SCIENCE_EXPORT Isotope
{
	public:
		/**
		 * Constructs a new empty isotope.
		 */
		Isotope();

		/**
		 * Destructor
		 */
		~Isotope();

		/**
		 * This struct stores the information how the nucleons in the 
		 * isotopes are split into neutrons and protons.
		 */
		struct Nucleons
		{
			/**
			 * the number of neutrons of the isotope
			 */
			int neutrons;
			
			/**
			 * the number of protons of the isotope
			 */
			int protons;
		};

		/**
		 * @return the mass of the isotope
		 */
		double mass() const;

		/**
		 * @return the errormargin ( delta mass ) of the isotope
		 */
		QString errorMargin() const;

		/**
		 * If the isotope belongs to Iron, this method will return "26"
		 * @return the number of the element the isotope belongs to
		 */
		int parentElementNumber() const;

		/**
		 * If the isotope belongs to Iron, this method will return "Fe"
		 * @return the symbol of the element the isotope belongs to
		 */
		QString parentElementSymbol() const;

		QString spin() const;

                /**
                 * @return the magnetic moment of the Isotope
                 */
		QString magmoment() const;

		QString abundance() const;		

                /**
                 * @return for example '17' if halflife of this Isotope is 17 seconds
                 * @ref halflife()
                 */
		double halflife() const;

                /**
                 * @return for example 's' if the unit of the halflife of this Isotope is given in
                 * seconds
                 */
                QString halflifeUnit() const;
		
		/**
		 * add the ChemicalDataObject @p o
		 */
		void addData( ChemicalDataObject o );

                /**
                 * Set the number of nucleons of the isotope to @number
                 */ 
		void setNucleons( int number );
		
		/**
		 * @return the sum of protons and neutrons
		 */
		int nucleons() const;

		/**
		 * @return decay
		 */
		double ecdecay() const;

		/**
		 * @return decay likeliness
		 */
		double eclikeliness() const;

		/**
		 * @return decay
		 */
		double betaminusdecay() const;
		/**
		 * @return decay likeliness
		 */
		double betaminuslikeliness() const;

		/**
		 * @return decay
		 */
		double betaplusdecay() const;
		
		/**
		 * @return decay likeliness
		 */
		double betapluslikeliness() const;

		/**
		 * @return decay
		 */
		double alphadecay() const;

		
		/**
		 * @return decay
		 */
		double alphalikeliness() const;

		/**
		 * This enum stores the different kinds of decay
		 */
		enum Decay
		{
			ALPHA/**<alpha decay*/,
			BETAPLUS/**<beta plus decay*/,
			BETAMINUS/**<beta minus decay*/,
			EC/**ec decay*/
		};

		/**
		 * @return the nucleons of neutrons of the Isotope after the decay
		 */
		Isotope::Nucleons nucleonsAfterDecay( Decay kind );


	private:
		/**
		 * the symbol of the element the isotope belongs to
		 */
		ChemicalDataObject m_parentElementSymbol;
		
		/**
		 * stores the information about the mass of the Isotope
		 */
		ChemicalDataObject m_mass;

		/**
		 * stores the atomicNumber of the Isotope
		 */
		ChemicalDataObject m_identifier;
		
		/**
		 * stores the spin of the Isotope
		 */
		ChemicalDataObject m_spin;
		
		/**
		 * stores the magneticMoment of the Isotope
		 */
		ChemicalDataObject m_magmoment;
		
		/**
		 * stores the relative abundance of the Isotope
		 */
		ChemicalDataObject m_abundance; 

		/**
		 * stores the halfLife of the Isotope
		 */
		ChemicalDataObject m_halflife;
		
		/**
		 * stores decay energy of the isotope
		 */
		ChemicalDataObject m_ecdecay;
		ChemicalDataObject m_betaplusdecay;
		ChemicalDataObject m_betaminusdecay; 
		ChemicalDataObject m_alphadecay;

		/**
		 * stores the likeliness of a decay of the isotope
		 */
		ChemicalDataObject m_eclikeliness;
		ChemicalDataObject m_betapluslikeliness;
		ChemicalDataObject m_betaminuslikeliness; 
		ChemicalDataObject m_alphalikeliness;
	
		int m_numberOfNucleons;
};

#endif // ISOTOPE_H
