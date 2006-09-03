#ifndef CHEMICALDATAOBJECT_H
#define CHEMICALDATAOBJECT_H
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

#include <QVariant>
#include <QString>

#include <libkdeedu_science_export.h>

class ChemicalDataObjectPrivate;

/**
 * A ChemicalDataObject is an object which contains information about 
 * a chemical element. This can for example be a boiling point. The information
 * is stored in a QVariant.
 * This class supports the CML-format defined by the BlueObelisk-Project.
 *
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class EDUSCIENCE_EXPORT ChemicalDataObject
{
	public:
		/**
		 * The BlueObelisk-project defines in their XML file the dataset
		 * with the names of the enum plus "bo:". So for symbol
		 * it is "bo:symbol". To avoid confusion I will choose the very
		 * same naming
		 */
		enum BlueObelisk
		{
			atomicNumber = 0 /**< The atomic number of the element */,
			symbol/**< the symbol of the element */,
			name/**< The IUPAC name of the element */,
			mass/**< # IUPAC Official Masses */,
			exactMass/**< exact masses of the most common isotopes for each element */,
			ionization/**< First inizationenergy */,
			electronAffinity/**< the electron affinity of the element */,
			electronegativityPauling/**< the electronegativity in the definition of Pauling*/,
			radiusCovalent/**< the covalent radius */,
			radiusVDW/**< the van der Waals radius */,
			meltingpoint/**< the meltingpoint */,
			boilingpoint/**< the boilingpoint */,
			periodTableBlock/**< the block of the element */,
			family/**< "Noblegas" "Non-Metal" "Rare_Earth" "Alkaline_Earth" "Alkali_Earth" "Transition" "Other_Metal" "Metalloids" "Halogene" */,
			acidicbehaviour/**< 0 means acidic, 1 means basic, 2 means neutral, 3 means amphoteric*/,
			crystalstructure/**< own, bcc, hdp, ccp, hcp, fcc, d, sc, tet, rh, or, mono*/,
			electronicConfiguration/**< the electronic configuration, for example 1s2 for He*/,
			group/**< This is a value between 1 and 8*/,
			density/**< The density */,
			nameOrigin/**< the origin of the name */,
			orbit/**< the quantumorbit of the element */,
			period/**< the period of the element */,
			date/**< date of discovery of the element. When 0, the element has been known in ancient times. */,
			discoverers/** The name of the discoverers, separated by semicolomns */,
			relativeAbundance/** The abundance, relative to 100 */,
			spin/**< The spin */,
			magneticMoment/**< The magnetic dipole moment */,
			halfLife/**< The halflife */,
			alphapercentage/**< The percentage of alphadecay */,
			alphaDecay/**< The decayenergy of alphadecay in MeV */,
			betapluspercentage/**< The percentage of betaplusdecay */,
			betaplusDecay/**< The decayenergy of betaplusdecay in MeV */,
			betaminuspercentage/**< The percentage of betaminusdecay */,
			betaminusDecay/**< The decayenergy of betaminusdecay in MeV */,
			ecpercentage/**< The percentage of ecdecay */,
			ecDecay/**< The decayenergy of ecminusdecay in MeV */,
			dangerSymbol/**< the danger symbols are the dangers associated with an element, for example Xn,T+ */,
			RPhrase/**< */,
			SPhrase/**< */,
      discoveryCountry
		};

		/**
		 * The BlueObelisk-project defines in their XML file the dataset
		 * with the units in the namespace "bo". 
		 */
		enum BlueObeliskUnit
		{
			kelvin = 0/**< Degree Kelvin */,
			ev/**< electron volt */,
			nm/**< nanometer */,
			pm/**< picometer */,
			y/**< years */,
			s/**< seconds */,
			noUnit/**< no unit */
		};

		/**
		 * Constructor.
		 */
		ChemicalDataObject();

		/**
		 * Constructor.
		 * @param v the data of the object
		 * @param type the type of the data
		 * @param errorValue the error margin of the value @p v
		 *
		 * @see errorValue()
		 */
		ChemicalDataObject( const QVariant& v, 
				BlueObelisk type,
				const QVariant& errorValue = QVariant(0) );

		/**
		 * Set the data of this object to @p v
		 * @param v the value of the object
		 */
		void setData( const QVariant& v );

		/**
		 * Set the error value of this object to @p v. 
		 * The error has to have the same unit as the value.
		 * @param v the value of the object
		 */
		void setErrorValue(  const QVariant& v );
		
		/**
		 * Destructor.
		 */
		~ChemicalDataObject();

		/**
		 * Every ChemicalDataObject contains one data. For example a
		 * integer value which represents the boiling point. This method
		 * returns the value as a QString.
		 * 
		 * For bool, the returned string will be "false" or "true"
		 * For a QString, the QString will be returned
		 * For a int or double, the value will be returned as a QString
		 *
		 * @return the value as a QString
		 */
		QString valueAsString() const;
		
		/**
		 * Every ChemicalDataObject contains one data. For example a
		 * integer value which represents the boiling point. This method
		 * returns the value as a QVariant.
		 *
		 * @return the value as a QVariant
		 */
		QVariant value() const;

		/**
		 * @return the error margin of the object
		 */
		QVariant errorValue() const;

		/**
		 * @return the type of dataset of this object
		 */
		BlueObelisk type() const;

		/**
		 * @param type the type of this object
		 */
		void setType( BlueObelisk type );

		/**
		 * @overload
		 */
		void setType( int type );
		
		/**
		 * Compare the value @p v with the data of this object
		 */
		bool operator== ( const int v ) const;
		
		/**
		 * Compare the value @p v with the data of this object
		 */
		bool operator== ( const double v ) const;
		
		/**
		 * Compare the value @p v with the data of this object
		 */
		bool operator== ( const bool v ) const;
		
		/**
		 * Compare the value @p v with the data of this object
		 */
		bool operator== ( const QString& v ) const;

		/**
		 * @return the dictRef attribute of the XML. This is an
		 * identifier. For example, for the mass it is "bo:mass"
		 */
		QString dictRef() const;

		/**
		 * @return the unit of the object as a QString. For example kelvin 
		 * will be returned as "bo:kelvin"
		 */
		QString unitAsString() const;

		/**
		 * @return the unit of the object
		 */
		BlueObeliskUnit unit() const;

		/**
		 * set the unit of this object to @p unit
		 * @param unit the BlueObeliskUnit for this object
		 */
		void setUnit( BlueObeliskUnit unit );

		/**
		 * Looks up a name @p unitname. The valid names are
		 * hard-coded in the C++ code, currently
		 *   - bo:kelvin
		 *   - bo:ev
		 *   - bo:nm
		 *   - bo:pm
		 *   - bo:y
		 *   - bo:s
		 *   - bo:noUnit
		 *
		 * @return the BlueObeliskUnit of a ChemicalDataObject 
		 *   corresponding to @p unitname, or noUnit if the name
		 *   doesn't match any of the known values.
		 * @param unitname the attribute-text of the XML parsed
		 */
		static BlueObeliskUnit unit( const QString& unitname );
		
	private:
		ChemicalDataObjectPrivate *d;
};

#endif // CHEMICALDATAOBJECT_H
