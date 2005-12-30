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

#ifndef MOLECULEPARSER_H
#define MOLECULEPARSER_H

#include "element.h"
#include "parser.h"

#include <qmap.h>
#include <qvaluelist.h>


/**
 * @class Element
 */
class ElementCount {
 public:
    ElementCount(Element *_element, int _count)
    {
	    m_element = _element;
	    m_count   = _count;
	}
    ElementCount(Element *_element)
        {
	    m_element = _element;
	    m_count   = 0;
	}
	
    ~ElementCount();

    Element *element() const    { return m_element;   }
    int   count()  const        { return m_count;     }
    void  add(int _count)       { m_count += _count;  }
	void  multiply(int _factor) { m_count *= _factor; }

    Element  *m_element;
    int       m_count;
};


/**
 * @class ElementCountMap
 */
class ElementCountMap {
 public:
    ElementCountMap();
    ~ElementCountMap();

    void  clear()          { m_map.clear(); }

    ElementCount  *search(Element *_element);
    void           add(ElementCountMap &_map);
    void           add(Element *_element, int _count);
	void           multiply(int _factor);

	typedef QValueList<ElementCount*>::Iterator  Iterator;
	Iterator   begin() { return  m_map.begin(); }
	Iterator   end()   { return  m_map.end();   }

 private:
    QValueList<ElementCount*>  m_map;
};



/**
 * Parse molecule formulas.
 *
 * Usage:
 * @code
 *   MoleculeParser  parser;
 *   QString         chemical_formula = "C2H5OH";
 *   double          weight;
 *
 *   if (parser.weight(chemical_formula, &weight))
 *     cout << "Weight of " << chemical_formula << " = " << weight << ".\n";
 *   else
 *     cout << "Parse error\n";
 * @endcode
 *
 * @author Inge Wallin
 */
class MoleculeParser : public Parser {

public:
    static const int  ELEMENT_TOKEN = 300;

    MoleculeParser();
    MoleculeParser( const QString& _str);
    ~MoleculeParser();

    /**
     * Try to parse the molecule @p molecule and get the weight of it.
     * The calculated weight is stored in @p _result.
     *
     * @return whether the parsing was successful or not
     */
    bool  weight(QString         _moleculeString,
				 double          *_resultMass,
				 ElementCountMap *_resultMap);

 private:
    // Helper functions
    bool      parseSubmolecule(double          *_resultMass,
							   ElementCountMap *_resultMap);
    bool      parseTerm(double          *_resultMass, 
						ElementCountMap *_resultMap);

    Element  *lookupElement( const QString& _name );

    QMap<Element*, int> m_elementMap;

	//if this booloean is "true" the parser found an error
	bool m_error;

protected:

    /**
     * Extends the standard tokenizer in Parser::getNextToken().
     */
    virtual int  getNextToken();

private:
    Element  *m_elementVal;	// Valid if m_nextToken == ELEMENT_TOKEN
};

#endif
