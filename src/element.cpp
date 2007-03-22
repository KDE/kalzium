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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#include "element.h"
#include "prefs.h"
#include "spectrum.h"
#include "isotope.h"
#include "kalziumdataobject.h"
#include "kalziumutils.h"
#include "tempunit.h"

#include <qdom.h>
#include <qfile.h>
#include <qpainter.h>
#include <qregexp.h>
#include <qfontmetrics.h>

#include <kdebug.h>
#include <klocale.h>
#include <kurl.h>
#include <kstandarddirs.h>

Element::Element()
{
	m_radioactive = false;
	m_artificial = false;
	m_abundance = 0;
}

Isotope* Element::isotopeByNucleons( int numberOfNucleons )
{
	QValueList<Isotope*>::ConstIterator it = m_isotopeList.begin();
	const QValueList<Isotope*>::ConstIterator itEnd = m_isotopeList.end();

	for ( ; it != itEnd; ++it )
	{
		if ( ( ( *it )->neutrons() + ( *it )->protones() ) == numberOfNucleons )
			return *it;
	}
	return 0;
}

QString Element::parsedOrbits( bool canBeEmpty )
{
	if ( m_orbits.isEmpty() )
		if ( !canBeEmpty )
			return i18n( "structure means orbital configuration in this case", "Unknown structure" );
		else
			return "";
	
	QString orbits = m_orbits;
	QRegExp rxs("([a-z])([0-9]+)");
	QRegExp rxb("([a-z]{2}) ",false);
	orbits.replace(rxs,"\\1<sup>\\2</sup>"); //superscript around electron number
	orbits.replace(rxb,"<b>\\1</b> "); //bold around element symbols
	return orbits;
}

Element::~Element()
{
}

double Element::meanmass()
{
	return m_mass/m_number;
}

const QString Element::adjustRadius( RADIUSTYPE rtype )
{
	double val = 0.0;
	QString v;

	switch ( rtype )
	{
		case ATOMIC:
			val = m_RadiusAR;
			break;
		case IONIC:
			val = m_RadiusIon;
			break;
		case COVALENT:
			val = m_RadiusCR;
			break;
		case VDW:
			val = m_RadiusVDW;
			break;
	}

	if ( val <= 0 )
		v = i18n( "Value unknown" );
	else
		v = i18n( "%1 is a length, eg: 12.3 pm", "%1 pm" ).arg( KalziumUtils::localizedValue( val, 6 ) );
	return v;
}

const QString Element::adjustUnits( const int type, double value )
{
	QString v;
	if ( type == IE  ) //an ionization energy
	{
		if ( Prefs::energies() == 0 )
		{
			value*=96.6;
			v = KalziumUtils::localizedValue( value, 6 );
			v.append( " kJ/mol" );
		}
		else // use electronvolt
		{
			v = KalziumUtils::localizedValue( value, 6 );
			v.append( " eV" );
		}
	}
	return v;
}

const QString Element::adjustUnits( const int type )
{
	QString v = QString::null;

	double val = 0.0; //the value to convert
	
	if ( type == BOILINGPOINT || type == MELTINGPOINT ) // convert a temperature
	{
		if ( type == BOILINGPOINT )
			val = boiling();
		else
			val = melting();

		if ( val <= 0 )
			v = i18n( "Value unknown" );
		else
		{
			double newvalue = TempUnit::convert( val, (int)TempUnit::Kelvin, Prefs::temperature() );
			QString strVal = KalziumUtils::localizedValue( newvalue, 6 );
			switch (Prefs::temperature()) {
				case 0: //Kelvin
					v = i18n( "%1 is the temperature in Kelvin", "%1 K" ).arg( strVal );
					break;
				case 1://Kelvin to Celsius
					v = i18n( "%1 is the temperature in Celsius", "%1 %2C" ).arg( strVal ).arg( QChar(0xB0) );
					break;
				case 2: // Kelvin to Fahrenheit
					v = i18n( "%1 is the temperature in Fahrenheit", "%1 %2F" ).arg( strVal ).arg( QChar(0xB0) );
					break;
				case 3: // Kelvin to Rankine
					v = i18n( "%1 is the temperature in Rankine", "%1 %2Ra" ).arg( strVal ).arg( QChar(0xB0) );
					break;
				case 4: // Kelvin to Reaumur
					v = i18n( "%1 is the temperature in Reaumur", "%1 %2R" ).arg( strVal ).arg( QChar(0xB0) );
					break;
			}
		}
	}
	else if ( type == EN ) //Electronegativity
	{
		val = electroneg();
		if ( val <= 0 )
			v = i18n( "Value not defined" );
		else {
			v = KalziumUtils::localizedValue( val, 6 );
		}
	}
	else if ( type == EA ) //Electron affinity
	{
		val = electroaf();
		if ( val == 0.0 )
			v = i18n( "Value not defined" );
		else
		{
			if ( Prefs::energies() == 0 )
			{
				v = i18n( "%1 kJ/mol" ).arg( KalziumUtils::localizedValue( val, 6 ) );
			}
			else // use electronvolt
			{
				val/=96.6;
				v = i18n( "%1 eV" ).arg( KalziumUtils::localizedValue( val, 6 ) );
			}
		}
	}
	else if ( type == MASS ) // its a mass
	{
		val = mass();
		if ( val <= 0 )
			v = i18n( "Value unknown" );
		else
			v = i18n( "%1 u" ).arg( KalziumUtils::localizedValue( val, 6 ) );
	}
	else if ( type == DENSITY ) // its a density
	{
		val = density();

		if ( val <= 0 )
			v = i18n( "Value unknown" );
		else
		{
			if ( boiling() < 295.0 && melting() < 295.0)//gasoline
			{
				v = i18n( "%1 g/L" ).arg( KalziumUtils::localizedValue( val, 6 ) );
			}
			else//liquid or solid
			{
				v = i18n( "%1 g/cm<sup>3</sup>" ).arg( KalziumUtils::localizedValue( val, 6 ));
			}
		}
	}
	else if ( type == DATE ) //its a date
	{
		val = date();
		if ( val < 1600 )
		{
			v = i18n( "This element was known to ancient cultures" );
		}
		else
		{
			v = i18n( "This element was discovered in the year %1" ).arg( QString::number( val ) );
		}
	}

	return v;
}

void Element::drawStateOfMatter( QPainter* p, double temp )
{
	//the height of a "line" inside an element
	int h_small = 15; //the size for the small units like elementnumber

	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();
	
	QColor color = currentColor( temp );
	
	p->setPen( color );
	p->fillRect( X, Y,ELEMENTSIZE,ELEMENTSIZE, color );
	
	QString text;
	QFont symbol_font = p->font();

	symbol_font.setPointSize( 10 );
	QFont f = p->font();
	f.setPointSize( 9 );
		
	p->setFont( f );

	//top left
	p->setPen( Qt::black );
	text = KalziumUtils::localizedValue( KalziumUtils::strippedValue( mass( ) ), 6 );
	p->drawText( X,Y ,ELEMENTSIZE,h_small,Qt::AlignCenter, text );

	text = QString::number( number() );
	p->drawText( X,Y+ELEMENTSIZE-h_small , ELEMENTSIZE, h_small,Qt::AlignCenter, text );

	p->setFont( symbol_font );
	p->drawText( X,Y, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignCenter, symbol() );
	
	//border
	p->setPen( Qt::black );
	p->drawRect( X, Y,ELEMENTSIZE+1,ELEMENTSIZE+1);
}

QColor Element::currentColor( const double temp )
{
	QColor color;
	
	//take the colours for the given temperature
	const double iButton_melting = melting();
	const double iButton_boiling = boiling();

	//If either the mp or bp is not known return
	//This is to avoid undefined behaviour
//	if ( iButton_boiling <= 0.0 || iButton_melting <= 0.0 )
//		return Qt::lightGray;

	if ( temp < iButton_melting )
	{ //the element is solid
		color= Prefs::color_solid();
	}
	else if ( temp > iButton_melting &&
			temp < iButton_boiling ) 
	{ //the element is liquid
		color= Prefs::color_liquid();
	}
	else if ( temp > iButton_boiling && iButton_boiling > 0.0 )
	{ //the element is vaporous
		color= Prefs::color_vapor();
	}
	else
		color = Qt::lightGray;

	return color;
}

	
void Element::drawGradient( QPainter* p, const QString& value, const QColor& c)
{
	//Set the elementColor to c to make the overviewwidget show
	//the correct color
	setElementColor( c );
	
	//the height of a "line" inside an element
	int h_small = 10; //the size for the small units like elementnumber

	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();

	p->setPen( c );
	p->fillRect( X, Y,ELEMENTSIZE,ELEMENTSIZE, c );
	
	p->setPen( Qt::black );
	QFont symbol_font = p->font();
	QFont f = p->font();

	f.setPointSize( KalziumUtils::maxSize(value, QRect( X,Y+ELEMENTSIZE-h_small, ELEMENTSIZE, h_small ),f, p ) );
	p->setFont( f );

	p->drawText( X,Y+ELEMENTSIZE-h_small , ELEMENTSIZE, h_small,Qt::AlignCenter, value );

	const QRect rect = QRect( X,Y,ELEMENTSIZE-2,ELEMENTSIZE-10 );
	int goodsize = KalziumUtils::maxSize( symbol(), rect, symbol_font, p );
	symbol_font.setPointSize( goodsize );
	p->setFont( symbol_font );
	p->drawText( X+1,Y+5, ELEMENTSIZE-2,ELEMENTSIZE-10,Qt::AlignCenter, symbol() );
	
	//border
	p->setPen( Qt::black );
	p->drawRect( X, Y,ELEMENTSIZE+1,ELEMENTSIZE+1);
}

void Element::drawGrayedOut( QPainter *p )
{ 
	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();
	
	p->fillRect( X, Y,ELEMENTSIZE,ELEMENTSIZE, Qt::lightGray );
	
	p->setPen( Qt::darkGray );
	
	QFont symbol_font = p->font();
	const QRect rect = QRect( X,Y,ELEMENTSIZE-2,ELEMENTSIZE-10 );
	int goodsize = KalziumUtils::maxSize( symbol(), rect, symbol_font, p );
	symbol_font.setPointSize( goodsize );
	p->setFont( symbol_font );
	p->drawText( X+1,Y+5, ELEMENTSIZE-2,ELEMENTSIZE-10,Qt::AlignCenter, symbol() );

	p->setPen( Qt::black );
	p->drawRect( X, Y,ELEMENTSIZE+1,ELEMENTSIZE+1);
}

void Element::drawSelf( QPainter* p, bool simple, bool isCrystal )
{
	//the height of a "line" inside an element
	int h_small = 12; //the size for the small units like elementnumber

	//The X-coordiante
	int X = xPos();
	
	//The Y-coordinate
	int Y = yPos();

	p->setPen( elementColor() );
	p->fillRect( X, Y,ELEMENTSIZE,ELEMENTSIZE, elementColor() );
	p->setPen( Qt::black );
	
	QString text;
	QFont symbol_font = p->font();
	
	const int max = ELEMENTSIZE-10;
	
	const QRect rect = QRect( X,Y,ELEMENTSIZE-2,max );

	int goodsize = KalziumUtils::maxSize( symbol(), rect, symbol_font, p );
	symbol_font.setPointSize( goodsize );
	p->setFont( symbol_font );
	
	if ( !simple )
		p->drawText( X+1,Y+5, ELEMENTSIZE-2,max,Qt::AlignCenter, symbol() );
	else
		p->drawText( X+1,Y+5, ELEMENTSIZE-2,max,Qt::AlignHCenter, symbol() );
	
	QFont f = p->font();

	QRect smallRect( X,Y ,ELEMENTSIZE-4,h_small );
	f.setPointSize( KalziumUtils::maxSize( QString::number( number() ), smallRect, f, p ) );
	
	p->setFont( f );

	if ( !simple )
	{//the user only wants a simple periodic table, don't weight the cell
		QString text;
		if ( isCrystal )
		{
			QString structure = crystalstructure();
			/**
			 * hcp: hexagonal close packed
			 * fcc: face centered cubic
			 * krz/bcc body centered cubic// kubisch raumzentriert
			 * kdp: kubisch dicht gepackt
			 * hdp: hexagonal dicht gepackt
			 * ccp: cubic close packed // kubisch dichteste Kugelpackung
			 * d  : diamond
			 * sc : simple cubic
			 * tet: tetragonal
			 * rh : rhombohedral
			 * or : orthorhombic
			 * mono: monoclinic
			 */
			if ( structure == "own")
				text = i18n( "this means, the element has its 'own' structur", "own" );
			else if ( structure == "bcc" )
				text = i18n( "Crystalsystem body centered cubic", "bcc" );
			else if ( structure == "hdp" )
				text = i18n( "Crystalsystem hexagonal dense packed", "hdp" );
			else if ( structure == "ccp" )
				text = i18n( "Crystalsystem cubic close packed", "ccp" );
		}
		else
			text = KalziumUtils::localizedValue( KalziumUtils::strippedValue( mass( ) ), 6 );
		p->drawText( X+2,Y ,ELEMENTSIZE-4 ,h_small,Qt::AlignCenter, text );
	}
	
	text = QString::number( number() );
	p->drawText( X+2,Y+ELEMENTSIZE-h_small , ELEMENTSIZE-4, h_small,Qt::AlignCenter, text );
	
	p->drawRect( X, Y,ELEMENTSIZE+1,ELEMENTSIZE+1);
}

/*!
    This looks pretty evil and it is. The problem is that a PerodicTableView is pretty
    irregular and you cannot "calculate" the position. 
 */
void Element::setupXY()
{
 static const int posXRegular[111] = {1,18,
	 								1,2,13,14,15,16,17,18,
									1,2,13,14,15,16,17,18,
 									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
 									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,  //Element 54 (Xe)
									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,     //Element 71 (Lu)
									      4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,     //Element 71 (Lr)
									      4,5,6,7,8,9,10,11};
 static const int posYRegular[111] = {1,1,
	 								2,2, 2, 2, 2, 2, 2, 2,
									3,3, 3, 3, 3, 3, 3, 3,
 									4,4,4,4,4,4,4,4,4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 									5,5,5,5,5,5,5,5,5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  //Element 54 (Xe)
						    		6,6,6,8,8,8,8,8,8, 8, 8, 8, 8, 8, 8, 8, 8,     //Element 71 (Lr)
									      6,6,6,6,6,6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
									7,7,7,9,9,9,9,9,9, 9, 9, 9, 9, 9, 9, 9, 9,
									      7,7,7,7,7,7,7,7};

 x = posXRegular[m_number-1];
 y = posYRegular[m_number-1];
}

void Element::setRadius( RADIUSTYPE type, double value, const QString& name )
{
	switch ( type )
	{
		case ATOMIC:
			m_RadiusAR = value;
			break;
		case IONIC:
			m_RadiusIon = value;
			m_ionvalue = name;
			break;
		case COVALENT:
			m_RadiusCR = value;
			break;
		case VDW:
			m_RadiusVDW = value;
			break;
	}
}

double Element::radius( RADIUSTYPE type )
{
	switch ( type )
	{
		case ATOMIC:
			return m_RadiusAR;
			break;
		case IONIC:
			return m_RadiusIon;
			break;
		case COVALENT:
			return m_RadiusCR;
			break;
		case VDW:
			return m_RadiusVDW;
			break;
	}
	return 0.0;
}

int Element::xPos() const
{
	return ( x-1 )*ELEMENTSIZE;
}

int Element::yPos() const
{
	// mind the small gap over rare earth!
	int tmp_y = ( y-1 )*ELEMENTSIZE + ELEMENTSIZE;

	// 57=Lanthanum, 72=Hafnium, 89=Actinium & 104=Rutherfordium (i.e., if
	// n_number is in rare earth's block)
	if ( (m_number > 57 && m_number < 72) || (m_number > 89 && m_number < 104) )
		tmp_y += ELEMENTSIZE/3;

	return tmp_y;
}

QPoint Element::pos() const
{
	return QPoint( xPos(), yPos() );
}

QPoint Element::coords() const
{
	return QPoint( x, y );
}
