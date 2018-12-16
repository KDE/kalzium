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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "spectrum.h"

#include "element.h"

#include <KLocalizedString>
#include <KUnitConversion/Converter>

#include <math.h>

double Spectrum::minPeak()
{
    double value = m_peaklist.first()->wavelength;

    foreach (peak *p, m_peaklist) {
        if (value > p->wavelength) {
            value = p->wavelength;
        }
    }
    return value;
}

double Spectrum::minPeak(const int unit)
{
    return KUnitConversion::Value(minPeak(), KUnitConversion::Angstrom).convertTo(KUnitConversion::UnitId(unit)).number();
}


double Spectrum::maxPeak()
{
    double value = m_peaklist.first()->wavelength;

    foreach (peak * p, m_peaklist) {
        if (value < p->wavelength) {
            value = p->wavelength;
        }
    }

    return value;
}

double Spectrum::maxPeak(const int unit)
{
    return KUnitConversion::Value(maxPeak(), KUnitConversion::Angstrom).convertTo(KUnitConversion::UnitId(unit)).number();
}


Spectrum* Spectrum::adjustToWavelength(double min, double max)
{
    Spectrum *spec = new Spectrum();

    foreach (peak * p , m_peaklist) {
        if (p->wavelength >= min || p->wavelength <= max) {
            spec->addPeak(p);
        }
    }

    return spec;
}

void Spectrum::adjustIntensities()
{
    int maxInt = 0;
    //find the highest intensity
    foreach (Spectrum::peak * p, m_peaklist) {
        if (p->intensity > maxInt) {
            maxInt = p->intensity;
        }
    }

    //check if an adjustment is needed or not
    if (maxInt == 1000) {
        return;
    }

    //now adjust the intensities.
    foreach (Spectrum::peak *p, m_peaklist) {
        double newInt = p->intensity * 1000 / maxInt;

        p->intensity = (int)qRound(newInt);
    }
}

QList<double> Spectrum::wavelengths(double min, double max)
{
    QList<double> list;

    foreach (peak * p , m_peaklist) {
        if (p->wavelength >= min || p->wavelength <= max) {
            list.append(p->wavelength);
        }
    }

    return list;
}

int Spectrum::parentElementNumber() const
{
    return  m_parentElementNumber;
}

Spectrum::~Spectrum()
{
    qDeleteAll(m_peaklist);
}

Spectrum::Spectrum()
{
    //FIXME this shouldn't be hardcoded
    m_parentElementNumber = 16;
}

double Spectrum::peak::wavelengthToUnit(const int unit)
{
    return KUnitConversion::Value(wavelength, KUnitConversion::Angstrom).convertTo(KUnitConversion::UnitId(unit)).number();
}
