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

#include "concCalculator.h"

concCalculator::concCalculator( QWidget * parent )
	: QFrame ( parent )
{	
	ui . setupUi( this );
	
	/**************************************************************************/
	//						 conc Calculator set up
	/**************************************************************************/	
    
    // initialise the initially selected values
    ui . amtSolute  		-> setValue ( 117.0 );
	ui . molarMass	 		-> setValue ( 58.5 );
	ui . eqtMass 			-> setValue ( 58.5 );
	ui . densitySolute		-> setValue ( 12.0 );
	ui . amtSolvent 		-> setValue ( 1.0 );
	ui . molarMassSolvent 	-> setValue ( 18.0 );
	ui . densitySolvent		-> setValue ( 1.0 );
	ui . concentration		-> setValue ( 2.0 );
	// Setup of the UI done
	
	// Initialise values
	m_amtSolute = 117.0;
	m_amtSolvent = 1.0;
	m_molarMass = 58.5;
	m_eqtMass = 58.5;
	m_molarMassSolvent = 18.0;
	m_densitySolute = 12.0;
	m_concentration = 2.0;
	m_densitySolvent = 1.0;
    // Initialisation of values done
    
    // Connect signals with slots
	connect ( ui . amtSolute  , SIGNAL ( valueChanged ( double ) ),
	         this, SLOT ( amtSoluteChanged  ( double ) ) );
	connect ( ui . molarMass  , SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( molarMassChanged  ( double ) ) );
	connect ( ui . eqtMass  , SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( eqtMassChanged  ( double ) ) );
	connect ( ui . densitySolute  ,  SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( densitySoluteChanged  ( double ) ) );
	connect ( ui . amtSolvent  , SIGNAL ( valueChanged ( double )),
			 this, SLOT ( amtSolventChanged ( double ) ) );
	connect ( ui . molarMassSolvent  , SIGNAL ( valueChanged ( double )),
			 this, SLOT ( molarMassSolventChanged ( double ) ) );
	connect ( ui . densitySolvent  , SIGNAL ( valueChanged ( double )),
			 this, SLOT ( densitySolventChanged ( double ) ) );
	connect ( ui . concentration  , SIGNAL ( valueChanged ( double )),
			 this, SLOT ( concentrationChanged ( double ) ) );			 			 			 
			 	 			 
	/**************************************************************************/        
	// concentration Calculator setup complete	         
	/**************************************************************************/
}

concCalculator:: ~concCalculator()
{

}
// Calculates the amount of solute
double concCalculator::calculateAmtSolute  ( void )
{
	double amount;
	
	return amount;
}

// Calculates the amount of solvent
double concCalculator::calculateAmtSolvent ( void )
{
	double amount;
	
	return amount;
}

// Calculates the molar mass
double concCalculator::calculateMolarMass ( void )
{
	double molarMass;
	
	return molarMass;
}

// Calculates the equivalent mass
double concCalculator::calculateEqtMass ( void )
{
	double eqtMass;
	
	return eqtMass;
}

// Calculates the calculate molar mass of the solvent
double concCalculator::calculateMolarMassSolvent ( void )
{
	double molarMass;
	
	return molarMass;
}

// Calculates the density of solute
double concCalculator::calculateDensitySolute ( void )
{
	double density;
	
	return density;
}

// Calculates the density of solvent
double concCalculator::calculateDensitySolvent ( void )
{
	double density;
	
	return density;
}

// occurs when the amount of solute is changed
void concCalculator::amtSoluteChanged ( double value )
{
	m_amtSolute = value;
}

// occurs when the amount of solvent is changed
void concCalculator::amtSolventChanged ( double value )
{
	m_amtSolvent = value;
}

// occurs when the molar mass of solute is changed
void concCalculator::molarMassChanged ( double value )
{
	m_molarMass = value;
}

// occurs when the equivalent mass of solute is changed
void concCalculator::eqtMassChanged ( double value )
{
	m_eqtMass = value;
}

// occurs when the molar mass of solvent is changed
void concCalculator::molarMassSolventChanged ( double value )
{
	m_molarMassSolvent = value;
}

// occurs when the number of moles is changed
void concCalculator::densitySoluteChanged ( double value )
{
	m_densitySolute = value;
}

// occurs when the density of solvent is changed
void concCalculator::densitySolventChanged ( double value )
{
	m_densitySolvent = value;
}

// occurs when any quantity is changed
void concCalculator::calculate ( void )
{

}
#include "concCalculator.moc"

