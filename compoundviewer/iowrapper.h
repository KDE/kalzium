/***************************************************************************
 *  Copyright (C) 2006 by Carsten Niehaus <cniehaus@kde.org>
 *  Copyright (C) 2007-2008 by Marcus D. Hanwell <marcus@cryos.org>
 *  Copyright (C) 2016 by Andreas Cord-Landwehr <cordlandwehr@kde.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef IOWRAPPER_H
#define IOWRAPPER_H

#include <avogadro/qtgui/molecule.h>

#include <libkdeedu_compoundviewer_export.h>

/**
 * @author Carsten Niehaus
 */
class COMPOUNDVIEWER_EXPORT IoWrapper
{
public:
    /**
     * This class reads the molecule in the file @p filename. It returns 0 if
     * the file couldn't be read.
     */
    static Avogadro::Core::Molecule * readMolecule(const QString &filename);

    static bool writeMolecule(const QString& filename, Avogadro::Core::Molecule *);

    static QString getFormula(Avogadro::QtGui::Molecule *molecule);

    static QString getPrettyFormula(Avogadro::QtGui::Molecule *molecule);
};

#endif
