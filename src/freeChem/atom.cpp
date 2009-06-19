// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * atom.cc
 *
 * Copyright (C) 2001-2009 Jean Bréfort <jean.brefort@normalesup.org>
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
#include "element.h"
#include "atom.h"
#include "bond.h"
#include "document.h"
#include "objprops.h"
#include "vector.h"
#include "xml-utils.h"
#include <cmath>
#include <cstring>
#include <sstream>

using namespace std;

namespace gcu
{

Atom::Atom (): Object (AtomType)
{
	m_Z = -1;
	m_x = m_y = m_z = 0.0;
	m_Charge = 0;
}

Atom::~Atom ()
{
	m_Bonds.clear ();
}

Atom::Atom (int Z, double x, double y, double z):
	Object (AtomType)
{
	SetZ (Z);
	m_x = x;
	m_y = y;
	m_z = z;
	m_Charge = 0;
}

Atom::Atom (Atom& a):
	Object (AtomType)
{
	SetZ (a.m_Z);
	m_x = a.m_x;
	m_y = a.m_y;
	m_z = a.m_z;
	m_Charge = a.m_Charge;
}

Atom& Atom::operator= (Atom& a)
{
	SetZ (a.m_Z);
	m_x = a.m_x;
	m_y = a.m_y;
	m_z = a.m_z;
	m_Charge = a.m_Charge;
	return *this ;
}

void Atom::SetZ (int Z)
{
	m_Z = Z;
}

double Atom::Distance (Atom* pAtom)
{
	return sqrt (square (m_x - pAtom->m_x) + square (m_y - pAtom->m_y) + square (m_z - pAtom->m_z));
}

void Atom::zoom (double ZoomFactor)
{
	m_x *= ZoomFactor;
	m_y *= ZoomFactor;
	m_z *= ZoomFactor;
}

bool Atom::GetCoords (double *x, double *y, double *z) const
{
	if (!x || !y)
		return false;
	*x = m_x;
	*y = m_y;
	if (z)
		*z = m_z;
	return true;
}

const char* Atom::GetSymbol () const
{
	Element* Elt = Element::GetElement(m_Z);
	return (Elt)? Element::Symbol(m_Z): NULL;
}

void Atom::AddBond (Bond* pBond)
{
	m_Bonds[pBond->GetAtom (this)] = pBond;
}

void Atom::RemoveBond (Bond* pBond)
{
	m_Bonds.erase (pBond->GetAtom (this));
}

Bond* Atom::GetFirstBond (map<Atom*, Bond*>::iterator& i)
{
	i = m_Bonds.begin ();
	if (i == m_Bonds.end ())
		return NULL;
	return (*i).second;
}

Bond* Atom::GetNextBond (map<Atom*, Bond*>::iterator& i)
{
	i++;
	if (i == m_Bonds.end())
		return NULL;
	return (*i).second;
}

void Atom::Move (double x, double y, double z)
{
	m_x += x;
	m_y += y;
	m_z += z;
}

void Atom::Transform2D (Matrix2D& m, double x, double y)
{
	m_x -= x;
	m_y -= y;
	m.Transform (m_x, m_y);
	m_x += x;
	m_y += y;
}

xmlNodePtr Atom::Save (xmlDocPtr xml) const
{
	xmlNodePtr parent;
	char buf[16];
	parent = xmlNewDocNode (xml, NULL, (xmlChar*) "atom", NULL);
	if (!parent)
		return NULL;
	SaveId (parent);

	if (m_Z) {
		strncpy(buf, GetSymbol(), sizeof(buf));
		xmlNewProp(parent, (xmlChar*) "element", (xmlChar*) buf);
	}
	
	if (m_Charge) {
		snprintf (buf, sizeof (buf), "%d", m_Charge);
		xmlNewProp (parent, (xmlChar*) "charge", (xmlChar*) buf);
	}
	if (!WritePosition (xml, parent, NULL, m_x, m_y, m_z)) {
		xmlFreeNode (parent);
		return NULL;
	}
	if (!SaveNode (xml, parent)) {
		xmlFreeNode (parent);
		return NULL;
	}
	return parent;
}

bool Atom::Load (xmlNodePtr node)
{
	char* tmp;
	tmp = (char*) xmlGetProp (node, (xmlChar*) "id");
	if (tmp) {
		SetId (tmp);
		xmlFree (tmp);
	}
	tmp = (char*) xmlGetProp (node, (xmlChar*) "element");
	if (tmp) {
		m_Z = Element::Z(tmp);	//Don't check if element exists. Applications that do not accept unknown elements should check
		xmlFree (tmp);
	}
	tmp = (char*) xmlGetProp (node, (xmlChar*) "charge");
	if (tmp) {
		m_Charge = (char) atoi (tmp);
		xmlFree (tmp);
	} else
		m_Charge = 0;
	if (!ReadPosition (node, NULL, &m_x, &m_y, &m_z) || (!LoadNode (node))) 
		return false;
	return true;
}

bool Atom::LoadNode (xmlNodePtr node)
{
	return true;
}

bool Atom::SaveNode (xmlDocPtr xml, xmlNodePtr node) const
{
	return true;
}

Bond* Atom::GetBond (Atom* pAtom) const
{
	std::map<Atom*, Bond*>::const_iterator i;
	i = m_Bonds.find (pAtom);
	return (i != m_Bonds.end ())? (*i).second: NULL;
}

bool Atom::SetProperty (unsigned property, char const *value)
{
	switch (property) {
	case GCU_PROP_POS2D: {
		double x, y;
		sscanf (value, "%lg %lg", &x, &y);
		Document *doc = GetDocument ();
		if (doc) {
			x *= 1;//doc->GetScale ();
			y *= 1;//doc->GetScale ();
		}
		SetCoords (x, y);
		break;
	}
	case GCU_PROP_X:
		m_x = strtod (value, NULL) * 1;//GetDocument ()->GetScale ();
		break;
	case GCU_PROP_Y:
		m_y = strtod (value, NULL) * 1;//GetDocument ()->GetScale ();
		break;
	case GCU_PROP_Z:
		m_z = strtod (value, NULL) * 1;//GetDocument ()->GetScale ();
		break;
	case GCU_PROP_ID: {
		// problem
		char *Id;
		if (*value == 'a')
		{
			Id = strdup (value);
		}
		else
		{
			 sprintf (Id, "a%s", value);
		}
		SetId (Id);
		break;
	}
	case GCU_PROP_ATOM_SYMBOL:
		SetZ (Element::Z (value));
		break;
	case GCU_PROP_ATOM_Z:
		SetZ (atoi (value));
		break;
	case GCU_PROP_ATOM_CHARGE:
		SetCharge (atoi (value));
		break;
	}
	return  true;
}

string Atom::GetProperty (unsigned property) const
{
	ostringstream res;
	switch (property) {
	case GCU_PROP_POS2D: {
		Document *doc = const_cast <Atom*> (this)->GetDocument ();
		if (doc)
			res << m_x / 1 /*doc->GetScale ()*/ << " " << m_y / 1;//doc->GetScale ();
		else
			res << m_x << " " << m_y;
		break;
	}
	case GCU_PROP_POS3D: {
		Document *doc = const_cast <Atom*> (this)->GetDocument ();
		if (doc)
			res << m_x / doc->GetScale () << " " << m_y / doc->GetScale () << " " << m_z / doc->GetScale ();
		else
			res << m_x << " " << m_y << " " << m_z;
		break;
	}
	case GCU_PROP_ATOM_SYMBOL:
		res << GetSymbol ();
		break;
	case GCU_PROP_ATOM_CHARGE:
		res << static_cast <int> (m_Charge);
		break;
	default:
		return Object::GetProperty (property);
	}
	return res.str ();
}

bool Atom::IsInCycle (Cycle* pCycle)
{
	map<gcu::Atom*, gcu::Bond*>::iterator i, end = m_Bonds.end ();
	for (i = m_Bonds.begin (); i != end; i++)
		if (((Bond*) (*i).second)->IsInCycle (pCycle))
		return true;
	return false;
}

bool Atom::Match (Atom *atom, AtomMatchState &state)
{
	if (m_Z != atom->m_Z)
		return false;
	if (m_Bonds.size () != atom->m_Bonds.size ())
		return false;
	unsigned n = state.mol1.size ();
	// add the atoms to state
	state.mol1[this] = n;
	state.mol2[atom] = n;
	state.atoms[n] = AtomPair (this, atom);
	// compare bonded atoms
	map<gcu::Atom*, gcu::Bond*>::iterator i, iend = m_Bonds.end ();
	map<gcu::Atom*, gcu::Bond*>::iterator j, jend = atom->m_Bonds.end ();
	for (i = m_Bonds.begin (); i != iend; i++) {
		if (state.mol1.find ((*i).first) != state.mol1.end ())
			continue; /* may be not enough: we might search which atom is
		  				associated with this one and see if it is effectively
			  			bonded to *atom */
		for (j = atom->m_Bonds.begin (); j != jend; j++) {
			if (state.mol2.find ((*j).first) != state.mol2.end ())
				continue;
			if ((*i).first->Match ((*j).first, state)) 
				break;
		}
		if (j == jend)
			break; // no matching atom has been fond;
	}
	if (i == iend)
		return true;
	// if we are there, something wrong happened, clean and return
	unsigned max = state.mol1.size ();
	for (unsigned i = n; i < max; i++) {
		state.mol1.erase (state.atoms[n].atom1);
		state.mol2.erase (state.atoms[n].atom2);
	}
	return false;
}

Vector Atom::GetVector () const
{
	return Vector (m_x, m_y, m_z);
}

}	//	namespace gcu
