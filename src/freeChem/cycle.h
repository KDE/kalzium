// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * libs/gcu/cycle.h 
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

#ifndef GCU_CYCLE_H
#define GCU_CYCLE_H
#include "chain.h"

/*!\file*/
namespace gcu {

/*! \class Cycle gcu/cycle.h

Represents a cycle.
*/
class Cycle: public Chain
{
public:
/*!
@param molecule a molecule.

Constructs an initially empty cycle for the molecule.
*/
	Cycle (Molecule* molecule);
/*!
The destructor.
*/
	virtual ~Cycle ();
	
/*!
Finds the smallest cycles collection in a polycyclic system.
*/
	void Simplify ();	//Reduce size of fused cycles
/*!
@param pAtom1 an atom in the source chain.
@param pAtom2 an atom in the source chain.

Erases a sub-chain. The result is not a cycle. This should be followed by
the insertion of a new chain to close the cycle again.
*/
	virtual void Erase (Atom* pAtom1, Atom* pAtom2);
/*!
@param pAtom1 an atom in the source chain.
@param pAtom2 an atom in the source chain.
@param chain the source chain.

Inserts a chain. Bothe atoms must already be in the target cycle and the
insertion should close the cycle.
*/
	virtual void Insert (Atom* pAtom1, Atom* pAtom2, Chain& chain);
/*!
@param pCycle another cycle sharing a bond.

Compares two cycles in competition to know which is the best to contain the
second line of a shared double bond.

@return true if the this instance is to be preferred, false otherwise.
*/
	bool IsBetterForBonds (Cycle* pCycle);
/*!
@param pBond a bond of the cycle.
@param a1 where to put the result for the first angle.
@param a2 where to put the result for the first angle.

Evaluates the angles between the bond and its two adjacent bonds in the cycle.
*/
	void GetAngles2D (Bond *pBond, double* a1, double* a2);
/*!
@return the number of bonds shared with other cycles.
*/
	int GetFusedBonds ();
};

}	//	namespace gcu

#endif // GCU_CYCLE_H
