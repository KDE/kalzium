// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * libs/gcu/cycle.cc 
 *
 * Copyright (C) 2001-2004 Jean Bréfort <jean.brefort@normalesup.org>
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
#include "cycle.h"
#include "atom.h"
#include "bond.h"
#include "molecule.h"

namespace gcu {

Cycle::Cycle (Molecule* molecule): Chain (molecule, (Atom*) NULL, CycleType)
{
}

Cycle::~Cycle ()
{
}

void Cycle::Simplify ()
{
	std::map<Atom*, ChainElt>::iterator i, end = m_Bonds.end (), k;
	for (i = m_Bonds.begin (); i != end; i++) {
		if ((*i).second.fwd->IsCyclic() > 1) {
			std::list<Cycle*>::iterator j;
			Cycle* pCycle = (*i).second.fwd->GetFirstCycle (j, this);
			while (pCycle) {
				for (k = pCycle->m_Bonds.begin (); (*k).second.fwd != (*i).second.fwd; k++) ;
				Atom *pAtom0 = (*i).first, *pAtom1, *pAtom2;
				bool bReverse = (pAtom0 != (*k).first);
				Bond *pBond0 = (*i).second.fwd, *pBond;
				Cycle Common (NULL);
				Common.m_Bonds[pAtom0].fwd = pBond0;
				pAtom1 = pAtom2 = pAtom0;
				pBond = pBond0;
				pAtom2 = (Atom*) pBond0->GetAtom (pAtom2);
				Common.m_Bonds[pAtom2].rev = pBond;
				if (bReverse) {
					//explore forward
					while ((pBond = m_Bonds[pAtom2].fwd) == pCycle->m_Bonds[pAtom2].rev) {
						Common.m_Bonds[pAtom2].fwd = pBond;
						pAtom2 = (Atom*) pBond->GetAtom (pAtom2);
						Common.m_Bonds[pAtom2].rev = pBond;
					}
					//explore reverse
					while ((pBond0 = m_Bonds[pAtom1].rev) == pCycle->m_Bonds[pAtom1].fwd) {
						Common.m_Bonds[pAtom1].rev = pBond0;
						pAtom1 = (Atom*) pBond0->GetAtom (pAtom1);
						Common.m_Bonds[pAtom1].rev = pBond0;
					}
				} else {
					//explore forward
					while ((pBond = m_Bonds[pAtom2].fwd) == pCycle->m_Bonds[pAtom2].fwd) {
						Common.m_Bonds[pAtom2].fwd = pBond;
						pAtom2 = (Atom*) pBond->GetAtom (pAtom2);
						Common.m_Bonds[pAtom2].rev = pBond;
					}
					//explore reverse
					while ((pBond0 = m_Bonds[pAtom1].rev) == pCycle->m_Bonds[pAtom1].rev) {
						Common.m_Bonds[pAtom1].rev = pBond0;
						pAtom1 = (Atom*) pBond0->GetAtom (pAtom1);
						Common.m_Bonds[pAtom1].rev = pBond0;
					}
				}
				//Search if another portion is common to the two cycles
				Atom* pAtom = (Atom*) m_Bonds[pAtom2].fwd->GetAtom (pAtom2);
				while (pAtom != pAtom1) {
					if (pAtom->IsInCycle (pCycle))
						break;
					pAtom = (Atom*) m_Bonds[pAtom].fwd->GetAtom (pAtom);
				}
				if (pAtom != pAtom1) {
					pCycle = (*i).second.fwd->GetNextCycle (j, this);
					continue;
				}
				//Test for possible simplification
				bool bLargest = (m_Bonds.size () > pCycle->m_Bonds.size ());
				unsigned n = (bLargest) ? pCycle->m_Bonds.size () : m_Bonds.size ();
				if (2 * (Common.m_Bonds.size () - 1) > n) {
					if (bLargest) {
						Erase (pAtom1, pAtom2);
						if (bReverse) {
							pCycle->Extract (pAtom1, pAtom2, Common);
						} else {
							pCycle->Extract (pAtom2, pAtom1, Common);
							Common.Reverse ();
						}
						Insert (pAtom1, pAtom2, Common);
					} else {
						Extract (pAtom2, pAtom1, Common);
						if (bReverse) {
							pCycle->Erase (pAtom2, pAtom1);
							pCycle->Insert (pAtom2, pAtom1, Common);
						} else {
							Common.Reverse ();
							pCycle->Erase (pAtom1, pAtom2);
							pCycle->Insert (pAtom1, pAtom2, Common);
						}
					}
					Simplify ();
					return;
				}
				pCycle = (*i).second.fwd->GetNextCycle (j, this);
			}
		}
	}
}

void Cycle::Erase (Atom* pAtom1, Atom* pAtom2)
{
//This function is not safe
	Atom *pAtom = (Atom*) m_Bonds[pAtom1].fwd->GetAtom (pAtom1), *pAtom0;
	m_Bonds[pAtom1].fwd->RemoveCycle (this);
	m_Bonds[pAtom1].fwd = NULL;
	if (m_Bonds[pAtom1].rev->GetOrder () == 2)
		m_Bonds[pAtom1].rev->SetDirty (true);
	while (pAtom != pAtom2) {
		m_Bonds[pAtom].fwd->RemoveCycle (this);
		pAtom = (Atom*)m_Bonds[pAtom].fwd->GetAtom (pAtom0 = pAtom);
		m_Bonds.erase (pAtom0);
	}
	m_Bonds[pAtom2].rev = NULL;
	if (m_Bonds[pAtom2].fwd->GetOrder () == 2)
		m_Bonds[pAtom2].fwd->SetDirty (true);
}

void Cycle::Insert (Atom* pAtom1, Atom* pAtom2, Chain& chain)
{
//This function is not safe
	if (m_Bonds[pAtom1].rev->GetOrder () == 2)
		m_Bonds[pAtom1].rev->SetDirty (true);
	m_Bonds[pAtom1].fwd = ((Cycle&) chain).m_Bonds[pAtom1].fwd;
	m_Bonds[pAtom1].fwd->AddCycle (this);
	Atom *pAtom = (Atom*) m_Bonds[pAtom1].fwd->GetAtom (pAtom1);
	while (pAtom != pAtom2) {
		m_Bonds[pAtom] = ((Cycle&) chain).m_Bonds[pAtom];
		m_Bonds[pAtom].fwd->AddCycle (this);
		pAtom = (Atom*) m_Bonds[pAtom].fwd->GetAtom (pAtom);
	}
	m_Bonds[pAtom2].rev = ((Cycle&) chain).m_Bonds[pAtom2].rev;
	if (m_Bonds[pAtom2].fwd->GetOrder () == 2) m_Bonds[pAtom2].fwd->SetDirty (true);
}

bool Cycle::IsBetterForBonds (Cycle* pCycle)
{
	//Returns true if pCycle is better
	//Prefered cycle should have a maximum unsaturation number
	unsigned n1 = GetUnsaturations (), n2 = pCycle->GetUnsaturations ();
	if (n1 < n2)
		return true;
	else if (n1 > n2)
		return false;
	//Prefered cycle should be smallest
	n1 = m_Bonds.size ();
	n2 = pCycle->m_Bonds.size ();
	if (n1 > n2)
		return true;
	else if (n1 < n2)
		return false;
	//Prefered Cycle should have a maximum heteroatoms
	n1 = GetHeteroatoms ();
	n2 = pCycle->GetHeteroatoms ();
	if (n1 < n2)
		return true;
	else if (n1 > n2)
		return false;
	//Prefered cycle should have the least common bonds
	n1 = GetFusedBonds ();
	n2 = pCycle->GetFusedBonds ();
	if (n1 > n2)
		return true;
	else
		return false;
	//if every thing is the same return false (this will be prefered)
}

void Cycle::GetAngles2D (Bond *pBond, double* a1, double* a2)
{
	Atom *pAtom1 = (Atom*) pBond->GetAtom (0), *pAtom2 = (Atom*) pBond->GetAtom (1);
	if (m_Bonds[pAtom1].fwd == pBond) {
		*a1 = m_Bonds[pAtom1].rev->GetAngle2DRad (pAtom1);
		*a2 = m_Bonds[pAtom2].fwd->GetAngle2DRad (pAtom2);
	} else {
		*a1 = m_Bonds[pAtom1].fwd->GetAngle2DRad (pAtom1);
		*a2 = m_Bonds[pAtom2].rev->GetAngle2DRad (pAtom2);
	}
}

int Cycle::GetFusedBonds ()
{
	int n = 0;
	std::map<Atom*, ChainElt>::iterator i, end = m_Bonds.end ();
	for (i = m_Bonds.begin (); i != end; i++)
		if ((*i).second.fwd->IsCyclic() > 1) n++;
	return n;
}

}	//	namespace gcu
