// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * bond.cc 
 *
 * Copyright (C) 2001-2008 Jean Bréfort <jean.brefort@normalesup.org>
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
#include "bond.h"
#include "atom.h"
#include "objprops.h"
#include <cmath>
#include <cstring>
#include <sstream>

using namespace std;

namespace gcu
{

Bond::Bond(): Object(BondType)
{
	m_Begin = NULL;
	m_End = NULL;
	m_order = 0;
}

Bond::Bond(Atom* first, Atom* last, unsigned char order): Object(BondType)
{
	m_Begin = first;
	m_End = last;
	m_order = order;
	first->AddBond(this);
	last->AddBond(this);
}

Bond::~Bond()
{
}

Atom* Bond::GetAtom(int which) const
{
	switch (which)
	{
	case 0: return m_Begin;
	case 1: return m_End;
	default: return NULL;
	}
}

Atom* Bond::GetAtom(Atom* pAtom, int which) const
{
	return (pAtom == m_Begin)? m_End: (pAtom == m_End)? m_Begin: NULL;
}

unsigned char Bond::GetOrder() const
{
	return m_order;
}

void Bond::SetOrder(unsigned char Order)
{
	m_order = Order;
}

xmlNodePtr Bond::Save(xmlDocPtr xml) const
{
	xmlNodePtr parent;
	char buf[16];
	parent = xmlNewDocNode(xml, NULL, (xmlChar*)"bond", NULL);
	if (!parent) return NULL;

	SaveId(parent);

	buf[0] = m_order + '0';
	buf[1] = 0;
	xmlNewProp(parent, (xmlChar*)"order", (xmlChar*)buf);

	if (m_Begin)
		xmlNewProp (parent, (xmlChar*) "begin", (xmlChar*) m_Begin->GetId ());


	if (m_End)
		xmlNewProp (parent, (xmlChar*) "end", (xmlChar*) m_End->GetId ());

	if (!SaveNode(xml, parent)) {xmlFreeNode(parent); return NULL;}
	return parent;
}

bool Bond::Load (xmlNodePtr node)
{
	char* tmp;
	xmlNodePtr child;
	Object* pObject;
	tmp = (char*) xmlGetProp (node, (xmlChar*) "id");
	if (tmp) {
		SetId (tmp);
		xmlFree (tmp);
	}
	tmp = (char*) xmlGetProp (node, (xmlChar*) "order");
	if (!tmp) m_order = 1;
	else {
		m_order = *tmp - '0';
		xmlFree (tmp);
	}
	if (m_order > 4)
		return false;
	tmp = (char*) xmlGetProp (node, (xmlChar*) "begin");
	if (!tmp) {
		child = GetNodeByName(node, "begin");
		tmp = (char*) xmlNodeGetContent(child); //necessary to read version 0.1.0 files
		if (!tmp)
			return false;
	}
	pObject = GetParent ()->GetDescendant (tmp);
	xmlFree (tmp);
	if (!pObject || (!dynamic_cast <Atom *> (pObject)))
		return false;
	m_Begin = (Atom*)(pObject);
	tmp = (char*) xmlGetProp (node, (xmlChar*) "end");
	if (!tmp) {
		child = GetNodeByName (node, "end");
		tmp = (char*) xmlNodeGetContent (child); //necessary to read version 0.1.0 files
		if (!tmp)
			return false;
	}
	pObject = GetParent ()->GetDescendant (tmp);
	xmlFree (tmp);
	if (!pObject || (!dynamic_cast <Atom *> (pObject)))
		return false;
	m_End = (Atom*)pObject;
	bool result = LoadNode (node);
	m_Begin->AddBond (this);
	m_End->AddBond (this);
	return result;
}

bool Bond::LoadNode(xmlNodePtr)
{
	return true;
}

bool Bond::SaveNode(xmlDocPtr, xmlNodePtr) const
{
	return true;
}

void Bond::IncOrder(int n)
{
	m_order += n;
	if (m_order > 4)  m_order %= 4;
}

void Bond::Move(double x, double y, double z)
{
}

void Bond::Transform2D(Matrix2D& m, double x, double y)
{
}

bool Bond::SetProperty (unsigned property, char const *value)
{
	switch (property) {
	case GCU_PROP_ID: {
		char *Id;
		if (*value == 'b')
		{
			Id = strdup (value);
		}
		else
		{
			sprintf( Id, "a%s", value);
		}
		SetId (Id);
		break;
	}
	case GCU_PROP_BOND_BEGIN: {
		char *tmp;
		if (*value == 'a')
		{
			tmp = strdup (value);
		}
		else
		{
			sprintf( tmp, "a%s", value);
		}
		Object *pObject = GetParent ()->GetDescendant (tmp);
		free (tmp);
		if (!pObject || (!dynamic_cast <Atom *> (pObject)))
			return false;
		m_Begin = (Atom*) pObject;
		if (m_End) {
			m_Begin->AddBond (this);
			m_End->AddBond (this);
		}
		break;
	}
	case GCU_PROP_BOND_END: {
		char *tmp;
		if (*value == 'a')
		{
			tmp = strdup (value);
		}
		else
		{
			sprintf( tmp, "a%s", value);
		}
		Object *pObject = GetParent ()->GetDescendant (tmp);
		free (tmp);
		if (!pObject || (!dynamic_cast <Atom *> (pObject)))
			return false;
		m_End = (Atom*) pObject;
		if (m_Begin) {
			m_Begin->AddBond (this);
			m_End->AddBond (this);
		}
		break;
	}
	case GCU_PROP_BOND_ORDER:
		m_order = atoi (value);
		if (m_Begin && m_End) {
			m_Begin->AddBond (this);
			m_End->AddBond (this);
		}
		break;
	}
	return  true;
}

string Bond::GetProperty (unsigned property) const
{
	ostringstream res;
	switch (property) {
	case GCU_PROP_BOND_BEGIN:
		res << m_Begin->GetId ();
		break;
	case GCU_PROP_BOND_END:
		res << m_End->GetId ();
		break;
	case GCU_PROP_BOND_ORDER:
		res << static_cast <unsigned> (m_order);
		break;
	default:
			return Object::GetProperty (property);
	}
	return res.str ();
}

void Bond::AddCycle (Cycle* pCycle)
{
	m_Cycles.push_back (pCycle);
}

void Bond::RemoveCycle (Cycle* pCycle)
{
	m_Cycles.remove (pCycle);
}

void Bond::RemoveAllCycles ()
{
	m_Cycles.clear ();
}

Cycle* Bond::GetFirstCycle (std::list<Cycle*>::iterator& i, Cycle * pCycle)
{
	i = m_Cycles.begin ();
	return GetNextCycle (i, pCycle);
}

Cycle* Bond::GetNextCycle (std::list<Cycle*>::iterator& i, Cycle * pCycle)
{
	if (*i == pCycle)
		i++;
	if (i == m_Cycles.end ())
		return NULL;
	pCycle = *i;
	i++;
	return pCycle;
}

bool Bond::IsInCycle (Cycle* pCycle)
{
	std::list<Cycle*>::iterator i, end = m_Cycles.end ();
	for (i = m_Cycles.begin (); i != end; i++)
		if ((*i) == pCycle)
			return true;
	return false;
}

double Bond::Get2DLength ()
{
	double x1, y1, x2, y2;
	m_Begin->GetCoords (&x1, &y1);
	m_End->GetCoords (&x2, &y2);
	return sqrt (square (x1 - x2) + square (y1 - y2));
}

double Bond::GetAngle2DRad (Atom* pAtom)
{
	double x1, y1, x2, y2;
	m_Begin->GetCoords (&x1, &y1);
	m_End->GetCoords (&x2, &y2);
	x2 -= x1;
	y2 -= y1;
	double length = square (x2) + square (y2);
	if (length == 0.0)
		return HUGE_VAL;
	if (pAtom == m_Begin)
		return atan2 (-y2, x2);
	else if (pAtom == m_End)
		return atan2 (y2, -x2);
	return HUGE_VAL;
}

bool Bond::ReplaceAtom (Atom* oldAtom, Atom* newAtom)
{
	if (oldAtom == m_Begin) {
		if (m_End)
			m_End->RemoveBond (this);
		m_Begin = newAtom;
		if (m_Begin && m_End)
			m_End->AddBond (this);
	} else if (oldAtom == m_End) {
		if (m_Begin)
			m_Begin->RemoveBond (this);
		m_End = newAtom;
		if (m_Begin && m_End)
			m_Begin->AddBond (this);
	}
	return true;
}

}	//	namespace gcu
