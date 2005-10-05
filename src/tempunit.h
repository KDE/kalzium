#ifndef TEMPUNIT_H
#define TEMPUNIT_H
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

#include <qpair.h>
#include <qstringlist.h>

/**
 * This class obtains all the info needed to work with the units of measure
 * of temperature.
 *
 * It has static methods you can call to get the various infos. The most
 * important methods is convert(), that performs conversions.
 *
 * @author Pino Toscano
 */
class TempUnit
{
	public:

		enum Unit { Kelvin = 0, 
			Celsius, 
			Fahrenheit,
			Rankine,
			Reaumur 
		};

		/**
		 * Convert the temperature unit @p value given in the unit
		 * @p from to the unit @p to.
		 *
		 * @return the value converted
		 */
		static double convert( double value,
		                       TempUnit::Unit from,
		                       TempUnit::Unit to );


		/**
		 * @param value the temperature to convert
		 * @param from the unit from which to convert
		 *
		 * @return the temperature to Kelvin
		 */
		static double convertToKelvin( const double value,
				TempUnit::Unit from );
		
		/**
		 * @overload
		 */
		static double convert( const double value,
		                       int from,
		                       int to );

		/**
		 * @return a list with the symbols of the temperature units
		 * we support
		 */
		static QStringList unitListSymbols();

		/**
		 * @return the symbol of the @p i -th unit 
		 */
		static QString unitListSymbol( int i );

		static QPair<double, double> rangeForUnit( TempUnit::Unit u );

		/**
		 * @overload
		 */
		static QPair<double, double> rangeForUnit( int u );

};
#endif // TEMPUNIT_H
