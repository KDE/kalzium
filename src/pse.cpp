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
#include "infodialog_small_impl.h"
#include "detailinfodlg.h"

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
	EList::Iterator it = d->ElementList.begin();
	const EList::Iterator itEnd = d->ElementList.end();

	if ( nr == 0) //normal view, no colors
	{
		const QColor color = Prefs::noscheme();
		while ( it != itEnd )
		{
			( *it )->setElementColor( color );
			++it;
		}
	}
	else if ( nr == 1) //groups view
	{
		const QColor color_1 = Prefs::group_1();
		const QColor color_2 = Prefs::group_2();
		const QColor color_3 = Prefs::group_3();
		const QColor color_4 = Prefs::group_4();
		const QColor color_5 = Prefs::group_5();
		const QColor color_6 = Prefs::group_6();
		const QColor color_7 = Prefs::group_7();
		const QColor color_8 = Prefs::group_8();

		static QString group;

		while ( it != itEnd )
		{
			group = ( *it )->group();
			
			if (group == QString("1")) {
				( *it )->setElementColor( color_1 );
			}
			if (group == QString("2")){
				( *it )->setElementColor( color_2 );
			}
			if (group == QString("3")){
				( *it )->setElementColor( color_3 );
			}
			if (group == QString("4")){
				( *it )->setElementColor( color_4 );
			}
			if (group == QString("5")){
				( *it )->setElementColor( color_5 );
			}
			if (group == QString("6")){
				( *it )->setElementColor( color_6 );
			}
			if (group == QString("7")){
				( *it )->setElementColor( color_7 );
			}
			if (group == QString("8")){
				( *it )->setElementColor( color_8 );
			}

			++it;
		}
	}
	else if ( nr == 2) //block view
	{
		const QColor color_s = Prefs::block_s();
		const QColor color_p = Prefs::block_p();
		const QColor color_d = Prefs::block_d();
		const QColor color_f = Prefs::block_f();

		static QString block;
		while ( it != itEnd )
		{
			block = (*it)->block();

			if (block == QString("s")) {
				(*it)->setElementColor( color_s );
			}
			if (block == QString("p")) {
				(*it)->setElementColor( color_p );
			}
			if (block == QString("d")) {
				(*it)->setElementColor( color_d );
			}
			if (block == QString("f")) {
				(*it)->setElementColor( color_f );
			}
			++it;
		}
	}
	else if ( nr == 3) //acidic beh
	{
		const QColor color_ba = Prefs::beh_basic();
		const QColor color_ac = Prefs::beh_acidic();
		const QColor color_neu = Prefs::beh_neutral();
		const QColor color_amp = Prefs::beh_amphoteric();

		static QString acidicbeh;
		
		while ( it != itEnd )
		{
			acidicbeh = ( *it )->acidicbeh();

			if (acidicbeh == QString("0")) {
				(*it)->setElementColor( color_ac );
				continue;
			}
			if (acidicbeh == QString("1")){
				(*it)->setElementColor( color_ba );
				continue;
			}
			if (acidicbeh == QString("2")){
				(*it)->setElementColor( color_neu );
				continue;
			}
			if (acidicbeh == QString("3")){
				(*it)->setElementColor( color_amp );
				continue;
			}
			++it;
		}
	}
}

void PSE::setDate( int date )
{
//6 16 26 29 33 47 50 51 79 80 82 83
}

void PSE::paintEvent( QPaintEvent *e )
{
	QPainter p;
	p.begin( this );

	if ( m_showSOM )
		drawSOMPSE( &p );
	else
		drawPSE( &p, m_isSimple );
	p.end();
}

void PSE::drawSOMPSE( QPainter* p )
{
	EList::Iterator it = d->ElementList.begin();

	while ( it != d->ElementList.end() )
	{
		( *it )->drawStateOfMatter( p, m_temperature );
		++it;
	}

}

void PSE::mouseReleaseEvent( QMouseEvent *mouse )
{
	///first: find out the position
	
	kdDebug() << mouse->x() << " " << mouse->y() << endl;
	int X = mouse->x()/45;
	int Y = mouse->y()/45;
	if ( m_isSimple )
	{
		if ( mouse->x() > ( 2*45 ) )
		{
			X += 10;
		}
	}
		
	X += 1;
	Y += 1;

	//from this on I can use X and Y. Both contain the position of an element in the
	//complete PSE. Eg, He is 1,18 and Na is 2,1
	
	CList::Iterator it = d->CoordinateList.begin();
	while ( it != d->CoordinateList.end() )
	{//iterate through the list of coordinates and compare the x/y values.
	 //finally, if the 20'es iterator has the same cooridnates Element 20
	 //has been clicked.
		if ( (*it ).x == X )
		{
			if ( (*it ).y == Y )
			{//coordinates match. Get the position of the it in the list.
				coordinate c = *it;
				kdDebug() << c.x << " " << c.y << endl;
//				kdDebug() << "Element: " << d->CoordinateList.findIndex( c ) << endl;
			}
		}
	}

	return;
	
	Element *e = new Element( 4 );
	
	int small = Prefs::lMBbeh();
	if ( small == 1 )
	{
		if ( mouse->button() == LeftButton )
		{
			infoDlgSmallImpl *smallDlg = new infoDlgSmallImpl( e, this , "smallDlg" );
			smallDlg->show();
		}else
		{
			DetailedInfoDlg *detailedDlg = new DetailedInfoDlg( e, this , "detailedDlg" );
			detailedDlg->show();
		}	
	}
	if ( small == 0 )
	{
		if ( mouse->button() == LeftButton )
		{
			DetailedInfoDlg *detailedDlg = new DetailedInfoDlg( e, this , "detailedDlg" );
			detailedDlg->show();
		}else
		{
			infoDlgSmallImpl *smallDlg = new infoDlgSmallImpl( e, this , "smallDlg" );
			smallDlg->show();
		}	
	}
}

void PSE::drawPSE( QPainter* p, bool useSimpleView )
{
	EList::Iterator it = d->ElementList.begin();

	while ( it != d->ElementList.end() )
	{
		( *it )->drawSelf( p, useSimpleView );
		++it;
	}

}

#include "pse.moc"
