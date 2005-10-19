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
#include "spectrum.h"
#include "isotope.h"
#include "tempunit.h"
#include "chemicaldataobject.h"

#include <kdebug.h>
#include <klocale.h>
#include <kurl.h>
#include <kstandarddirs.h>

#include <QVariant>

Element::Element()
{
	m_radioactive = false;
	m_artificial = false;
	m_abundance = 0;
}

ChemicalDataObject& Element::data(ChemicalDataObject::BlueObelisk type)
{
	foreach( ChemicalDataObject*o, dataList ) {
		if ( o->type() == type )
			return *o;
	}
}

QVariant Element::dataAsVariant( ChemicalDataObject::BlueObelisk type )
{
	foreach( ChemicalDataObject* o, dataList ) {
		if ( o->type() == type )
			return o->value();
	}
}

QString Element::dataAsString(ChemicalDataObject::BlueObelisk type)
{
	foreach( ChemicalDataObject*o, dataList ) {
		if ( o->type() == type )
			return o->valueAsString();
	}
	return "";
}

Isotope* Element::isotopeByNucleons( int numberOfNucleons )
{
	QList<Isotope*>::ConstIterator it = m_isotopeList.begin();
	const QList<Isotope*>::ConstIterator itEnd = m_isotopeList.end();

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
		v = i18n( "%1 is a length, eg: 12.3 pm", "%1 pm" ).arg( QString::number( val ) );
	return v;
}

const QString Element::adjustUnits( const int type, double value )
{
	QString v;
	if ( type == IE  ) //an ionization energy
	{
//X 		if ( Prefs::energies() == 0 )
//X 		{
//X 			value*=96.6;
//X 			v = QString::number( value );
//X 			v.append( " kJ/mol" );
//X 		}
//X 		else // use electronvolt
//X 		{
//X 			v = QString::number( value );
//X 			v.append( " eV" );
//X 		}
	}
	return v;
}

const QString Element::adjustUnits( const int type )
{
	QString v = QString::null;

	double val = 0.0; //the value to convert
	
//X 	if ( type == BOILINGPOINT || type == MELTINGPOINT ) // convert a temperature
//X 	{
//X 		if ( type == BOILINGPOINT )
//X 			val = boiling();
//X 		else
//X 			val = melting();
//X 
//X 		if ( val <= 0 )
//X 			v = i18n( "Value unknown" );
//X 		else
//X 		{
//X 			double newvalue = TempUnit::convert( val, (int)TempUnit::Kelvin, Prefs::temperature() );
//X 			switch (Prefs::temperature()) {
//X 				case 0: //Kelvin
//X 					v = i18n( "%1 is the temperature in Kelvin", "%1 K" ).arg( newvalue );
//X 					break;
//X 				case 1://Kelvin to Celsius
//X 					v = i18n( "%1 is the temperature in Celsius", "%1 %2C" ).arg( newvalue ).arg( QChar(0xB0) );
//X 					break;
//X 				case 2: // Kelvin to Fahrenheit
//X 					v = i18n( "%1 is the temperature in Fahrenheit", "%1 %2F" ).arg( newvalue ).arg( QChar(0xB0) );
//X 					break;
//X 				case 3: // Kelvin to Rankine
//X 					v = i18n( "%1 is the temperature in Rankine", "%1 %2Ra" ).arg( newvalue ).arg( QChar(0xB0) );
//X 					break;
//X 				case 4: // Kelvin to Reamur
//X 					v = i18n( "%1 is the temperature in Reamur", "%1 %2R" ).arg( newvalue ).arg( QChar(0xB0) );
//X 					break;
//X 			}
//X 		}
//X 	}
//X 	else if ( type == EN ) //Electronegativity
//X 	{
//X 		val = electroneg();
//X 		if ( val <= 0 )
//X 		v = i18n( "Value not defined" );
//X 		else
//X 			v = QString::number( val );
//X 	}
//X 	else if ( type == EA ) //Electron affinity
//X 	{
//X 		val = electroaf();
//X 		if ( val == 0.0 )
//X 			v = i18n( "Value not defined" );
//X 		else
//X 		{
//X 			if ( Prefs::energies() == 0 )
//X 			{
//X 				v = i18n( "%1 kJ/mol" ).arg( QString::number( val ) );
//X 			}
//X 			else // use electronvolt
//X 			{
//X 				val/=96.6;
//X 				v = i18n( "%1 eV" ).arg( QString::number( val ) );
//X 			}
//X 		}
//X 	}
//X 	else if ( type == MASS ) // its a mass
//X 	{
//X 		val = mass();
//X 		if ( val <= 0 )
//X 			v = i18n( "Value unknown" );
//X 		else
//X 			v = i18n( "%1 u" ).arg( QString::number( val ) );
//X 	}
//X 	else if ( type == DENSITY ) // its a density
//X 	{
//X 		val = density();
//X 
//X 		if ( val <= 0 )
//X 			v = i18n( "Value unknown" );
//X 		else
//X 		{
//X 			if ( boiling() < 295.0 )//gasoline
//X 			{
//X 				v = i18n( "%1 g/L" ).arg( QString::number( val ) );
//X 			}
//X 			else//liquid or solid
//X 			{
//X 				v = i18n( "%1 g/cm<sup>3</sup>" ).arg( QString::number( val ) );
//X 			}
//X 		}
//X 	}
//X 	else if ( type == DATE ) //its a date
//X 	{
//X 		val = date();
//X 		if ( val < 1600 )
//X 		{
//X 			v = i18n( "This element was known to ancient cultures" );
//X 		}
//X 		else
//X 		{
//X 			v = i18n( "This element was discovered in the year %1" ).arg( QString::number( val ) );
//X 		}
//X 	}

	return v;
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

