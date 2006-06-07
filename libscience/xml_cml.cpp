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
	return TRUE;
}

bool CMLParser::startElement(  const QString&, const QString&, 
		const QString& qName, 
		const QXmlAttributes& attr )
{
	if ( qName.toUpper() == "ATOM" ) {
		tmp_atom = new CML::Atom();
	
		QString x2 = attr.value( "x2");
		if ( x2 != QString( "" ) )
			tmp_atom->setX2( x2.toDouble() );
		
		QString x3 = attr.value( "x3");
		if ( x3 != QString( "" ) )
			tmp_atom->setX3( x3.toDouble() );
		
		QString y2 = attr.value( "y2");
		if ( y2 != QString( "" ) )
			tmp_atom->setY2( y2.toDouble() );
		
		QString y3 = attr.value( "y3");
		if ( y3 != QString( "" ) )
			tmp_atom->setY3( y3.toDouble() );
		
		QString z3 = attr.value( "z3");
		if ( z3 != QString( "" ) )
			tmp_atom->setZ3( z3.toDouble() );

		tmp_atom->setID( attr.value( "id" ) );
	}
	else if ( qName.toUpper() == "BOND" ) {
		tmp_bond = new CML::Bond();
		
		QString order = attr.value( "order");
		if ( order != QString( "" ) )
			tmp_bond->setOrder( order.toInt() );
		
		QString atomRef = attr.value( "atomRefs2");
		if ( atomRef != QString( "" ) )
		{
			QStringList Split = atomRef.split( " " );
			tmp_bond->setAtoms( Split.at( 0 ), Split.at( 1 ) );
		}
	}
	return TRUE;
}

bool CMLParser::endElement(  const QString&, const QString&, 
		const QString& qName )
{
	if ( qName.toUpper() == "ATOM" ) {
		localAtoms.append( tmp_atom );
		tmp_atom = 0;
	}
	if ( qName.toUpper() == "BOND" ) {
		localBonds.append( tmp_bond );
		tmp_bond = 0;
	}
	return TRUE;
}

bool CMLParser::characters(  const QString &ch ) 
{
	return TRUE;
}

bool CMLParser::ignorableWhitespace(  const QString &ch )
{
	return TRUE;
}

QList<CML::Atom*> CMLParser::getAtoms()
{ 
	return localAtoms; 
}

QList<CML::Bond*> CMLParser::getBonds()
{ 
	return localBonds; 
}
