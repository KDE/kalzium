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

#define ELEMENTSIZE 40

#include <qcolor.h>
#include <qvaluelist.h>

class Element;
class QDomDocument;
class QPainter;
class QPoint;
class QFont;
class QRect;
class Spectrum;
class Isotope;

struct coordinate;

typedef QValueList<Element*> EList;
typedef QValueList<coordinate> CList;
typedef QValueList<double> doubleList;

struct coordinate{
	int x;
	int y;
};

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
		Isotope* isotopeByNucleons( int numberOfNucleons );
		
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

		/**
		 * @return if the Element is radioactive
		 */
		bool radioactive() const{
			return m_radioactive;
		}
		
		/**
		 * @return if the Element is artificial
		 */
		bool artificial() const{
			return m_artificial;
		}

		/**
		 * @return the information where the name of the Element comes from
		 */
		QString nameOrigin() const{
			return m_origin;
		}

		QString orbits() const{
			return m_orbits;
		}
		
		void setMass( double value ) { m_mass = value; }
		void setEN( double value ) { m_EN = value; }
		void setEA( double value ) { m_EA = value; }
		void setMeltingpoint( double value ) { m_MP = value; }
		void setBoilingpoint( double value ) { m_BP = value; }

		/**
		 * sets the density of the Element
		 * @param value the density of the Element
		 */
		void setDensity( double value ) { m_Density = value; }

		/**
		 * set the radius of the radiustype @p type to the value @p value.
		 * The ionicradius also has a name @p name. This will store the charge of
		 * the ion (for example, +2 or -3 )
		 */
		void setRadius( RADIUSTYPE type, double value, const QString& name = 0 );
		
		void setDate( int date ) { m_date = date; }
		void setPeriod( int period ){ m_period = period; }
		void setBiologicalMeaning( int value ) { m_biological = value; }
		void setNumber( int num ){ m_number = num; }

		/**
		 * set the abundance in crustal rocks [pm]
		 * @param abundance the abundace in crustal rocks
		 */
		void setAbundance( int abundance ){ m_abundance = abundance; }

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
		
		void setIonisationList( doubleList l ){ m_ionenergies = l; }

		QValueList<Isotope*> isotopes() const{
			return m_isotopeList;
        }

		QValueList<double> spectrumList() const{
			return m_spectrumList;
		}

		void setIsotopeList( QValueList<Isotope*> list ){
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
		 * @return the abundance in crustal rocks in parts per million
		 */
		int abundance() const {
			return m_abundance;
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
		 * @return the crystal structure of the element
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
		
		int period() const {
			return m_period;
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
		 * @return the density of the element in gramms per mol
		 */
		double density() const {
			return m_Density;
		}
		
		/**
		 * @return the radius of the element in picometers
		 */
		double radius( RADIUSTYPE type );
		
		/**
		 * @return the mean mass of the element
		 */
		double meanmass();

		int x, y; //for the RegularPerodicTableView

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

		QPoint pos() const;
		QPoint coords() const;

		/**
		 * accessor for the element's color
		 */
		QColor elementColor() const {
			return m_Color; 
		}

		void setupXY();

	private:
		/**
		 * the integer num represents the number of the element
		 */
		int m_ElementNumber;

		Spectrum *m_spectrum;

		bool m_hasSpectrum;

		QValueList<Isotope*> m_isotopeList;

		QValueList<double> m_spectrumList;
    	
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
			m_biological,
			m_period,
			m_abundance;

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
		 * draw the rectangle with the information
		 * @param p painter to do the drawing on
		 * @param value the value to display as text
		 * @param c the color used to paint the element
		 */
		virtual void drawGradient( QPainter* p, const QString& value, const QColor& c);
		
		/**
		 * Draw the Element grayed out. Used in the timeline
		 * @param p the painter used for the painting
		 */
		virtual void drawGrayedOut( QPainter* p );
		
		/**
		 * draw the rectangle with the information
		 * @param p painter to do the drawing on
		 * @param simple if True more information will be shown
		 * @param isCrystal whether the elements should draw its crystal structure
		 */
		virtual void drawSelf( QPainter* p, bool simple = false, bool isCrystal = false );

		virtual void drawStateOfMatter( QPainter* p, double temperature );
};


#endif
