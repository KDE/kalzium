/* 
 * Gnome Chemistry Utils
 * value.h 
 *
 * Copyright (C) 2002-2008 Jean Bréfort <jean.brefort@normalesup.org>
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
 * USA
 */

#ifndef GCU_VALUE_H
#define GCU_VALUE_H

#include "chemistry.h"
#include <string>
#include <map>
#include <string>
#include <stdexcept>

/*!\file*/
namespace gcu
{

/*!\class Value gcu/value.h
Base class used for data storing.
*/
class Value
{
public:
/*!
Default constructor
*/
	Value ();
/*!
Default destructor
*/
	virtual ~Value ();

/*!
@return the value representation as a string.
*/
	virtual char const *GetAsString () const;

/*!
@return the value representation as a double or 0. if not a numeric value.
*/
	virtual double GetAsDouble () const;
};

/*!\class SimpleValue gcu/value.h
Used to store adimensional data.
*/
class SimpleValue: public Value
{
friend class Element;

public:
/*!
Default constructor
*/
	SimpleValue ();
	SimpleValue (GcuValue value);
/*!
Default destructor
*/
	virtual ~SimpleValue ();

/*!
@return the literal representation of the embedded adimensional data
*/
	char const *GetAsString () const;
/*!
@return the embedded value.
*/
	double GetAsDouble () const;
/*!
@return the embedded GcuValue structure.
*/
	GcuValue const GetValue () {return val;}
	SimpleValue operator+ (SimpleValue const &value) const;
	SimpleValue operator* (int n) const;

private:
	GcuValue val;
	std::string str;
};
/*!\class DimensionalValue gcu/value.h
Used for data with a dimension e.g. a time interval expressed in days.
*/

class DimensionalValue: public Value
{
friend class Element;

public:
/*!
Default constructor
*/
	DimensionalValue ();
/*!
Default destructor
*/
	virtual ~DimensionalValue ();

/*!
@return the literal representation of the embedded dimensional data
*/
	char const *GetAsString () const;
/*!
@return the embedded value. Note that the unit is lost.
*/
	double GetAsDouble () const;

	DimensionalValue operator+ (DimensionalValue const &value) const throw (std::invalid_argument);
	DimensionalValue operator* (int n) const;
/*!
@return the embedded GcuDimensionalValue structure.
*/
	GcuDimensionalValue const GetValue () const {return val;}

private:
	GcuDimensionalValue val;
	std::string str;
};

/*!\class StringValue gcu/value.h
Used to store literal data.
*/
class StringValue: public Value
{
friend class Element;

public:
/*!
Default constructor
*/
	StringValue ();
/*!
Default destructor
*/
	virtual ~StringValue ();

/*!
@return the embedded string.
*/
	char const *GetAsString () const;

private:
	std::string val;
};

/*!\class LocalizedStringValue gcu/value.h
Used to store translatable strings.
*/
class LocalizedStringValue: public Value
{
friend class Element;

public:
/*!
Default constructor
*/
	LocalizedStringValue ();
/*!
Default destructor
*/
	virtual ~LocalizedStringValue ();

/*!
@return the string translated to the current locale if known, otherwise, the untranslated
string.
*/
	char const *GetAsString () const;
/*!
@param lang a locale name.
@return the string translated to the requested locale if known, otherwise, the untranslated
string.
*/
	char const *GetLocalizedString (char const *lang) const;

private:
	std::map <std::string, std::string> vals;
};

}	// namespace gcu

#endif	//	GCU_VALUE_H
