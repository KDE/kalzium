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
#include <kdebug.h>

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
	m_block=config.readEntry( "Block","s" );
	m_EN=config.readDoubleNumEntry( "EN", -1 );
	m_MP=config.readDoubleNumEntry( "MP", -1 );
	m_IE=config.readDoubleNumEntry( "IE", -1 );
	m_IE2=config.readDoubleNumEntry( "IE2", -1 );
	m_AR=config.readDoubleNumEntry( "AR", -1 );
	m_BP=config.readDoubleNumEntry( "BP", -1 );
	m_Density=config.readDoubleNumEntry( "Density", -1 );
	m_az=config.readEntry( "az","0" );
	m_date=config.readEntry( "date","0" );
	m_Group=config.readEntry( "Group","1" );
	m_orbits=config.readEntry( "Orbits","0" );
	m_biological=config.readNumEntry(  "biological" , -1 );

	setupXY();
}


Element::~Element()
{
}

double Element::meanweight()
{
	return m_weight/m_number;
}

/*!
    \fn Element::setupXY()
    This looks pretty evil and it is. The problem is that a PSE is pretty
    irregular and you cannot "calculate" the position. This means that 
    every type of PSE need such a complicated list of x/y-pairs...
 */
void Element::setupXY()
{
 static const int posXRegular[110] = {1,18,
	 								1,2,13,14,15,16,17,18,
									1,2,13,14,15,16,17,18,
 									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
 									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,  //Element 54 (Xe)
									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,     //Element 71 (Lu)
									      4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,
									1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,     //Element 71 (Lr)
									      4,5,6,7,8,9,10};
 static const int posYRegular[110] = {1,1,
	 								2,2, 2, 2, 2, 2, 2, 2,
									3,3, 3, 3, 3, 3, 3, 3,
 									4,4,4,4,4,4,4,4,4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
 									5,5,5,5,5,5,5,5,5, 5, 5, 5, 5, 5, 5, 5, 5, 5,  //Element 54 (Xe)
						    		6,6,6,8,8,8,8,8,8, 8, 8, 8, 8, 8, 8, 8, 8,     //Element 71 (Lr)
									      6,6,6,6,6,6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
									7,7,7,9,9,9,9,9,9, 9, 9, 9, 9, 9, 9, 9, 9,
									      7,7,7,7,7,7,7};
										  
								
static const int posXSimplified[110] = {
	1,8,
	1,2,3,4,5,6,7,8,
	1,2,
	3,4,5,6,7,8,
	1,2,
	0,0,0,0,0,0,0,0,0,0, //dummy
	
	3,4,5,6,7,8,//36
	1,2,
	0,0,0,0,0,0,0,0,0,0, //dummy
	3,4,5,6,7,8,
	1,2,//56
	0,0,0,0,0,0,0,0,0,0, //dummy 66
	0,0,0,0,0,0,0,0,0,0, //dummy 76
	0,0,0,0, //dummy 80
	3,4,5,6,7,8,
	1,2};

	
static const int posYSimplified[110] = {
	1,1,
	2,2,2,2,2,2,2,2,
	3,3,
	3,3,3,3,3,3,
	4,4,
	0,0,0,0,0,0,0,0,0,0, //dummy
	
	4,4,4,4,4,4,//36
	5,5,
	0,0,0,0,0,0,0,0,0,0, //dummy
	5,5,5,5,5,5,
	6,6,
	0,0,0,0,0,0,0,0,0,0, //dummy
	0,0,0,0,0,0,0,0,0,0, //dummy
	0,0,0,0, //dummy 80
	6,6,6,6,6,6,
	7,7};
 
 x = posXRegular[m_number-1];
 y = posYRegular[m_number-1];

 
 s_y = posYSimplified[m_number-1];
 s_x = posXSimplified[m_number-1];
}

KalziumDataObject::KalziumDataObject()
{
	for( int i = 1 ; i < 111 ; ++i )
	{
		ElementList.append( new Element( i ) );
	}
}

KalziumDataObject::~KalziumDataObject()
{}
