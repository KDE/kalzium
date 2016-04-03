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

#include <sstream>
#include <iostream>
#include <fstream>

#include <openbabel/obconversion.h>
#include <openbabel/mol.h>

#include <QDebug>
#include <QMessageBox>
#include <QMouseEvent>
#include <QRegExp>
#include <QFile>

#include <KLocalizedString>

Avogadro::QtGui::Molecule * OpenBabel2Wrapper::readMolecule(const QString &filename)
{
    OpenBabel::OBConversion Conv;
    OpenBabel::OBFormat *inFormat = nullptr;

    // the Avogadro Molecule
    Avogadro::QtGui::Molecule *mol = new Avogadro::QtGui::Molecule;
    OpenBabel::OBMol *obmol = new OpenBabel::OBMol;
    std::ifstream inFileStream(QFile::encodeName(filename));
    if (!inFileStream) {
        QMessageBox::warning(nullptr, i18n("Problem while opening the file"),
                           i18n("Cannot open the specified file."));
        delete mol;
        return nullptr;
    }

    //find out which format the file has...
    inFormat = Conv.FormatFromExt(QFile::encodeName(filename));
    if (!inFormat || !Conv.SetInFormat(inFormat)) {
        QMessageBox::warning(nullptr, i18n("Cannot read the file format. Check your OpenBabel installation."),
                           i18n("Problem reading file format"));
        delete mol;
        return nullptr;
    }
    Conv.SetInAndOutFormats(inFormat,inFormat);
    Conv.Read(obmol, &inFileStream);

    qDebug() << QString::fromLatin1(obmol->GetFormula().c_str())
        << " (Weight: " << obmol->GetMolWt() << ", Title: "<< obmol->GetTitle() << ")";

//     mol->setOBMol(obmol);///*FIXME:Avogadro2*/ method gone
    return mol;
}

bool OpenBabel2Wrapper::writeMolecule(const QString &filename, Avogadro::QtGui::Molecule *mol)
{
    OpenBabel::OBConversion Conv;
    OpenBabel::OBFormat *outFormat = nullptr;

    std::ofstream outFileStream(QFile::encodeName(filename));
    if (!outFileStream) {
        QMessageBox::warning(nullptr, i18n("Sorry"), i18n("Cannot save to the specified file."));
        return false;
    }
    outFormat = Conv.FormatFromExt(QFile::encodeName(filename));
    if (!outFormat || !Conv.SetOutFormat(outFormat)) {
        QMessageBox::warning(nullptr, i18n("Unrecognized file format extension. Please append an extension to the file name, "),
                                     i18n("for example \".cml\"."));
        delete mol;
        return false;
    }
    Conv.SetInAndOutFormats(outFormat,outFormat);
//     OpenBabel::OBMol obmol = mol->OBMol();//FIXME:Avogadro2 method gone
//     Conv.Write(&obmol, &outFileStream);//FIXME:Avogadro2
    return true;
}

QString OpenBabel2Wrapper::getFormula(Avogadro::QtGui::Molecule *molecule)
{
    return QString::fromStdString(molecule->formula());
}

QString OpenBabel2Wrapper::getPrettyFormula(Avogadro::QtGui::Molecule *molecule)
{
    QString formula = QString::fromStdString(molecule->formula());
    formula.replace(QRegExp("(\\d+)"), "<sub>\\1</sub>");
    return formula;
}
