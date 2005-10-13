/***************************************************************************
 *   Copyright (C) 2005 by Pino Toscano                                    *
 *   toscano.pino@tiscali.it                                               *
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

#include "tempunit.h"

double TempUnit::convert( double value, TempUnit::Unit from, TempUnit::Unit to )
{
	/**
	 * These are the four formula needed to convert from
	 * Kelvin the the other units:

	 Kelvin  Celsius  °C = K - 273,15
	 Kelvin  Fahrenheit  °F = K × 1,8 - 459,67
	 Kelvin  Rankine  °Ra = K × 1,8
	 Kelvin  Reaumur  °R = ( K - 273,15 ) × 0,8
	 */

	if ( from != Kelvin )
		value = convertToKelvin( value, from );

	//now 'value' is in Kelvin
	switch( to )
	{
		case Celsius:
			return value - 273.16;
		case Fahrenheit:
			return value * 1.8 - 459.67;
		case Rankine:
			return value * 1.8;
		case Reaumur:
			return ( value - 273.15 )* 0.8;
		case Kelvin:
			return value;
	}
	return value;
}

double TempUnit::convertToKelvin( const double value, TempUnit::Unit from )
{
	switch( from )
	{
		case Kelvin:
			return value;
		case Celsius:
			return value + 273.16;
		case Fahrenheit:
			return ( value + 459.67 )/1.8;
		case Rankine:
			return value / 1.8;
		case Reaumur:
			return ( value * 1.25 ) + 273.15;
	}
	return value;
}

double TempUnit::convert( const double value, int from, int to )
{
	return convert( value, (TempUnit::Unit)from, (TempUnit::Unit)to );
}

QStringList TempUnit::unitListSymbols()
{
	QStringList list;
	list << "K";
	list << QString::fromUtf8("°C");
	list << QString::fromUtf8("°F");
	list << QString::fromUtf8("°Ra");
	list << QString::fromUtf8("°R");
	return list;
}

QString TempUnit::unitListSymbol( int i )
{
	QStringList l = unitListSymbols();
	if ( ( i < 0 ) || ( i >= (int)l.count() ) )
		return l[0];
	return l[i];
}

QPair<double, double> TempUnit::rangeForUnit( TempUnit::Unit u )
{
	switch( u )
	{
		case Kelvin:
		{
			return QPair<double, double>( 0.0, 5000.0 );
			break;
		}
		case Celsius:
		{
			return QPair<double, double>( -273.16, 4726.84 );
			break;
		}
		case Fahrenheit:
		{
			return QPair<double, double>( -459.67, 8540.33 );
			break;
		}
		case Rankine:
		{
			return QPair<double, double>( 0.0, 9000.0 );
			break;
		}
		case Reaumur:
		{
			return QPair<double, double>( -218.52, 3781.48 );
			break;
		}
	}
	return QPair<double, double>( 0.0, 5000.0 );
}

QPair<double, double> TempUnit::rangeForUnit( int u )
{
	return rangeForUnit( (TempUnit::Unit)u );
}

