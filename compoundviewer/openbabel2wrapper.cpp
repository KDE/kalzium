/***************************************************************************
 *  Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
 *  Copyright (C) 2007-2008 by Marcus D. Hanwell <marcus@cryos.org>
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
#include <KMessageBox>

#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

#include <QMouseEvent>
#include <QRegExp>
#include <QFile>


Avogadro::Molecule* OpenBabel2Wrapper::readMolecule( const QString& filename )
{
	OpenBabel::OBConversion Conv;
	OpenBabel::OBFormat *inFormat = NULL;

	// the Avogadro Molecule
	Avogadro::Molecule *mol = new Avogadro::Molecule;
	std::ifstream inFileStream( QFile::encodeName(filename) );
	if ( !inFileStream ) {
            KMessageBox::error(  0, 
                    i18n( "Problem while opening the file" ),
                    i18n( "Cannot open the specified file." ) 
                    );
            delete mol;
            return 0;
	}

	//find out which format the file has...
	inFormat = Conv.FormatFromExt( QFile::encodeName(filename) );
  if (!inFormat || !Conv.SetInFormat(inFormat))
  {
    KMessageBox::error( 0, i18n("Cannot read the file format. Check your OpenBabel installation."), i18n("Problem reading file format"));
    delete mol;
    return 0;
  }
	Conv.SetInAndOutFormats( inFormat,inFormat );
	Conv.Read( mol, &inFileStream );

	kDebug() << QString::fromLatin1( mol->GetFormula().c_str() )  << " (Weight: " << mol->GetMolWt() << ", Title: "<< mol->GetTitle() << ")";

	return mol;
}

QString OpenBabel2Wrapper::getFormula( Avogadro::Molecule* molecule )
{
	QString formula( molecule->GetFormula().c_str() );
	return formula;
}
		
QString OpenBabel2Wrapper::getPrettyFormula( Avogadro::Molecule* molecule )
{
	QString formula( molecule->GetSpacedFormula(1,"").c_str() );
	formula.replace( QRegExp( "(\\d+)" ), "<sub>\\1</sub>" );
	return formula;
}
