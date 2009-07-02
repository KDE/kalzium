#ifndef concCalculator_H
#define concCalculator_H
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

#include <kdebug.h>
#include <prefs.h>
#include <kalziumdataobject.h>
#include <converter.h>

#include "ui_concCalculator.h"

using namespace Conversion;

// Enumeration for type of error used in the error() function
enum ERROR_TYPE_CONC {
    PERCENTAGE = 0,
    DENSITY_ZERO,
    MASS_ZERO,
    VOLUME_ZERO,
    MOLES_ZERO,
    MOLAR_MASS_ZERO,
    EQT_MASS_ZERO,
    MOLAR_SOLVENT_ZERO,
    EQTS_ZERO,
    CONC_ZERO,
    INSUFFICIENT_DATA_EQT,
    INSUFFICIENT_DATA_MOLE,
    INSUFFICIENT_DATA_MOLES,
    INSUFFICIENT_DATA_SOLVENT
};

// enumeration for the mode of calculation in the setMode(int) function
enum MODE_CALCULATION_CONC {
	AMT_SOLUTE = 0,
	MOLAR_MASS,
	EQT_MASS,
	AMT_SOLVENT,
	MOLAR_MASS_SOLVENT,
	CONCENTRATION
};

class concCalculator : public QFrame
{
    Q_OBJECT

public:
    concCalculator(QWidget * parent = 0);
    ~concCalculator();

public slots:
    // Sub-routines involved in calculations of the unit
    
    /// Calculates the amount of solute
    void calculateAmtSolute(void);
    
    /// Calculates the amount of solvent         
    void calculateAmtSolvent(void);
    
    /// Calculates the molar mass
    void calculateMolarMass(void);
    
    /// Calculates the equivalent mass
    void calculateEqtMass(void);
    
    /// Calculates the calculate molar mass of the solvent
    void calculateMolarMassSolvent(void);
    
    /// calculates the concentration
    void calculateConcentration(void);

    // Functions ( slots ) that occur on changing a value
    // Sub routines which act as quantity change event handlers

	/// occurs when the amount of solute is changed
    void amtSoluteChanged(void);            
    
    /// occurs when the amount of solvent is changed
    void amtSolventChanged(void);
    
    /// occurs when the molar mass of solute is changed
    void molarMassChanged(double);          
    
    /// occurs when the equivalent mass of solute is changed
    void eqtMassChanged(double);            
    
    /// occurs when the molar mass of solvent is changed
    void molarMassSolventChanged(double);   
    
    /// occurs when the number of moles is changed
    void densitySoluteChanged(void);
    
    /// occurs when the density of solvent is changed
    void densitySolventChanged(void);
    
    /// occurs when the concentration is changed
    void concentrationChanged(double);
    
    /// occurs when any quantity is changed
    void calculate(void);                   

	/// returns volume of solvent in liters
    double volumeSolvent(void);
    
    /// returns mass of solvent in grams
    double massSolvent(void);
    
    /// returns number of moles of solvent
    double molesSolvent(void);
    
    /// returns density of solvent in grams per liter
    double densitySolvent(void);
    
    /// returns volume of solute in liters
    double volumeSolute(void);
    
    /// returns mass of solute in grams
    double massSolute(void);
    
    /// returns the number of moles of solute
    double molesSolute(void);
    
    /// returns the number of equivalents of solute
    double eqtsSolute(void);
    
    /// returns density of solute in grams per liter
    double densitySolute(void);             

	/// Performs initialisation of the class.
	void init(void);
	
	/// outputs error messages on the screen
    void error(int);
    
    /// Used to set mode of calculation
    void setMode(int);						
private:
    Ui::concCalculator ui;                      // The user interface

    Value m_amtSolute;                          // amount of solute
    Value m_amtSolvent;                         // amount of solvent
    double m_molesSolute;                       // amount of solute in moles
    double m_molesSolvent;                      // amount of solvent in moles
    double m_molarMass;                         // molar mass of solute
    double m_eqtMass;                           // equivalent mass of solute
    double m_molarMassSolvent;                  // molar mass of solvent
    Value m_densitySolute;                      // density of solute
    Value m_densitySolvent;                     // density of the solvent
    double m_concentration;                     // concentration of the solution
    
    int m_mode;									// specifies the mode of calculation

};

#endif // concCalculator_H

