// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * residue.h 
 *
 * Copyright (C) 2007-2008 Jean Bréfort <jean.brefort@normalesup.org>
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

#ifndef GCU_RESIDUE_H
#define GCU_RESIDUE_H

#include "macros.h"
#include <libxml/parser.h>
#include <map>
#include <set>
#include <string>

/*!\file*/
namespace gcu {

class Residue;
class Molecule;
class Document;

/*!\struct SymbolResidue gcu/residue.h
Structure used to store a collection of standard atoms groups, ndexed by symbol.
*/
typedef struct {
/*!
The Residue* representing the atoms group. A symbol is ambiguous if it can also
represent a single atom, such as Ac, or Pr.
Two Residue instances can't share the same symbol.
*/
	Residue *res;
/*!
whether the symbol is ambiguous.
*/
	bool ambiguous;
} SymbolResidue;

typedef std::map<std::string, SymbolResidue>::iterator ResidueIterator;

/*!\class Residue gcu/residue.h
Represents the meaning of group symbols, such as Me, Ph or Bz. They have one
or more associated symbols and a molecule which atoms and bonds are represented
by the symbol. The molecule starts from a pseudo-atom with Z = 0, which
represents the atom linked to the group. Currently only one pseudo-atom with
one single bond to the group is allowed.
*/
class Residue
{
public:
/*!
Creates a residue.
*/
	Residue ();
/*!
@param name the name of the residue.
@param doc the document owning the residue when it does not have global scope.

Creates a residue and sets its name.
*/
	Residue (char const *name, Document *doc = NULL);
/*!
The destructor.
*/
	virtual ~Residue ();

/*!
@return the raw formula of the residue as a map of integers (number of atoms
of an element) indexed by the atomic number of the element.

The following code gives the number of carbon atoms:
\code
		Residue *residue;
		... // initialize the residue
		cout << "the residue has " << residue->GetRawFormula ()[6] << " carbon atoms." << endl;

\endcode
			
*/
	std::map<int,int> const &GetRawFormula () const {return m_Raw;}
/*!
@return the symbols of the atoms group as a std::map. The symbols index boolean
values telling whether the symbol is ambiguous or not.

A symbol is ambiguous if it can also represent a single atom, such as Ac, or Pr.
*/
	std::map<std::string, bool> const &GetSymbols () const {return m_Symbols;}
/*!
@return the names of the atoms group, as a std::map indexed by the
correspondig locale.
*/
	std::map<std::string, std::string> const &GetNames () const {return m_Names;}
/*!
@param name the name to give to the Residue.
*/
	void SetName (char const *name);
/*!
@param symbol a chemical symbol.

Adds \a symbol to the list of valid symbols for the atoms group.
*/
	void AddSymbol (char const *symbol);
/*!
@param symbol a chemical symbol.

Removes \a symbol from the list of valid symbols for the atoms group.
*/
	void RemoveSymbol (char const *symbol);
/*!
@param node a XMLNode* representing the residue.

loads the residu from an XML file.
*/
	virtual void Load (xmlNodePtr node);
/*!
@param symbol the symbol for which a Residue* is searched.
@param ambiguous where to store the boolean telling if the symbol is ambiguous
or NULL.

@return the Residue* found or NULL.
*/
	static Residue const *GetResidue (char const *symbol, bool *ambiguous = NULL);
/*!
@param name the name for which a Residue* is searched.

@return the Residue* found or NULL.
*/
	static Residue const *GetResiduebyName (char const *name);
/*!
@param i an iterator.

Used to iterate through all known atoms groups and their symbols. Initializes
the iterator.
@return the first known symbol. If not NULL, the iterator gives access to
the associated Residue througn a gcu::SymbolResidue structure.
*/
	static std::string const *GetFirstResidueSymbol (ResidueIterator &i);
/*!
@param i an iterator initialized by Residue::GetFirstResidueSymbol.

Used to iterate through all known atoms groups and their symbols. Initializes
the iterator.
@return the next known symbol. If not NULL, the iterator gives access to
the associated Residue through a gcu::SymbolResidue structure.

*/
	static std::string const *GetNextResidueSymbol (ResidueIterator &i);
/*!
@param mol a molecule to compare to the atoms group.
@return true if the molecule is the same as the one associated with \a this.
*/
	virtual bool operator== (Molecule const &mol) const {return false;}

public:
/*!
@return the maxium length of all known symbols. This is used by the framework
when parsing a string formula.
*/
	static unsigned MaxSymbolLength;

private:
	std::map<int,int> m_Raw;
	std::map<std::string, bool> m_Symbols; // boolean is true if the symbol is ambiguous
	std::map<std::string, std::string> m_Names;

/*!\fn GetName()
@return the name of the Residue instance.
*/
GCU_RO_PROP (char const *, Name)
/*!\fn SetGeneric(bool Generic)
@param Generic whether the symbol is a generic symbol or not.

Generic symbols are such symbols as R, Ar, or X which might represent various
atoms or groups of atoms. When expanded, they will be replaced by a default
value, for example, R will expand to a methyl, and Ar to a phenyl group.
*/
/*!\fn GetGeneric()
@return whether the symbol is a generic symbol or not.
*/
/*!\fn GetRefGeneric()
@return whether the symbol is a generic symbol or not as a reference to a
boolean variable.
*/
GCU_PROP (bool, Generic)
/*!\fn GetDocument()
Returns the gcu::Document* used to store the molecule associated with
the atoms group.
*/
GCU_PROT_POINTER_PROP (Document, Document);
/*!\fn GetMolecule()
Returns the gcu::Molecule* describing the structure of the atoms group.

*/
GCU_PROT_POINTER_PROP (Molecule, Molecule);
/*!\fn GetOwner()
@return the Document for which the Residue is valid. If NULL,
the Residue is of global scope.
*/
GCU_RO_PROP (Document *, Owner)
};

}	//	namespace gcu

#endif	//	GCU_RESIDUE_H
