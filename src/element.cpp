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
#include "element.h"
#include "prefs.h"
#include <qregexp.h>
#include <kdebug.h>
#include <klocale.h>

#include <math.h>

#include <qpainter.h>

#define ELEMENTSIZE 45

Element::Element( int num )
{
	m_number = num;

	/*
	 * this object is static because this reduces the access to the file 
	 * by 90% (strace counts 16 calls instead of 116 )
	 */
	static KSimpleConfig config ( locate( "data", "kalzium/kalziumrc" ) );
	
	if ( config.hasGroup( QString::number( num ) ) )

	config.setGroup( QString::number( num ) );
	m_name=config.readEntry( "Name", "Unknown" );
	m_symbol=config.readEntry( "Symbol", "Unknown" );
	m_weight=config.readDoubleNumEntry( "Weight",0.0 );

	m_oxstage=config.readEntry( "Ox","0" );
	m_acidbeh=config.readEntry( "acidbeh","0" );
	m_isotopes=config.readEntry( "Isotopes", "0" );

	m_block=config.readEntry( "Block","s" );
	m_EN=config.readDoubleNumEntry( "EN", -1 );
	m_MP=config.readDoubleNumEntry( "MP", -1 );
	m_IE=config.readDoubleNumEntry( "IE", -1 );
	m_IE2=config.readDoubleNumEntry( "IE2", -1 );
	m_AR=config.readDoubleNumEntry( "AR", -1 );
	m_BP=config.readDoubleNumEntry( "BP", -1 );
	m_Density=config.readDoubleNumEntry( "Density", -1 );
	m_group=config.readEntry( "Group","1" );
	m_orbits=config.readEntry( "Orbits","0" );
	m_biological=config.readNumEntry(  "biological" , -1 );
	m_az=config.readNumEntry( "az",-1 );
	m_date=config.readNumEntry( "date",-1 );

	setupXY();
}

QString Element::parsedOrbits()
{
	QString orbits = m_orbits;
	QRegExp rxs("([a-z])([0-9]+)");
	QRegExp rxb("([a-z]{2}) ",false);
	orbits.replace(rxs,"\\1<sup>\\2</sup>"); //superscript around electron number
	orbits.replace(rxb,"<b>\\1</b> "); //bold around element symbols
	return orbits;
}


double Element::strippedWeight( double num )
{
	if ( !finite( num ) )
		return num;

	double power;
	power = 1e-6;
	while ( power < num )
		power *= 10;

	num = num / power * 10000;
	num = round( num );

	return num * power / 10000;
}


Element::~Element()
{
}



double Element::meanweight()
{
	return m_weight/m_number;
}

const QString Element::adjustUnits( const int type )
{
	QString v = QString::null;

	double val = 0.0; //the value to convert
	
	if ( type == IE || type == IE2 ) //an ionization energy
	{
		if ( type == IE )
			val = ie();
		else
			val = ie2();
	
		if ( val == -1 )
			v = i18n( "Value unknown" );
		else 
		{
			if ( Prefs::energies() == 0 )
			{
				val*=96.6;
				v = QString::number( val );
				v.append( "kj/mol" );
			}
			else // use electronvolt
			{
				v = QString::number( val );
				v.append( "eV" );
			}
		}
	}
	else if ( type == BOILINGPOINT || type == MELTINGPOINT ) // convert a temperature
	{
		if ( type == BOILINGPOINT )
			val = boiling();
		else
			val = melting();

		switch (Prefs::temperature()) {
			case 0: //Kelvin
				v = QString::number( val );
				v.append( "K" );
				break;
			case 1://Kelvin to Celsius
				val-=273.15;
				v = QString::number( val );
				v.append( "C" );
				break;
			case 2: // Kelvin to Fahrenheit
				val = val * 1.8 - 459.67;
				v = QString::number( val );
				v.append( "F" );
				break;
		}
	}
	else if ( type == RADIUS ) // its a length
	{
		val = radius();

		switch ( Prefs::units() )
		{
			case 0:
				v = QString::number( val );

				v.append( " 10<sup>-12</sup>m" );
				break;
			case 1:
				v = QString::number( val );

				v.append( i18n( " pm" ) );
				break;
		}
	}
	else if ( type == WEIGHT ) // its a weight
	{
		val = weight();

		v = QString::number( val );
		switch ( Prefs::units() )
		{
			case 0:
				v.append( i18n( "g/mol" ) );
				break;
			case 1:
				v.append( i18n( " u" ) );
				break;
		}
	}
	else if ( type == DENSITY ) // its a density
	{
		val = density();

		switch ( Prefs::units() )
		{
			case 0:
				val *= 1000;
				v = QString::number( val );
				v.append( " kg/m<sup>3</sup>" );
				break;
			case 1:
				if ( az() == 2 )//gasoline
				{
					v = QString::number( val );
					v.append( " g/L" );
				}
				else//liquid or solid
				{
					v = QString::number( val );
					v.append( " g/cm<sup>3</sup>" );
				}
				break;
		}
	}
	else if ( type == DATE ) //its a date
	{
		val = date();
		if ( val < 1600 )
		{
			v = i18n( "This element was known to ancient cultures" );
		}
		else
		{
			v = i18n( "This element was discovered in the year %1" ).arg( QString::number( val ) );
		}
	}

	return v;
}

void Element::drawSelf( QPainter* p, bool useSimpleView )
{
	int h_small = 15; //the size for the small units like elementnumber
	
	p->setPen( elementColor() );
	p->fillRect( ( x-1 )*ELEMENTSIZE+3, ( y-1 )*ELEMENTSIZE+3,ELEMENTSIZE-6,ELEMENTSIZE-6, elementColor() );
	p->drawRoundRect( ( x-1 )*ELEMENTSIZE+2, ( y-1 )*ELEMENTSIZE+2,ELEMENTSIZE-4,ELEMENTSIZE-4 );
	
	QString text;
	QFont symbol_font = p->font();
	symbol_font.setPointSize( 18 );
	QFont f = p->font();
	f.setPointSize( 9 );
		
	p->setFont( f );

	//top left
	p->setPen( Qt::black );
	text = QString::number( strippedWeight( weight( ) ) );
	p->drawText( ( x-1 )*ELEMENTSIZE+2,( y-1 )*ELEMENTSIZE ,ELEMENTSIZE-2,h_small,Qt::AlignLeft, text );

	text = QString::number( number() );
	p->drawText( ( x-1 )*ELEMENTSIZE + 2,( y )*ELEMENTSIZE - h_small, ELEMENTSIZE-2, h_small,Qt::AlignLeft, text );

	p->setFont( symbol_font );
	p->drawText( ( x-1 )*ELEMENTSIZE,( y-1 )*ELEMENTSIZE, ELEMENTSIZE,ELEMENTSIZE,Qt::AlignCenter, symbol() );
	
	//border
	p->setPen( Qt::green );
	p->drawRoundRect( ( x-1 )*ELEMENTSIZE+1, ( y-1 )*ELEMENTSIZE+1,ELEMENTSIZE-2,ELEMENTSIZE-2);
}

/*!
    \fn Element::setupXY()
    This looks pretty evil and it is. The problem is that a PSE is pretty
    irregular and you cannot "calculate" the position. This means that 
    every type of PSE need such a complicated list of x/y-pairs...
 */
void Element::setupXY()
{
 static const int posXRegular[111] = {1,18,
	 								1,2,13,14,15,16,17,18,
									1,2,13,14,15,16,17,18,
 									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
 									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,  //Element 54 (Xe)
									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,     //Element 71 (Lu)
									      4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,     //Element 71 (Lr)
									      4,5,6,7,8,9,10,11};
 static const int posYRegular[111] = {1,1,
	 								2,2, 2, 2, 2, 2, 2, 2,
									3,3, 3, 3, 3, 3, 3, 3,
 									4,4,4,4,4,4,4,4,4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 									5,5,5,5,5,5,5,5,5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  //Element 54 (Xe)
						    		6,6,6,8,8,8,8,8,8, 8, 8, 8, 8, 8, 8, 8, 8,     //Element 71 (Lr)
									      6,6,6,6,6,6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
									7,7,7,9,9,9,9,9,9, 9, 9, 9, 9, 9, 9, 9, 9,
									      7,7,7,7,7,7,7,7};
 
 x = posXRegular[m_number-1];
 y = posYRegular[m_number-1];
}

KalziumDataObject::KalziumDataObject()
{
	for( int i = 1 ; i < 112 ; ++i )
	{
		ElementList.append( new Element( i ) );
	}
}

KalziumDataObject::~KalziumDataObject()
{}

