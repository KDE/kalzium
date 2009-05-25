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
	//						 concentration Calculator set up
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
	m_amtSolute = Value ( 117.0, "grams");
	m_amtSolvent = Value ( 1.0, "liter");
	m_molarMass = 58.5;
	m_eqtMass = 58.5;
	m_molesSolute = 2.0;
	m_molesSolvent = 55.5;
	m_molarMassSolvent = 18.0;
	m_densitySolute = Value ( 12.0, "grams per milliliter" );
	m_concentration = 2.0;
	m_densitySolvent = Value ( 1.0, "grams per milliliter" );
    // Initialisation of values done
    
    // Connect signals with slots
	connect ( ui . amtSolute  , SIGNAL ( valueChanged ( double ) ),
	         this, SLOT ( amtSoluteChanged  ( ) ) );
	connect ( ui . amtSltType  , SIGNAL ( activated ( int ) ),
	         this, SLOT ( amtSoluteChanged  ( ) ) );
	connect ( ui . amtSlt_unit  , SIGNAL ( activated ( int ) ),
	         this, SLOT ( amtSoluteChanged  ( ) ) );
	connect ( ui . amtSlt_unit2  , SIGNAL ( activated ( int ) ),
	         this, SLOT ( amtSoluteChanged  ( ) ) );		         
	connect ( ui . molarMass  , SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( molarMassChanged  ( double ) ) );
	connect ( ui . eqtMass  , SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( eqtMassChanged  ( double ) ) );
	connect ( ui . densitySolute  ,  SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( densitySoluteChanged  ( ) ) );
	connect ( ui . densSlt_unit  ,  SIGNAL ( activated ( int ) ),
			 this, SLOT ( densitySoluteChanged  ( ) ) );			 
	connect ( ui . amtSolvent  , SIGNAL ( valueChanged ( double )),
			 this, SLOT ( amtSolventChanged ( ) ) );
	connect ( ui . amtSlvtType  , SIGNAL ( activated (int ) ),
			 this, SLOT ( amtSolventChanged ( ) ) );
	connect ( ui . amtSlvt_unit  , SIGNAL ( activated (int ) ),
			 this, SLOT ( amtSolventChanged ( ) ) );
	connect ( ui . amtSlvt_unit  , SIGNAL ( activated (int ) ),
			 this, SLOT ( amtSolventChanged ( ) ) );			 			 			 
	connect ( ui . molarMassSolvent  , SIGNAL ( valueChanged ( double )),
			 this, SLOT ( molarMassSolventChanged ( double ) ) );
	connect ( ui . densitySolvent  , SIGNAL ( valueChanged ( double ) ),
			 this, SLOT ( densitySolventChanged ( ) ) );
	connect ( ui . densSlvt_unit  , SIGNAL ( activated ( int )),
			 this, SLOT ( densitySolventChanged ( ) ) );			 
	connect ( ui . concentration  , SIGNAL ( valueChanged ( double )),
			 this, SLOT ( concentrationChanged ( double ) ) );			 			 			 
			 	 			 
	ui.amtSlt_unit2-> hide();			// Mass is the default mode of specification of amount of solvent
	ui.amtSlvt_unit-> hide();			// volume is the default for solvent
	/**************************************************************************/        
	// 				concentration Calculator setup complete	         
	/**************************************************************************/
}

concCalculator:: ~concCalculator()
{

}
// Calculates the amount of solute
void concCalculator::calculateAmtSolute  ( void )
{
	int type1 = ui . conc_unit -> currentIndex ();
	int type2 = ui . amtSltType -> currentIndex ();
	
	double molesSolute , eqtsSolute, massSolute, volSolute;		// variables
	int mode = 0;
	/*
	 * mode = 1 ( molesSolute) mode = 2 eqtsSolute, mode = 3 mass, 4 volume
	 */
	// Calculate the number of moles of the solute
	switch ( type1 )
	{
		// calculate the number of moles of solute
		case 0:	// molarity specified
			molesSolute = m_concentration * volumeSolvent();																
			mode = 1;
			break;
		// Calculate the number of equivalents of solute
		case 1:	// Normality specified
			eqtsSolute = m_concentration * volumeSolvent();
			mode = 2;
			break;
		// Calculate the number of moles of solute
		case 2:	// molality specified
			molesSolute  = m_concentration * massSolvent() * 1000.0;
			mode = 1;
			break;
		// Calculate the mass of solute
		case 3:	// mass percentage specified
			massSolute  = m_concentration / ( 100.0 - m_concentration );
			massSolute *= massSolvent();
			mode = 3;
			break;	
		// Calculate the volume of solute
		case 4:	// volume percentage specified
			volSolute  = m_concentration / ( 100.0 - m_concentration );
			volSolute *= volumeSolvent();
			mode = 4;
			break;
		// Calculate the moles of solute
		case 5: //mole percentage specified
			molesSolute  = m_concentration / ( 100.0 - m_concentration );
			molesSolute *= molesSolvent ();
			mode = 1;
			break;
		default:
			break;
	}
			
		// We have the amount of solvent in some form ( moles, equivalents, mass, volume etc )
		// Now we have to present it in the UI
	switch ( type2 )
	{
		case 0:	// amount should be specified interms of mass
			switch ( mode )
			{
				case 1:	// we should get mass from moles
					massSolute = molesSolute * m_molarMass;
					break;
				case 2:	// we should obtain mass from number of equivalents
					massSolute = eqtsSolute * m_eqtMass;
					break;
				case 3: // we already know the mass of the solute
					break;
				case 4: // we should get the mass from volume
					massSolute = volSolute * densitySolute ();
					break;					
			}
			break;
	
		case 1: // amount should be specified in terms of volume
			switch ( mode )
			{
				case 1: // we should get the volume from moles
					volSolute = molesSolute * m_molarMass / densitySolute ();
					break;
				case 2: // we should get the volume from equivalents
					volSolute = eqtsSolute * m_eqtMass / densitySolute ();
					break;
				case 3:	// we should get the volume from mass
					volSolute = massSolute / densitySolute ();
					break;
				case 4: // we already know the volume
					break;					
			}
			break;
			
		case 2: // amount should be specified in terms of moles
			switch ( mode )
			{
				case 1:	// we already know the moles of solute
					break;
				case 2: // we should obtain moles from equivalents ( not possible )
					molesSolute = 0.0;
					break;
				case 3: // we should obtain moles from mass
					molesSolute = massSolute / m_molarMass;
					break;
				case 4: // we should obtain moles from volume
					molesSolute = volSolute / densitySolute() / m_molarMass;
					break;
			}
			break;
	}
	return;
}

// Calculates the molar mass
void concCalculator::calculateMolarMass ( void )
{
	// molarity / molality / mole fraction required
	int type = ui . conc_unit -> currentIndex ();
	int type2 = ui . amtSlvtType -> currentIndex ();
	double numMoles;
	switch ( type )
	{
		case 0:		//molarity specified
			// number of moles = volume * concentration
			numMoles = volumeSolvent() * m_concentration;
			break;
		case 1:		// cannot be calculated ( insufficient data )
			break;
		case 2:		// molality specified
			numMoles = massSolvent ()/1000.0 * m_concentration;
			break;
		case 3:		// cannot be calculated ( insufficient data )
		case 4:
			break;
		case 5:		// mole fraction specified
			numMoles = m_concentration / ( 100.0 - m_concentration ) * molesSolvent ();
			break;
	}
	
	if ( type2 == 2)	// amount of solute is specified in moles, cannot calculate
		return;
	else
	{
		m_molarMass = massSolvent () / numMoles;
	}
}

// Calculates the equivalent mass
void concCalculator::calculateEqtMass ( void )
{
	// Normality required
	int type = ui . conc_unit -> currentIndex ();
	int type2 = ui . amtSltType -> currentIndex ();
	
	double numEqts;
	switch ( type )
	{
		// Normality required	
		case 0:	// molarity not sufficient
			break;
		case 1: // normality specified
			numEqts = volumeSolvent() * m_concentration;
		case 2:
		case 3:
		case 4:
		case 5:
			break;
	}
	
	if ( type2 == 2)	// Amount of solute specified in moles, cannot calculate
		return;
	else
	{
		m_eqtMass = massSolvent () / numEqts;
	}
	return;
}

// Calculates the calculate molar mass of the solvent
void concCalculator::calculateMolarMassSolvent ( void )
{
	// molarity / molality / mole fraction required
	int type  = ui . conc_unit -> currentIndex ();
	int type2 = ui . amtSlvtType -> currentIndex ();
	double numMoles;
	switch ( type )
	{
		case 0:			// molarity specified
		case 1:			// normality specified
		case 2:			// molality specified
		case 3:			// mass fraction specified
		case 4:			// volume fraction specified
			break;		// cannot be calculated ( insufficient data )
		case 5:			// mole fraction specified
			numMoles = ( 100.0 - m_concentration ) /m_concentration * molesSolute ();
			break;
	}
	if ( type2 == 2 )	// amount specified in moles
		return;			// cannot be calculated ( insufficient data )
	else
	{
		m_molarMassSolvent = massSolvent () / numMoles;
	}
	return;
}

// Calculates the amount of solvent
void concCalculator::calculateAmtSolvent ( void )
{
	int type1 = ui . conc_unit -> currentIndex ();
	int type2 = ui . amtSlvtType -> currentIndex ();
	
	double moleSolvent , massSolvent, volSolvent;
	
	int mode = 0;				// Indicates the mode in which we have calculated the amount of solvent
	/*
	 * mode = 1 ( molessolvent) mode = 2 eqtssolvent, mode = 3 mass, 4 volume
	 */
	// Calculate the number of moles of the solvent
	switch ( type1 )
	{
		// calculate the number of moles of solvent
		case 0:	// molarity specified
			volSolvent = molesSolute () / m_concentration;																
			mode = 3;
			break;
		// Calculate the number of equivalents of solvent
		case 1:	// Normality specified
			volSolvent = molesSolute() / m_concentration;
			mode = 3;
			break;
		// Calculate the number of moles of solvent
		case 2:	// molality specified
			massSolvent  = molesSolute() / 1000.0 / m_concentration;
			mode = 2;
			break;
		// Calculate the mass of solvent
		case 3:	// mass percentage specified
			massSolvent  = ( 100.0 - m_concentration ) / m_concentration;
			massSolvent *= massSolute();
			mode = 2;
			break;	
		// Calculate the volume of solvent
		case 4:	// volume percentage specified
			volSolvent  = ( 100.0 - m_concentration ) / m_concentration;
			volSolvent *= volumeSolvent();
			mode = 3;
			break;
		// Calculate the moles of solvent
		case 5: //mole percentage specified
			moleSolvent  = ( 100.0 - m_concentration ) / m_concentration;
			moleSolvent *= molesSolvent ();
			mode = 1;			
			break;
		default:
			break;
	}
			
		// We have the amount of solvent in some form ( moles, equivalents, mass, volume etc )
		// Now we have to present it in the UI
	switch ( type2 )
	{
		case 0:	// amount specified interms of volume
			switch ( mode )
			{
				case 1:	// obtain volume from moles
					volSolvent = moleSolvent * m_molarMassSolvent / densitySolvent ();
					break;
				case 2: // obtain volume from mass
					volSolvent = massSolvent / densitySolvent();
					break;
				case 3:	// volume already known
					break;
			}
			break;
			
		case 1: // amount specified in terms of volume
			switch ( mode )
			{
				case 1: // obtain mass from moles
					massSolvent = moleSolvent / m_molarMassSolvent;
					break;
				case 2:	// mass already known
					break;
				case 3: // obtain mass from volume
					massSolvent = volSolvent / densitySolvent ();
					break;
			}
			break;
			
		case 2: // amount specified in terms of moles
			switch ( mode )
			{
				case 1: // moles already known				
					break;
				case 2: // obtain moles from mass
					moleSolvent = massSolvent / m_molarMassSolvent;
					break;
				case 3: // obtain moles from volume
					moleSolvent = volSolvent * densitySolvent () / m_molarMassSolvent;
					break;
			}
			break;
	}
	return;
}

// calculates the concentration
void concCalculator::calculateConcentration ( void )
{
	int type = ui . conc_unit -> currentIndex ();
	
	switch ( type )
	{
		case 0:		// molarity
			m_concentration = molesSolute() / volumeSolvent ();
			break;
		case 1:		// normality
			m_concentration = eqtsSolute()/ volumeSolvent ();
			break;
		case 2:		// molality
			m_concentration = molesSolute () / massSolvent () * 1000.0;
			break;
		case 3:		// mass fraction
			m_concentration = massSolute () / ( massSolute () + massSolvent ());
			break;
		case 4:		// volume fraction
			m_concentration = volumeSolute () / ( volumeSolute () + volumeSolvent() );
			break;
		case 5:		// mole fraction
			m_concentration = molesSolute () / ( molesSolute () + molesSolvent ());
			break;
		default:
			break;	
	}
	return;
}

double concCalculator::volumeSolvent ()
{
	int type = ui . amtSlvtType -> currentIndex ();
	double volume;
	switch ( type )
	{
		case 0:
			volume =( Converter::self() -> convert( m_amtSolvent ,"liter")) . number ();
			break;
		case 1:
			volume = massSolvent () / densitySolvent ();
			break;
		case 2:
			volume = massSolvent () / densitySolvent ();
		default:
			break;
	}
	return volume;
}

double concCalculator::molesSolvent ()
{
	int type = ui . amtSlvtType -> currentIndex ();
	
	double moles;
	switch ( type )
	{
		case 0:
			moles = massSolvent() / m_molarMassSolvent;
			break;
		case 1:
			moles = massSolvent() / m_molarMassSolvent;
			break;
		case 2:
			moles =  m_molesSolvent;
			break;
		default:
			break;
	}
	return moles;

}
double concCalculator::massSolvent ()
{
	int type = ui . amtSlvtType -> currentIndex ();
	double mass;
	switch ( type )
	{
		case 0:
			mass = volumeSolvent () / densitySolvent ();
			break;
		case 1:
			mass = ( Converter::self() -> convert( m_amtSolvent,"gram")) .number ();
			break;
		case 2:
			mass = m_molesSolvent * m_molarMassSolvent;
			break;
		default:
			break;
	}
	return mass;
}

double concCalculator::densitySolvent ()
{
	return ( ( Converter::self() -> convert( m_densitySolvent, "grams per liter")) . number () );
}

double concCalculator::volumeSolute ()
{
	int type = ui . amtSltType -> currentIndex ();
	double volume;
	switch ( type )
	{
		case 0:
			volume =( Converter::self() -> convert( m_amtSolute ,"liter")) . number ();
			break;
		case 1:
			volume = massSolute () / densitySolute ();
			break;
		case 2:
			volume = massSolute () / densitySolute ();
		default:
			break;
	}
	return volume;
}

double concCalculator::molesSolute ()
{
	int type = ui . amtSltType -> currentIndex ();
	
	double moles;
	switch ( type )
	{
		case 0:
			moles = massSolute() / m_molarMass;
			break;
		case 1:
			moles = massSolute() / m_molarMass;
			break;
		case 2:
			moles =  m_molesSolute;
			break;
		default:
			break;
	}
	return moles;
}

double concCalculator::eqtsSolute ()
{
	int type = ui . amtSltType -> currentIndex ();
	
	double moles;
	switch ( type )
	{
		case 0:
			moles = massSolute() / m_eqtMass;
			break;
		case 1:
			moles = massSolute() / m_eqtMass;
			break;
		case 2:
			// Cannot be calculated
			moles = 0.0;
			break;
		default:
			break;
	}
	return moles;

}

double concCalculator::massSolute ()
{
	int type = ui . amtSltType -> currentIndex ();
	double mass;
	switch ( type )
	{
		case 0:
			mass = volumeSolute () / densitySolute ();
			break;
		case 1:
			mass = ( Converter::self() -> convert( m_amtSolute,"gram")) .number ();
			break;
		case 2:
			mass = m_molesSolute * m_molarMass;
			break;
		default:
			break;
	}
	return mass;
}

double concCalculator::densitySolute ()
{
	return ( ( Converter::self() -> convert( m_densitySolute, "grams per liter")) . number () );
}


// occurs when the amount of solute is changed
void concCalculator::amtSoluteChanged ( void )
{
	int type = ui . amtSltType -> currentIndex ();
	if ( type == 0 ) 		// amount of solute specified in terms of mass
	{
		ui.amtSlt_unit -> show();
		ui.amtSlt_unit2-> hide();
		m_amtSolute = Value ( ui . amtSolute -> value (), ui . amtSlt_unit -> currentText () );	
	}
	else if ( type == 1)	// amount of solute is specified in terms of volume
	{
		ui.amtSlt_unit -> hide();
		ui.amtSlt_unit2-> show();
		m_amtSolute = Value ( ui . amtSolute -> value (), ui . amtSlt_unit2 -> currentText () );	
	}
	else					// amount of solute is specified in terms of moles
	{
		m_molesSolute = ui . amtSolute -> value ();
		ui.amtSlt_unit -> hide();
		ui.amtSlt_unit2-> hide();
	}
}

// occurs when the amount of solvent is changed
void concCalculator::amtSolventChanged ( void )
{
	int type = ui . amtSlvtType -> currentIndex ();
	if ( type == 0 ) 	// amount of solvent specified in terms of mass
	{
		ui.amtSlvt_unit -> show();
		ui.amtSlvt_unit2-> hide();
		m_amtSolvent = Value ( ui . amtSolvent -> value (), ui . amtSlvt_unit -> currentText () );
	}
	else if ( type == 1 ) // amount of solvent is specified in terms of mass
	{
		ui.amtSlvt_unit -> hide();
		ui.amtSlvt_unit2-> show();
		m_amtSolvent = Value ( ui . amtSolvent -> value (), ui . amtSlvt_unit2 -> currentText () );
	}
	else
	{
		ui.amtSlvt_unit -> hide();
		ui.amtSlvt_unit2-> hide();
		m_molesSolvent = ui . amtSolvent -> value ();
	}
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
void concCalculator::densitySoluteChanged ( void )
{
	m_densitySolute = Value ( ui . densitySolute -> value (), ui . densSlt_unit -> currentText () );
}

// occurs when the density of solvent is changed
void concCalculator::densitySolventChanged ( void )
{
	m_densitySolvent = Value ( ui . densitySolvent -> value (), ui . densSlvt_unit -> currentText () );
}

// occurs when the concentration is changed
void concCalculator::concentrationChanged( double value )
{
	m_concentration = value;
}
// occurs when any quantity is changed
void concCalculator::calculate ( void )
{
	// Calculate the amount of solute
	if ( ui . r1 -> isChecked () )
	{
		if ( ui . conc_unit -> currentIndex () > 2 && ui . concentration -> value () > 100 )
			return;		
		calculateAmtSolute ();
	}
	// Calculate the molar mass of solute
	else if ( ui . r2 -> isChecked () )
	{
		calculateMolarMass ();
	}
	// Calculate the equivalent mass of solute
	else if ( ui . r3 -> isChecked () )
	{
		calculateEqtMass ();
	}
	// Calculate the amount of solvent
	else if ( ui . r5 -> isChecked () )
	{
		calculateAmtSolvent ();
	}
	// Calculate the molar mass of solvent
	else if ( ui . r6 -> isChecked () )
	{
		calculateMolarMassSolvent ();
	}
	// Calculate the concentration of the solution
	else
	{
		calculateConcentration ();
	}
}
#include "concCalculator.moc"

