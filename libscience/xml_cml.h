/****************************************************************************
 * ** $Id: xml_cml.h,v 1.2 2002/07/21 23:27:05 atenderholt Exp $
 * **
 * ** Copyright ( C ) 1992-2000 Trolltech AS.  All rights reserved.
 * **
 * ** This file is part of an example program for Qt.  This example
 * ** program may be used, distributed and modified without limitation.
 * **
 * *****************************************************************************/

#ifndef XML_CML_H
#define XML_CML_H

#include <libkdeedu_science_export.h>

#include <qxml.h>
#include <QList>

#include "cmlclasses.h"

class QString;

// possible states (CMLParser::states)
// set in defs.h
//X #define CML_NONE 0
//X #define CML_ATOM 1
//X #define CML_BOND 2
//X #define CML_MOLECULE 10
//X #define CML_NONE   1
//X #define CML_ATOM   2
//X #define CML_BOND   3
//X #define CML_ATOMARRAY 4
//X #define CML_BONDARRAY 5


class EDUSCIENCE_EXPORT CMLParser : public QXmlDefaultHandler
{
	public:
		CMLParser();
		virtual ~CMLParser();

		bool startDocument();
		
		bool startElement( const QString&, 
				const QString&, 
				const QString& ,
				const QXmlAttributes& );
		
		bool endElement( const QString&, const QString&, const QString& );
		
		bool characters( const QString& );
		
		bool ignorableWhitespace( const QString& );
		
		QList<CML::Atom*> getAtoms();
		QList<CML::Bond*> getBonds();
		
		CML::Molecule* getMolecule(){
			return tmp_molecule;
		}

	private:
		QList<CML::Atom*> localAtoms;
		QList<CML::Bond*> localBonds;
		
		CML::Bond *tmp_bond;
		
		CML::Molecule *tmp_molecule;

		CML::Atom * tmp_atom;		
};

#endif
