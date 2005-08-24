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
    bool                  weight(QString _molecule, double *_result);
    QMap<Element*, int>   elementMap();
    QValueList<Element*>  elementList();

 private:
    // Helper functions
    bool      parseSubmolecule(double *_result);
    bool      parseTerm(double *_result);

    Element  *lookupElement( const QString& _name );
    void      addElementToMolecule( Element* el, const int n );

    QMap<Element*, int> m_elementMap;

protected:

    /**
     * Extends the standard tokenizer in Parser::getNextToken().
     */
    virtual int  getNextToken();

private:
    Element  *m_elementVal;	// Valid if m_nextToken == ELEMENT_TOKEN
};

#endif
