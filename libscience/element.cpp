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

Element::~Element()
{
}

const QString Element::adjustRadius( RADIUSTYPE rtype )
{
//X 	double val = 0.0;
//X 	QString v;
//X 
//X 	switch ( rtype )
//X 	{
//X 		case ATOMIC:
//X 			val = m_RadiusAR;
//X 			break;
//X 		case IONIC:
//X 			val = m_RadiusIon;
//X 			break;
//X 		case COVALENT:
//X 			val = m_RadiusCR;
//X 			break;
//X 		case VDW:
//X 			val = m_RadiusVDW;
//X 			break;
//X 	}
//X 
//X 	if ( val <= 0 )
//X 		v = i18n( "Value unknown" );
//X 	else
//X 		v = i18n( "%1 is a length, eg: 12.3 pm", "%1 pm" ).arg( QString::number( val ) );
//X 	return v;
}

const QString Element::adjustUnits( const int type, double value )
{
//X 	QString v;
//X 	if ( type == IE  ) //an ionization energy
//X 	{
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
//X 	}
//X 	return v;
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

