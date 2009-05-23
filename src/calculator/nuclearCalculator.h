#ifndef nuclearCalculator_H
#define nuclearCalculator_H
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
#include "ui_nuclearCalculator.h"

using namespace Conversion;

class nuclearCalculator : public QFrame
{
	Q_OBJECT
	
	public:
		nuclearCalculator  ( QWidget * parent = 0 );
		~nuclearCalculator ();
	
	public slots:	
		void calculateInitAmount  ( void );	// Calculates the initial amount
		void calculateFinalAmount ( void );	// Calculates the final amount
		void calculateTime ( void );			// Calculates the time required
		
		// Functions ( slots ) that occur on changing a value
		void elementChanged ( int index );		// occurs when the element is changed
		void isotopeChanged ( int index );		// occurs when the isotope is changed
		void halfLifeChanged ( void );  // occurs when the halfLife is changed
		void calculate ( void );				// occurs when any quantity is changed
		void initAmtChanged ( void );				// occurs when the initial amount is changed in the UI
		void finalAmtChanged ( void );				// occurs when the final amount is changed in the UI
		void timeChanged ( void );					// occurs when the time is changed in the UI
		void sliderMoved ( int );				// occurs when the slider in the ui is moved
		
	private:
		Ui::nuclearCalculator ui;				// The user interface
		
		Element m_element;						// Current element
		Isotope m_isotope;						// current isotope
		
		// Note all values are stored in SI units in the class
		Value m_halfLife;						// The halfLife
		Value m_initAmount;						// initial amount present
		Value m_finalAmount;					// amount after time
		Value m_time;							// the time involved in calculation
		double m_mass;							// the atomic mass of the isotope
};

#endif // nuclearCalculator_H
