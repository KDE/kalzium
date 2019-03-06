/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus <cniehaus@kde.org>              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#include "isotope.h"

#include "chemicaldataobject.h"

#include <QDebug>

Isotope::Isotope()
{
}

Isotope::~Isotope()
{
}

void Isotope::addData(const ChemicalDataObject &o)
{
    if (o.type() == ChemicalDataObject::exactMass) {
        m_mass = o;
    } else if (o.type() == ChemicalDataObject::atomicNumber) {
        m_identifier = o;
    } else if (o.type() == ChemicalDataObject::symbol) {
        m_parentElementSymbol = o;
    } else if (o.type() == ChemicalDataObject::spin) {
        m_spin = o;
    } else if (o.type() == ChemicalDataObject::magneticMoment) {
        m_magmoment = o;
    } else if (o.type() == ChemicalDataObject::relativeAbundance) {
        m_abundance = o;
    } else if (o.type() == ChemicalDataObject::halfLife) {
        m_halflife = o;
    } else if (o.type() == ChemicalDataObject::ecDecay) {
        m_ecdecay = o;
    } else if (o.type() == ChemicalDataObject::ecDecayLikeliness) {
        m_eclikeliness = o;
    } else if (o.type() == ChemicalDataObject::neutronDecay) {
        m_neutrondecay = o;
    } else if (o.type() == ChemicalDataObject::neutronDecayLikeliness) {
        m_neutronlikeliness = o;
    } else if (o.type() == ChemicalDataObject::protonDecay) {
        m_protondecay = o;
    } else if (o.type() == ChemicalDataObject::protonDecayLikeliness) {
        m_protonlikeliness = o;
    } else if (o.type() == ChemicalDataObject::protonalphaDecay) {
        m_protonalphadecay = o;
    } else if (o.type() == ChemicalDataObject::protonalphaDecayLikeliness) {
        m_protonalphalikeliness = o;
    } else if (o.type() == ChemicalDataObject::betaplusDecay) {
        m_betaplusdecay = o;
    } else if (o.type() == ChemicalDataObject::betaplusDecayLikeliness) {
        m_betapluslikeliness = o;
    } else if (o.type() == ChemicalDataObject::betaplusprotonDecay) {
        m_betaplusprotondecay = o;
    } else if (o.type() == ChemicalDataObject::betaplusprotonDecayLikeliness) {
        m_betaplusprotonlikeliness = o;
    } else if (o.type() == ChemicalDataObject::betaplusalphaDecay) {
        m_betaplusalphadecay = o;
    } else if (o.type() == ChemicalDataObject::betaplusalphaDecayLikeliness) {
        m_betaplusalphalikeliness = o;
    } else if (o.type() == ChemicalDataObject::betaminusDecay) {
        m_betaminusdecay = o;
    } else if (o.type() == ChemicalDataObject::betaminusDecayLikeliness) {
        m_betaminuslikeliness = o;
    } else if (o.type() == ChemicalDataObject::betaminusneutronDecay) {
        m_betaminusneutrondecay = o;
    } else if (o.type() == ChemicalDataObject::betaminusneutronDecayLikeliness) {
        m_betaminusneutronlikeliness = o;
    } else if (o.type() == ChemicalDataObject::betaminusfissionDecay) {
        m_betaminusfissiondecay = o;
    } else if (o.type() == ChemicalDataObject::betaminusfissionDecayLikeliness) {
        m_betaminusfissionlikeliness = o;
    } else if (o.type() == ChemicalDataObject::betaminusalphaDecay) {
        m_betaminusalphadecay = o;
    } else if (o.type() == ChemicalDataObject::betaminusalphaDecayLikeliness) {
        m_betaminusalphalikeliness = o;
    } else if (o.type() == ChemicalDataObject::alphaDecay) {
        m_alphadecay = o;
    } else if (o.type() == ChemicalDataObject::alphaDecayLikeliness) {
        m_alphalikeliness = o;
    } else if (o.type() == ChemicalDataObject::alphabetaminusDecay) {
        m_alphabetaminusdecay = o;
    } else if (o.type() == ChemicalDataObject::alphabetaminusDecayLikeliness) {
        m_alphabetaminuslikeliness = o;
    }
}

double Isotope::mass() const
{
    return  m_mass.value().toDouble();
}

QString Isotope::errorMargin() const
{
    return m_mass.errorValue().toString();
}

int Isotope::parentElementNumber() const
{
    return  m_identifier.value().toInt();
}

QString Isotope::spin() const
{
    return m_spin.value().toString();
}

QString Isotope::magmoment() const
{
    return m_magmoment.value().toString();
}

QString Isotope::abundance() const
{
    return m_abundance.value().toString();
}

double Isotope::halflife() const
{
    return m_halflife.value().toDouble();
}

QString Isotope::halflifeUnit() const
{
    return m_halflife.unitAsString();
}

double Isotope::ecdecay() const
{
    return m_ecdecay.value().toDouble();
}
double Isotope::eclikeliness() const
{
    return m_eclikeliness.value().toDouble();
}
double Isotope::neutrondecay() const
{
    return m_neutrondecay.value().toDouble();
}
double Isotope::neutronlikeliness() const
{
    return m_neutronlikeliness.value().toDouble();
}
double Isotope::protondecay() const
{
    return m_protondecay.value().toDouble();
}
double Isotope::protonlikeliness() const
{
    return m_protonlikeliness.value().toDouble();
}
double Isotope::protonalphadecay() const
{
    return m_protonalphadecay.value().toDouble();
}
double Isotope::protonalphalikeliness() const
{
    return m_protonalphalikeliness.value().toDouble();
}
double Isotope::betaplusdecay() const
{
    return m_betaplusdecay.value().toDouble();
}

double Isotope::betapluslikeliness() const
{
    return m_betapluslikeliness.value().toDouble();
}

double Isotope::betaplusprotondecay() const
{
    return m_betaplusprotondecay.value().toDouble();
}

double Isotope::betaplusprotonlikeliness() const
{
    return m_betaplusprotonlikeliness.value().toDouble();
}
double Isotope::betaplusalphadecay() const
{
    return m_betaplusalphadecay.value().toDouble();
}

double Isotope::betaplusalphalikeliness() const
{
    return m_betaplusalphalikeliness.value().toDouble();
}

double Isotope::betaminusdecay() const
{
    return m_betaminusdecay.value().toDouble();
}

double Isotope::betaminuslikeliness() const
{
    return m_betaminuslikeliness.value().toDouble();
}

double Isotope::betaminusneutrondecay() const
{
    return m_betaminusneutrondecay.value().toDouble();
}

double Isotope::betaminusneutronlikeliness() const
{
    return m_betaminusneutronlikeliness.value().toDouble();
}
double Isotope::betaminusfissiondecay() const
{
    return m_betaminusfissiondecay.value().toDouble();
}

double Isotope::betaminusfissionlikeliness() const
{
    return m_betaminusfissionlikeliness.value().toDouble();
}

double Isotope::betaminusalphadecay() const
{
    return m_betaminusalphadecay.value().toDouble();
}

double Isotope::betaminusalphalikeliness() const
{
    return m_betaminusalphalikeliness.value().toDouble();
}


double Isotope::alphadecay() const
{
    return m_alphadecay.value().toDouble();
}

double Isotope::alphalikeliness() const
{
    return m_alphalikeliness.value().toDouble();
}

double Isotope::alphabetaminusdecay() const
{
    return m_alphabetaminusdecay.value().toDouble();
}

double Isotope::alphabetaminuslikeliness() const
{
    return m_alphabetaminuslikeliness.value().toDouble();
}

QString Isotope::parentElementSymbol() const
{
    return m_parentElementSymbol.value().toString();
}

void Isotope::setNucleons(int number)
{
    m_numberOfNucleons = number;
}

int Isotope::nucleons() const
{
    return m_numberOfNucleons;
}

Isotope::Nucleons Isotope::nucleonsAfterDecay(Decay kind)
{
    Isotope::Nucleons n;
    int protons = m_identifier.value().toInt();
    int neutrons = m_numberOfNucleons - protons;
    n.protons = protons;
    n.neutrons = neutrons;

    switch (kind) {
    case ALPHA:
        n.protons -= 2;
        break;
    case BETAMINUS:
        n.protons += 1;
        n.neutrons -= 1;
        break;
    case BETAPLUS:
        n.protons -= 1;
        break;
    case EC:
        n.protons -= 1;
        n.neutrons += 1;
        break;
    case NEUTRON:
	n.neutrons -=1;
	break;
    case PROTON:
	n.protons -=1;
	break;


    }


    return n;
}
