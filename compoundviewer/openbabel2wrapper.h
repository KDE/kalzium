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

#ifndef OPENBABEL2WRAPPER_H
#define OPENBABEL2WRAPPER_H

//includes for OpenBabel2
#include <openbabel/obconversion.h>
#include <openbabel/mol.h>
#include <qglobal.h>
#include <avogadro/molecule.h>

#include <libkdeedu_compoundviewer_export.h>

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
    static Avogadro::Molecule *readMolecule(const QString& filename);

    bool writeMolecule(const QString& filename, Avogadro::Molecule *);

    QString getFormula(Avogadro::Molecule *molecule);

    QString getPrettyFormula(Avogadro::Molecule *molecule);
};

#endif // OPENBABEL2WRAPPER_H
