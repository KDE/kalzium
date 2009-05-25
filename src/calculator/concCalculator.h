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

class concCalculator : public QFrame
{
	Q_OBJECT
	
	public:
		concCalculator  ( QWidget * parent = 0 );
		~concCalculator ();
		
	public slots:	

		// Sub-routines involved in calculations of the unit
		void calculateAmtSolute(void);			// Calculates the amount of solute
		void calculateAmtSolvent(void);			// Calculates the amount of solvent
		void calculateMolarMass(void);			// Calculates the molar mass
		void calculateEqtMass(void);			// Calculates the equivalent mass
		void calculateMolarMassSolvent(void);	// Calculates the calculate molar mass of the solvent
		// void calculateDensitySolute(void);		// Calculates the density of solute
		// void calculateDensitySolvent(void);		// Calculates the density of solvent
		void calculateConcentration(void);		// calculates the concentration		
		
		// Functions ( slots ) that occur on changing a value
		// Sub routines which act as quantity change event handlers
		
		void amtSoluteChanged(void);			// occurs when the amount of solute is changed
		void amtSolventChanged(void);			// occurs when the amount of solvent is changed
		void molarMassChanged(double);  		// occurs when the molar mass of solute is changed
		void eqtMassChanged(double);			// occurs when the equivalent mass of solute is changed
		void molarMassSolventChanged(double);	// occurs when the molar mass of solvent is changed
		void densitySoluteChanged(void); 		// occurs when the number of moles is changed
		void densitySolventChanged(void);		// occurs when the density of solvent is changed
		void concentrationChanged(double);		// occurs when the concentration is changed
		void calculate(void);					// occurs when any quantity is changed		

		double volumeSolvent(void);				// returns volume of solvent in liters
		double massSolvent(void);				// returns mass of solvent in grams
		double molesSolvent(void);				// returns number of moles of solvent
		double densitySolvent(void);			// returns density of solvent in grams per liter
		double volumeSolute(void);				// returns volume of solute in liters
		double massSolute(void);				// returns mass of solute in grams
		double molesSolute(void);				// returns the number of moles of solute
		double eqtsSolute(void);				// returns the number of equivalents of solute
		double densitySolute(void);				// returns density of solute in grams per liter
		
		void debug(void);
	private:
		Ui::concCalculator ui;						// The user interface
		
		Value m_amtSolute;							// amount of solute
		Value m_amtSolvent;							// amount of solvent
		double m_molesSolute;						// amount of solute in moles
		double m_molesSolvent;						// amount of solvent in moles
		double m_molarMass;							// molar mass of solute
		double m_eqtMass;							// equivalent mass of solute
		double m_molarMassSolvent;					// molar mass of solvent
		Value m_densitySolute;						// density of solute
		Value m_densitySolvent;						// density of the solvent
		double m_concentration;						// concentration of the solution		

};

#endif // concCalculator_H

