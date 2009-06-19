// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * libs/gcu/chain.h 
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

#ifndef GCU_CHAIN_H
#define GCU_CHAIN_H

#include "object.h"
#include <map>

/*!\file*/
namespace gcu {

class Atom;
class Bond;
class Molecule;

/*!\struct ChainElt gcu/chain.h
This structure stores the two bonds of sharing one atom in a chain. One
(or both) can be NULL if the atom is at start or end of the chain.

*/
typedef struct
{
/*!
The bond in the forward direction
*/
	Bond *fwd;
/*!
The bond in the backward direction
*/
	Bond *rev;
} ChainElt;

/*!\class Chain gcu/chain.h
This class is used to represent chains of atoms.

*/
class Chain: public Object
{
public:
/*!
@param pBond a bond.
@param pAtom an atom.
@param Type an optional type id used when constructing a derived type
(i. e. a cycle).

Constructs a Chain starting from a bond and an optional atom. Choosing the atom
forces the direction of the chain, otherwise, the first atom will be the
first atom of the bond.
*/
	Chain (Bond* pBond, Atom* pAtom = NULL, TypeId Type = ChainType);
/*!
@param molecule a molecule.
@param pBond a bond in the molecule or NULL.
@param Type an optional type id used when constructing a derived type

Explores a molecule and find all cycles, starting from pBond or an empty chain
if pBond is NULL.
*/
	Chain (Molecule* molecule, Bond* pBond, TypeId Type = ChainType);
/*!
@param molecule a molecule.
@param pAtom an atom in the molecule.
@param Type an optional type id used when constructing a derived type

Explores a molecule and find all cycles, starting from pAtom or an empty chain
if pAtom is NULL.
*/
	Chain (Molecule* molecule, Atom* pAtom, TypeId Type = ChainType);
/*!
The destructor.
*/
	virtual ~Chain();
	
/*!
@param pAtom an atom.

Searches all cycles in a molecule starting from Atom pAtom. Found cycles are
added to the molecule cycles list.
*/
	void FindCycles (Atom* pAtom);
/*!
@param pAtom an atom.
@param pBond a bond.

Searches for a cycle containing the atom pAtom and the bond pBond. pAtom must
be one of the ends of pBond. If a cycle is found, it is added to the molecule
cycles list.
@return true on success, false otherwise.
*/
	bool FindCycle (Atom* pAtom, Bond* pBond);
/*!
@param pAtom1 an atom in the source chain.
@param pAtom2 an atom in the source chain.

Erases a sub-chain. One of the atoms must already be one of the chain ends.
*/
	virtual void Erase (Atom* pAtom1, Atom* pAtom2);
/*!
@param pAtom1 an atom in the source chain.
@param pAtom2 an atom in the source chain.
@param chain the source chain.

Inserts a chain. One of the atoms must already be in the target chain.
*/
	virtual void Insert (Atom* pAtom1, Atom* pAtom2, Chain& chain);
/*!
@param pAtom1 an atom in the chain.
@param pAtom2 an atom in the chain.
@param chain the target chain.

Extracts a sub-chain to the chain variable which should be empty before the call.
*/
	void Extract (Atom* pAtom1, Atom* pAtom2, Chain& chain);
/*!
Reverses the chain order.
*/
	void Reverse ();
/*!
@param start an atom.
@param end an atom.

Adds the two atoms and the bond between them to the chain. The two atoms must be bonded.
*/
	void AddBond (Atom* start, Atom* end);
/*!
@return the number of multiple bonds in the chain so not really the unsaturations number.
*/
	unsigned GetUnsaturations ();
/*!
@return the number of heteroatoms in the chain.
*/
	unsigned GetHeteroatoms ();
/*!
@param pAtom an atom.

@return true if pAtom is in the chain.
*/
	bool Contains (Atom* pAtom);
/*!
@param pBond a bond.

@return true if pBond is in the chain.
*/
	bool Contains (Bond* pBond);
/*!
@return the number of bonds in the chain
*/
	unsigned GetLength ();
/*!
@return the mean bond length in the chain.
*/
	double GetMeanBondLength ();
/*!
@param pAtom an atom in the chain.
@return the next atom in the chain or NULL if pAtom is the last one.
*/
	Atom* GetNextAtom (Atom* pAtom);

protected:
/*!
	 The gcu::ChainElt elements in the chain indexed by their common atom.
*/
	std::map<Atom*, ChainElt> m_Bonds;
/*!
The molecule which owns the chain.
*/
	Molecule* m_Molecule;
};

}	//	namespace gcu

#endif // GCHEMPAINT_CHAIN_H
