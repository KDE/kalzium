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
#include <qpoint.h>
#include <qradiobutton.h>
#include <qbuttongroup.h>
#include <qcursor.h>
#include <qpainter.h>

PSE::PSE(KalziumDataObject *data, QWidget *parent, const char *name)
  : QWidget(parent, name), table(0)
{
	d = data;

	connect( this, SIGNAL( tableClicked( QPoint ) ), this, SLOT( slotUpdatePoint( QPoint ) ) );
	connect( this, SIGNAL( ToolTip( int ) ), this, SLOT( slotToolTip( int ) ) );
	connect(  &HoverTimer, SIGNAL(  timeout() ), this, SLOT(  slotTransientLabel() ) );

	setMouseTracking( true );

	m_molcalcIsActive = false;
	m_learningMode = false;
	m_showTooltip = true;
	m_timeline = false;
	m_showSOM = false;

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

void PSE::slotToolTip( int number )
{
	m_tooltipElementNumber = number;
	update();
}

PSE::~PSE(){}

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
	else if ( nr == 4) //familiy of the element
	{
		const QColor c_alkalie = Prefs::alkalie();
		const QColor c_rare = Prefs::rare();
		const QColor c_nonmetal = Prefs::nonmetal();
		const QColor c_alkaline = Prefs::alkaline();
		const QColor c_other_metal = Prefs::other_metal();
		const QColor c_halogene = Prefs::halogene();
		const QColor c_transition = Prefs::transition();
		const QColor c_noble_gas = Prefs::noble_gas();
		const QColor c_metalloid = Prefs::metalloid();

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
}

void PSE::paintEvent( QPaintEvent * /*e*/ )
{
	QPainter p;

	if ( doFullDraw ) {
		p.begin( table );
		p.fillRect( 0, 0, width(), height(), paletteBackgroundColor() ); //CN what is this line for?
		drawPSE( &p, m_isSimple );

		if ( m_showLegend )
			drawLegend( &p );

		drawNumeration( &p );
		
		if ( m_showTooltip )
		{
			if ( m_tooltipElementNumber < 112 && m_tooltipElementNumber > 0 )
			{
				Element *e = d->element( m_tooltipElementNumber );
				drawToolTip( &p, e );
			}
		}
		
		p.end();
		//JH: Uncomment when ready for this
		//    doFullDraw = false;
	}

	bitBlt( this, 0, 0, table );
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
	
	QFontMetrics fmB = QFontMetrics( fB );

	p->setFont( fB );
	p->drawText( x1, y1+15, i18n( "Name: %1").arg(e->elname().utf8() )); 
	p->drawText( x1, y1+35, i18n("Number: %1").arg( QString::number( e->number() )));
	p->drawText( x1, y1+55, i18n("Weight: %1 u").arg(QString::number( e->weight() ))); 
}


void PSE::drawLegend( QPainter* p )
{
	if ( !p ) return;

	p->drawText( 10, height()-80, i18n( "Name test123 ")); 
}

void PSE::drawNumeration( QPainter* p )
{
	if ( !p ) return;

	const int ELEMENTSIZE = 45;

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
	int X = mapFromGlobal( QCursor::pos() ).x()/45;
	int Y = mapFromGlobal( QCursor::pos() ).y()/45;
	if ( m_isSimple )
	{
		if ( mapFromGlobal( QCursor::pos() ).x() > ( 2*45 ) )
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
	HoverTimer.start(  2000, false );
}

void PSE::mouseReleaseEvent( QMouseEvent *mouse )
{
	///first: find out the position
	
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

	QPoint point( X,Y );
	emit tableClicked( point );
	
	const int num = ElementNumber( X, Y );
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
	 //finally, if the 20'es iterator has the same cooridnates Element 20
	 //has been clicked.
	
		coordinate c = *it;
		if ( c.x == X )
		{
			if ( c.y == Y )
			{//coordinates match. Get the position of the it in the list.
				return counter;
			}
		}
		++it;
		++counter;
	}

	return 0;
}

void PSE::slotLock(bool locked)
{
	if(locked){
		kdDebug() << "die bool ist true" << endl;
		setShowTooltip(false);
	}
	else{
		kdDebug() << "die bool ist false" << endl;
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


#include "pse.moc"
