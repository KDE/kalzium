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
#include<prefs.h>
#include <converter.h>
#include <kalziumdataobject.h>

#include "ui_gasCalculator.h"


class gasCalculator : public QFrame
{
	Q_OBJECT
	
	public:
		gasCalculator  ( QWidget * parent = 0 );
		~gasCalculator ();
	
	public slots:	
		double calculatePressure  ( void );		// Calculates the Pressure
		double calculateVol ( void );			// Calculates the Volume
		double calculateTemp ( void );			// Calculates the Temperature
		double calculateMoles ( void );			// Calculates the number of moles
		double calculateMass ( void );			// Calculates the mass of substance
		double calculateVand_a ( void );		// Calculates the vander Val's constant a
		double calculateVand_b ( void );		// Calculates the vander Val's constant b
		
		// Functions ( slots ) that occur on changing a value
		void elementChanged ( int index );		// occurs when the element is changed
		void volChanged ( double value );		// occurs when the volume is changed
		void tempChanged ( double value );  	// occurs when the temperature is changed
		void pressureChanged ( double value );	// occurs when the pressure is changed	
		void massChanged ( double value );		// occurs when the mass is changed
		void molesChanged ( double value ); 	// occurs when the number of moles is changed
		void Vand_aChanged ( double value );	// occurs when Vander Val's constant a is changed
		void Vand_bChanged ( double value );	// occurs when Vander Val's constant b is changed
		void calculate ( void );				// occurs when any quantity is changed		
		
	private:
		Ui::gasCalculator ui;				// The user interface
		
		Element m_element;						// Current element
		double m_mass;							// mass
		double m_moles;							// Number of moles
		double m_temp;							// Temperature
		double m_pressure;						// pressure
		double m_vol;							// volume
		double m_Vand_a;						// Vander val's constant a
		double m_Vand_b;						// vander val's constant b
};

#endif // gasCalculator_H
