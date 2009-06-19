// -*- C++ -*-

/* 
 * Gnome Chemistry Utils
 * formula.h 
 *
 * Copyright (C) 2005-2008 Jean Bréfort <jean.brefort@normalesup.org>
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

#ifndef GCU_FORMULA_H
#define GCU_FORMULA_H

#include <string>
#include <map>
#include <list>
#include <stdexcept>
#include "isotope.h"
#include "macros.h"
#include "value.h"

/*!\file*/
namespace gcu
{

/*!\enum FormulaParseMode gcu/formula.h
The way formula should be interpreted when they contain symbols that might
represent both an atom or a group of atoms such as Ac and Pr. Possible values
are:
- GCU_FORMULA_PARSE_GUESS: try to determine the most probable interpretation of
the symbol,
- GCU_FORMULA_PARSE_ATOM: always interpret as atoms,
- GCU_FORMULA_PARSE_RESIDUE: always interpret as groups,
- GCU_FORMULA_PARSE_ASK: ask the user for each encountered instance,
- GCU_FORMULA_PARSE_NO_CASE: this one is unrelated and can ba combined with any
of the previous values, just make the parsing stricktly case sensitive.
*/
typedef enum {
	GCU_FORMULA_PARSE_GUESS,
	GCU_FORMULA_PARSE_ATOM,
	GCU_FORMULA_PARSE_RESIDUE,
	GCU_FORMULA_PARSE_ASK,
	GCU_FORMULA_PARSE_NO_CASE=8
} FormulaParseMode;

/*!\class parse_error gcu/formula.h
Exception class derived from std::exception used for errors encountered
when parsing a formula.
*/

class parse_error: public std::exception
{
public:
/*! Takes a character string describing the error and two integers
* indicating where the error occured.
*/
    explicit 
    parse_error (const std::string&  __arg, int start, int length);

    virtual 
    ~parse_error () throw ();

/*! Returns a C-style character string describing the general cause of
 *  the current error (the same string passed to the constructor).
*/
    virtual const char* 
    what () const throw ();
/*! Returns a C-style character string describing the general cause of
 *  the current error (the same string passed to the constructor).
*/
    const char* 
    what (int& start, int& length) const throw ();

/*! Adds an offset to the start of the error. This is used by the Formula
class when an exception occurs while parsing a substring.
*/
	void add_offset (int offset) {m_start += offset;}

private:
	std::string m_msg;
	int m_start, m_length;

};

/*! class FormulaElt gcu/formula.h
Bas class for elements found in a formula.
*/

class FormulaElt
{
public:
/*!
The constructor.
*/
	FormulaElt ();
/*!
The destructor.
*/
	virtual ~FormulaElt ();
/*!
@return the string representation of the element as a markup.
*/
	virtual std::string Markup ();
/*!
@return the string representation of the element as raw text.
*/
	virtual std::string Text ();
/*!
@param raw where to put the result

Adds the number of atoms of each element to the map taking the stoichiometry
coefficient into account.
*/
	virtual void BuildRawFormula (std::map<int, int> &raw) = 0;
/*!
@return the (most probable) valence of the element.
*/
	virtual int GetValence () = 0;
/*!
The stoichiometry coefficient associated with he element.
*/
	int stoich;
/*!
The start index in bytes of the element in the formula input string.
*/
	unsigned start;
/*!
The end index in bytes of the element in the formula input string.
*/
	unsigned end;
};

/*! class FormulaAtom gcu/formula.h
Represents an atom in a formula.
*/

class FormulaAtom: public FormulaElt
{
public:
/*!
@param Z
*/
	FormulaAtom (int Z);
/*!
The destructor.
*/
	virtual ~FormulaAtom ();
/*!
@return the string representation of the element as a markup.
*/
	std::string Markup ();
/*!
@return the string representation of the element as raw text.
*/
	std::string Text ();
/*!
@param raw where to put the result

Adds the stoichiometry coefficient to the entry correponding to the element.
*/
	void BuildRawFormula (std::map<int, int> &raw);
/*!
@return the (most probable) valence of the element.
*/
	int GetValence ();
/*!
*/
	int elt;
};

/*! class FormulaBlock gcu/formula.h
Represents a sub formula delimited by a pair of matching brackets.
*/

class FormulaBlock: public FormulaElt
{
public:
/*!
*/
	FormulaBlock ();
/*!
The destructor.
*/
	virtual ~FormulaBlock ();
/*!
@return the string representation of the element as a markup.
*/
	std::string Markup ();
/*!
@return the string representation of the element as raw text.
*/
	std::string Text ();
/*!
@param raw where to put the result

Adds the number of atoms of each element to the map taking the stoichiometry
coefficient into account.
*/
	void BuildRawFormula (std::map<int, int> &raw);
/*!
@return the (most probable) valence of the element.
*/
	int GetValence ();
/*!
The list of the formula elements delimited by the pair of brackets.
*/
	std::list<FormulaElt *> children;
/*!
0 for "()", 1 for "[]" or 2 for "{}".
*/
	int parenthesis;
};

class Residue;

/*! class FormulaResidue gcu/formula.h
*/

class FormulaResidue: public FormulaElt
{
public:
/*!
@param res a gcu::Residue.
@param symbol the atoms group symbol
@param Z the atomic number of the element with the same symbol or 0.
*/
	FormulaResidue (Residue const *res, char const *symbol, int Z);
/*!
The destructor.
*/
	virtual ~FormulaResidue ();
/*!
@return the string representation of the element as a markup.
*/
	std::string Markup ();
/*!
@return the string representation of the element as raw text.
*/
	std::string Text ();
/*!
@param raw where to put the result

Adds the number of atoms of each element to the map taking the stoichiometry
coefficient into account.
*/
	void BuildRawFormula (std::map<int, int> &raw);
/*!
@return the (most probable) valence of the element.
*/
	int GetValence ();
/*!
The gcu::Residue correponding to the found symbol.
*/
	Residue const *residue;
/*!
The symbol of the atoms group.
*/
	std::string Symbol;
/*!\fn GetZ()
@return the atomic number of the element with the same symbol or 0.
*/
GCU_RO_PROP (int, Z);
};


/*!\class Formula gcu/formula.h
This class interprets a chemical formula provided as a string and make
some calculations using it. Currently, it is able to calculate the raw formula,
the molecular weight and the isotopic pattern.
*/
class Formula
{
public:
/*!
@param entry: the formula to parse as a string.
@param mode: the way ambiguous symbols shoud be interpreted.
The constructor will emit a parse_error exception.
if it cannot parse the given formula.
*/
	Formula (std::string entry, FormulaParseMode mode = GCU_FORMULA_PARSE_GUESS) throw (parse_error);

/*!
The destructor.
*/
	virtual ~Formula ();

/*!
@return the original formula as a pango markup.
*/
	char const *GetMarkup ();
/*!
@return the raw formula as a map of atoms numbers indexed by atomic number Z.
*/
	std::map<int,int> &GetRawFormula ();
/*!
@return the raw formula as a pango markup.
*/
	char const *GetRawMarkup ();
/*!
@param entry: the formula to parse as a string.
Calls Formula::Clear before parsing the new formula.
The method will emit a parse_error exception
if it cannot parse the given formula.
*/
	void SetFormula (std::string entry) throw (parse_error);
/*!
Clears all data.
*/
	void Clear ();
/*!
@param artificial: will be true if the formula contains an artificial element (with
no natural isotope).
@returns the molecular weight corresponding to the formula.
*/
	DimensionalValue GetMolecularWeight (bool &artificial);
/*!
@param pattern: the IsotopicPattern to be filled
This method evaluates the isotopic pattern and fills the pattern parameter
with the calculated data.
*/
	void CalculateIsotopicPattern (IsotopicPattern &pattern);

/*!
Returns the parsed formula as a list of elements.
*/
	std::list<FormulaElt *> const &GetElements () const {return Details;}

private:
	bool BuildConnectivity ();
	void Parse (std::string &formula, std::list<FormulaElt *>&result) throw (parse_error);
	bool AnalString (char *sz, std::list<FormulaElt *> &result, bool &ambiguous, int offset);
	bool TryReplace (std::list<FormulaElt *> &result, std::list<FormulaElt *>::iterator it);
		
private:
	std::string Entry, Markup, RawMarkup;
	std::map<int,int> Raw;
	std::list<FormulaElt *> Details;
	DimensionalValue m_Weight;
	bool m_WeightCached;
	bool m_Artificial;
	bool m_ConnectivityCached;

/*!\fn SetParseMode(FormulaParseMode ParseMode)
@param ParseMode the new FormulaParseMode.

Sets the way ambiguous symbols are interpreted.
*/
/*!\fn GetParseMode()
@return the current parse mode.
*/
/*!\fn GetRefParseMode()
@return the current parse mode as a reference.
*/
GCU_PROP (FormulaParseMode, ParseMode);
};
	
}

#endif // GCU_FORMULA_H
