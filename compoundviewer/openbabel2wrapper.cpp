/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "openbabel2wrapper.h"

#include <kdebug.h>
#include <klocale.h>
#include <QMessageBox>
#include <QRegExp>

#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <malloc.h>

#include <QMouseEvent>
#include <QLayout>
#include <QListWidget>
#include <QMessageBox>


Avogadro::Molecule* OpenBabel2Wrapper::readMolecule( const QString& filename )
{
	OpenBabel::OBConversion Conv;
	OpenBabel::OBFormat *inFormat = NULL;

	//the OB2 Molecule
	Avogadro::Molecule *mol = new Avogadro::Molecule;
	std::ifstream inFileStream(  filename.toLatin1() );
	if ( !inFileStream ) {
		QMessageBox::warning(  0, i18n( "Problem while opening the file" ),
				i18n( "Cannot open the specified file." ) );
		delete mol;
		return 0;
	}

	//find out which format the file has...
	inFormat = Conv.FormatFromExt( filename.toLatin1() );
	Conv.SetInAndOutFormats( inFormat,inFormat );
	Conv.Read( mol, &inFileStream );

	kDebug() << QString::fromLatin1( mol->GetFormula().c_str() )  << " (Weight: " << mol->GetMolWt() << ", Title: "<< mol->GetTitle() << ")" << endl;

	return mol;
}

QString OpenBabel2Wrapper::getFormula( Avogadro::Molecule* molecule )
{
	QString formula( molecule->GetFormula().c_str() );
	return formula;
}
		
QString OpenBabel2Wrapper::getPrettyFormula( Avogadro::Molecule* molecule )
{
	QString formula( molecule->GetFormula().c_str() );
	formula.replace( QRegExp( "(\\d+)" ), "<sub>\\1</sub>" );
	return formula;
}
