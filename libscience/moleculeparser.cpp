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

/* PORTING
 * #include "kalziumdataobject.h"
 */

#include "moleculeparser.h"


// ================================================================
//                    class ElementCountMap



ElementCountMap::ElementCountMap()
{
	m_map.clear();
}


ElementCountMap::~ElementCountMap()
{
}


ElementCount *
ElementCountMap::search(Element *_element)
{
	//not yet tested but the commented code should be the same as
	//the foreach... so lets use Qt4 power
//X 	QList<ElementCount *>::ConstIterator       it    = m_map.constBegin();
//X 	const QList<ElementCount *>::ConstIterator itEnd = m_map.constEnd();
//X 
//X 	for (; it != itEnd; ++it) {
//X 		if ((*it)->element() == _element)
//X 			return *it;
//X 	}

	foreach( ElementCount* c, m_map ){
		if ( c->element() == _element )
			return c;
	}

	return 0;
}


void
ElementCountMap::add(ElementCountMap &_map)
{
	//not yet tested but the commented code should be the same as
	//the foreach... so lets use Qt4 power
//X 	QList<ElementCount *>::ConstIterator       it    = _map.m_map.constBegin();
//X 	const QList<ElementCount *>::ConstIterator itEnd = _map.m_map.constEnd();
//X 
//X 	// Step throught _map and for each element, add it to the current one.
//X 	for (; it != itEnd; ++it) {
//X 		add((*it)->m_element, (*it)->m_count);
//X 	}

	foreach( ElementCount* c, _map.m_map ){
		add( c->m_element, c->m_count );
	}
	
}


void
ElementCountMap::add(Element *_element, int _count)
{
	ElementCount  *elemCount;

	elemCount = search(_element);
	if (elemCount)
		elemCount->m_count += _count;
	else
		m_map.append(new ElementCount(_element, _count));
}


void
ElementCountMap::multiply(int _factor)
{
	Iterator  it    = begin();
	Iterator  itEnd = end();

	for (; it != itEnd; ++it)
		(*it)->multiply(_factor);
}


// ================================================================
//                    class MoleculeParser


MoleculeParser::MoleculeParser( const QList<Element*>& list)
    : Parser()
{
	m_elementList = list;
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
MoleculeParser::weight(QString         _moleculeString, 
					   double          *_resultMass,
					   ElementCountMap *_resultMap)
{
	// Clear the result variables and set m_error to false
	_resultMap->clear();
	m_error = false;
	*_resultMass = 0.0;

	// Initialize the parsing process, and parse te molecule.
	start(_moleculeString);
	parseSubmolecule(_resultMass, _resultMap);

	if (nextToken() != -1)
		return false;

	if ( m_error )//there was an error in the input...
		return false;

	return true;
}


// ----------------------------------------------------------------
//            helper methods for the public methods


// Parse a submolecule.  This is a list of terms.
//

bool
MoleculeParser::parseSubmolecule(double          *_resultMass,
								 ElementCountMap *_resultMap)
{
    double           subMass = 0.0;
    ElementCountMap  subMap;

    *_resultMass = 0.0;
	_resultMap->clear();
    while (parseTerm(&subMass, &subMap)) {
		//kdDebug() << "Parsed a term, weight = " << subresult << endl;

		// Add the mass and composition of the submolecule to the total.
		*_resultMass += subMass;
		_resultMap->add(subMap);
    }

    return true;
}


// Parse a term within the molecule, i.e. a single atom or a
// submolecule within parenthesis followed by an optional number.
// Examples: Bk, Mn2, (COOH)2
//
// Return true if correct, otherwise return false.  

// If correct, the mass of the term is returned in *_resultMass, and
// the flattened composition of the molecule in *_resultMap.
//

bool
MoleculeParser::parseTerm(double          *_resultMass,
						  ElementCountMap *_resultMap)
{
    *_resultMass = 0.0;
	_resultMap->clear();
 
    if (nextToken() == ELEMENT_TOKEN) {
		//kdDebug() << "Parsed an element: " << m_elementVal->symbol() << endl;
		*_resultMass = m_elementVal->dataAsVariant( ChemicalDataObject::mass ).toDouble();
		_resultMap->add(m_elementVal, 1);

		getNextToken();
    }

    else if (nextToken() == '(') {
		// A submolecule.

		getNextToken();
		parseSubmolecule(_resultMass, _resultMap);

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

    	*_resultMass *= intVal();
		_resultMap->multiply(intVal());

		getNextToken();
    }

    kdDebug() << "Weight of term = " << *_resultMass << endl;
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
//X     QList<Element*> elementList = KalziumDataObject::instance()->ElementList;

    kdDebug() << "looking up " << _name << endl;

//X     QList<Element*>::ConstIterator        it  = elementList.constBegin();
//X     const QList<Element*>::ConstIterator  end = elementList.constEnd();

	foreach( Element* e, m_elementList ){
		if ( e->dataAsVariant(ChemicalDataObject::symbol) == _name ) {
			kdDebug() << "Found element " << _name << endl;
			return e;
		}
	}

//X 	for (; it != end; ++it) {
//X 		if ( (*it)->dataAsVariant(ChemicalDataObject::symbol) == _name ) {
//X 			kdDebug() << "Found element " << _name << endl;
//X 			return *it;
//X 		}
//X 	}

	//if there is an error make m_error true.
	m_error = true;

	kdDebug() << k_funcinfo << "no such element, parsing error!: " << _name << endl;

	return NULL;
}
