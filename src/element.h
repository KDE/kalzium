/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef ELEMENT_H
#define ELEMENT_H

#include <ksimpleconfig.h>
#include <kstandarddirs.h>

class Element;

typedef QValueList<Element*> EList;

/**
 * @short this class contains all Element-objects as
 * a QValueList<Element*>
 */
class KalziumDataObject
{
	public:
		KalziumDataObject();
		~KalziumDataObject();

		/**
		 * The list of element in a QValueList<Element*>
		 */
		EList ElementList;

};

/**
 * In this class all information available are stored. This means that
 * both the chemical date and the data about the position are stored
 * in this class.
 * @short This class is the represention of a chemical element
 * @author Carsten Niehaus
*/
class Element{
	public:
		Element( int );
		~Element();

		/**
		 * @return the number of the element
		 */
		int number() const {
			return m_number;
		}
		
		/**
		 * @return the date of discovery of the element
		 */
		int date() const { 
			return m_date; 
		}
		
		/**
		 * @return the importance of the element for biological
		 * systems.
		 * @li 0: blah
		 * @li 1: blub
		 * @li 2: blub
		 * @li 3: blub
		 * @li 4: blub
		 * @li 5: blub
		 * @li 6: blub
		 */
		int biological() const {
			return m_biological;
		}
		
		/**
		 * @return the condition of aggrgation of the element at 
		 * room temperature. 0 means solid, 1 means liquid, 2 means vapor
		 */
		int az() const {return m_az;
		}
		
		/**
		 * @return the symbol of the element
		 */
		QString symbol() const {
			return m_symbol;
		}
		/**
		 * @return the name of the element
		 */
		QString elname() const {
			return m_name;
		}
		/**
		 * @return the chemical block (s, p, d, f) of the element
		 */
		QString block() const {
			return m_block;
		}
		
		/**
		 * @return the group of the element
		 */
		QString group() const {
			return m_group;
		}
		
		/**
		 * @return the acidic behavior of the element
		 */
		QString acidicbeh() const {
			return m_acidbeh;
		}

		/**
		 * @return the isotopes of the element
		 */
		QString Isotopes() const {
			return m_isotopes;
		}
		
		/**
		 * @return the oxydationstages of the element
		 */
		
		QString oxstage() const {
			return m_oxstage;
		}
		
		/**
		 * @return the orbits of the element. The QString is already
		 * parsed so that the numbers are superscripts and the first
		 * block is bold.
		 */
		QString parsedOrbits();
		
		/**
		 * @return the boiling point of the element in kelvin
		 */
		double boiling() const {
			return m_BP;
		}
		
		/**
		 * @return the melting point of the element in kelvin
		 */
		double melting() const {
			return m_MP;
		}
		
		/**
		 * @return the electronegativity of the element in the 
		 * Pauling-scale
		 */
		double electroneg() const {
			return m_EN;
		}
		
		/**
		 * @return the atomic weight of the element in units
		 */
		double weight() const {
			return m_weight;
		}
		
		/**
		 * @return the density of the element in gramm per mol
		 */
		double density() const {
			return m_Density;
		}
		
		/**
		 * @return the radius of the element in picometers
		 */
		double radius() const {
			return m_AR;
		}
		
		/**
		 * @return the first ionizationenergie of the element
		 */
		double ie() const {
			return m_IE;
		}
		
		/**
		 * @return the first ionizationenergie of the element
		 */
		double ie2() const {
			return m_IE2;
		}
		
		/**
		 * @return the meanweight of the element
		 */
		double meanweight();

		int x, y; //for the RegularPSE
		int s_x, s_y; //for the SimplifiedPSE

		/**
		 * adjusts the units for the data. The user can
		 * eg define if Fahrenheit, kelvin or Degrees Celsius
		 * should be used for the temperature. This method
		 * takes care of that.
		 * @param val the value which has to be adjusted
		 * @param type the TYPE of the data
		 * @return the adjusted datastring
		 */
		const QString adjustUnits( const int type );

		/**
		 * missing
		 */
		enum TYPE
		{
			RADIUS = 0,
			WEIGHT,
			DENSITY,
			DATE,
			BOILINGPOINT,
			MELTINGPOINT,
			IE,
			IE2
		};





	private:
		void setupXY();
		

		double  m_weight,
			m_MP, 
			m_BP, 
			m_EN, 
			m_Density, 
			m_IE, 
			m_IE2, 
			m_AR;

		int     m_number, 
			m_date,
			m_az,
			m_biological;

		QString m_symbol,
			m_name,
			m_oxstage,
			m_block,
			m_group,
			m_acidbeh,
			m_orbits,
			m_isotopes;
};


#endif
