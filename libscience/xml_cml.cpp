/****************************************************************************
 * ** Copyright ( C ) 1992-2000 Trolltech AS.  All rights reserved.
 * ** Copyright ( C ) 2006 Carsten Niehaus <cniehaus@kde.org>
 * **
 * ** This file is part of an example program for Qt.  This example
 * ** program may be used, distributed and modified without limitation.
 * **
 * *****************************************************************************/

#include "xml_cml.h"

#include <QFile>
#include <QString>

#include <QtDebug>

CMLParser::CMLParser()
  : QXmlDefaultHandler()
{
}

CMLParser::~CMLParser()
{
}

bool CMLParser::startDocument()
{
	qDebug() << "New CML parser started.";
	return TRUE;
}

bool CMLParser::startElement(  const QString&, const QString&, 
		const QString& qName, 
		const QXmlAttributes& attr )
{
	qDebug() << "Start:" << qName;
	if ( qName.toUpper() == "ATOM" ) {
		states = CML_ATOM;
		tmp_atom = new CML::Atom();
		tmp_atom->setID( attr.value( "id" ) );
		qDebug() << "CML::Atom id=" << attr.value( "id" );
	}
	if ( qName.toUpper() == "BOND" ) {
		states = CML_BOND;
		tmp_bond = new CML::Bond();
		atom1 = 0; atom2 = 0;
	}
//X 	if ( qName.toUpper() == "FLOAT" ) {
//X 		last_builtin = attr.value( "builtin" ).toUpper();
//X 		if ( last_builtin == "X3" ) last_builtin = "X2";
//X 		if ( last_builtin == "X3" ) last_builtin = "Y2";
//X 	}
//X 	if ( qName.toUpper() == "STRING" ) {
//X 		last_builtin = attr.value( "builtin" ).toUpper();
//X 	}
	return TRUE;
}

bool CMLParser::endElement(  const QString&, const QString&, 
		const QString& qName )
{
	qDebug() << "End:" << qName;
	if ( qName.toUpper() == "ATOM" ) {
		localAtoms.append( tmp_atom );
		tmp_atom = 0;
		states = CML_NONE;
		qDebug() << "finished atom";
	}
	if ( qName.toUpper() == "BOND" ) {
		tmp_bond->setAtoms( atom1, atom2 );
		localBonds.append( tmp_bond );
		tmp_bond = 0;
		states = CML_NONE;
		qDebug() << "finished bond";
	}
	return TRUE;
}

bool CMLParser::characters(  const QString &ch ) {
	qDebug() << "char:" << ch << ":";
	if ( states == CML_ATOM ) 
	{
		if ( last_builtin == "ELEMENTTYPE" ) 
			tmp_atom->setElementType( ch );
		
		if ( last_builtin == "X2" ) 
			tmp_atom->setX2(  ch.toDouble() );

		if ( last_builtin == "Y2" ) 
			tmp_atom->setY2(  ch.toDouble() );
		
		if ( last_builtin == "X3" ) 
			tmp_atom->setX3(  ch.toDouble() );

		if ( last_builtin == "Y3" ) 
			tmp_atom->setY3(  ch.toDouble() );
		
		if ( last_builtin == "Z3" ) 
			tmp_atom->setZ3( ch.toDouble() );
	}
	if ( states == CML_BOND ) {
		
		if ( last_builtin == "ATOMREFS2" ) 
		{
		}
		
		if ( last_builtin == "ORDER" ) 
			tmp_bond->setOrder( ch.toInt() );
		
//X 		if ( last_builtin == "STEREO" ) {
//X 			if ( ch == "H" ) tmp_bond->setOrder( 7 );
//X 			if ( ch == "W" ) tmp_bond->setOrder( 5 );
//X 		}
	}
	return TRUE;
}

bool CMLParser::ignorableWhitespace(  const QString &ch ) {
	qDebug() << "ignored:" << ch << ":";
	return TRUE;
}

QList<CML::Atom*> CMLParser::getAtoms()
{ 
	return localAtoms; 
}
