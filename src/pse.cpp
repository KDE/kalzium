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
#include "element.h"
#include "infodialog_small_impl.h"
#include "detailinfodlg.h"

#include <klocale.h>
#include <kdebug.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qpoint.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qcursor.h>
#include <qpainter.h>
#include <qcolor.h>

PSE::PSE(KalziumDataObject *data, QWidget *parent, const char *name)
  : QWidget(parent, name), table(0), table2(0)
{
	d = data;

	connect( this, SIGNAL( tableClicked( QPoint ) ), this, SLOT( slotUpdatePoint( QPoint ) ) );
	connect( this, SIGNAL( ToolTip( int ) ), this, SLOT( slotToolTip( int ) ) );
	connect(  &HoverTimer, SIGNAL(  timeout() ), this, SLOT(  slotTransientLabel() ) );

	setMouseTracking( true );
  
  //JH: eliminates flicker on redraw
  setBackgroundMode( QWidget::NoBackground );
 
	m_molcalcIsActive = false;
	m_learningMode = false;
	m_showTooltip = true;
	m_timeline = false;
	m_showSOM = false;
	m_showGradient = false;
	
	showLegend( Prefs::showlegend() );

	reloadColours();

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
	table2 = new QPixmap();

	//JH: Start with a full draw
	doFullDraw = true;
}

void PSE::reloadColours()
{
	color_s = Prefs::block_s();
	color_p = Prefs::block_p();
	color_d = Prefs::block_d();
	color_f = Prefs::block_f();
	color_1 = Prefs::group_1();
	color_2 = Prefs::group_2();
	color_3 = Prefs::group_3();
	color_4 = Prefs::group_4();
	color_5 = Prefs::group_5();
	color_6 = Prefs::group_6();
	color_7 = Prefs::group_7();
	color_8 = Prefs::group_8();
	color_ba = Prefs::beh_basic();
	color_ac = Prefs::beh_acidic();
	color_neu = Prefs::beh_neutral();
	color_amp = Prefs::beh_amphoteric();
	c_alkalie = Prefs::alkalie();
	c_rare = Prefs::rare();
	c_nonmetal = Prefs::nonmetal();
	c_alkaline = Prefs::alkaline();
	c_other_metal = Prefs::other_metal();
	c_halogene = Prefs::halogene();
	c_transition = Prefs::transition();
	c_noble_gas = Prefs::noble_gas();
	c_metalloid = Prefs::metalloid();
}

void PSE::slotToolTip( int number )
{
	if ( !m_showTooltip ) return; //don't update if the table is locked
	m_tooltipElementNumber = number;
	update();
}

PSE::~PSE(){}

void PSE::activateColorScheme( const int nr )
{
	m_currentScheme = nr;
	
	EList::Iterator it = d->ElementList.begin();
	const EList::Iterator itEnd = d->ElementList.end();

	if ( m_currentScheme == PSE::NOCOLOUR ) //normal view, no colors
	{
		const QColor color = Prefs::noscheme();
		while ( it != itEnd )
		{
			( *it )->setElementColor( color );
			++it;
		}
	}
	else if ( m_currentScheme == PSE::GROUPS ) //groups view
	{

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
	else if ( m_currentScheme == PSE::BLOCK ) //block view
	{
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
	else if ( m_currentScheme == PSE::ACIDIC ) //acidic beh
	{
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
	else if ( m_currentScheme ==  PSE::FAMILY ) //familiy of the element
	{
		static QString family;

		while ( it != itEnd )
		{
			family = ( *it )->family();

			if ( family == "Noblegas" ){
				(*it)->setElementColor( c_noble_gas );
			}
			if ( family == "Non-Metal" ){
				(*it)->setElementColor( c_nonmetal );
			}
			if ( family == "Rare_Earth" ){
				(*it)->setElementColor( c_rare );
			}
			if ( family == "Alkaline_Earth" ){
				(*it)->setElementColor( c_alkaline );
			}
			if ( family == "Alkali_Earth" ){
				(*it)->setElementColor( c_alkalie );
			}
			if ( family == "Transition" ){
				(*it)->setElementColor( c_transition );
			}
			if ( family == "Other_Metal" ){
				(*it)->setElementColor( c_other_metal );
			}
			if ( family == "Metalloids" ){
				(*it)->setElementColor( c_metalloid );
			}
			if ( family == "Halogene" ){
				(*it)->setElementColor( c_halogene );
			}
			
			++it;
		}
	}
		
}

void PSE::resizeEvent( QResizeEvent * /*e*/ ) 
{
  table->resize( width(), height() );
  table2->resize( width(), height() );
}

void PSE::paintEvent( QPaintEvent * /*e*/ )
{
	QPainter p;


  //JH: I have split the drawing into two pixmaps: table and table2.
  //table contains the "static" PSE table, and does not change very often.
  //table2 contains the tooltips and any other dynamic overlays.
  //Usually, we can skip the code which renders the table, and just use the 
  //image stored in table...when doFullDraw==false, the rendering code is skipped.
  if ( doFullDraw ) 
  {
	  //DEBUG
	  kdDebug() << "Drawing full table" << endl;

	  p.begin( table );
	  p.fillRect( 0, 0, width(), height(), paletteBackgroundColor() ); 
	  if ( m_timeline ){ //use timeline
		  drawTimeLine(& p );
		  p.end();

		  *table2 = *table;
		  bitBlt( this, 0, 0, table2 );
		  return;
	  }
	  if ( som() ){//use state of matter
		  drawSOMPSE(& p );
		  p.end();

		  *table2 = *table;
		  bitBlt( this, 0, 0, table2 );
		  return;
	  }
	  if ( gradient() ){//show a gradient
		  kdDebug() << "Show Gradient" << endl;
		  calculateGradient(& p );
		  p.end();

		  *table2 = *table;
		  bitBlt( this, 0, 0, table2 );
		  return;
	  }
	  drawPSE( &p, m_isSimple );

	  drawNumeration( &p );

	  if ( m_showLegend )
		  drawLegend( &p );

	  p.end();

	  doFullDraw = false;
  }

  //JH: Ok, now table contains the static PSE table, and we may need to draw
  //a tooltip on it.  However, we don't want to ruin the stored table pixmap, 
  //so let's copy it to table2 and add the tooltip there.
  *table2 = *table;
 
  if ( m_showTooltip )
  {
    if ( m_tooltipElementNumber < 112 && m_tooltipElementNumber > 0 )
    {
      QPainter p2;
      p2.begin( table2 );
      Element *e = d->element( m_tooltipElementNumber );
      drawToolTip( &p2, e );
      p2.end();
    }
  }
		
  //JH: Finally, bitBlt the table2 pixmap to the widget
  bitBlt( this, 0, 0, table2 );
}

void PSE::drawToolTip( QPainter* p, Element *e )
{
	if ( !e ) return;

	if(!m_showTooltip) return;
	
	const int x1 = mapFromGlobal( QCursor::pos() ).x();
	const int y1 = mapFromGlobal( QCursor::pos() ).y();
	const int w = 200;
	const int h = 75;

	p->setBrush(Qt::SolidPattern);
	p->setBrush( Qt::yellow );
	p->drawRect( x1 , y1 , w, h );

	p->setBrush( Qt::black );
	p->setBrush(Qt::NoBrush);

	QFont fB = KGlobalSettings::generalFont();
	fB.setPointSize( fB.pointSize() + 4 );
	p->setFont( fB );
	
	p->drawText( x1, y1+15, i18n( "Name: %1").arg(e->elname().utf8() )); 
	p->drawText( x1, y1+35, i18n("Number: %1").arg( QString::number( e->number() )));
	p->drawText( x1, y1+55, i18n("Weight: %1 u").arg(QString::number( e->weight() ))); 
}

void PSE::drawTimeLine( QPainter* p )
{
	if ( !p ) return;
	kdDebug() << "PSE::drawTimeLine: " << m_date << endl;
	
	EList::Iterator it = d->ElementList.begin();

	/**
	 * this loop iterates through all elements. The Elements
	 * draw themselfs, the PSE only tells them to do so
	 */
	while ( it != d->ElementList.end() )
	{
		if ( ( *it )->date() <= m_date )
			( *it )->drawSelf( p, false );
		++it;
	}
}

void PSE::drawLegend( QPainter* p )
{
	if ( !p ) return;
	
	QFont legendFont = KGlobalSettings::generalFont();
	legendFont.setPointSize( legendFont.pointSize() + 1 );
	p->setFont( legendFont );

	int Y = ELEMENTSIZE;
	int tableW = ELEMENTSIZE;

	//If the table is the simple type the width has to be smaller.
	//For the full PSE the width will be bigger.
	//The second line calculates the position of the legend
	m_isSimple ? tableW *= 8 : tableW *= 18;
	m_isSimple ? Y *= 9 : Y *= 11;

	int fieldsize = tableW/8;    //the width of a legend-field
	int fieldheight = 20;        //the height of a legend field

	switch ( m_currentScheme ) {
		//No Legend to be drawn as only one colour is used
		case PSE::NOCOLOUR:
			break;
		case PSE::BLOCK:
			{
				p->fillRect(fieldsize*2, Y, fieldsize, fieldheight, color_s ); 
				p->fillRect(fieldsize*3, Y, fieldsize, fieldheight, color_p ); 
				p->fillRect(fieldsize*4, Y, fieldsize, fieldheight, color_d ); 
				p->fillRect(fieldsize*5, Y, fieldsize, fieldheight, color_f ); 
				p->drawText(fieldsize*2, Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("s-Block") ); 
				p->drawText(fieldsize*3, Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("p-Block") ); 
				p->drawText(fieldsize*4, Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("d-Block") ); 
				p->drawText(fieldsize*5, Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("f-Block") ); 
				break;
			}
		case PSE::GROUPS:
			{
				p->fillRect( fieldsize , Y, fieldsize, fieldheight, color_1); 
				p->fillRect( fieldsize*2 , Y, fieldsize, fieldheight, color_2); 
				p->fillRect( fieldsize*3 , Y, fieldsize, fieldheight, color_3); 
				p->fillRect( fieldsize*4 , Y, fieldsize, fieldheight, color_4); 
				p->fillRect( fieldsize , Y+fieldheight+1, fieldsize, fieldheight, color_5); 
				p->fillRect( fieldsize*2 , Y+fieldheight+1, fieldsize, fieldheight, color_6); 
				p->fillRect( fieldsize*3 , Y+fieldheight+1, fieldsize, fieldheight, color_7); 
				p->fillRect( fieldsize*4 , Y+fieldheight+1, fieldsize, fieldheight, color_8 ); 
				p->drawText( fieldsize , Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Group 1") ); 
				p->drawText( fieldsize*2 , Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Group 2")); 
				p->drawText( fieldsize*3 , Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Group 3")); 
				p->drawText( fieldsize*4 , Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Group 4")); 
				p->drawText( fieldsize , Y+fieldheight+1, fieldsize, fieldheight, Qt::AlignCenter, i18n("Group 5")); 
				p->drawText( fieldsize*2 , Y+fieldheight+1, fieldsize, fieldheight, Qt::AlignCenter, i18n("Group 6")); 
				p->drawText( fieldsize*3 , Y+fieldheight+1, fieldsize, fieldheight, Qt::AlignCenter, i18n("Group 7")); 
				p->drawText( fieldsize*4 , Y+fieldheight+1, fieldsize, fieldheight, Qt::AlignCenter, i18n("Group 8")); 
				break;
			}
		case PSE::ACIDIC:
			{
				p->fillRect(fieldsize*2, Y, fieldsize, fieldheight, color_ba ); 
				p->fillRect(fieldsize*3, Y, fieldsize, fieldheight, color_ac ); 
				p->fillRect(fieldsize*4, Y, fieldsize, fieldheight, color_neu ); 
				p->fillRect(fieldsize*5, Y, fieldsize, fieldheight, color_amp ); 
				p->drawText(fieldsize*2, Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Basic") ); 
				p->drawText(fieldsize*3, Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Acidic") ); 
				p->drawText(fieldsize*4, Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Neutral") ); 
				p->drawText(fieldsize*5, Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("both acidic and basic behaviour","Amphoteric") ); 
				break;
			}
		case PSE::FAMILY:
			{
				//increase the width a little as the names are rather long
				fieldsize += 20;
				
				p->fillRect( fieldsize , Y, fieldsize, fieldheight, c_alkaline ); 
				p->fillRect( fieldsize*2 , Y, fieldsize, fieldheight, c_rare ); 
				p->fillRect( fieldsize*3 , Y, fieldsize, fieldheight, c_nonmetal ); 
				p->fillRect( fieldsize*4 , Y, fieldsize, fieldheight, c_alkalie ); 
				p->fillRect( fieldsize*5 , Y, fieldsize, fieldheight, c_other_metal ); 
				p->fillRect( fieldsize*2 , Y+fieldheight+1, fieldsize, fieldheight, c_halogene ); 
				p->fillRect( fieldsize*3 , Y+fieldheight+1, fieldsize, fieldheight, c_transition ); 
				p->fillRect( fieldsize*4 , Y+fieldheight+1, fieldsize, fieldheight, c_noble_gas ); 
				p->fillRect( fieldsize*5 , Y+fieldheight+1, fieldsize, fieldheight, c_metalloid ); 

				p->drawText( fieldsize , Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Alkaline") ); 
				p->drawText( fieldsize*2 , Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Rare Earth")); 
				p->drawText( fieldsize*3 , Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Non-Metals")); 
				p->drawText( fieldsize*4 , Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Alkalie-Metals")); 
				p->drawText( fieldsize*5 , Y, fieldsize, fieldheight, Qt::AlignCenter, i18n("Other Metal")); 
				p->drawText( fieldsize*2 , Y+fieldheight+1, fieldsize, fieldheight, Qt::AlignCenter, i18n("Halogene")); 
				p->drawText( fieldsize*3 , Y+fieldheight+1, fieldsize, fieldheight, Qt::AlignCenter, i18n("Transition-Metal")); 
				p->drawText( fieldsize*4 , Y+fieldheight+1, fieldsize, fieldheight, Qt::AlignCenter, i18n("Noble Gas")); 
				p->drawText( fieldsize*5 , Y+fieldheight+1, fieldsize, fieldheight, Qt::AlignCenter, i18n("Metalloid")); 
				break;
			}
	}
}

void PSE::drawNumeration( QPainter* p )
{
	if ( !p ) return;

	//I always need to check if the user is locking at the simple or the 
	//regular PSE. Depending on that I need diffrent positions
	switch(m_num){
		case PSE::NO:
			return;
		case PSE::CAS:
			if (m_isSimple)
			{
				for(int i = 0; i < 18 ; ++i )
				{
					if(i > 2 && i < 12 ) continue;
					if(i < 2 ) 
						p->drawText( i*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, QString::number(i+1));
					if(i > 11 )
						p->drawText( (i-10)*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, QString::number(i+1));
				}
			}else
				for(int i = 0; i < 18 ; ++i )
				{
					p->drawText( i*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, QString::number(i+1));
				}
			break;
		case PSE::IUPAC:
			if (m_isSimple)
			{
				for(int i = 0; i < 18 ; ++i )
				{
					if(i > 2 && i < 12 ) continue;
					if(i < 2 ) 
						p->drawText( i*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, m_IUPAClist[i]);
					if(i > 11 )
						p->drawText( (i-10)*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, m_IUPAClist[i]);
				}
			}else
				for(int i = 0; i < 18 ; ++i )
				{
					p->drawText( i*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, m_IUPAClist[i]);
				}
			break;
		case PSE::IUPACOLD:
			if (m_isSimple)
			{
				for(int i = 0; i < 18 ; ++i )
				{
					if(i > 2 && i < 12 ) continue;
					if(i < 2 ) 
						p->drawText( i*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, m_IUPACOLDlist[i]);
					if(i > 11 )
						p->drawText( (i-10)*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, m_IUPACOLDlist[i]);
				}
			}else
			for(int i = 0; i < 18 ; ++i )
			{
				p->drawText( i*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, m_IUPACOLDlist[i]);
			}
			break;
	}
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

void PSE::slotTransientLabel( void )
{
	int X = mapFromGlobal( QCursor::pos() ).x()/ELEMENTSIZE;
	int Y = ( mapFromGlobal( QCursor::pos() ).y( )-ELEMENTSIZE)/ELEMENTSIZE;
	if ( m_isSimple )
	{
		if ( mapFromGlobal( QCursor::pos() ).x() > ( 2*ELEMENTSIZE ) )
		{
			X += 10;
		}
	}
		
	X += 1;
	Y += 1;

	QPoint point( X,Y );

	const int num = ElementNumber( X, Y );
	if ( num )
		emit ToolTip( num );
}

void PSE::mouseMoveEvent( QMouseEvent * /*mouse*/ )
{
  //JH: only update() if we were showing a tooltip
  if ( m_tooltipElementNumber ) {
  	m_tooltipElementNumber = 0; //this invalidates the number. If the mouse
	                            //is moved, the number is invalid. 
  	update();       
  }
  
  HoverTimer.start(  2000, true ); //JH: true = run timer once, not continuously
}

void PSE::mouseReleaseEvent( QMouseEvent *mouse )
{
	///first: find out the position
	int X = mouse->x()/ELEMENTSIZE;
	
	//for the y-position I need to substract ELEMENTSIZE pixel because
	//the whole table doesn't start at (0,0) but at (0,ELEMENTSIZE)
	int Y = ( mouse->y()-ELEMENTSIZE)/ELEMENTSIZE;
	if ( m_isSimple )
	{
		if ( mouse->x() > ( 2*ELEMENTSIZE ) )
		{
			X += 10;
		}
	}
		
	X += 1;
	Y += 1;

	QPoint point( X,Y );
	emit tableClicked( point );
	
	const int num = ElementNumber( X, Y );

	//If no element was clicked ElementNumber() will return 0
	if ( num )
		emit ElementClicked( num );
}

int PSE::ElementNumber( int X, int Y )
{
	//from this on I can use X and Y. Both contain the position of an element in the
	//complete PSE. Eg, He is 1,18 and Na is 2,1
	
	CList::ConstIterator it = d->CoordinateList.begin();

	int counter = 1;
	while ( it != d->CoordinateList.end() )
	{//iterate through the list of coordinates and compare the x/y values.
	 //finally, if the 20'es iterator has the same coordinates Element 20
	 //has been clicked.
	
		coordinate c = *it;
		if ( c.x == X )
		{
			if ( c.y == Y )
			{//coordinates match. Return the number of the element.
				return counter;
			}
		}
		++it;
		++counter;
	}

	return 0;
}

void PSE::slotLock()
{
	setShowTooltip(true);
}

void PSE::slotLock(bool locked)
{
	if(locked){
		setShowTooltip(false);
	}
	else{
		setShowTooltip(true);
	}
}

void PSE::slotUpdatePoint( QPoint point )
{
	m_currentPoint = point;

	update();
}

void PSE::drawPSE( QPainter* p, bool useSimpleView )
{
	EList::Iterator it = d->ElementList.begin();

	int coordinate = 0;
	m_Vertikal ? coordinate = m_currentPoint.x() : coordinate = m_currentPoint.y();
	
	/**
	 * this loop iterates through all elements. The Elements
	 * draw themselfs, the PSE only tells them to do so
	 */
	while ( it != d->ElementList.end() )
	{
		( *it )->drawSelf( p, useSimpleView );
		if ( m_learningMode )
			( *it )->drawHighlight( p, coordinate, m_Vertikal );
		++it;
	}
}

//CN This is called for *every* drawing of the table. This means
//a lot overload... I would be better to chache the values in
//member variables an only check if they need an update. 
void PSE::calculateGradient( QPainter *p )
{
	EList::Iterator it = d->ElementList.begin();
	const EList::Iterator itEnd = d->ElementList.end();

	QValueList<double> tmpList;
	switch ( m_gradientType )
	{
		case Element::RADIUS:
			for (; it != itEnd; ++it )
			{
				tmpList.append( ( *it )->radius() );
			}
			break;
		case Element::WEIGHT:
			for (; it != itEnd; ++it )
			{
				tmpList.append( ( *it )->weight() );
			}
			break;
		case Element::DENSITY:
			for (; it != itEnd; ++it )
			{
				tmpList.append( ( *it )->density() );
			}
			break;
		case Element::BOILINGPOINT:
			for (; it != itEnd; ++it )
			{
				tmpList.append( ( *it )->boiling() );
			}
			break;
		case Element::MELTINGPOINT:
			for (; it != itEnd; ++it )
			{
				tmpList.append( ( *it )->melting() );
			}
			break;
		case Element::EN:
			for (; it != itEnd; ++it )
			{
				tmpList.append( ( *it )->electroneg() );
			}
			break;
	}

	QValueList<double>::iterator dit = tmpList.begin();
	const QValueList<double>::iterator ditEnd = tmpList.end();

	double tmpMin = 0.0;
	double tmpMax = 0.0;
	for ( ; dit != ditEnd; ++dit )
	{
		if ( ( *dit ) < tmpMin )
			tmpMin = *dit;
		if ( ( *dit ) > tmpMax )
			tmpMax = *dit;
	}
	kdDebug() << tmpMin << " :: " << tmpMax << endl;
	drawGradientPSE( p, Element::EN, tmpMin, tmpMax );
}



void PSE::drawGradientPSE( QPainter *p, Element::TYPE type, const double min, const double max )
{
	const double var = max-min;
	EList::Iterator it = d->ElementList.begin();
	const EList::Iterator itEnd = d->ElementList.end();

	/**
	 * this loop iterates through all elements. The Elements
	 * draw themselfs, the PSE only tells them to do so
	 */
	it = d->ElementList.begin();
	switch ( m_gradientType )
	{
		case Element::RADIUS:
			while ( it != d->ElementList.end() )
			{
				double coeff = ( (*it)->radius() - min )/var;

				QColor c = calculateColor( coeff );

				( *it )->drawGradient( p, QString::number( ( *it )->radius() ), c );
				++it;
			}
			break;
		case Element::WEIGHT:
			while ( it != d->ElementList.end() )
			{
				double coeff = ( (*it)->weight() - min )/var;

				QColor c = calculateColor( coeff );

				( *it )->drawGradient( p, QString::number( ( *it )->weight() ), c );
				++it;
			}
			break;
		case Element::DENSITY:
			while ( it != d->ElementList.end() )
			{
				double coeff = ( (*it)->density() - min )/var;

				QColor c = calculateColor( coeff );

				( *it )->drawGradient( p, QString::number( ( *it )->density() ), c );
				++it;
			}
			break;
		case Element::BOILINGPOINT:
			while ( it != d->ElementList.end() )
			{
				double coeff = ( (*it)->boiling() - min )/var;

				QColor c = calculateColor( coeff );

				( *it )->drawGradient( p, QString::number( ( *it )->boiling() ), c );
				++it;
			}
			break;
		case Element::MELTINGPOINT:
			while ( it != d->ElementList.end() )
			{
				double coeff = ( (*it)->melting() - min )/var;

				QColor c = calculateColor( coeff );

				( *it )->drawGradient( p, QString::number( ( *it )->melting() ), c );
				++it;
			}
			break;
		case Element::EN:
			while ( it != d->ElementList.end() )
			{
				double coeff = ( (*it)->electroneg() - min )/var;

				QColor c = calculateColor( coeff );

				( *it )->drawGradient( p, QString::number( ( *it )->electroneg() ), c );
				++it;
			}
			break;
	}
}

QColor PSE::calculateColor( const double coeff )
{
	const QColor color2 = Qt::white;
	const QColor color1 = Qt::red;

	int red = (int)( (color1.red() - color2.red()) * coeff + color2.red() );
	int green = (int)( (color1.green() - color2.green()) * coeff + color2.green() );
	int blue = (int)( (color1.blue() - color2.blue()) * coeff + color2.blue() );

	return QColor( red, green, blue );
}


#include "pse.moc"
