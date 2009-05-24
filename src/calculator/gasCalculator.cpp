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

#include "gasCalculator.h"

gasCalculator::gasCalculator( QWidget * parent )
	: QFrame ( parent )
{	
	ui . setupUi( this );
	
	/**************************************************************************/
	//						 Gas Calculator set up
	/**************************************************************************/
	KalziumDataObject *kdo = KalziumDataObject::instance();
	
	// add all element names to the comboBox	
	foreach (Element * e, kdo -> ElementList) {
    	ui . element -> addItem ( e->dataAsString ( ChemicalDataObject::name ) );
    }
    
    // initialise the initially selected values
	ui.element->setCurrentIndex( 0 );
	ui.temp->setValue( 273.0 );
	ui.vol->setValue( 22.400);
	ui.pressure->setValue( 1.0 );
	ui.Vand_a->setValue( 0.0 );
	ui.Vand_b->setValue( 0.0 );
	ui.mass->setValue( 1.008 );
	ui.moles->setValue( 1.0 );
	// Setup of the UI done
	
	// Initialise values
	m_temp = 273.0;
	m_element = * KalziumDataObject::instance() -> element ( 1 );
	m_pressure = 1.0;
	m_mass = 1.008;
	m_moles = 1.0;
	m_Vand_a = 0.0;
	m_Vand_b = 0.0;
	m_vol = 22.4;
    // Initialisation of values done
    // Connect signals with slots
    	connect ( ui.element, SIGNAL ( activated ( int ) ), 
             this, SLOT ( elementChanged ( int ) ) );
	connect ( ui.temp  , SIGNAL ( valueChanged ( double ) ),
	         this, SLOT ( tempChanged  ( double ) ) );
	connect ( ui.vol, SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( volChanged  ( double ) ) );
	connect ( ui.pressure, SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( pressureChanged  ( double ) ) );
	connect ( ui.mass,  SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( massChanged ( double ) ) );
	connect ( ui.moles, SIGNAL ( valueChanged ( double )),
			 this, SLOT ( molesChanged ( double ) ) );
	connect ( ui.moles, SIGNAL ( valueChanged ( double )),
			 this, SLOT ( molesChanged ( double ) ) );
	connect ( ui.moles, SIGNAL ( valueChanged ( double )),
			 this, SLOT ( molesChanged ( double ) ) );			 			 
			 	 			 
	/**************************************************************************/        
	// gas Calculator setup complete	         
	/**************************************************************************/
}

gasCalculator:: ~gasCalculator()
{

}

// Calculates the Pressure
double gasCalculator::calculatePressure  ( void )
{

	//cniehaus: Why not a simple "return double"?
	double pressure;
	
	return pressure;
}	

// Calculates the Volume
double gasCalculator::calculateVol ( void )
{
	double vol;
	
	return vol;
}

// Calculates the Temperature
double gasCalculator::calculateTemp ( void )
{
	double temp;
	
	return temp;
}	

// Calculates the number of moles
double gasCalculator::calculateMoles ( void )
{
	double moles;
	
	return moles;
}

// Calculates the mass of substance
double gasCalculator::calculateMass ( void )
{
	double mass;
	
	return mass;
}

// Calculates the vander Val's constant a
double gasCalculator::calculateVand_a ( void )
{
	double a;
	
	return a;
}

// Calculates the vander Val's constant b
double gasCalculator::calculateVand_b ( void )
{
	double b;
	
	return b;
}
		

// Functions ( slots ) that occur on changing a value

// occurs when the element is changed
void gasCalculator::elementChanged ( int index )
{
	m_element = * KalziumDataObject::instance() -> element ( index + 1);
	calculate ();	
}

// occurs when the volume is changed
void gasCalculator::volChanged ( double value )
{
	m_vol = value;
	calculate ();	
}

// occurs when the temperature is changed
void gasCalculator::tempChanged ( double value )
{
	m_temp = value;
	calculate ();	
}

// occurs when the pressure is changed	
void gasCalculator::pressureChanged ( double value )
{
	m_pressure = value;
		calculate ();
}

// occurs when the mass is changed
void gasCalculator::massChanged ( double value )
{
	m_mass = value;
	calculate ();
}

// occurs when the number of moles is changed
void gasCalculator::molesChanged ( double value )
{
	m_moles = value;
	calculate ();	
}

// occurs when the number of moles is changed
void gasCalculator::Vand_aChanged ( double value )
{
	m_Vand_a = value;
	calculate ();	
}

// occurs when the number of moles is changed
void gasCalculator::Vand_bChanged ( double value )
{
	m_Vand_b = value;
	calculate ();
}

// occurs when any quantity is changed
void gasCalculator::calculate ( void )
{

}

#include "gasCalculator.moc"


