// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * isotope.cc
 *
 * Copyright (C) 2005-2007 Jean Bréfort <jean.brefort@normalesup.org>
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
#include "isotope.h"
#include <cstdlib>

using namespace std;

namespace gcu
{

Isotope::Isotope ()
{
	A = 0;
	name = NULL;
	abundance.value = 0.;
	abundance.prec = 0;
	abundance.delta = 0;
	mass.value = 0.;
	mass.prec = 0;
	mass.delta = 0;
	spin = 0;
	decay_modes = NULL;
	decay_period.value = 0.;
	decay_period.prec = 0;
	decay_period.unit = NULL;
}

Isotope::~Isotope ()
{
	if (name != NULL)
		free (name);
	if (decay_modes != NULL)
		free (decay_modes);
}

IsotopicPattern::IsotopicPattern ()
{
	m_min = m_max = m_mono = 0;
	ref_count = 1;
}

IsotopicPattern::IsotopicPattern (int min, int max)
{
	if (max >= min) {
		m_min = min;
		m_max = max;
	} else {
		m_max = min;
		m_min = max;
	}
	m_mono = 0;
	m_values.resize (max - min + 1);
	ref_count = 1;
}

IsotopicPattern::~IsotopicPattern ()
{
}

double IsotopicPattern::epsilon = 1e-6;

IsotopicPattern *IsotopicPattern::Simplify ()
{
	int min = 0, max = m_max - m_min;
	int i, j, imax = max + 1;
	double vmax = m_values[0], minval;
	for (i = 1; i < imax; i++)
		if (m_values[i] > vmax) {
			vmax = m_values[i];
		}
	minval = epsilon * vmax;
	while (m_values[min] < minval)
		min++;
	while (m_values[max] < minval)
		max--;
	IsotopicPattern *pat = new IsotopicPattern (min + m_min, max + m_min);
	pat->m_mono = m_mono;
	pat->m_mono_mass = m_mono_mass;
	vmax /= 100.;
	for (i = min, j = 0; i <= max; i++, j++)
		pat->m_values[j] = m_values[i] / vmax;
	return pat;
}

IsotopicPattern *IsotopicPattern::Multiply (IsotopicPattern &pattern)
{
	IsotopicPattern *pat = new IsotopicPattern (m_min + pattern.m_min, m_max + pattern.m_max);
	pat->m_mono = m_mono + pattern.m_mono;
	pat->m_mono_mass = m_mono_mass + pattern.m_mono_mass;
	int i, j, k, imax = pat->m_max - pat->m_min + 1, jmax = m_values.size () - 1, kmax = pattern.m_values.size ();
	for (i = 0; i < imax; i++) {
		pat->m_values[i] = 0.;
		for (j = min (i, jmax), k = i - j; (k < kmax) && (j >= 0); j--, k++) {
			pat->m_values[i] += pattern.m_values[k] * m_values[j];
		}
	}
	return pat;
}

IsotopicPattern *IsotopicPattern::Square ()
{
	IsotopicPattern *pat = new IsotopicPattern (2 * m_min, 2 * m_max);
	pat->m_mono = 2 * m_mono;
	pat->m_mono_mass = m_mono_mass * 2;
	int i, j, k, imax = pat->m_max - pat->m_min + 1, jmax = m_values.size () - 1;
	for (i = 0; i < imax; i++) {
		pat->m_values[i] = 0.;
		for (j = max (0, i - jmax), k = i - j; k > j; k--, j++) {
			pat->m_values[i] += 2. * m_values[k] * m_values[j];
		}
		if (j == k)
			pat->m_values[i] += m_values[j] * m_values[j];
	}
	return pat;
}

void IsotopicPattern::SetValue (int A, double percent)
{
	if (A >= m_min && A <= m_max) {
		A -= m_min;
		m_values[A] = percent;
	}
}

void IsotopicPattern::Normalize ()
{
	double max = m_values[0];
	int i, maxi = m_max - m_min + 1;
	m_mono = 0;
	for (i = 1; i < maxi; i++)
		if (m_values[i] > max) {
			m_mono = i;
			max = m_values[i];
		}
	m_mono += m_min;
	max /= 100.;
	for (i = 0; i < maxi; i++)
		m_values[i] /= max;
}

void IsotopicPattern::Unref ()
{
	ref_count--;
	if (!ref_count)
		delete this;
}

int IsotopicPattern::GetValues (double **values)
{
	int i, result = m_values.size ();
	*values = new double[result];
	for (i = 0; i < result; i++)
		(*values)[i] = m_values[i];
	return result;
}

void IsotopicPattern::Copy (IsotopicPattern& pattern)
{
	m_min = pattern.m_min;
	m_max = pattern.m_max;
	m_mono = pattern.m_mono;
	m_mono_mass = pattern.m_mono_mass;
	int i, max = pattern.m_values.size();
	m_values.resize (max);
	for (i = 0; i < max; i++) {
		m_values[i] = pattern.m_values[i];
	}
}

void IsotopicPattern::Clear ()
{
	m_min = m_max = m_mono = 0;
	m_mono_mass = SimpleValue ();
}

void IsotopicPattern::SetMonoMass (SimpleValue mass)
{
	if (m_mono_mass.GetAsDouble () == 0.)
		m_mono_mass = mass;
}

}	//	namespace gcu
