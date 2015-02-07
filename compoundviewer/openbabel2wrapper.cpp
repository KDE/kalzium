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
#include <QApplication>
#include <QDebug>
#include <QLocale>
#include <QMessageBox>
#include <KLocalizedString>


#include <sstream>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>

#include <QMouseEvent>
#include <QRegExp>
#include <QFile>

Avogadro::Molecule* OpenBabel2Wrapper::readMolecule(const QString& filename)
{
    OpenBabel::OBConversion Conv;
    OpenBabel::OBFormat *inFormat = NULL;

    // the Avogadro Molecule
    Avogadro::Molecule *mol = new Avogadro::Molecule;
    OpenBabel::OBMol *obmol = new OpenBabel::OBMol;
    std::ifstream inFileStream(QFile::encodeName(filename));
    if (!inFileStream) {
        QMessageBox::warning(0,i18n("Problem while opening the file"),
                           i18n("Cannot open the specified file."));
        delete mol;
        return 0;
    }

    //find out which format the file has...
    inFormat = Conv.FormatFromExt(QFile::encodeName(filename));
    if (!inFormat || !Conv.SetInFormat(inFormat)) {
        QMessageBox::warning(0,i18n("Cannot read the file format. Check your OpenBabel installation."),
                           i18n("Problem reading file format"));
        delete mol;
        return 0;
    }
    Conv.SetInAndOutFormats(inFormat,inFormat);
    Conv.Read(obmol, &inFileStream);

    qDebug() << QString::fromLatin1(obmol->GetFormula().c_str()) << " (Weight: " << obmol->GetMolWt() << ", Title: "<< obmol->GetTitle() << ")";
    mol->setOBMol(obmol);
    return mol;
}

bool OpenBabel2Wrapper::writeMolecule(const QString& filename, Avogadro::Molecule* mol)
{
    OpenBabel::OBConversion Conv;
    OpenBabel::OBFormat *outFormat = NULL;

    std::ofstream outFileStream(QFile::encodeName(filename));
    if (!outFileStream) {
        QMessageBox::warning(0,i18n("Sorry"),i18n("Cannot save to the specified file."));
        return false;
    }
    outFormat = Conv.FormatFromExt(QFile::encodeName(filename));
    if (!outFormat || !Conv.SetOutFormat(outFormat)) {
        QMessageBox::warning(0, i18n("Unrecognized file format extension. Please append an extension to the file name, "),
                                     i18n("for example \".cml\"."));
        delete mol;
        return false;
    }
    Conv.SetInAndOutFormats(outFormat,outFormat);
    OpenBabel::OBMol obmol = mol->OBMol();
    Conv.Write(&obmol, &outFileStream);
    return true;
}

QString OpenBabel2Wrapper::getFormula(Avogadro::Molecule* molecule)
{
    QString formula(molecule->OBMol().GetFormula().c_str());
    return formula;
}

QString OpenBabel2Wrapper::getPrettyFormula(Avogadro::Molecule* molecule)
{
    QString formula(molecule->OBMol().GetSpacedFormula(1, "").c_str());
    formula.replace(QRegExp("(\\d+)"), "<sub>\\1</sub>");
    return formula;
}
