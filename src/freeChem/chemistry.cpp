// -*- C -*-

/* 
 * Gnome Chemisty Utils
 * chemistry.cc
 *
 * Copyright (C) 2003-2005 Jean Bréfort <jean.brefort@normalesup.org>
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
#include "chemistry.h"
#include "element.h"
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <stdio.h>

using namespace gcu;
const double* gcu_element_get_default_color(int Z)
{
	return Element::GetElement(Z)->GetDefaultColor();
}

const char* gcu_element_get_symbol(int Z)
{
	return Element::Symbol(Z);
}

const char* gcu_element_get_name(int Z)
{
	return Element::GetElement(Z)->GetName();
}

int gcu_element_get_Z(char* symbol)
{
	return Element::Z(symbol);
}

bool gcu_element_get_radius(GcuAtomicRadius* radius)
{
	return Element::GetRadius(radius);
}

bool gcu_element_get_electronegativity(GcuElectronegativity* en)
{
	return Element::GetElectronegativity(en);
}

const GcuAtomicRadius** gcu_element_get_radii(int Z)
{
	return Element::GetElement(Z)->GetRadii();
}

const GcuElectronegativity** gcu_element_get_electronegativities(int Z)
{
	return Element::GetElement(Z)->GetElectronegativities();
}

void gcu_element_load_databases (char const *name, ...)
{
	va_list l;
	char const *base = name;
	va_start (l, name);
	while (base != NULL) {
		if (!strcmp (base, "radii"))
			Element::LoadRadii ();
		else if (!strcmp (base, "elecprops"))
			Element::LoadElectronicProps ();
		else if (!strcmp (base, "isotopes"))
			Element::LoadIsotopes ();
		base = va_arg (l, char const*);
	}
	va_end (l);
}

char* gcu_value_get_string (GcuValue const *value)
{
	char *format, *str;
	if (value->delta > 0) {
		int delta = value->delta, prec = value->prec;
		while (delta >= 100) {
			delta /= 10;
			prec--;
		}
		sprintf (format, "%%.%df(%%d)", prec);
		sprintf (str, format, value->value, delta);
	} else {
		sprintf (format, "%%.%df", value->prec);
		sprintf (str, format, value->value);
	}
	free (format);
	return str;
}

char* gcu_dimensional_value_get_string (GcuDimensionalValue const *value)
{
	char *format, *str;
	if (value->delta > 0) {
		int delta = value->delta, prec = value->prec;
		while (delta >= 100) {
			delta /= 10;
			prec--;
		}
		// FIXME: what to do if we have a negative precision?
		if (prec > 0)
		{
			sprintf (format, "%%.%df(%%d) %%s", prec);
		}
		else
		{
			sprintf (format, "%.0f %s");
		}
		sprintf (str, format, value->value, delta, value->unit);
	} else {
		sprintf (format, "%%.%df %%s", value->prec);
		sprintf (str, format, value->value, value->unit);
	}
	free (format);
	return str;
}

GcuDimensionalValue const *gcu_element_get_ionization_energy (int Z, int rank)
{
	return Element::GetElement(Z)->GetIonizationEnergy (rank);
}

GcuDimensionalValue const *gcu_element_get_electron_affinity (int Z, int rank)
{
	return Element::GetElement(Z)->GetElectronAffinity (rank);
}

