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
#include "pse.h"
#include "prefs.h"

#include <klocale.h>
#include <kdebug.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qlabel.h>

#include <qpainter.h>

PSE::PSE(KalziumDataObject *data, QWidget *parent, const char *name)
 : QWidget(parent, name)
{
	d = data;

	m_molcalcIsActive = false;

//IUPAC
	    m_IUPAClist.append( "IA");
	    m_IUPAClist.append( "IIA");
	    m_IUPAClist.append( "IIIB");
	    m_IUPAClist.append( "IVB");
	    m_IUPAClist.append( "VB");
	    m_IUPAClist.append( "VIB");
	    m_IUPAClist.append( "VIIB");
	    m_IUPAClist.append( "VIII");
	    m_IUPAClist.append( "VIII");
	    m_IUPAClist.append( "VIII");
	    m_IUPAClist.append( "IB");
	    m_IUPAClist.append( "IIB");
	    m_IUPAClist.append( "IIIA");
	    m_IUPAClist.append( "IVA");
	    m_IUPAClist.append( "VA");
	    m_IUPAClist.append( "VIA");
	    m_IUPAClist.append( "VIIA");
	    m_IUPAClist.append( "VIIIA");

//oldIUPAC
	    m_IUPACOLDlist.append( "1A");
	    m_IUPACOLDlist.append( "2A");
	    m_IUPACOLDlist.append( "3A");
	    m_IUPACOLDlist.append( "4A");
	    m_IUPACOLDlist.append( "5A");
	    m_IUPACOLDlist.append( "6A");
	    m_IUPACOLDlist.append( "7A");
	    m_IUPACOLDlist.append( "8");
	    m_IUPACOLDlist.append( "8");
	    m_IUPACOLDlist.append( "8");
	    m_IUPACOLDlist.append( "1B");
	    m_IUPACOLDlist.append( "2B");
	    m_IUPACOLDlist.append( "3B");
	    m_IUPACOLDlist.append( "4B");
	    m_IUPACOLDlist.append( "5B");
	    m_IUPACOLDlist.append( "6B");
	    m_IUPACOLDlist.append( "7B");
	    m_IUPACOLDlist.append( "0");
	
}

PSE::~PSE(){}

void PSE::updateNumeration()
{
}

void PSE::activateColorScheme( const int nr )
{
}

void PSE::setDate( int date )
{
//6 16 26 29 33 47 50 51 79 80 82 83
}

void PSE::paintEvent( QPaintEvent *e )
{
	QPainter p;
	p.begin( this );
	drawPSE( &p, true );
	p.end();
}

void PSE::drawPSE( QPainter* p, bool useSimpleView )
{
	kdDebug() << "PSE::drawPSE" << endl;

	EList::Iterator it = d->ElementList.begin();

	while ( it != d->ElementList.end() )
	{
		( *it )->drawSelf( p, false );
		++it;
	}

}


void PSE::setTemperature( const double temp )
{
//X 	//Important: The value temp is in Kelvin, not Degree Celsius!
//X 	
//X 	{//iterate through all buttons
//X 		const int az = button->e->az();
//X 		if ( az == 3 || az == 4 )
//X 		{ //check if the element is radioactive or artificial
//X 			if ( az == 3 ) button->setPaletteBackgroundColor(Prefs::color_radioactive() );
//X 			if ( az == 4 ) button->setPaletteBackgroundColor( Prefs::color_artificial() );
//X 			continue;
//X 		}
//X 
//X 		double iButton_melting = button->e->melting();
//X 		double iButton_boiling = button->e->boiling();
//X 		
//X 		if ( temp < iButton_melting )
//X 		{ //the element is solid
//X 			button->setPaletteBackgroundColor( Prefs::color_solid());
//X 			continue;
//X 		}
//X 		if ( temp > iButton_melting &&
//X 			temp < iButton_boiling )
//X 		{ //the element is liquid
//X 			button->setPaletteBackgroundColor( Prefs::color_liquid() );
//X 			continue;
//X 		}
//X 		if ( temp > iButton_boiling )
//X 		{ //the element is vaporous
//X 			button->setPaletteBackgroundColor( Prefs::color_vapor() );
//X 			continue;
//X 		}
}

#include "pse.moc"
