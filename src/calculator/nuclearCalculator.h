/***************************************************************************
 *   Copyright (C) 2009     by Kashyap R Puranik, kashthealien@gmail.com   *
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

#ifndef NUCLEARCALCULATOR_H
#define NUCLEARCALCULATOR_H

#include <QDebug>

#include <KUnitConversion/UnitCategory>
#include <KUnitConversion/Converter>

#include <element.h>
#include <isotope.h>
#include <prefs.h>
#include <kalziumdataobject.h>
#include "ui_nuclearCalculator.h"

// This is required for the unit conversion
using namespace KUnitConversion;

// This is the enumeration for the error type required in the error(int mode) function
enum ERROR_MODE_NUKE {
    RESET_NUKE_MESSAGE = 0,
    INIT_AMT_ZERO,
    FINAL_AMT_ZERO,
    HALFLIFE_ZERO,
    FINAL_AMT_GREATER
};

// This is the enumeration for the mode of calculation in the nuclear calculator
enum MODE_CALCULATION_NUKE {
    INIT_AMT =0,
    FINAL_AMT,
    TIME
};

/*
 * This class implements the nuclear calculator which calculates the amount of substance,
 * remaining after a given time and given initial amount and so on after a radio-active decay.
 *
 * @author Kashyap R Puranik
 */
class nuclearCalculator : public QFrame
{
    Q_OBJECT

public:
    nuclearCalculator(QWidget * parent = nullptr);
    ~nuclearCalculator();

public slots:
    /// Calculates the initial amount and updates the UI
    void calculateInitAmount();

    /// Calculates the final amount and updates the UI
    void calculateFinalAmount();

    /// Calculates the time required and updates the UI
    void calculateTime();

    /// This function is called when the element is changed
    void elementChanged(int index);

    /// This function is called when the isotope is changed
    void isotopeChanged(int index);

    /// This function is called when the halfLife is changed
    void halfLifeChanged();

    /// This function is called when any quantity is changed
    void calculate();

    /// This function is called when the initial amount is changed in the UI
    void initAmtChanged();

    /// This function is called when the final amount is changed in the UI
    void finalAmtChanged();

    /// This function is called when the time is changed in the UI
    void timeChanged();

    /// Fills a Combobox with vulumina units
    void timeUnitCombobox(QComboBox *comboBox);

    /// Fills a Combobox with mass units
    void massUnitCombobox(QComboBox *comboBox);

    /// Fetch the active unit id (KUnitConversion) from the combobox
    KUnitConversion::UnitId getUnitIdFromCombobox(QComboBox *comboBox);

    /*
     * This function is called when the slider in the ui is moved
     * @param x, is 10 times the number of halfLives
     * The slider is used to change the halfLife
     */
    void sliderMoved(int x);

    /*
     * This function is called when the mode is changed
     * @param indicates the mode of calculation.
     * Refer MODE_CALCULATION_NUKE for various modes
     */
    void setMode(int mode);

    /// This function is called during initialisation
    void init();

    /*
     * This function is called when an error occurs
     * @param mode indicates the mode of error
     * Refer ERROR_MODE_NUKE for various modes
     */
    void error(int mode);

private:
    Ui::nuclearCalculator ui;               // The user interface

    Element m_element;                      // Current element
    Isotope m_isotope;                      // current isotope

    Value m_halfLife;                       // The halfLife
    Value m_initAmount;                     // initial amount present
    Value m_finalAmount;                    // amount after time
    Value m_time;                           // the time involved in calculation
    double m_mass;                          // the atomic mass of the isotope

    int m_mode;                             // the mode of calculation
};

#endif // NUCLEARCALCULATOR_H
