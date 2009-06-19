// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * residue.cc 
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

// #include "config.h"
#include "document.h"
#include "element.h"
#include "molecule.h"
#include "object.h"
#include "residue.h"
#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

namespace gcu {

class ResiduesTable {
public:
	ResiduesTable ();
	~ResiduesTable ();

	map<string, SymbolResidue> rtbs; // indexed by symbols
	map<string, Residue*> rtbn; // indexed by name
};

ResiduesTable::ResiduesTable ()
{
}

ResiduesTable::~ResiduesTable ()
{
	while (!rtbn.empty ())
		delete (*rtbn.begin ()).second;
}

static ResiduesTable tbl;

Residue::Residue ():
	m_Name (NULL),
	m_Generic (false),
	m_Document (NULL),
	m_Molecule (NULL),
	m_Owner (NULL)
{
}

Residue::Residue (char const *name, Document *doc):
	m_Generic (false),
	m_Document (NULL),
	m_Molecule (NULL),
	m_Owner (doc)
{
	if (name) {
		m_Name = strdup (name);
		if (!m_Owner)
			tbl.rtbn[name] = this;
	} else
		m_Name = NULL;
}

Residue::~Residue ()
{
	if (!m_Owner) {
		if (m_Name)
			tbl.rtbn.erase (m_Name);
		std::map<std::string, bool>::iterator i, end = m_Symbols.end ();
		for (i = m_Symbols.begin (); i != end ; i++)
			tbl.rtbs.erase ((*i).first);
	}
	free (const_cast<char*> (m_Name));
	if (m_Document)
		delete m_Document;
}

unsigned Residue::MaxSymbolLength = 0;

void Residue::SetName (char const *name)
{
	if (!m_Owner && m_Name)
		tbl.rtbn.erase (m_Name);
	free (const_cast<char*> (m_Name));
	m_Name = strdup (name);
	if (!m_Owner)
		tbl.rtbn[name] = this;
}

void Residue::AddSymbol (char const *symbol)
{
	bool ambiguous = Element::Z (symbol) > 0;
	m_Symbols[symbol] = ambiguous;
	if (!m_Owner) {
		tbl.rtbs[symbol].res = this;
		tbl.rtbs[symbol].ambiguous = ambiguous;
	}
	unsigned l = strlen (symbol);
	if (l > MaxSymbolLength)
		MaxSymbolLength = l;
}

void Residue::RemoveSymbol (char const *symbol)
{
	m_Symbols.erase (symbol);
	if (!m_Owner)
		tbl.rtbs.erase (symbol);
}

void Residue::Load (xmlNodePtr node)
{
	static char *lang = getenv ("LANG");
	if (m_Name)
		return;
	char *buf = reinterpret_cast <char *> (xmlGetProp (node, reinterpret_cast <xmlChar const *> ("generic"))); 
	if (buf) {
		if (!strcmp (buf, "true"))
			m_Generic = true;
		xmlFree (buf);
	}
	buf = reinterpret_cast <char *> (xmlGetProp (node, reinterpret_cast <xmlChar const *> ("raw"))); 
	if (buf) {
		int i = 0, j = 1, n, z = strlen (buf);
		char *end;
		while (i < z) {
			while (buf[j] > '9')
				j++;
			n = strtol (buf + j, &end, 10);
			buf[j] = 0;
			m_Raw[Element::Z (buf + i)] = n;
			i = end - buf;
			j = i + 1;
		}
		xmlFree (buf);
	}
	xmlNodePtr child = node->children;
	char *name = NULL, *node_lang, *symbols;
	bool lang_matched = false;
	while (child) {
		if (!strcmp (reinterpret_cast <char const *> (child->name), "name")) {
			node_lang = (char*) xmlNodeGetLang (child);
			if (node_lang) {
				if (lang) {
					if (!strcmp (lang, node_lang) || (!lang_matched && !strncmp (lang, node_lang, 2))) {
						if (name)
							xmlFree (name);
						name = reinterpret_cast <char *> (xmlNodeGetContent (child));
						lang_matched = true;
					}
				}
			} else if (!lang_matched) {
				if (name)
					xmlFree (name);
				name = reinterpret_cast <char *> (xmlNodeGetContent (child));
			}
			m_Names[(node_lang)? node_lang: "C"] = reinterpret_cast <char const *> (xmlNodeGetContent (child));
			if (node_lang)
				xmlFree (node_lang);
		} else if (!strcmp (reinterpret_cast <char const *> (child->name), "symbols")) {
			symbols = reinterpret_cast <char *> (xmlNodeGetContent (child));
			std::istringstream s(symbols);
			char buf[10];
			while (!s.eof ()) {
				s.getline(buf, 10, ';');
				if (strlen (buf) > 8) {
					// Symbols longer than 8 chars are not currently allowed
						cerr << "Symbol \"" << buf << "\" has more than eight characters and is not allowed" << endl;
					delete this;
					return;
				} else if (!m_Owner) {
					if (GetResidue (buf) != NULL) {
						cerr << "A residue with symbol \"" << buf << "\" already exists" << endl;
						delete this;
						return;
					}
					AddSymbol (buf);
				}
			}
			xmlFree (symbols);
		} else if (!strcmp (reinterpret_cast <char const *> (child->name), "molecule") && m_Molecule == NULL) {
			m_Document = dynamic_cast <Document *> (Object::CreateObject ("document"));
			if (m_Document) {
				m_Molecule = dynamic_cast <Molecule *> (Object::CreateObject ("molecule", m_Document));
				if (m_Molecule)
					m_Molecule->Load (child);
			}
		}
		child = child->next;
	}
	if (!m_Owner) {
		if (name) {
			if (GetResiduebyName (name) != NULL) {
				cerr << "A residue named \"" << name << "\" already exists" << endl;
				delete this;
				return;
			}
			SetName (name);
			xmlFree (name);
		} else
			delete this;
	}
}

Residue const *Residue::GetResidue (char const *symbol, bool *ambiguous)
{
	map<string, SymbolResidue>::iterator i = tbl.rtbs.find (symbol);
	if (i != tbl.rtbs.end ()) {
		if (ambiguous)
			*ambiguous = (*i).second.ambiguous;
		return (*i).second.res;
	} else
		return NULL;
}

Residue const *Residue::GetResiduebyName (char const *name)
{
	map<string, Residue*>::iterator i = tbl.rtbn.find (name);
	return (i != tbl.rtbn.end ())?  (*i).second: NULL;
}

string const *Residue::GetFirstResidueSymbol (ResidueIterator &i)
{
	i = tbl.rtbs.begin ();
	return (i == tbl.rtbs.end ())? NULL: &(*i).first;
}

string const *Residue::GetNextResidueSymbol (ResidueIterator &i)
{
	i++;
	return (i == tbl.rtbs.end ())? NULL: &(*i).first;
}

}	//	namespace gcu
