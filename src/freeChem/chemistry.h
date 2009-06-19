// -*- C -*-

/* 
 * Gnome Chemisty Utils
 * chemistry.h 
 *
 * Copyright (C) 2003-2006 Jean Bréfort <jean.brefort@normalesup.org>
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


/*! \mainpage
  \section intro Introduction
The Gnome Chemistry Utils Library provides some widgets and C++ classes related to chemistry.

Available widgets are:
- GcpFontSel: a font selector.
- GtkPeriodic: a periodic table of the elements.
- GtkComboPeriodic: a combo box witha dropdown periodic table widget.
- GtkCrystalViewer: a crystal structure viewer.
- GtkChem3DViewer: a 3D molecular structure viewer.
- GtkSpectrumViewer: a spectrum viewer.

The C++ classes are grouped in the gcu, gccv, and gcp namespaces. A C interface
exists to use some (not so many) of the functionalities offered in these classes;
the corresponding documentation is available in the chemistry.h file.
*/

#ifndef GCU_CHEMISTRY_H
#define GCU_CHEMISTRY_H

/** \file
* C interface to the chemistry library.

To use this file, add the following line to your source file:
\code
#include <gcu/chemistry.h>
\endcode
*/

// G_BEGIN_DECLS

/**
The greatest atomic number known in this version of the Gnome Chemictry Utils
*/
#define MAX_ELT 116

/** gcu_spin_state
* gcu_spin_state describes the spin state of a central atom in a complex.
* Possible values are:
* - GCU_N_A_SPIN: unknown,
* - GCU_LOW_SPIN: low spin,
* - GCU_HIGH_SPIN: high spin.
*
*  This enumeration is used in the GcuAtomicRadius structure.
*/
enum gcu_spin_state
{
	GCU_N_A_SPIN,
	GCU_LOW_SPIN,
	GCU_HIGH_SPIN
};

/** gcu_radius_type
* gcu_radius_type describes the type of the atomic radius
* Possible values are:
* - GCU_RADIUS_UNKNOWN: unknown,.
* - GCU_ATOMIC: atomic radius,
* - GCU_IONIC: ionic radius,
* - GCU_METALLIC: metallic radius,
* - GCU_COVALENT: covalent radius,
* - GCU_VAN_DER_WAALS: van der Waals radius,
*
*  This enumeration is used in the GcuAtomicRadius structure.
*/
enum gcu_radius_type
{
	GCU_RADIUS_UNKNOWN,
	GCU_ATOMIC,
	GCU_IONIC,
	GCU_METALLIC,
	GCU_COVALENT,
	GCU_VAN_DER_WAALS
};

/** GcuValue
Structure used to store a numerical value.
*/
typedef struct
{
	/** The value. */
	double value;
	/** The precision (number of significative decimal figures). */
	int prec;
	/** The standard error (* 10^prec). */
	int delta;
} GcuValue;

/** GcuDimensionalValue
Structure used to store a dimensional value.
*/
typedef struct
{
	/** The value.*/
	double value;
	/** The precision (number of significative decimal figures).*/
	int prec;
	/** The standard error (* 10^prec). */
	int delta;
	/** The unit symbol.*/
	const char *unit;
} GcuDimensionalValue;

/** GcuAtomicRadius
* Structure used to describe an atomic radius.
*/
typedef struct
{
	/** The atomic number. */
	unsigned char Z;
	/** The radius type (unknown, ionic, metallic, covalent, or van der Waals).*/
	enum gcu_radius_type type;
	/** The radius value (the unit should always be pm). */
	GcuDimensionalValue value;
	/** The charge of the atom (or ion). */
	char charge;
	/** The scale name, e.g. "Pauling" or "Shannon". */
	char const* scale;
	/** the coordination number of the atom or -1 if unknown. */
	char cn;	//coordination number: -1: unspecified
	/** The spin state (unknown, low or high).*/
	enum gcu_spin_state spin;
} GcuAtomicRadius;

/*! GcuElectronegativity
Structure used to describe an electronegativity value.
*/
typedef struct
{
	/** The atomic number. */
	unsigned char Z;
	/*! The electronegativity value.*/
	GcuValue value;
	/** The scale name, e.g. "Pauling" or "Mulliken". */
	char const *scale;
} GcuElectronegativity;

/*! GcuIsotope
Structure used to describe an isotope
*/
typedef struct
{
	/** The nucleon number. */
	unsigned char A;
	/** The name of the isotope if any.*/
	char *name;
	/*! The terrestrial percent abundance.*/
	GcuValue abundance;
	/*! The mass of an atom in atomic units.*/
	GcuValue mass;
	/*! The spin of an atom multipiled by 2.*/
	char spin;
	/*! The decay modes as a comma separated list.*/
	char *decay_modes;
	/*! The decay period.*/
	GcuDimensionalValue decay_period;
} GcuIsotope;

/*!
\param Z: the atomic number of the element.

Retrieves the default color used for the element.
\return an array of three double values for the red, green and blue components of the color.
*/
const double* gcu_element_get_default_color (int Z);
/*!
gcu_element_get_symbol:
\param Z: the atomic number of the element.
\return the symbol of the element.
*/
const char* gcu_element_get_symbol (int Z);
/*!
\param Z: the atomic number of the element.
\return the name of the element in the current locale or in english if the current locale is not supported in the database.
*/
const char* gcu_element_get_name (int Z);
/*!
gcu_element_get_Z:
\param symbol: the symbol of the element (e.g. "C" ot "Pt").
\return the atomic number of the element.
*/
int gcu_element_get_Z (char* symbol);
/*!
\param radius: a pointer to a GcuAtomicRadius structure.

Before calling this function, most fields in the GcuAtomicRadius structure must be filled:
- Z: the atomic number, mandatory
- type: the type of the radius searched
- charge: the charge of the atom. 0 for all radii except ionic radii.
- cn: the coordination number or -1 if not significant
- spin: the spin state or GCU_N_A_SPIN if not significant
- scale: the name of the scale (e.g. "Pauling") or NULL

The programs searches a value corresponding to the fields having a non default value. If one is found
the other fields are given the corresponding values f the first match before returning.

\return TRUE if a radius has been found and FALSE if not.

*/
bool gcu_element_get_radius (GcuAtomicRadius* radius);
/*!
\param en: a pointer to a GcuElectronegativity structure.

Before calling this function, the following fields in the GcuElectronegativity structure must be filled:
- Z: the atomic number, mandatory
- scale: the name of the scale (e.g. "Pauling") or NULL

The programs searches an electronegativity value for the element in the scale if given. If one is found
the value and the scale (if NULL on calling)  are given the corresponding values of the first match before returning.

\return TRUE if a match has been found and FALSE if not.
*/
bool gcu_element_get_electronegativity (GcuElectronegativity* en);
/*!
\param Z: the atomic number.
\param N: the rank of the ionization.

The program searches for the Nth ionization energy value for the element and returns
it if found.

\return the ionization energy as a GcuDimensionalValue if known or NULL.
*/
GcuDimensionalValue const *gcu_element_get_ionization_energy (int Z, int N);
/*!
\param Z: the atomic number.
\param N: the rank of the extra electron.

For some elements, second and even third electron affinities have been estimated.
The program searches the Nth electron affinity value for the element and returns
it if found.
A value of 0 means that the anion is unstable in the gaseous phase and that no better
etimation is available.

\return the electronic affinity as a GcuDimensionalValue if known or NULL.
*/
GcuDimensionalValue const *gcu_element_get_electron_affinity (int Z, int N);
/*!
\param Z: the atomic number of the element.

\return a pointer to the array of pointers to GcuAtomicRadius structures for all known radii for the element.
Last value in the array is NULL.
*/
const GcuAtomicRadius** gcu_element_get_radii (int Z);
/*!
\param Z: the atomic number of the element.

\return a pointer to the array of pointers to GcuElectronegativity structures for all known electronegativities for the element.
Last value in the array is NULL.
*/
const GcuElectronegativity** gcu_element_get_electronegativities (int Z);
/*!
\param name: name of a database to load such as "radii", "elecprops", "isotopes",
the first name is followed by a list of other database terminated by NULL.

Loads the listed databases.
*/
void gcu_element_load_databases (char const *name, ...);

/*!
\param value: the GcuValue to represent as a string.

\return the GcuValue as a string taking into account the precision and
incertitude
*/
char* gcu_value_get_string (GcuValue const *value);

/*!
\param value: the GcuDimensionalValue to represent as a string.

\return the GcuDimensionalValue as a string taking into account the precision and
incertitude
*/
char* gcu_dimensional_value_get_string (GcuDimensionalValue const *value);

// G_END_DECLS

#endif //GCU_CHEMISTRY_H
