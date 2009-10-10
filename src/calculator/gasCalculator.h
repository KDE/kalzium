/***************************************************************************
 *   Copyright (C) 2009 	by Kashyap R Puranik, kashthealien@gmail.com   *
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

#ifndef gasCalculator_H
#define gasCalculator_H

#include <element.h>
#include <isotope.h>

#include <kdebug.h>
#include <prefs.h>
#include <kalziumdataobject.h>
#include <kunitconversion/unitcategory.h>

#include "ui_gasCalculator.h"

// The universal Gas constant is defined here.
#define R 0.08206

// This is required for the units conversion
using namespace KUnitConversion;

// This is the enumeration for the error type required in the error(int mode) function
enum ERROR_TYPE_GAS {
	RESET_GAS_MESSAGE = 0,
    VOL_ZERO,
    MOLAR_MASS_ZERO_
};

// This is the enumeration for the mode of calculation for the gas calculator
enum MODE_CALCULATION_GAS {
	MOLES = 0,
	PRESSURE,
	TEMPERATURE,
	VOLUME
};

/*
 * This class implements the gas calculator. It performs basic calculations like
 * calculation of volume given pressure, temerature, amount etc. and so on.
 *
 * @author Kashyap R Puranik
 */
class gasCalculator : public QFrame
{
    Q_OBJECT

public:
    gasCalculator(QWidget * parent = 0);
    ~gasCalculator();

public slots:
	/// Calculates the Pressure and updates the UI
    void calculatePressure();
    
    /// Calculates the Volume and updates the UI
    void calculateVol();
    
    /// Calculates the Temperature and updates the UI
    void calculateTemp();
    
    /// Calculates the number of moles and updates the UI
    void calculateMoles();
    
    /// Calculates the mass of substance and updates the UI
    void calculateMass();
    
    /// Calculates the molar mass of the substance and updates the UI
    void calculateMolarMass();

    /// Functions ( slots ) that occur on changing a value
    /// This function is called when the volume is changed
    void volChanged();
    
    /// This function is called when the temperature is changed
    void tempChanged();
    
    /// This function is called when the pressure is changed
    void pressureChanged();
    
    /// This function is called when the mass is changed          
    void massChanged();
    
    /*
     * This function is called when the number of moles is changed
     * @param value is the number of moles
     */
    void molesChanged(double value);
    
    /*
     * This function is called when the molar mass is changed
     * @param value is the molar mass
     */
    void molarMassChanged(double value);
    
    /// This function is called when Vander Val's constant a is changed
    void Vand_aChanged();
    
    /// This function is called when Vander Val's constant b is changed        
    void Vand_bChanged();
    
    /// This function is called when any quantity is changed        
    void calculate();
    
     /*
     * This function is called when an error occurs
     * @param mode indicates the mode of error
     * Refer ERROR_MODE_GAS for various modes
     */
    void error(int);
    
    /* 
     * This function is called when the mode is changed
     * @param indicates the mode of calculation.
     * Refer MODE_CALCULATION_GAS for various modes
     */
	void setMode(int);
	
	// Initialises the gasCalculator
	void init();
	
private:
    Ui::gasCalculator ui;               // The user interface

    double m_moles;                     // Number of moles
    double m_molarMass;                 // molarMass
    Value m_mass;                       // mass
    Value m_temp;                       // Temperature
    Value m_pressure;                   // pressure
    Value m_vol;                        // volume
    Value m_Vand_b;                     // vander val's constant b

    //( Unit conversion library not available for the following quantities)
    double m_Vand_a;                    // Vander val's constant a
    
    int m_mode;							// indicates the volume that should be calculated
};

#endif // gasCalculator_H
