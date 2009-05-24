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
		void calculateAmtSolute  ( void );			// Calculates the amount of solute
		void calculateAmtSolvent ( void );			// Calculates the amount of solvent
		void calculateMolarMass ( void );				// Calculates the molar mass
		void calculateEqtMass ( void );				// Calculates the equivalent mass
		void calculateMolarMassSolvent ( void );		// Calculates the calculate molar mass of the solvent
		void calculateDensitySolute ( void );			// Calculates the density of solute
		void calculateDensitySolvent ( void );		// Calculates the density of solvent
		void calculateConcentration ( void );
		// Functions ( slots ) that occur on changing a value
		void amtSoluteChanged ( double value );			// occurs when the amount of solute is changed
		void amtSolventChanged ( double value );		// occurs when the amount of solvent is changed
		void molarMassChanged ( double value );  		// occurs when the molar mass of solute is changed
		void eqtMassChanged ( double value );			// occurs when the equivalent mass of solute is changed
		void molarMassSolventChanged ( double value );	// occurs when the molar mass of solvent is changed
		void densitySoluteChanged ( double value ); 	// occurs when the number of moles is changed
		void densitySolventChanged ( double value );	// occurs when the density of solvent is changed
		void concentrationChanged ( double value );
		void calculate ( void );						// occurs when any quantity is changed		

		
	private:
		Ui::concCalculator ui;					// The user interface
		
		Value m_amtSolute;								// amount of solute
		Value m_amtSolvent;							// amount of solvent
		double m_molarMass;								// molar mass of solute
		double m_eqtMass;								// equivalent mass of solute
		Value m_molarMassSolvent;						// molar mass of solvent
		Value m_densitySolute;							// density of solute
		Value m_concentration;							// concentration of the solution
		Value m_densitySolvent;						// density of the solvent
		
};

#endif // concCalculator_H

