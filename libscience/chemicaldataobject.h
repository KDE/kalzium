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

/**
 * A ChemicalDataObject is an object which contains information about 
 * a chemical element. This can for example be a boiling point. The information
 * is stored in a QVariant.
 * This class supports the CML-format defined by the BlueObelisk-Project.
 *
 * @author Carsten Niehaus <cniehaus@kde.org>
 */
class ChemicalDataObject
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
			nameOrigin/**< the origin of the name */,
			orbit/**< the quantumorbit of the element */,
			period/**< the period of the element */,
			date/**< date of discovery of the element. When 0, the element has been known in ancient times. */,
			discoverer/** The name of the discoverer(s) */
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
		 */
		ChemicalDataObject(QVariant v, 
				BlueObelisk type);

		/**
		 * Set the data of this object to @p v
		 * @param v the value of the object
		 */
		void setData( QVariant v ){
			m_value = v;
		}
		
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
		QString valueAsString();
		
		/**
		 * Every ChemicalDataObject contains one data. For example a
		 * integer value which represents the boiling point. This method
		 * returns the value as a QVariant.
		 *
		 * @return the value as a QVariant
		 */
		QVariant value() const{
			return m_value;
		}

		/**
		 * @return the type of dataset of this object
		 */
		BlueObelisk type() const{
			return m_type;
		}

		/**
		 * @param type the type of this object
		 */
		void setType( BlueObelisk type ){
			m_type = type;
		}

		/**
		 * @overload
		 */
		void setType( int type ){
			m_type = ( BlueObelisk ) type;
		}
		
		/**
		 * Compare the value @p v with the data of this object
		 */
		bool operator== ( const int v );
		
		/**
		 * Compare the value @p v with the data of this object
		 */
		bool operator== ( const double v );
		
		/**
		 * Compare the value @p v with the data of this object
		 */
		bool operator== ( const bool v );
		
		/**
		 * Compare the value @p v with the data of this object
		 */
		bool operator== ( const QString& v );

		/**
		 * @return the dictRef attribute of the XML. This is an
		 * identifier. For example, for the mass it is "bo:mass"
		 */
		QString dictRef();

		/**
		 * @return the unit of the object as a QString. For example kelvin 
		 * will be returned as "bo:kelvin"
		 */
		QString unitAsString();

		/**
		 * @return the unit of the object
		 */
		BlueObeliskUnit unit() const{
			return m_unit;
		}

		/**
		 * set the unit of this object to @p unit
		 * @param the BlueObeliskUnit of this object
		 */
		void setUnit( BlueObeliskUnit unit ){
			m_unit = unit;
		}
		
	private:
		QVariant m_value;
		BlueObelisk m_type;
		BlueObeliskUnit m_unit;
};

#endif // CHEMICALDATAOBJECT_H
