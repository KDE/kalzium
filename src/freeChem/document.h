// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * chemistry/document.h 
 *
 * Copyright (C) 2004-2008 Jean Bréfort <jean.brefort@normalesup.org>
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


#ifndef GCU_DOCUMENT_H
#define GCU_DOCUMENT_H

#include "object.h"
// #include <gcu/dialog-owner.h>
#include "macros.h"
#include <string>
#include <set>

/*!\file*/
namespace gcu
{

// class Application;
// class Dialog;
class Molecule;
class Residue;

/*!\class Document gcu/document.h
This class is the base document class.
*/
class Document: public Object //, virtual public DialogOwner
{
friend class Object;
// friend class Dialog;
public:
/*!
@param App the Appllcation which owns the new document.

The default constructor. Creates an empty document.
*/
	Document (); //Application *App = NULL);
/*!
The destructor of Document.
*/
	virtual ~Document ();

public:

/*!
@param id the original id to tranlate

When pasting, objects added to the document might have the same Id as objects already existing. In such cases, the document
maintains a table to update links using Ids as identifiers. GetTranslatedId returns
the translated id corresponding to the parameter id.
*/
	std::string& GetTranslatedId (const char* id) {return m_TranslationTable[id];}

/*!
@param Id the entry to remove

When pasting, objects added to the document might have the same Id as objects already existing. In such cases, the document
maintains a table to update links using Ids as identifiers. The EraseTranslationTable method removes thenentry correspondig to id.
*/
	void EraseTranslationId (const char* Id) {m_TranslationTable.erase (Id);}

/*!
When pasting, objects added to the document might have the same Id as objects already existing. In such cases, the document
maintains a table to update links using Ids as identifiers. The EmptyTranslationTable method should be called after pasting to reinitialize the table
to avoid errors on the next paste event.
*/
	void EmptyTranslationTable() {m_TranslationTable.clear();}

/*!
@param title the new document title.
*/
	void SetTitle (std::string& title) {m_Title = title;}
/*!
@param title the new document title.
*/
	void SetTitle (char const *title) {m_Title = title;}
/*!
@return the current document title.
*/
	std::string &GetTitle () {return m_Title;}

/*!
@param pObject an object needing some update.

The gcu::Document class just stores dirty objects, but don't do anything with them. Derived classes
need to implement that, if meaningful.
*/
	void NotifyDirty (Object* pObject) {m_DirtyObjects.insert (pObject);}
/*!
Saves the document. Need to be overloaded by derived class if meaningful. Default
implementation doesn't do anything.
*/
	virtual void Save () const {;}
/*!
@param name the name of the new residue.
@param symbol the symbol of the new residue.
@param molecule a molecule with a pseudo atom which describes the structure
of the residue.

This methodshould be overloaded by derived classes necessitating it. Default
implementation just returns NULL.
@return the new Residue on success or NULL.
*/
	virtual Residue *CreateResidue (char const *name, char const *symbol, Molecule *molecule);
/*!
@param symbol the symbol for which a Residue* is searched.
@param ambiguous where to store the boolean telling if the symbol is ambiguous
or NULL.

Documents might own not global residues with the samesymbol or name
but a different meaning from the standard residue.
@return the Residue* found or NULL.
*/
	virtual Residue const *GetResidue (char const *symbol, bool *ambiguous = NULL);

private:

/*!
@param id: the original id
@param Cache: 

When pasting, objects added to the document might have the same Id as objects already existing. In such cases, the document
maintains a table to update links using Ids as identifiers. If Cache is set to true GetId adds a new entry in
the table.
GetNewId returns the translated id
*/
	char* GetNewId (char* id, bool Cache = true);

private:
	std::map <std::string, std::string> m_TranslationTable;//used when Ids translations are necessary (on pasting...)

protected:
/*!
The document title.
*/
	std::string m_Title;

/*!
		 The set of dirty objects, see gcu::Document::NotifyDirty.
*/
	std::set<Object*> m_DirtyObjects;

/*!\var m_App
The Application instance owning the document.
*/
/*!\fn GetApp()
@return a pointer to the Appication instance owning the ocument or NULL for
an orphan document.
*/
// Note application removed
// GCU_PROT_PROP (Application *, App)
/*!\var m_Empty
Tells if the document is empty or not.
*/
/*!\fn GetEmpty()
@return true if the document does not contain anything, false otherwise.
*/
GCU_PROT_PROP (bool, Empty);
/*!\fn SetScale(double scale)
@param scale the new scale factor.

Sets the new scale factor.
*/
/*!\fn GetScale()
@return the current scale factor.
*/
/*!\fn GetRefScale()
@return the current scale factor as a reference.
*/
GCU_PROP (double, Scale);
};


}
#endif	//GCU_DOCUMENT_H
