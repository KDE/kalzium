/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "stateofmatterdialog_impl.h"
#include "prefs.h"

#include <kdebug.h>
#include <klocale.h>
#include <knuminput.h>

#include <qlcdnumber.h>
#include <qslider.h>
#include <qdial.h>

somDialogImpl::somDialogImpl(PSE* p, QWidget* parent, const char* name)
 : somDialog(parent,name)
{
	kdDebug() << "somDialogImpl()" << endl;
	connect( Slider, SIGNAL( valueChanged(int) ), this, SLOT( slotValueChanged(int) ) );
	connect( Dial, SIGNAL( valueChanged(int) ), this, SLOT( slotValueChanged(int) ) );
	connect( UserInput, SIGNAL( valueChanged(int) ), this, SLOT( slotValueChanged(int) ) );

	switch ( Prefs::temperature() ){
		case 0:
			UserInput->setSuffix( i18n( "The symbol for Kelvin. There is a space in front of the K!", " K" ) );
			Dial->setMinValue( 0 );
			Dial->setMaxValue(5700 );
			UserInput->setMinValue( 0 );
			UserInput->setMaxValue( 5700 );
			break;
		case 1:
			UserInput->setSuffix( i18n( "The symbol for Degree Fahrenheit. There is a space before the F!", " F" ) );
			//XXX find out about the scale of fahrenheit...
			UserInput->setMinValue( -273 );
			UserInput->setMaxValue( 6000 );
			break;
		case 2:
			UserInput->setSuffix( i18n( "The symbol for Degree Celsius. There is a space in front of the °C!", " °C" ) );
			Dial->setMinValue( -273 );
			Dial->setMaxValue( 6000 );
			UserInput->setMinValue( -273 );
			UserInput->setMaxValue( 6000 );
			break;
	}
	
	m_pse = p;

}

void somDialogImpl::updateNumbers()
{
	lcdKelvin->display( QString::number( m_temp ) );
	lcdFahrenheit->display( QString::number( m_temp ) );
	lcdCelsius->display( QString::number( m_temp+273 ) );
}

void somDialogImpl::slotValueChanged( int v )
{
	updateNumbers();

	Slider->setValue( v );
	Dial->setValue( v );
	UserInput->setValue( v );
	
	m_temp = v;
	
	switch ( Prefs::temperature() ){
		case 0: //Kelvin is used
			break;
		case 1: //Fahrenheit is used
			//XXX convert to Fahrenheit
			break;
		case 2: //Celsius is used
			m_temp += 273;
			break;
	}

	m_pse->setTemperature( (double)m_temp );
}


#include "stateofmatterdialog_impl.moc"
