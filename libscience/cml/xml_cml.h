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

#include <qxml.h>
#include <QList>

#include "cmlclasses.h"

class QString;

// possible states (CMLParser::states)
// // set in defs.h
// //#define CML_NONE 0
// //#define CML_ATOM 1
// //#define CML_BOND 2
#define CML_MOLECULE 10
#define CML_NONE   1
#define CML_ATOM   2
#define CML_BOND   3
#define CML_ATOMARRAY 4
#define CML_BONDARRAY 5


class CMLParser : public QXmlDefaultHandler
{
	public:
		CMLParser()
		{
		};

		~CMLParser()
		{
		};
	
		bool startDocument();
		
		bool startElement( const QString&, const QString&, const QString& ,
				const QXmlAttributes& );
		
		bool endElement( const QString&, const QString&, const QString& );
		
		bool characters( const QString& );
		
		bool ignorableWhitespace( const QString& );
		
		QList<CML::Atom*> getAtoms();

	private:
		QList<CML::Atom*> localAtoms;
		QList<CML::Bond*> localBonds;
		
		CML::Bond *tmp_bond;

		//temporary atoms for the creation of a Bond
		CML::Atom * atom1;
		CML::Atom * atom2;

		CML::Atom * tmp_atom;
		
		QString indent, last_builtin;
		
		int states;
};

#endif
