#ifndef OPENBABEL2WRAPPER_H
#define OPENBABEL2WRAPPER_H
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

//includes for OpenBabel2
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>

#include <libkdeedu_compoundviewer_export.h>

#include <QString>

/**
 * @author Carsten Niehaus
 */
class COMPOUNDVIEWER_EXPORT OpenBabel2Wrapper
{
	public:
		/**
		 * This class reads the molecule in the file @p filename. It returns 0 if
		 * the file couldn't be read.
		 */
		static OpenBabel::OBMol* readMolecule( const QString& filename );
		
		static QString getFormula( OpenBabel::OBMol* molecule );
		
		static QString getPrettyFormula( OpenBabel::OBMol* molecule );
};

#endif // OPENBABEL2WRAPPER_H
