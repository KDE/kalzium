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
#include <qpixmap.h>

#include <qpainter.h>

PSE::PSE(KalziumDataObject *data, QWidget *parent, const char *name)
  : QWidget(parent, name), table(0)
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
	
      table = new QPixmap();
      
      //JH: For now, always do a full draw
      doFullDraw = true;
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
			}
			if (acidicbeh == QString("1")){
				(*it)->setElementColor( color_ba );
			}
			if (acidicbeh == QString("2")){
				(*it)->setElementColor( color_neu );
			}
			if (acidicbeh == QString("3")){
				(*it)->setElementColor( color_amp );
			}
			++it;
		}
	}
}

void PSE::setDate( int date )
{
//6 16 26 29 33 47 50 51 79 80 82 83
}

void PSE::resizeEvent( QResizeEvent *e ) 
{
  table->resize( width(), height() );  
}

void PSE::paintEvent( QPaintEvent *e )
{
	QPainter p;
  
  if ( doFullDraw ) {
    p.begin( table );
    p.fillRect( 0, 0, width(), height(), paletteBackgroundColor() );
    if ( m_showSOM )
      drawSOMPSE( &p );
    else
      drawPSE( &p, m_isSimple );

	if ( m_showLegend )
		drawLegend( &p );
    p.end();
  
    //JH: Uncomment when ready for this
    //    doFullDraw = false;
  }
  
  bitBlt( this, 0, 0, table );
}

void PSE::drawSOMPSE( QPainter* p )
{
//X 	const QColor color_1 = Prefs::group_1();
//X 	const QColor color_2 = Prefs::group_2();
//X 	const QColor color_3 = Prefs::group_3();
//X 	const QColor color_4 = Prefs::group_4();
//X 	const QColor color_5 = Prefs::group_5();
//X 	const QColor color_6 = Prefs::group_6();
//X 	const QColor color_7 = Prefs::group_7();
//X 	const QColor color_8 = Prefs::group_8();
//X 
//X 	const QColor color_s = Prefs::block_s();
//X 	const QColor color_p = Prefs::block_p();
//X 	const QColor color_d = Prefs::block_d();
//X 	const QColor color_f = Prefs::block_f();
//X 		
//X 	const QColor color_ba = Prefs::beh_basic();
//X 	const QColor color_ac = Prefs::beh_acidic();
//X 	const QColor color_neu = Prefs::beh_neutral();
//X 	const QColor color_amp = Prefs::beh_amphoteric();
//X 	
//X 	const QColor c_liquid = Prefs::color_liquid();
//X 	const QColor c_solid = Prefs::color_solid();
//X 	const QColor c_vapor = Prefs::color_vapor();
//X 	const QColor c_artificial = Prefs::color_artificial();
//X 	const QColor c_radioactive = Prefs::color_radioactive();
//X 
//X 	switch ( scheme ) {
//X 		case 0:
//X 			one->hide();
//X 			two->hide();
//X 			three->hide();
//X 			four->hide();
//X 			five->hide();
//X 			six->hide();
//X 			seven->hide();
//X 			eight->hide();
//X 			break;
//X 		case 1:
//X 			one->show();
//X 			two->show();
//X 			three->show();
//X 			four->show();
//X 			five->show();
//X 			six->show();
//X 			seven->show();
//X 			eight->show();
//X 			one->setText( i18n( "Group 1" ) );
//X 			two->setText( i18n( "Group 2" ) );
//X 			three->setText( i18n( "Group 3" ) );
//X 			four->setText( i18n( "Group 4" ) );
//X 			five->setText( i18n( "Group 5" ) );
//X 			six->setText( i18n( "Group 6" ) );
//X 			seven->setText( i18n( "Group 7" ) );
//X 			eight->setText( i18n( "Group 8" ) );
//X 			one->setPaletteBackgroundColor( color_1 );
//X 			two->setPaletteBackgroundColor( color_2 );
//X 			three->setPaletteBackgroundColor( color_3 );
//X 			four->setPaletteBackgroundColor( color_4 );
//X 			five->setPaletteBackgroundColor( color_5 );
//X 			six->setPaletteBackgroundColor( color_6 );
//X 			seven->setPaletteBackgroundColor( color_7 );
//X 			eight->setPaletteBackgroundColor( color_8 );
//X 			break;
//X 		case 2:
//X 			one->setPaletteBackgroundColor( color_s );
//X 			two->setPaletteBackgroundColor( color_p );
//X 			three->setPaletteBackgroundColor( color_d );
//X 			four->setPaletteBackgroundColor( color_f );
//X 			one->setText( i18n( "s-Block" ) );
//X 			two->setText( i18n( "p-Block" ) );
//X 			three->setText( i18n( "d-Block" ) );
//X 			four->setText( i18n( "f-Block" ) );
//X 			one->show();
//X 			two->show();
//X 			three->show();
//X 			four->show();
//X 			five->hide();
//X 			six->hide();
//X 			seven->hide();
//X 			eight->hide();
//X 			break;
//X 		case 3:
//X 			one->setPaletteBackgroundColor( color_ba );
//X 			two->setPaletteBackgroundColor( color_ac );
//X 			three->setPaletteBackgroundColor( color_neu );
//X 			four->setPaletteBackgroundColor( color_amp );
//X 			one->setText( i18n( "Basic-Block" ) );
//X 			two->setText( i18n( "Acidic" ) );
//X 			three->setText( i18n( "Neutral" ) );
//X 			four->setText( i18n( "Amphoteric" ) );
//X 			one->show();
//X 			two->show();
//X 			three->show();
//X 			four->show();
//X 			five->hide();
//X 			six->hide();
//X 			seven->hide();
//X 			eight->hide();
//X 			break;
//X 		case 4:
//X 			one->setPaletteBackgroundColor( c_liquid );
//X 			two->setPaletteBackgroundColor( c_solid );
//X 			three->setPaletteBackgroundColor( c_vapor );
//X 			four->setPaletteBackgroundColor( c_artificial );
//X 			five->setPaletteBackgroundColor( c_radioactive );
//X 			one->setText( i18n( "Liquid" ) );
//X 			two->setText( i18n( "Solid" ) );
//X 			three->setText( i18n( "Vaporous" ) );
//X 			four->setText( i18n( "Artificial" ) );
//X 			five->setText( i18n( "Radioactive" ) );
//X 			one->show();
//X 			two->show();
//X 			three->show();
//X 			four->show();
//X 			five->show();
//X 			six->hide();
//X 			seven->hide();
//X 			eight->hide();
//X 			break;
//X 	}
//X 
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
	
	//kdDebug() << mouse->x() << " " << mouse->y() << endl;
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

	bool notFound = true;

	int counter = 1;
	int numb = 1;
	while ( it != d->CoordinateList.end() && notFound == true )
	{//iterate through the list of coordinates and compare the x/y values.
	 //finally, if the 20'es iterator has the same cooridnates Element 20
	 //has been clicked.
	
		coordinate c = *it;
		if ( c.x == X )
		{
			if ( c.y == Y )
			{//coordinates match. Get the position of the it in the list.
				//kdDebug() << "Count: " << counter << "  ---  x: " << c.x << " y: " << c.y << endl;
				
				numb = counter;
				notFound = false;
			}
		}
		++it;
		++counter;
	}


	if ( numb > 112 | numb < 0 )
		return; //invalid value

	Element *e = new Element( numb );

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
