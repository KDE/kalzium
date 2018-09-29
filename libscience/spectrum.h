/***************************************************************************
 *   Copyright (C) 2005, 2006 by Carsten Niehaus <cniehaus@kde.org>        *
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

#ifndef SPECTRUM_H
#define SPECTRUM_H

#include "libkdeedu_science_export.h"

#include <QList>

class Element;

/**
 * @author Carsten Niehaus
 *
 * This class represents a spectrum with all its properties
 */
class SCIENCE_EXPORT Spectrum
{
public:
    /**
     * This spectrum doesn't belong to any element
     */
    Spectrum();

    /**
     * public destructor
     */
    ~Spectrum();

    /**
     * a peak is one line in the spectrum of an element
     */
    class SCIENCE_EXPORT peak
    {
    public:
        peak() {
            wavelength = -1.0;
            intensity = -1;
        }

        peak(double wl, int in) {
            wavelength = wl;
            intensity = in;
        }

        ///relative. The highest is per definition 1000
        int intensity;
        double wavelength;

        double wavelengthToUnit(const int unit);
    };

    /**
     * adds the peak @p b to the internal
     * lists of peaks
     */
    void addPeak(Spectrum::peak* b) {
        if (b) {
            m_peaklist.append(b);
        }
    }

    /**
     * @param min the lowest allowed wavelength in nanometer
     * @param max the highest allowed wavelength in nanometer
     *
     * @returns a spectrum with the wavelength in the range
     * of @p min to @p max. The intensities are readjusted
     * so that the biggest intensity is again 1000 and the
     * others are adopted.
     */
    Spectrum* adjustToWavelength(double min, double max);

    /**
     * sets the highest intensity to 1000 and adjusts the
     * others
     */
    void adjustIntensities();

    /**
     * @param min the lowest allowed wavelength in nanometer
     * @param max the highest allowed wavelength in nanometer
     *
     * @return the wavelength in a QList<double>
     */
    QList<double> wavelengths(double min, double max);

    /**
     * @return the list of peaks of the spectrum
     */
    QList<Spectrum::peak*> peaklist() {
        return m_peaklist;
    }

    /**
    * If the spectrum belongs to Iron, this method will return "26"
    * @return the number of the element the spectrum belongs to
    */
    int parentElementNumber() const;

    /**
     * @return the smallest wavelength
     */
    double minPeak();
    double minPeak(const int unit);

    /**
     * @return the biggest wavelength
     */
    double maxPeak();
    double maxPeak(const int unit);

    void setParentElementNumber(int num)
    {
        m_parentElementNumber = num;
    }

private:
    /**
     * the internal dataset
     */
    QList<peak*> m_peaklist;

    int m_parentElementNumber;
};

#endif // SPECTRUM_H
