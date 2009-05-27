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

using namespace Conversion;

gasCalculator::gasCalculator( QWidget * parent )
	: QFrame ( parent )
{	
	ui.setupUi( this );
	
	/**************************************************************************/
	//						 Gas Calculator set up
	/**************************************************************************/
    
    // initialise the initially selected values
    ui.molarMass-> setValue ( 2.008 );
    ui.temp	 	-> setValue ( 273.0 );
	ui.volume	-> setValue ( 22.400);
	ui.pressure	-> setValue ( 1.0 );
	ui.a 		-> setValue ( 0.0 );
	ui.b	 	-> setValue ( 0.0 );
	ui.mass 	-> setValue ( 2.016 );
	ui.moles	-> setValue ( 1.0 );
	// Setup of the UI done
	
	// Initialise values
	m_temp = Value(273.0,"kelvins");
	m_molarMass = 2.016;
	m_pressure = Value(1.0,"atmosphere");
	m_mass = Value(2.016,"grams");
	m_moles = 1.0;
	m_Vand_a = 0.0;
	m_Vand_b = Value (0.0,"liters");
	m_vol = Value (22.4,"liters");
    // Initialisation of values done
    // Connect signals with slots
	connect ( ui.temp , SIGNAL( valueChanged (double)),
	         this, SLOT ( tempChanged ()));
	connect ( ui.temp_unit , SIGNAL( activated(int)),
	         this, SLOT ( tempChanged ()));
	connect ( ui.volume , SIGNAL( valueChanged (double)),
			 this, SLOT ( volChanged ()));
	connect ( ui.volume_unit , SIGNAL( activated(int)),
			 this, SLOT ( volChanged ()));			 
	connect ( ui.pressure , SIGNAL( valueChanged (double)),
			 this, SLOT ( pressureChanged ()));
	connect ( ui.pressure_unit , SIGNAL( activated(int)),
			 this, SLOT ( pressureChanged ()));			 
	connect ( ui.mass ,  SIGNAL( valueChanged (double)),
			 this, SLOT ( massChanged ()));
	connect ( ui.mass_unit , SIGNAL( activated(int)),
			 this, SLOT ( massChanged ()));			 
	connect ( ui.moles , SIGNAL( valueChanged (double)),
			 this, SLOT ( molesChanged (double)));
	connect ( ui.a ,  SIGNAL( valueChanged (double)),
			 this, SLOT ( massChanged ()));
	connect ( ui.a_unit , SIGNAL( activated(int)),
			 this, SLOT ( massChanged ()));
	connect ( ui.b ,  SIGNAL( valueChanged (double)),
			 this, SLOT ( massChanged ()));
	connect ( ui.b_unit , SIGNAL( activated(int)),
			 this, SLOT ( massChanged ()));	
	/**************************************************************************/        
	// gas Calculator setup complete	         
	/**************************************************************************/
}

gasCalculator:: ~gasCalculator()
{

}

// Calculates the Pressure
void gasCalculator::calculatePressure  ( void )
{
	double pressure;
	double volume = ((Converter::self()->convert(m_vol, "liters")).number());
	double temp = ((Converter::self()->convert(m_temp, "kelvins")).number());
	pressure = m_moles * R * temp / volume;
	m_pressure = Value(pressure,"atmospheres");
	m_pressure = (Converter::self()->convert(m_pressure, ui.pressure_unit->currentText()));
	ui.pressure->setValue(m_pressure.number());
	
	//pressure = 
}	

// Calculates the molar mass of the gas
void gasCalculator::calculateMolarMass ( void )
{
	double mass = ((Converter::self()->convert( m_mass, "grams")).number());
	double volume = ((Converter::self()->convert(m_vol, "liters")).number());
	double pressure = ((Converter::self()->convert(m_pressure, "atmospheres")).number());
	double temp = ((Converter::self()->convert(m_temp, "kelvins")).number());
	
	m_molarMass = mass * R * temp / pressure / volume;
	ui.molarMass->setValue(m_molarMass);	
}

// Calculates the Volume
void gasCalculator::calculateVol ( void )
{
	double volume;
	double pressure = ((Converter::self()->convert(m_pressure, "atmospheres")).number());
	double temp = ((Converter::self()->convert(m_temp, "kelvins")).number());
	
	volume = m_moles * R * temp / pressure;
	m_vol = Value(volume,"liters");
	m_vol = (Converter::self()->convert(m_vol, ui.volume_unit->currentText()));
	ui.volume->setValue(m_vol.number());
}

// Calculates the Temperature
void gasCalculator::calculateTemp ( void )
{
	double temp;
	double volume = ((Converter::self()->convert(m_vol, "liters")).number());
	double pressure = ((Converter::self()->convert(m_pressure, "atmospheres")).number());
	
	temp = pressure * volume / m_moles * R;
	m_temp = Value(temp,"kelvins");
	m_temp = (Converter::self()->convert(m_temp, ui.temp_unit->currentText()));
	ui.temp->setValue(m_temp.number());
}	

// Calculates the number of moles
void gasCalculator::calculateMoles ( void )
{
	double volume = ((Converter::self()->convert(m_vol, "liters")).number());
	double pressure = ((Converter::self()->convert(m_pressure, "atmospheres")).number());
	double temp = ((Converter::self()->convert(m_temp, "kelvins")).number());
	
	m_moles = pressure * volume / R / temp;
	ui.moles->setValue(m_moles);
}

// Calculates the mass of substance
void gasCalculator::calculateMass ( void )
{
	double mass;
	double volume = ((Converter::self()->convert(m_vol, "liters")).number());
	double pressure = ((Converter::self()->convert(m_pressure, "atmospheres")).number());
	double temp = ((Converter::self()->convert(m_temp, "kelvins")).number());
	
	mass = pressure * volume * m_molarMass / R / temp;
	m_mass = Value(mass,"grams");
	m_mass = (Converter::self()->convert(m_mass, ui.mass_unit->currentText()));
	ui.mass->setValue(m_mass.number());
}


// Functions ( slots ) that occur on changing a value
// occurs when the volume is changed
void gasCalculator::volChanged ( void )
{
	m_vol = Value( ui.volume->value(), ui.volume_unit->currentText());
	calculate ();	
}

// occurs when the temperature is changed
void gasCalculator::tempChanged ( void )
{
	m_temp = Value( ui.temp->value(),ui.temp_unit->currentText());
	calculate ();	
}

// occurs when the pressure is changed	
void gasCalculator::pressureChanged ( void )
{
	m_pressure = Value(ui.pressure->value(),ui.pressure_unit->currentText());
	calculate ();
}

// occurs when the mass is changed
void gasCalculator::massChanged ( void )
{
	m_mass = Value(ui.mass->value(),ui.mass_unit->currentText());
	m_moles = ((Converter::self()->convert( m_mass, "grams")).number()) / m_molarMass;
	ui.moles->setValue(m_moles);
	calculate ();
}

// occurs when the number of moles is changed
void gasCalculator::molesChanged ( double value )
{
	m_moles = value;
	m_mass = Value(m_moles*m_molarMass,"grams");
	m_mass = (Converter::self()->convert( m_mass, ui.mass_unit->currentText()));
	ui.mass->setValue(m_mass.number());	
	calculate ();	
}

// occurs when the molar mass is changed
void gasCalculator::molarMassChanged ( double value )
{
	m_molarMass = value;
	m_mass = Value (m_molarMass * m_moles,"grams");
	m_mass = (Converter::self()->convert( m_mass, ui.mass_unit->currentText()));
	ui.mass->setValue(m_mass.number());
	calculate();
}

// occurs when the number of moles is changed
void gasCalculator::Vand_aChanged ( void )
{
	m_Vand_a = ui.a->value();
	calculate ();	
}

// occurs when the number of moles is changed
void gasCalculator::Vand_bChanged ( void )
{
	m_Vand_b = Value( ui.b->value(), ui.b_unit->currentText());
	calculate ();
}

// occurs when any quantity is changed
void gasCalculator::calculate ( void )
{
	if ( ui.r1->isChecked())
	{
		calculateMoles();
	}
	else if ( ui.r2->isChecked())
	{
		calculatePressure();
	}
	else if ( ui.r3->isChecked())
	{
		calculateTemp();
	}
	else if ( ui.r4->isChecked())
	{
		calculateVol();
	}
}

void gasCalculator::error ( int mode )
{
	switch ( mode )
	{
		case VOL_ZERO :
			ui.error->setText("Volume cannot be zero, please correct the error!");	
		default:
			break;
	}

}

void gasCalculator::debug (void)
{
	QString a = m_temp.toString() + m_vol.toString() + m_pressure.toString() + m_mass.toString();
	QString b;
	b.setNum(m_moles);
	a += b + "  ";
	b.setNum(m_molarMass);
	a += b + "  ";
	ui.result->setText(a);
}
#include "gasCalculator.moc"


