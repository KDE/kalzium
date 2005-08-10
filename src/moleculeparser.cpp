/***************************************************************************
    copyright            : (C) 2005 by Inge Wallin
    email                : inge@lysator.liu.se
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <ctype.h>

#include <kdebug.h>

#include "kalziumdataobject.h"
#include "moleculeparser.h"


MoleculeParser::MoleculeParser()
    : Parser()
{
}


MoleculeParser::MoleculeParser(QString _str)
    : Parser(_str)
{
}


MoleculeParser::~MoleculeParser()
{
    //Parser::~Parser();
}


// ----------------------------------------------------------------
//                            public methods


// Try to parse the molecule and get the weight of it.
//
// This method also acts as the main loop.

bool
MoleculeParser::weight(QString _molecule, double *_result)
{
    *_result = 0.0;
    start(_molecule);

    parseSubmolecule(_result);

    if (nextToken() != -1)
	return false;

    return true;
}


// ----------------------------------------------------------------
//            helper methods for the public methods


// Parse a submolecule.  This is a list of terms.
//

bool
MoleculeParser::parseSubmolecule(double *_result)
{
    double  subresult = 0.0;

    *_result = 0.0;
    while (parseTerm(&subresult))
	*_result += subresult;

    return true;
}


// Parse a term in a molecule.
//
// In this context, a term is an element or a submolecule in
// parenthesis followed by an optional number.  Return true if
// correct, otherwise return false.  If correct, the weight of the
// term is returned in *_result.
//

bool
MoleculeParser::parseTerm(double *_result)
{
    *_result = 0.0;
 
#if 0
    kdDebug() << "parseTerm(): Next token =  "
	      << nextToken() << endl;
#endif

    if (nextToken() == ELEMENT_TOKEN) {
	*_result = m_elementVal->mass();
	getNextToken();
    }

    else if (nextToken() == '(') {
	// A submolecule.

	getNextToken();
	parseSubmolecule(_result);

	// Must end in a ")".
	if (nextToken() == ')')
	    getNextToken();
	else
	    return false;
    }

    else 
	// Neither an element nor a list within ().
	return false;

    // Optional number.
    if (nextToken() == INT_TOKEN) {
	*_result *= intVal();
	getNextToken();
    }

    return true;
}


// ----------------------------------------------------------------
//                           protected methods


// Extend Parser::getNextToken with elements.

int
MoleculeParser::getNextToken()
{
    QString  elementName;

#if 0
    kdDebug() << "getNextToken(): Next character = "
	      << nextChar() << endl;
#endif

    // Check if the token is an element name.
    if ('A' <= nextChar() && nextChar() <= 'Z') {
	elementName = char(nextChar());
	getNextChar();

	if ('a' <= nextChar() && nextChar() <= 'z') {
	    elementName.append(char(nextChar()));
	    getNextChar();
	}

	// Look up the element from the name..
	m_elementVal = lookupElement(elementName);
	if (m_elementVal)
	    m_nextToken = ELEMENT_TOKEN;
	else
	    m_nextToken = -1;
    }
    else
	return Parser::getNextToken();

    return m_nextToken;
}


// ----------------------------------------------------------------
//                          private methods


Element *
MoleculeParser::lookupElement(QString _name)
{
    EList  elementList = KalziumDataObject::instance()->ElementList;;

    //kdDebug() << "looking up " << _name << endl;

    EList::ConstIterator        it  = elementList.begin();
    const EList::ConstIterator  end = elementList.end();

    for (; it != end; ++it) {
	if ( (*it)->symbol() == _name )
	    return *it;
    }

    return NULL;
}

