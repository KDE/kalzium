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

// Parse molecule formulas.
//
//
// Usage:
//   MoleculeParser  parser;
//   QString         substance = "C2H5OH";
//   double          weight;
//
//   if (parser.weight(substance, &weight))
//       cout << "Weight of " << substance << " = " << weight << ".\n";
//   else
//       cout << "Parse error\n";


/**
@author Inge Wallin
*/

class MoleculeParser : public Parser {

public:
    static const int  ELEMENT_TOKEN = 300;

    MoleculeParser();
    MoleculeParser(QString _str);
    ~MoleculeParser();

    // Try to parse the molecule and get the weight of it.
    bool      weight(QString _molecule, double *_result);

 private:
    // Helper functions
    bool      parseSubmolecule(double *_result);
    bool      parseTerm(double *_result);

    Element  *lookupElement(QString _name);

protected:

    // Extends the standard tokenizer in Parser::getNextToken().
    virtual int  getNextToken();

private:
    Element  *m_elementVal;	// Valid if m_nextToken == ELEMENT_TOKEN
};

#endif
