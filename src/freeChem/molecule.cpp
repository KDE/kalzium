// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * molecule.h 
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
#include "molecule.h"
#include "atom.h"
#include "bond.h"
#include "chain.h"
#include "cycle.h"
#include "document.h"
#include "formula.h"
#include "residue.h"
#include <stack>
#include <iostream>

using namespace std;

namespace gcu
{

Molecule::Molecule (TypeId Type): Object (Type)
{
}

Molecule::Molecule (Atom* pAtom): Object (MoleculeType)
{
	SetParent (pAtom->GetDocument ());
	AddAtom (pAtom);
	Chain* pChain = new Chain (this, pAtom); //will find the cycles
	delete pChain;
}

Molecule::~Molecule ()
{
	Clear ();
}

void Molecule::Clear ()
{
	std::list<Bond*>::iterator n, end = m_Bonds.end ();
	for (n = m_Bonds.begin (); n != end; n++)
		(*n)->RemoveAllCycles ();
	while (!m_Cycles.empty ()) {
		delete m_Cycles.front ();
		m_Cycles.pop_front ();
	}
	while (!m_Chains.empty ()) {
		delete m_Chains.front ();
		m_Chains.pop_front ();
	}
}

void Molecule::AddChild (Object* object)
{
	switch (object->GetType ()) {
	case AtomType: {
		Atom *atom = reinterpret_cast<Atom *> (object);
		AddAtom (atom);
		break;
	}
	case gcu::BondType: {
		Bond *bond = reinterpret_cast<Bond *> (object);
		m_Bonds.remove (bond);
		AddBond (bond);
		break;
	}
	}
}

void Molecule::AddAtom (Atom* pAtom)
{
	m_Atoms.remove (pAtom); // avoid duplicates
	m_Atoms.push_back (pAtom);
	Object::AddChild (pAtom);
}

void Molecule::AddBond (Bond* pBond)
{
	m_Bonds.remove (pBond); // avoid duplicates
	m_Bonds.push_back (pBond);
	Object::AddChild (pBond);
}

void Molecule::Remove (gcu::Object* pObject)
{
	switch (pObject->GetType ()) {
	case AtomType:
		m_Atoms.remove ((Atom*) pObject);
		break;
	case gcu::BondType:
		m_Bonds.remove ((Bond*) pObject);
		break;
	}
	pObject->SetParent (GetParent ());
}


void Molecule::UpdateCycles (Bond* pBond)	//FIXME: function not totally implemented
{
	Chain* pChain = new Chain (this, pBond); //will find the cycles
	delete pChain;
}

void Molecule::UpdateCycles ()
{
	Lock (true);
	std::list<Bond*>::iterator n, end = m_Bonds.end ();
	for (n = m_Bonds.begin (); n != end; n++)
		(*n)->RemoveAllCycles ();
	while (!m_Cycles.empty()) {
		delete m_Cycles.front ();
		m_Cycles.pop_front ();
	}
	if (!m_Atoms.empty()) {
		std::list<Atom*>::iterator i = m_Atoms.begin (), end = m_Atoms.end ();
		i++;
		for (; i != end; i++)
			(*i)->SetParent(NULL);
		Chain* pChain = new Chain (this, *(m_Atoms.begin ())); //will find the cycles
		delete pChain;
		end = m_Atoms.end ();
		list<Atom*> orphans;
		for (i = m_Atoms.begin (); i != end; i++)
			if ((*i)->GetParent () == NULL)
				orphans.push_back (*i);
		end = orphans.end ();
		for (i = orphans.begin (); i != end; i++)
			(*i)->SetParent (this);
	}
	Lock (false);
}

bool Molecule::operator== (Molecule const& molecule) const
{
	// first examine each atom of each molecule and sort by Z.
	map<int, set<Atom*> > atoms1, atoms2;
	list<Atom*>::const_iterator ia, enda = m_Atoms.end ();
	for (ia = m_Atoms.begin ();  ia != enda; ia++)
		atoms1[(*ia)->GetZ ()].insert (*ia);
	enda = molecule.m_Atoms.end ();
	for (ia = molecule.m_Atoms.begin ();  ia != enda; ia++){
		atoms2[(*ia)->GetZ ()].insert (*ia);
	}
	if (atoms1.size () != atoms2.size ())
		return false;
	map<int, set<Atom*> >::iterator ib, endb = atoms1.end (), ic, endc = atoms2.end ();
	unsigned n = m_Atoms.size (), m;
	int z = 200;
	for (ib = atoms1.begin (); ib != endb; ib++) {
		if ((ic = atoms2.find ((*ib).first)) == endc)
			return false;
		if ((m = (*ib).second.size ()) != (*ib).second.size ())
			return false;
		if (m < n)
			n = m;
		if (m == n && (*ib).first < z)
			z = (*ib).first;
	}
	AtomMatchState state;
	state.atoms.reserve (GetAtomsNumber ());
	if (z == 200) {
		return false; // should do something more meaningful the molecule contains no normal atoms, only groups, probably
	}
	Atom *atom = *atoms1[z].begin (); // take the firts atom, and try to match it with the other molecule atoms of same Z.
	set<Atom*> &starters = atoms2[z];
	set<Atom*>::iterator j, jend = starters.end ();
	for (j = starters.begin (); j != jend; j++)
		if (atom->Match (*j, state))
			return true;
	return false;
}

Molecule *Molecule::MoleculeFromFormula (Document *Doc, Formula const &formula, bool add_pseudo)
{
	Molecule *mol = reinterpret_cast <Molecule*> (Object::CreateObject ("molecule", Doc));
	if (!mol)
		return NULL;
	stack <Atom*> atoms;
	Atom *atom = NULL;
	list<FormulaElt *> const &elts = formula.GetElements ();
	list<FormulaElt *>::const_reverse_iterator i, iend = elts.rend ();
	FormulaAtom *fatom;
	FormulaResidue *fresidue;
	int valence, in;
	unsigned PendingHs = 0;
	stack<Atom*> PendingAtoms;
	Bond *bond;
	bool done = false;
	for (i = elts.rbegin (); i != iend; i++) {
		if (done) {
			cout<<"Can't interpret formula"<<endl;
			// destroy the molecule
			mol->SetParent (NULL);
			delete mol;
			return NULL;			
		}
		if ((fatom = dynamic_cast <FormulaAtom *> (*i))) {
			valence = fatom->GetValence ();
			if (valence == 1) {
				if (fatom->elt == 1)
					PendingHs += fatom->stoich;
				else for (in = 0; in < fatom->stoich; in++) {
					atom = reinterpret_cast <Atom*> (CreateObject ("atom", mol));
					atom->SetZ (fatom->elt);
					PendingAtoms.push (atom);
				}
			} else {
				int n = valence - PendingHs - PendingAtoms.size ();
				if (n == 1) {
					atom = reinterpret_cast <Atom*> (CreateObject ("atom", mol));
					atom->SetZ (fatom->elt);
					while (!PendingAtoms.empty ()) {
						// FIXME: we do not support multiple bonds !!!
						bond = reinterpret_cast <Bond*> (CreateObject ("bond", mol));
						bond->SetOrder (1);
						bond->ReplaceAtom (NULL, atom);
						bond->ReplaceAtom (NULL, PendingAtoms.top ());
						PendingAtoms.top ()->AddBond (bond);
						PendingAtoms.pop ();
					}
					PendingAtoms.push (atom);
					PendingHs = 0;
				} else if (n == 0) {
					atom = reinterpret_cast <Atom*> (CreateObject ("atom", mol));
					atom->SetZ (fatom->elt);
					while (!PendingAtoms.empty ()) {
						// FIXME: we do not support multiple bonds !!!
						bond = reinterpret_cast <Bond*> (CreateObject ("bond", mol));
						bond->SetOrder (1);
						bond->ReplaceAtom (NULL, atom);
						bond->ReplaceAtom (NULL, PendingAtoms.top ());
						PendingAtoms.top ()->AddBond (bond);
						PendingAtoms.pop ();
					}
					done = true;
				}
			}
		} else if ((fresidue = dynamic_cast <FormulaResidue *> (*i))) {
			// get the residue molecule and duplicate it
			map<Atom*, Atom*> Corr;
			Residue const *residue = Doc->GetResidue (fresidue->Symbol.c_str ());
			Molecule const *orig = residue->GetMolecule ();
			Atom *pseudo = NULL;
			std::list<Atom*>::const_iterator ai, aiend = orig->m_Atoms.end ();
			for (in = 0; in < fresidue->stoich; in++) {
				for (ai = orig->m_Atoms.begin ();  ai != aiend; ai++) {
					if ((*ai)->GetZ () == 0)
						pseudo = atom = reinterpret_cast <Atom*> (CreateObject ("pseudo-atom", mol));
					else {
						atom = reinterpret_cast <Atom*> (CreateObject ("atom", mol));
						*atom = *(*ai);
					}
					atom->SetId ("a1");
					Corr[*ai] = atom;
					atom->SetId ("a1");
				}
				std::list<Bond*>::const_iterator bi, biend = orig->m_Bonds.end ();
				for (bi = orig->m_Bonds.begin ();  bi != biend; bi++) {
					// stereochemistry is lost for now
					bond = reinterpret_cast <Bond*> (CreateObject ("bond", mol));
					bond->SetId ("b1");
					bond->SetOrder ((*bi)->GetOrder ());
					bond->ReplaceAtom (NULL, Corr[(*bi)->GetAtom (0)]);
					bond->ReplaceAtom (NULL, Corr[(*bi)->GetAtom (1)]);
					Corr[(*bi)->GetAtom (1)]->AddBond (bond);
				}
				// remove the pseudo-atom
				// FIXME: we drop the orientation of the bond and the positionof the pseudo-atom
				// which will make 2D autogeneration problematic
				map<Atom*, Bond*>::iterator ci;

				bond = pseudo->GetFirstBond (ci);
				atom = bond->GetAtom (pseudo);
				if (atom) {
					PendingAtoms.push (atom);
					atom->RemoveBond (bond);
				}
				mol->Remove (bond);
				delete bond;
				mol->Remove (pseudo);
				delete pseudo;
			}
		} else {
			// FIXME: need to support blocks as well
			mol->SetParent (NULL); // ensure children wil be destroyed
			delete mol;
			return NULL;
		}
	}
	if (add_pseudo) {
		if (PendingHs + PendingAtoms.size () != 1) {
			mol->SetParent (NULL); // ensure children wil be destroyed
			delete mol;
			return NULL;
		}
		atom = reinterpret_cast <Atom*> (Object::CreateObject ("pseudo-atom", mol));
		bond = reinterpret_cast <Bond*> (CreateObject ("bond", mol));
		bond->SetOrder (1);
		bond->ReplaceAtom (NULL, atom);
		if (PendingAtoms.size () > 0)
			atom = PendingAtoms.top ();
		else {
			atom = reinterpret_cast <Atom*> (Object::CreateObject ("atom", mol));
			atom->SetZ (1);
		}
		bond->ReplaceAtom (NULL, atom);
		atom->AddBond (bond);
		PendingAtoms.pop ();
	} else if (PendingHs + PendingAtoms.size () == 2) {
		bond = reinterpret_cast <Bond*> (CreateObject ("bond", mol));
		bond->ReplaceAtom (NULL, PendingAtoms.top ());
		PendingAtoms.pop ();
		bond->ReplaceAtom (NULL, PendingAtoms.top ());
	} else if (PendingHs + PendingAtoms.size () != 0) {
		mol->SetParent (NULL); // ensure children wil be destroyed
		delete mol;
		return NULL;
	}
	return mol;
}

Atom const *Molecule::GetFirstAtom (std::list<Atom*>::const_iterator &i) const
{
	i = m_Atoms.begin ();
	return (i == m_Atoms.end ())? NULL: *i;
}

Atom const *Molecule::GetNextAtom (std::list<Atom*>::const_iterator &i) const
{
	i++;
	return (i == m_Atoms.end ())? NULL: *i;
}

Bond const *Molecule::GetFirstBond (std::list<Bond*>::const_iterator &i) const
{
	i = m_Bonds.begin ();
	return (i == m_Bonds.end ())? NULL: *i;
}

Bond const *Molecule::GetNextBond(std::list<Bond*>::const_iterator &i) const
{
	i++;
	return (i == m_Bonds.end ())? NULL: *i;
}

}	//namespace gcu
