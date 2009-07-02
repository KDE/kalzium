#ifndef gasCalculator_H
#define gasCalculator_H
/***************************************************************************
    copyright            : Kashyap R Puranik
    email                : kashthealien@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <element.h>
#include <isotope.h>

#include <kdebug.h>
#include <prefs.h>
#include <kalziumdataobject.h>
#include <converter.h>
#include <value.h>

#include "ui_gasCalculator.h"

#define R 0.08206

using namespace Conversion;

enum ERROR_TYPE_GAS {
    VOL_ZERO = 0
};

enum MODE_CALCULATION_GAS {
	MOLES = 0,
	PRESSURE,
	TEMPERATURE,
	VOLUME
};

class gasCalculator : public QFrame
{
    Q_OBJECT

public:
    gasCalculator(QWidget * parent = 0);
    ~gasCalculator();

public slots:
	/// Calculates the Pressure
    void calculatePressure();
    
    /// Calculates the Volume
    void calculateVol();         
    
    /// Calculates the Temperature    
    void calculateTemp();        
    
    /// Calculates the number of moles
    void calculateMoles();       
    
    /// Calculates the mass of substance    
    void calculateMass();        
    
    /// Calculates the molar mass of the substance    
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
    
    /// This function is called when the number of moles is changed 
    void molesChanged(double value);    
    
    /// This function is called when the molar mass is changed
    void molarMassChanged(double value);     
    
    /// This function is called when Vander Val's constant a is changed
    void Vand_aChanged();
    
    /// This function is called when Vander Val's constant b is changed        
    void Vand_bChanged();
    
    /// This function is called when any quantity is changed        
    void calculate();            
    
    /// Prints error messages on the screen.
    void error(int);
    
	/// This function is called to change the mode of calculation
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
