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

#include "moleculeparser.h"

#include <ctype.h>

#include <kdebug.h>

#include "kalziumdataobject.h"

MoleculeParser::MoleculeParser()
    : Parser()
{
}


MoleculeParser::MoleculeParser(const QString& _str)
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
	m_elementMap.clear();
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
    while (parseTerm(&subresult)) {
	//kdDebug() << "Parsed a term, weight = " << subresult << endl;
	*_result += subresult;
    }

    return true;
}


// Parse a term in a molecule.
//
// In this context, a term is an element or a submolecule in
// parenthesis followed by an optional number.  Return true if
// correct, otherwise return false.  If correct, the weight of the
// term is returned in *_result.
//

QValueList<Element*> MoleculeParser::elementList()
{
	QValueList<Element*> el;
	QMap<Element*, int>::ConstIterator it = m_elementMap.constBegin();
	QMap<Element*, int>::ConstIterator itEnd = m_elementMap.constEnd();
	for ( ; it != itEnd; ++it ) {
		for ( int i = 0; i < it.data(); i++ )
			el.append( it.key() );
        }
	return el;
}

QMap<Element*, int> MoleculeParser::elementMap()
{
	return m_elementMap;
}

bool
MoleculeParser::parseTerm(double *_result)
{
    *_result = 0.0;
 
#if 0
    kdDebug() << "parseTerm(): Next token =  "
	      << nextToken() << endl;
#endif
    bool maybeadded = false;

    if (nextToken() == ELEMENT_TOKEN) {
	//kdDebug() << "Parsed an element: " << m_elementVal->symbol() << endl;
	*_result = m_elementVal->mass();
	addElementToMolecule( m_elementVal, 1 );
	maybeadded = true;
	getNextToken();
    }

    else if (nextToken() == '(') {
	// A submolecule.

	getNextToken();
	parseSubmolecule(_result);

	// Must end in a ")".
	if (nextToken() == ')') {
	    //kdDebug() << "Parsed a submolecule. weight = " << *_result << endl;
	    getNextToken();
	}
	else
	    return false;
    }
    else 
	// Neither an element nor a list within ().
	return false;

    // Optional number.
    if (nextToken() == INT_TOKEN) {
	//kdDebug() << "Parsed a number: " << intVal() << endl;

    	*_result *= intVal();
	addElementToMolecule( m_elementVal, maybeadded ? intVal() - 1 : intVal() );
	getNextToken();
    }

    kdDebug() << "Weight of term = " << *_result << endl;
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
	{
	    m_nextToken = ELEMENT_TOKEN;
	}
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
MoleculeParser::lookupElement( const QString& _name )
{
    EList elementList = KalziumDataObject::instance()->ElementList;

    //kdDebug() << "looking up " << _name << endl;

    EList::ConstIterator        it  = elementList.constBegin();
    const EList::ConstIterator  end = elementList.constEnd();

    for (; it != end; ++it) {
	if ( (*it)->symbol() == _name ) {
	    kdDebug() << "Found element " << _name << endl;
	    return *it;
	}
    }

    kdDebug() << k_funcinfo << "no such element: " << _name << endl;
    return NULL;
}

void MoleculeParser::addElementToMolecule( Element* el, const int n )
{
	QMap<Element*, int> newelements;
	QMap<Element*, int>::ConstIterator it = m_elementMap.constBegin();
	QMap<Element*, int>::ConstIterator itEnd = m_elementMap.constEnd();
	bool found = false;
	for ( ; it != itEnd; ++it ) {
		if ( it.key()->elname() == el->elname() )
		{
			newelements.insert( it.key(), it.data() + n );
			found = true;
		}
		else
			newelements.insert( it.key(), it.data() );
        }
	if ( !found )
		newelements.insert( el, n );
	m_elementMap = newelements;
}

