/***************************************************************************
 *   Copyright (C) 2003, 2004, 2005 by Carsten Niehaus                     *
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
#ifndef ELEMENT_H
#define ELEMENT_H

#include <QColor>
#include <QList>
#include <QVariant>

#include "chemicaldataobject.h"

class Spectrum;
class Isotope;
class Element;

typedef QList<Element*> EList;

/**
 * In this class all information about an element are stored. This means that
 * both the chemical date and the data about the position are stored
 * in this class.
 * @short This class is the represention of a chemical element
 * @author Carsten Niehaus
*/
class Element{
	public:
		Element();

		virtual ~Element();

		/**
		 * @returns a pointer to the istope with @p numberOfNucleons
		 * nucleons
		 */
		Isotope* isotopeByNucleons( int numberOfNucleons ) const;
		
		enum RADIUSTYPE
		{
			ATOMIC = 0,
			IONIC,
			VDW /**< van der Waals radius */,
			COVALENT
		};

		QList<Isotope*> isotopes() const{
			return m_isotopeList;
        }

		QList<double> spectrumList() const{
			return m_spectrumList;
		}

		void setIsotopeList( const QList<Isotope*> &list ){
			m_isotopeList = list;
		}

		/**
		 * sets the Spectrum of the Element
		 * @param spec the Spectrum of the Element
		 */
		void setSpectrum( Spectrum *spec ){
			m_spectrum = spec;
		}

		/**
		 * @return if the element has information about spectra
		 */
		bool hasSpectrum() const{
			return m_hasSpectrum;
		}

		/**
		 * define if the element has a known Spectrum
		 * @param value if true, the Element has a Spectrum
		 */
		void setHasSepctrum(bool value){
			m_hasSpectrum = value;
		}

		/**
		 * @return the Spectrum of the element
		 */
		Spectrum* spectrum() const{
			return m_spectrum;
		}

		/**
		 * adjusts the units for the data. The user can
		 * for example define if Fahrenheit, Kelvin or 
		 * Degrees Celsius should be used for the temperature. 
		 * This method takes care of that and adjust the 
		 * values.
		 * @param type the TYPE of the data
		 * @return the adjusted datastring
		 */
		const QString adjustUnits( const int type );

		const QString adjustRadius( RADIUSTYPE rtype );

		/**
		 * adjusts the units for the data. The user can
		 * for example define if Fahrenheit, Kelvin or 
		 * Degrees Celsius should be used for the temperature. 
		 * This method takes care of that and adjust the 
		 * values. Depending on @p type a unit will be
		 * added to the adjusted value.
		 * @param type the TYPE of the data
		 * @param value the value of the data. 
		 * @return the adjusted datastring
		 */
		const QString adjustUnits( const int type, double value );

		/**
		 * types of datas
		 */
		enum TYPE
		{
			NOGRADIENT = 0,
			ATOMICRADIUS,
			COVALENTRADIUS,
			VDWRADIUS,
			MASS,
			DENSITY,
			BOILINGPOINT,
			MELTINGPOINT,
			EN,
			EA,
			DATE,
			IE,
			IONICRADIUS
		};

		/**
		 * Sdd the ChemicalDataObject @p o to this Element
		 * @param o the ChemicalDataObject to be added
		 */
		void addData( ChemicalDataObject*o ){
			dataList.append( o );
		}

		/**
		 * Sdd a ChemicalDataObject with @p value of @p type to this
		 * Element
		 * @param value the QVariant to be added
		 * @param type the BlueObelisk type to be added
		 */
		void addData( const QVariant& value, ChemicalDataObject::BlueObelisk type ) {
			ChemicalDataObject* tmp = new ChemicalDataObject( value, type );
			dataList.append( tmp );
		}

		/**
		 * @return the ChemicalDataObject which stores the information
		 * of the type @p type
		 * @param type the type of the requested data
		 */
		ChemicalDataObject& data( ChemicalDataObject::BlueObelisk type ) const;

		/**
		 * @return the requested data of the type @p type as a QVariant
		 */
		QVariant dataAsVariant( ChemicalDataObject::BlueObelisk type ) const;
		
		/**
		 * @return the requested data of the type @p type as a QString
		 */
		QString dataAsString( ChemicalDataObject::BlueObelisk type ) const;
		
		/**
		 * @return the data of the Element
		 */
		QList<ChemicalDataObject*> data()const{
			return dataList;
		}
	private:
		/**
		 * this QList stores all information about an element
		 */
		QList<ChemicalDataObject*> dataList;

		/**
		 * the integer num represents the number of the element
		 */
		int m_ElementNumber;

		Spectrum *m_spectrum;

		bool m_hasSpectrum;

		QList<Isotope*> m_isotopeList;

		QList<double> m_spectrumList;
		
		QList<double> m_ionenergies;
};

#endif
