/* 
 * Gnome Chemistry Utils
 * value.cc 
 *
 * Copyright (C) 2002-2007 Jean Bréfort <jean.brefort@normalesup.org>
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

// #include "config.h"
#include "value.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <iostream>

using namespace std;

namespace gcu
{

Value::Value ()
{
}

Value::~Value ()
{
}

char const *Value::GetAsString () const
{
	return "";
}

double Value::GetAsDouble () const
{
	return 0.;
}

SimpleValue::SimpleValue (): Value ()
{
	val.value = 0.;
	val.prec = val.delta = 0;
}

SimpleValue::SimpleValue (GcuValue value):
	Value (), val (value)
{
}

SimpleValue::~SimpleValue ()
{
}

char const *SimpleValue::GetAsString () const
{
	if (str.length () == 0) {
		char *buf = gcu_value_get_string (&val);
		const_cast<SimpleValue*> (this)->str = buf;
		free (buf);
	}
	return str.c_str ();
}

double SimpleValue::GetAsDouble () const
{
	return val.value;
}

SimpleValue SimpleValue::operator+ (SimpleValue const &value) const
{
	SimpleValue result;
	result.val.value = val.value + value.val.value;
	if (val.prec > value.val.prec) {
		result.val.prec = value.val.prec;
		int n = 1;
		while (result.val.prec < val.prec) {
			n *= 10;
			result.val.prec++;
		}
		result.val.delta = val.delta + n * value.val.delta;
	} else {
		result.val.prec = val.prec;
		int n = 1;
		while (result.val.prec < value.val.prec) {
			n *= 10;
			result.val.prec++;
		}
		result.val.delta = value.val.delta + n * val.delta;
	}
	return result;
}

SimpleValue SimpleValue::operator* (int n) const
{
	SimpleValue result;
	result.val.prec = val.prec;
	result.val.value = val.value * n;
	result.val.delta = val.delta * n;
	return result;
}

DimensionalValue::DimensionalValue (): Value ()
{
	val.value = 0.;
	val.prec = val.delta = 0;
	val.unit = NULL;
}

DimensionalValue::~DimensionalValue ()
{
}

char const *DimensionalValue::GetAsString () const
{
	if (str.length () == 0) {
		char *buf = gcu_dimensional_value_get_string (&val);
		const_cast<DimensionalValue*> (this)->str = buf;
		free (buf);
	}
	return str.c_str ();
}

double DimensionalValue::GetAsDouble () const
{
	return val.value;
}

DimensionalValue DimensionalValue::operator+ (DimensionalValue const &value) const throw (std::invalid_argument)
{
	DimensionalValue result;
	// only make the addition if units are the same, otherwise throw an exception
	if (strcmp (val.unit, value.val.unit))
	{
		cout << "error: Attempt to add two values with different units"<<endl;
		// throw new invalid_argument ("Attempt to add two values with different units.");
	}
	result.val.unit = val.unit;
	result.val.value = val.value + value.val.value;
	if (val.prec > value.val.prec) {
		result.val.prec = value.val.prec;
		int n = 1;
		while (result.val.prec < val.prec) {
			n *= 10;
			result.val.prec++;
		}
		result.val.delta = val.delta + n * value.val.delta;
	} else {
		result.val.prec = val.prec;
		int n = 1;
		while (result.val.prec < value.val.prec) {
			n *= 10;
			result.val.prec++;
		}
		result.val.delta = value.val.delta + n * val.delta;
	}
	return result;
}

DimensionalValue DimensionalValue::operator* (int n) const
{
	DimensionalValue result;
	result.val.unit = val.unit;
	result.val.prec = val.prec;
	result.val.value = val.value * n;
	result.val.delta = val.delta * n;
	return result;
}

StringValue::StringValue ()
{
}

StringValue::~StringValue ()
{
}

char const *StringValue::GetAsString () const
{
	return val.c_str ();
}

LocalizedStringValue::LocalizedStringValue ()
{
}

LocalizedStringValue::~LocalizedStringValue ()
{
	vals.clear ();
}

char const *LocalizedStringValue::GetAsString () const
{
	char *lang = getenv ("LANG");
	map <string, string>::const_iterator i, end = vals.end ();
	if (lang) {
		i = vals.find (lang);
		if (i == end || (*i).second.length () == 0) {
			lang = strdup (lang);
			char *dot = strchr (lang, '.');
			if (dot) {
				*dot = 0;
				i = vals.find (lang);
				if (i == end || (*i).second.length () > 0) {
					free (lang);
					return (*i).second.c_str ();
				}
			}
			if (strlen (lang) > 2) {
				lang[2] = 0;
				i = vals.find (lang);
				if (i == end || (*i).second.length () > 0) {
					free (lang);
					return (*i).second.c_str ();
				}
			}
			free (lang);
		} else
			return (*i).second.c_str ();
	}
	// if we are there, try "C" or "en" locales
	i = vals.find ("C");
	if (i != end && (*i).second.length () > 0)
		return (*i).second.c_str ();
	i = vals.find ("en");
	if (i != end && (*i).second.length () > 0)
		return (*i).second.c_str ();
	// if not found, return first occurence or a void string
	if (vals.size () > 0)
		return (*vals.begin ()).second.c_str ();
	return "";
}

char const *LocalizedStringValue::GetLocalizedString (char const *lang) const
{
	string s;
	if (lang)
		s = const_cast<LocalizedStringValue*> (this)->vals[lang];
	return (s.length () > 0)? s.c_str (): GetAsString ();
}

}	//	namespace gcu
