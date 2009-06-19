// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * isotope.h 
 *
 * Copyright (C) 2005-2008 Jean Bréfort <jean.brefort@normalesup.org>
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

#ifndef GCU_ISOTOPE_H
#define GCU_ISOTOPE_H

#include "chemistry.h"
#include "value.h"
#include "vector"

/*!\file*/
namespace gcu
{

/*!\class Isotope gcu/isotope.h
This class just wrap a GcuIsotope structure.
*/
class Isotope: public GcuIsotope
{
public:
/*!
The constructor initialize the member of the GcuIsotope structure with
nul values.
*/
	Isotope ();
/*!
The denstructor.
*/
	~Isotope ();
};

/*!\class IsotopicPattern gcu/isotope.h
Objects of this class represent the isotopic pattern corresponding to a
chemical formula.
<br>
The formalism used is similar to polynoms algebra, since calculate the
isotopic pattern of the reunion of two fragments is equivalent to a
polynomial multiplication.
*/
class IsotopicPattern
{
public:
/*!
Default constructor. Initialize members to nul values. The IsotopicPattern
is given an initial reference count of 1.
*/
	IsotopicPattern ();
/*!
Sets the minimum and maximum mass numbers of the pattern, and reserves memory to
store the abundances of the mass fragments. The IsotopicPattern
is given an initial reference count of 1.
*/
	IsotopicPattern (int min, int max);
	~IsotopicPattern ();

/*!
This method creates a copy of the original object with abundances normalized
as a percentage of the largest value, and removes very small values to save time
in subsequent calculations.
@return the resulting object.
*/
	IsotopicPattern *Simplify (void);
/*!
Effects a polynomial multiplication to calculate the pattern correponding to
the reunion of the two fragments.
@return the result of the multiplication.
*/
	IsotopicPattern *Multiply (IsotopicPattern& pattern);
/*!
Squares the original pattern to get the pattern corresponding to twice the
original formula.
@return the result of the multiplication.
*/
	IsotopicPattern *Square (void);
/*!
@param pattern: the isotopic pattern to be copied.
Set the values of the isotopic pattern so that it becomes identical to pattern.
*/
	void Copy (IsotopicPattern& pattern);

/*!
@param A: the mass number of the isotope.
@param percent: the abundance of the isotope.

This method is used when building an Isotopic Pattern from raw data.
IsotopicPattern::SetMonoMass should be called when data for each isotope have been entered.
*/
	void SetValue (int A, double percent);
/*!
Effects the same multiplication on all abundances so that the largest becomes
100.
*/
	void Normalize ();
/*!
Clears the contents of an isotopic pattern for reuse.
*/
	void Clear ();
/*!
Increments the reference count of the pattern.
*/
	void Ref () {ref_count++;}
/*!
Decrements the reference count of the pattern. If the reference count becomes 0,
the object is destroyed.
*/
	void Unref ();
/*!
@return the mass (actually the nucleons number) of the fragment with the
lowest mass. This might not be the real minimu, since fragments with very low
abundance are discarded during the evaluation.
*/
	int GetMinMass () {return m_min;}
/*!
@return the nucleons number of the fragment made with most abundant
isotopes of each element.
*/
	int GetMonoNuclNb () {return m_mono;}
/*!
@return the mass of the fragment made with most abundant
isotopes of each element.
*/
	SimpleValue const &GetMonoMass () {return m_mono_mass;}
/*!
@param mass: the mass of the most abundant isotope.

This method is used when building an Isotopic Pattern from raw data.
To enter the data for each isotope, use IsotopicPattern::SetValue. It
has no effect if the current monoisotopic mass is not nul.
*/
	void SetMonoMass (SimpleValue mass);
/*!
@param values: where to store the pointer to the abundances of the various
mass fragments as a percentage of the most abundant one. The pointer should be
freed with g_free when not anymore nedded.

@return the number of values in the array.	
*/
	int GetValues (double **values);

private:
	int m_min, m_max, m_mono;
	int ref_count;
	std::vector<double> m_values;
	SimpleValue m_mono_mass;
	static double epsilon;
};

}
#endif	// GCU_ISOTOPE_H
