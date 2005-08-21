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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "tempunit.h"

double TempUnit::convert( const double value, TempUnit::Unit from, TempUnit::Unit to )
{
	switch( from )
	{
		case Kelvin:
		{
			if ( to == Celsius )
				return value - 273.16;
			if ( to == Fahrenheit )
				return value * 1.8 - 459.67;
			break;
		}
		case Celsius:
		{
			if ( to == Kelvin )
				return value + 273.16;
			if ( to == Fahrenheit )
				return ( value + 273.16 ) * 1.8 - 459.67;
			break;
		}
		case Fahrenheit:
		{
			if ( to == Kelvin )
				return ( value + 459.67 ) / 1.8;
			if ( to == Celsius )
				return ( value + 459.67 ) / 1.8 + 273.16;
			break;
		}
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
	}
	return QPair<double, double>( 0.0, 5000.0 );
}

QPair<double, double> TempUnit::rangeForUnit( int u )
{
	return rangeForUnit( (TempUnit::Unit)u );
}

