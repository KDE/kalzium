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

#define ELEMENTSIZE 40

#include <qcolor.h>
#include <qvaluelist.h>

class Element;
class QDomDocument;
class QPainter;
class QPoint;

struct coordinate;

typedef QValueList<Element*> EList;
typedef QValueList<coordinate> CList;
typedef QValueList<double> doubleList;

class Isotope
{
	public:
		Isotope( int neutrons, double percentage, double weight, double halflife, QString format );

		bool seconds() {
			if ( m_format == "seconds" )
				return true;
			else
				return false;
		}

		double halflife() const{
			return m_halflife;
		}
		
		double percentage() const{
			return m_percentage;
		}

		int neutrons() const{
			return m_neutrons;
		}

		double weight() const{
			return m_weight;
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
};

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
		
		CList CoordinateList;

		Element* element( int number );

		/**
		 * @return the number of elements we have
		 */
		const int numberOfElements() const;

	private:
		EList readData( QDomDocument &dataDocument );

		// caching the number of elements
		int m_numOfElements;
};

struct coordinate{
	int x;
	int y;
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
		Element();

		virtual ~Element();
		
		enum RADIUSTYPE
		{
			ATOMIC = 0,
			IONIC,
			VDW, //van der Waals radius
			COVALENT
		};


		/**
		 * @return the number of the element
		 */
		int number() const {
			return m_number;
		}

		bool radioactive() const{
			return m_radioactive;
		}
		
		bool artificial() const{
			return m_artificial;
		}

		QString nameOrigin() const{
			return m_origin;
		}
		
		void setMass( double value ) { m_mass = value; }
		void setEN( double value ) { m_EN = value; }
		void setEA( double value ) { m_EA = value; }
		void setMeltingpoint( double value ) { m_MP = value; }
		void setBoilingpoint( double value ) { m_BP = value; }
		void setDensity( double value ) { m_Density = value; }

		/**
		 * set the radius of the radiustype @p type to the value @p value.
		 * The ionicradius also has a name @p name. This will store the charge of
		 * the ion (for example, +2 or -3 )
		 */
		void setRadius( RADIUSTYPE type, double value, const QString& name = 0 );
		
		void setDate( int date ) { m_date = date; }
		void setBiologicalMeaning( int value ) { m_biological = value; }
		void setNumber( int num ){ m_number = num; }

		void setScientist( const QString& value ) { m_scientist = value; }
		void setCrysatalstructure( const QString& value ) { m_crystalstructure = value; }
		void setName( const QString& value ) { m_name = value; }
		void setOrigin( const QString& value ) { m_origin = value; }
		void setBlock( const QString& value ) { m_block = value; }
		void setGroup( const QString& value ) { m_group = value; }
		void setFamily( const QString& value ) { m_family = value; }
		void setOrbits( const QString& value ) { m_orbits = value; }
		void setSymbol( const QString& value ) { m_symbol = value; }
		void setOxydation( const QString& value ) { m_oxstage = value; }
		void setAcidicbehaviour( const QString& value ) { m_acidbeh = value; }
		void setIsotopes( const QString& value ) { m_isotopes = value; }

		void setArtificial(){ m_artificial = true; }
		void setRadioactive(){ m_radioactive = true; }

		void setIonisationList( doubleList l ){
			m_ionenergies = l;
		}

		QValueList<Isotope*> isotopeList()const{
			return m_isotopeList;
		}

		void setIsotopeList( QValueList<Isotope*> list ){
			m_isotopeList = list;
		}

		doubleList ionisationList() const{
			return m_ionenergies;
		}
		
		
		/**
		 * @return the date of discovery of the element
		 */
		int date() const { 
			return m_date; 
		}

		/**
		 * return the correct color of the element at the
		 * temperature @p temp
		 */
		QColor currentColor( const double temp );
    
		/**
		 * mutator for the element's color
		 */
		void setElementColor( const QColor &c ) { m_Color = c; }
		
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
		 * @return the symbol of the element
		 */
		QString symbol() const {
			return m_symbol;
		}

		/**
		 * @return the scientist who discovered the element
		 */
		QString scientist() const{
			return m_scientist;
		}

		/**
		 * @return the crystalstructure of the element
		 */
		QString crystalstructure() const{
			return m_crystalstructure;
		}
		
		/**
		 * @return the name of the element
		 */
		QString elname() const {
			return m_name;
		}

		//FIXME I need to add a way to have more than one ionic radius
		QString ioncharge() const{ 
			return m_ionvalue; 
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

		QString family() const {
			return m_family;
		}
		
		/**
		 * @return the acidic behavior of the element
		 */
		QString acidicbeh() const {
			return m_acidbeh;
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
		 * @param canBeEmpty specifies if the string returned can be
		 * empty instead of a "Unknown structure" one.
		 */
		QString parsedOrbits( bool canBeEmpty = false );
		
		/**
		 * @return the boiling point of the element in Kelvin
		 */
		double boiling() const {
			return m_BP;
		}
		
		/**
		 * @return the melting point of the element in Kelvin
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
		* @return the electroaffinity of the element
		*/
		double electroaf() const {
			return m_EA;
		}

		/**
		 * @return the atomic mass of the element in units
		 */
		double mass() const {
			return m_mass;
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
		double radius( RADIUSTYPE type );
		
		/**
		 * @return the meanmass of the element
		 */
		double meanmass();

		int x, y; //for the RegularPSE

		/**
		 * adjusts the units for the data. The user can
		 * eg define if Fahrenheit, Kelvin or Degrees Celsius
		 * should be used for the temperature. This method
		 * takes care of that.
		 * @param val the value which has to be adjusted
		 * @param type the TYPE of the data
		 * @return the adjusted datastring
		 */
		const QString adjustUnits( const int type );

		const QString adjustRadius( RADIUSTYPE rtype );


		/**
		 * needed for values stored in a QValueList<double>
		 */
		const QString adjustUnits( const int type, double value );

		/**
		 * missing
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
		 * calculate the 4-digit value of the value @p w
		 * @return the 4-digit value
		 */
		static double strippedValue( double w );

		QPoint pos() const;
		QPoint coords() const;

    /**
     * accessor for the element's color
     */
    QColor elementColor() const { 
		return m_Color; 
	}

	QValueList<Isotope*> isotopes() const{
		return m_isotopeList;
	}
		
	void setupXY();

	private:
		/*
		 * the integer num represents the number of the element
		 */
		int m_ElementNumber;

		QValueList<Isotope*> m_isotopeList;
    	
		QColor m_Color;

		int xPos() const;
		int yPos() const;

		double  m_mass,
			m_MP, 
			m_BP, 
			m_EN,
			m_EA, 
			m_Density,
			m_RadiusAR,
			m_RadiusCR,
			m_RadiusVDW,
			m_RadiusIon;
		

		int     m_number, 
			m_date,
			m_biological;

		QString m_symbol,
			m_name,
			m_origin,
			m_oxstage,
			m_block,
			m_group,
			m_family,
			m_acidbeh,
			m_orbits,
			m_isotopes,
			m_scientist,
			m_crystalstructure,
			m_ionvalue;

		bool m_artificial,
			 m_radioactive;

		doubleList m_ionenergies;
		
	public:
		/**
		 * draw the recatangle with the information
		 * @param showFullInformation if True more information will be shown
		 */
		virtual void drawGradient( QPainter* p, const QString& value, const QColor& );
		
		virtual void drawSelf( QPainter* p, bool simple = false );

		virtual void drawCrystalstructure( QPainter* p );
		
		virtual void drawStateOfMatter( QPainter* p, double temperature );
};


#endif
