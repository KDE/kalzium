/***************************************************************************
 *   Copyright (C) 2003-2005 by Carsten Niehaus                            *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#define ELEMENTSIZE 45

#include "periodictableview.h"
#include "prefs.h"
#include "element.h"
#include "kalziumtip.h"
#include "kalziumutils.h"
#include "kalziumdataobject.h"

#include <klocale.h>
#include <kdebug.h>
#include <kpixmapeffect.h>
#include <kimageeffect.h>

#include <QEvent>
#include <QImage>
#include <QString>
#include <QToolTip>
#include <QWhatsThis>
#include <QLabel>
#include <QPixmap>
#include <QPoint>
#include <QCursor>
#include <QMouseEvent>
#include <QPainter>
#include <QColor>
#include <QRect>

PeriodicTableView::PeriodicTableView(QWidget *parent, const char *name)
  : QWidget(parent, name), m_kalziumTip(0), table(0), table2(0)
{
	d = KalziumDataObject::instance();


	// No selection
	unSelect();

#if 0
	connect( this,        SIGNAL( tableClicked( QPoint ) ), 
			 this,        SLOT( selectPoint( QPoint ) ) );
#endif
	connect( this,        SIGNAL( ToolTip( int ) ), 
	         this,        SLOT( slotToolTip( int ) ) );

	connect( &HoverTimer, SIGNAL( timeout() ), 
	         this,        SLOT( slotTransientLabel() ) );
	
	connect( &MouseoverTimer, SIGNAL( timeout() ), 
	         this,        SLOT( slotMouseover() ) );

	setMouseTracking( true );

	//JH: eliminates flicker on redraw
	setBackgroundMode( Qt::NoBackground );

	m_molcalcIsActive = false;
	m_showTooltip = true;
	m_showLegend = Prefs::showlegend();
	m_showLegendTooltip = Prefs::tooltip();
	m_timeline = false;
	m_showSOM = false;
	m_showGradient = false;
	m_tooltipsEnabled = Prefs::tooltip();

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

	m_kalziumTip = new KalziumTip( this );

	//JH: Start with a full draw
	doFullDraw = true;

	// according to carsten :)
	setMinimumSize(ELEMENTSIZE*18+1, ELEMENTSIZE*10+30);
}

void PeriodicTableView::reloadColours()
{
	if ( !m_colors.isEmpty() )	// if the color map is not empty,
		m_colors.clear();	// clear it, so there are no souble entries

	m_colors["color_s"] = Prefs::block_s();
	m_colors["color_p"] = Prefs::block_p();
	m_colors["color_d"] = Prefs::block_d();
	m_colors["color_f"] = Prefs::block_f();
	m_colors["color_1"] = Prefs::group_1();
	m_colors["color_2"] = Prefs::group_2();
	m_colors["color_3"] = Prefs::group_3();
	m_colors["color_4"] = Prefs::group_4();
	m_colors["color_5"] = Prefs::group_5();
	m_colors["color_6"] = Prefs::group_6();
	m_colors["color_7"] = Prefs::group_7();
	m_colors["color_8"] = Prefs::group_8();
	m_colors["color_ba"] = Prefs::beh_basic();
	m_colors["color_ac"] = Prefs::beh_acidic();
	m_colors["color_neu"] = Prefs::beh_neutral();
	m_colors["color_amp"] = Prefs::beh_amphoteric();
	m_colors["c_alkalie"] = Prefs::alkalie();
	m_colors["c_rare"] = Prefs::rare();
	m_colors["c_nonmetal"] = Prefs::nonmetal();
	m_colors["c_alkaline"] = Prefs::alkaline();
	m_colors["c_other_metal"] = Prefs::other_metal();
	m_colors["c_halogene"] = Prefs::halogene();
	m_colors["c_transition"] = Prefs::transition();
	m_colors["c_noble_gas"] = Prefs::noble_gas();
	m_colors["c_metalloid"] = Prefs::metalloid();
	m_colors["c_solid"]= Prefs::color_solid();
	m_colors["c_vapor"] = Prefs::color_vapor();
	m_colors["c_liquid"] = Prefs::color_liquid();
}

void PeriodicTableView::slotToolTip( int number )
{
//X 	if ( !m_showTooltip || !m_tooltipsEnabled ) 
//X 		return; //don't update if the table is locked
//X 
//X 	m_tooltipElementNumber = number;
//X 
//X 	QWidget *p = 0;
//X 	if ( dynamic_cast<QWidget*>( parent() ) )
//X 		p = static_cast<QWidget*>( parent() );
//X 
//X 	if( p )
//X 		m_kalziumTip->showTip( mapFromGlobal(QCursor::pos()), 
//X 				d->element(number), 
//X 				p->width(), 
//X 				p->height() );	
}

PeriodicTableView::~PeriodicTableView(){}

void PeriodicTableView::activateColorScheme( const int nr )
{
//X 	m_currentScheme = nr;
//X 	
//X 	EList::ConstIterator it = d->ElementList.begin();
//X 	const EList::ConstIterator itEnd = d->ElementList.end();
//X 
//X 	if ( m_currentScheme == PeriodicTableView::NOCOLOUR ) //normal view, no colors
//X 	{
//X 		const QColor color = Prefs::noscheme();
//X 		while ( it != itEnd )
//X 		{
//X 			( *it )->setElementColor( color );
//X 			++it;
//X 		}
//X 	}
//X 	else if ( m_currentScheme == PeriodicTableView::GROUPS ) //groups view
//X 	{
//X 
//X 		static QString group;
//X 
//X 		while ( it != itEnd )
//X 		{
//X 			group = ( *it )->group();
//X 			
//X 			if (group == QString("1")) {
//X 				( *it )->setElementColor( m_colors["color_1"] );
//X 			}
//X 			if (group == QString("2")){
//X 				( *it )->setElementColor( m_colors["color_2"] );
//X 			}
//X 			if (group == QString("3")){
//X 				( *it )->setElementColor( m_colors["color_3"] );
//X 			}
//X 			if (group == QString("4")){
//X 				( *it )->setElementColor( m_colors["color_4"] );
//X 			}
//X 			if (group == QString("5")){
//X 				( *it )->setElementColor( m_colors["color_5"] );
//X 			}
//X 			if (group == QString("6")){
//X 				( *it )->setElementColor( m_colors["color_6"] );
//X 			}
//X 			if (group == QString("7")){
//X 				( *it )->setElementColor( m_colors["color_7"] );
//X 			}
//X 			if (group == QString("8")){
//X 				( *it )->setElementColor( m_colors["color_8"] );
//X 			}
//X 
//X 			++it;
//X 		}
//X 	}
//X 	else if ( m_currentScheme == PeriodicTableView::BLOCK ) //block view
//X 	{
//X 		static QString block;
//X 		while ( it != itEnd )
//X 		{
//X 			block = (*it)->block();
//X 
//X 			if (block == QString("s")) {
//X 				(*it)->setElementColor( m_colors["color_s"] );
//X 			}
//X 			if (block == QString("p")) {
//X 				(*it)->setElementColor( m_colors["color_p"] );
//X 			}
//X 			if (block == QString("d")) {
//X 				(*it)->setElementColor( m_colors["color_d"] );
//X 			}
//X 			if (block == QString("f")) {
//X 				(*it)->setElementColor( m_colors["color_f"] );
//X 			}
//X 			++it;
//X 		}
//X 	}
//X 	else if ( m_currentScheme == PeriodicTableView::ACIDIC ) //acidic beh
//X 	{
//X 		static QString acidicbeh;
//X 		
//X 		while ( it != itEnd )
//X 		{
//X 			acidicbeh = ( *it )->acidicbeh();
//X 
//X 			if (acidicbeh == QString("0")) {
//X 				(*it)->setElementColor( m_colors["color_ac"] );
//X 			}
//X 			if (acidicbeh == QString("1")){
//X 				(*it)->setElementColor( m_colors["color_ba"] );
//X 			}
//X 			if (acidicbeh == QString("2")){
//X 				(*it)->setElementColor( m_colors["color_neu"] );
//X 			}
//X 			if (acidicbeh == QString("3")){
//X 				(*it)->setElementColor( m_colors["color_amp"] );
//X 			}
//X 			++it;
//X 		}
//X 	}
//X 	else if ( m_currentScheme ==  PeriodicTableView::FAMILY ) //familiy of the element
//X 	{
//X 		static QString family;
//X 
//X 		while ( it != itEnd )
//X 		{
//X 			family = ( *it )->family();
//X 
//X 			if ( family == "Noblegas" ){
//X 				(*it)->setElementColor( m_colors["c_noble_gas"] );
//X 			}
//X 			if ( family == "Non-Metal" ){
//X 				(*it)->setElementColor( m_colors["c_nonmetal"] );
//X 			}
//X 			if ( family == "Rare_Earth" ){
//X 				(*it)->setElementColor( m_colors["c_rare"] );
//X 			}
//X 			if ( family == "Alkaline_Earth" ){
//X 				(*it)->setElementColor( m_colors["c_alkaline"] );
//X 			}
//X 			if ( family == "Alkali_Earth" ){
//X 				(*it)->setElementColor( m_colors["c_alkalie"] );
//X 			}
//X 			if ( family == "Transition" ){
//X 				(*it)->setElementColor( m_colors["c_transition"] );
//X 			}
//X 			if ( family == "Other_Metal" ){
//X 				(*it)->setElementColor( m_colors["c_other_metal"] );
//X 			}
//X 			if ( family == "Metalloids" ){
//X 				(*it)->setElementColor( m_colors["c_metalloid"] );
//X 			}
//X 			if ( family == "Halogene" ){
//X 				(*it)->setElementColor( m_colors["c_halogene"] );
//X 			}
//X 			
//X 			++it;
//X 		}
//X 	}
//X 		
}

void PeriodicTableView::paintEvent( QPaintEvent * /*e*/ )
{
//X 	QPainter p;
//X 
//X 	//JH: I have split the drawing into two pixmaps: table and table2.
//X 	//table contains the "static" PeriodicTableView table, and does not change very often.
//X 	//table2 contains the tooltips and any other dynamic overlays.
//X 	//Usually, we can skip the code which renders the table, and just use the 
//X 	//image stored in table...when doFullDraw==false, the rendering code is skipped.
//X 	if ( doFullDraw )
//X 	{
//X 		//DEBUG
//X 		kdDebug() << "Drawing full table" << endl;
//X 
//X 		p.begin( table );
//X 		p.fillRect( 0, 0, width(), height(), paletteBackgroundColor() ); 
//X 
//X 		// Draw the numbers above the table.
//X 		drawNumeration( &p );
//X 
//X 		drawLegend( &p );
//X 
//X 		if ( m_timeline )
//X 		{ //use timeline
//X 			drawTimeLine(& p );
//X 			p.end();
//X 
//X 			*table2 = *table;
//X 			bitBlt( this, 0, 0, table2 );
//X 			return;
//X 		}
//X 		if ( som() )
//X 		{//use state of matter
//X 			drawSOMPeriodicTableView(& p );
//X 			p.end();
//X 
//X 			*table2 = *table;
//X 			bitBlt( this, 0, 0, table2 );
//X 			return;
//X 		}
//X 		if ( gradient() )
//X 		{//show a gradient
//X 			calculateGradient(& p );
//X 			p.end();
//X 
//X 			*table2 = *table;
//X 			bitBlt( this, 0, 0, table2 );
//X 		return;
//X 		}
//X 
//X 		drawPeriodicTableView( &p, m_currentScheme == CRYSTAL );
//X 
//X 		paintCurrentSelection();
//X 
//X 		p.end();
//X 
//X 		doFullDraw = false;
//X 	}
//X 
//X 	//JH: Ok, now table contains the static PeriodicTableView table, and we may need to draw
//X 	//a tooltip on it.  However, we don't want to ruin the stored table pixmap, 
//X 	//so let's copy it to table2 and add the tooltip there.
//X 	*table2 = *table;
//X 
//X 	//JH: Finally, bitBlt the table2 pixmap to the widget
//X 	bitBlt( this, 0, 0, table2 );
}

void PeriodicTableView::paintCurrentSelection()
{
//X 	if (m_currentPoint.x() == -1)
//X 		return;
//X 
//X 	int x = (m_currentPoint.x()-1)*ELEMENTSIZE;
//X 	int y = m_currentPoint.y()*ELEMENTSIZE;
//X 
//X 	// make a small gap (ELEMENTSIZE/3) over the rare earths
//X 	if (m_currentPoint.y() > 7) y += ELEMENTSIZE/3;
//X 
//X 	QPainter p;
//X 	p.begin(table);
//X 
//X 	QPen pen;
//X 	pen.setStyle( Qt::DotLine );
//X 	pen.setWidth( 4 );
//X 	pen.setColor( Qt::blue );
//X 	p.setPen( pen );
//X 
//X 	p.drawEllipse( x-10,y-10,ELEMENTSIZE+20,ELEMENTSIZE+20 );
//X 	pen.setWidth( 3 );
//X 	pen.setColor( Qt::red );
//X 	p.setPen( pen );
//X 	p.drawEllipse( x-5,y-5,ELEMENTSIZE+10,ELEMENTSIZE+10 );
//X 
//X 	p.end();
}

void PeriodicTableView::drawLegendToolTip( QPainter* p )
{
//X 	kdDebug() << "PeriodicTableView::drawLegendToolTip()" << endl;
//X 	if(!m_showLegendTooltip || !m_showLegend) return;
//X 
//X 	QString text;
//X 
//X 	switch ( m_currentScheme ) {
//X 		//No Legend drawn as only one colour is used
//X 		case PeriodicTableView::NOCOLOUR:
//X 			break;
//X 		case PeriodicTableView::BLOCK:
//X 			text = i18n( "The periodic table can be split up into four areas:\n the s-, p-, d- and f-Block. The name indicates which orbit\n is being filled last. For example, all elements in the s-block\n fill up the s-orbits." );
//X 			break;
//X 		case PeriodicTableView::GROUPS:
//X 			text = i18n( "The periodic table can be split up into groups:\n All elements in a group show similar behaviour");
//X 			break;
//X 		case PeriodicTableView::ACIDIC:
//X 			text = i18n( "The periodic table can be split up in groups of \nelements with different acidic behaviour.");
//X 			break;
//X 		case PeriodicTableView::FAMILY:
//X 			text = i18n( "The periodic table can be split up into several families.");
//X 			break;
//X 	}
//X 
//X 	const int x1 = mapFromGlobal( QCursor::pos() ).x();
//X 	const int y1 = mapFromGlobal( QCursor::pos() ).y();
//X 
//X 	static const int padding = 3;
//X 
//X 	QFont fB = KGlobalSettings::generalFont();
//X 	fB.setPointSize( fB.pointSize() + 4 );
//X 	p->setFont( fB );
//X 
//X 	QRect bRect( 0, 0, 1000, 1000 );
//X 	bRect = p->boundingRect( bRect, Qt::AlignLeft|Qt::AlignTop, text );
//X 	bRect.moveBy( x1, y1 );
//X 	QRect bRectExt = bRect;
//X 	bRect.moveBy( padding, padding );
//X 	bRectExt.setWidth( bRectExt.width() + 2 * padding );
//X 	bRectExt.setHeight( bRectExt.height() + 2 * padding );
//X 
//X 	bool vertflipped = false;
//X 	if ( bRectExt.bottom() > height() )
//X 	{
//X 		bRectExt.moveBy( 0, - bRectExt.height() );
//X 		bRect.moveBy( 0, - bRect.height() - 2 * padding );
//X 		vertflipped = true;
//X 	}
//X 	if ( bRectExt.right() > width() )
//X 	{
//X 		bRectExt.moveBy( - bRectExt.width(), 0 );
//X 		bRect.moveBy( - bRect.width() - 2 * padding, 0 );
//X 	}
//X 	else if ( !vertflipped )
//X 	{
//X 		bRectExt.moveBy( 15, 0 );
//X 		bRect.moveBy( 15, 0 );
//X 	}
//X 
//X 	p->setBrush(Qt::SolidPattern);
//X 	p->setBrush( QColor( 255, 255, 220 ) );
//X 	p->drawRect( bRectExt );
//X 
//X 	p->setBrush( Qt::black );
//X 	p->setBrush(Qt::NoBrush);
//X 
//X 	p->drawText( bRect, Qt::AlignLeft|Qt::AlignTop, text );
}

void PeriodicTableView::drawTimeLine( QPainter* p )
{
//X 	if ( !p ) return;
//X 	kdDebug() << "PeriodicTableView::drawTimeLine: " << m_date << endl;
//X 	
//X 	if ( gradient() )
//X 		activateColorScheme( PeriodicTableView::NOCOLOUR );
//X 	
//X 	EList::ConstIterator it = d->ElementList.begin();
//X 	const EList::ConstIterator itEnd = d->ElementList.end();
//X 
//X 	bool simple = Prefs::pselook();
//X 	
//X 	/**
//X 	 * this loop iterates through all elements. The Elements
//X 	 * draw themselfs, the PeriodicTableView only tells them to do so
//X 	 */
//X 	while ( it != itEnd )
//X 	{
//X 		if ( ( *it )->date() <= m_date )
//X 			( *it )->drawSelf( p, simple );
//X 		else
//X 			( *it )->drawGrayedOut( p );
//X 		++it;
//X 	}
}

void PeriodicTableView::drawLegend( QPainter* p )
{
//X 	if ( !p ) return;
//X 
//X 	if ( !m_showLegend ) return;
//X 
//X 	/*
//X 	 * The legend is drawn in the big gap of the table. The gap has
//X 	 * this size:
//X 	 * 
//X 	 * width:  ELEMENTSIZE * 10
//X 	 * height: ELEMENTSIZE * 3
//X 	 *
//X 	 * We need to spare a few pixels on every side so that the legend
//X 	 * does not collide with the elements
//X 	 */
//X 	
//X 	QFont legendFont = KGlobalSettings::generalFont();
//X 	legendFont.setPointSize( legendFont.pointSize() + 1 );
//X 	p->setFont( legendFont );
//X 
//X 	int  legendLeft   = ELEMENTSIZE * 5 / 2;
//X 	int  legendTop    = ELEMENTSIZE * 3 / 4;
//X 	int  legendWidth  = ELEMENTSIZE * 9;
//X 	int  legendHeight = ELEMENTSIZE * 3;
//X 
//X 	int  x1 = legendLeft + ELEMENTSIZE / 2;
//X 	int  x2 = legendLeft + ELEMENTSIZE * 5;
//X 
//X 	/*
//X 	 * one field is allowed to be half the gap minus 10 pixel
//X 	 */
//X 	int fieldsize = ELEMENTSIZE*5-10;
//X 
//X 	/*
//X 	 * one field is a maximum of half the size of an element
//X 	 */
//X 	int fieldheight = ELEMENTSIZE/2;  
//X 
//X 
//X 	const  int square_w = 18;
//X 	const  int square_h = 18;
//X 	const  int textOffset = square_w + 10;
//X 	
//X 	if ( !m_currentScheme == PeriodicTableView::NOCOLOUR )
//X 		p->fillRect(legendLeft, legendTop, legendWidth, legendHeight, 
//X 				QColor(200, 200, 200));
//X 
//X 	if ( som() )
//X 	{
//X 		p->fillRect(x1, fieldheight*2, square_w, square_h, m_colors["c_solid"] );
//X 		p->fillRect(x1, fieldheight*3, square_w, square_h, m_colors["c_liquid"] );
//X 		p->fillRect(x1, fieldheight*4, square_w, square_h, m_colors["c_vapor"] );
//X 
//X 		p->drawText(x1 + textOffset, fieldheight*2, fieldsize, fieldheight, Qt::AlignLeft, i18n("Solid") ); 
//X 		p->drawText(x1 + textOffset, fieldheight*3, fieldsize, fieldheight, Qt::AlignLeft, i18n("Liquid") ); 
//X 		p->drawText(x1 + textOffset, fieldheight*4, fieldsize, fieldheight, Qt::AlignLeft, i18n("Vaporous") ); 
//X 		return;
//X 	}
//X 	switch ( m_currentScheme ){
//X 		//No Legend to be drawn as only one colour is used
//X 		case PeriodicTableView::NOCOLOUR:
//X 			break;
//X 		case PeriodicTableView::GROUPS:
//X 			p->fillRect( x1, fieldheight*2, square_w, square_h, m_colors["color_1"]); 
//X 			p->fillRect( x1, fieldheight*3, square_w, square_h, m_colors["color_2"]); 
//X 			p->fillRect( x1, fieldheight*4, square_w, square_h, m_colors["color_3"]); 
//X 			p->fillRect( x1, fieldheight*5, square_w, square_h, m_colors["color_4"]); 
//X 			p->fillRect( x2, fieldheight*2, square_w, square_h, m_colors["color_5"]); 
//X 			p->fillRect( x2, fieldheight*3, square_w, square_h, m_colors["color_6"]); 
//X 			p->fillRect( x2, fieldheight*4, square_w, square_h, m_colors["color_7"]); 
//X 			p->fillRect( x2, fieldheight*5, square_w, square_h, m_colors["color_8"]); 
//X 			
//X 			p->drawText( x1 + textOffset , fieldheight*2, fieldsize, fieldheight, Qt::AlignLeft, i18n("Group 1") ); 
//X 			p->drawText( x1 + textOffset , fieldheight*3, fieldsize, fieldheight, Qt::AlignLeft, i18n("Group 2")); 
//X 			p->drawText( x1 + textOffset , fieldheight*4, fieldsize, fieldheight, Qt::AlignLeft, i18n("Group 3")); 
//X 			p->drawText( x1 + textOffset , fieldheight*5, fieldsize, fieldheight, Qt::AlignLeft, i18n("Group 4")); 
//X 			p->drawText( x2 + textOffset , fieldheight*2, fieldsize, fieldheight, Qt::AlignLeft, i18n("Group 5")); 
//X 			p->drawText( x2 + textOffset , fieldheight*3, fieldsize, fieldheight, Qt::AlignLeft, i18n("Group 6")); 
//X 			p->drawText( x2 + textOffset , fieldheight*4, fieldsize, fieldheight, Qt::AlignLeft, i18n("Group 7")); 
//X 			p->drawText( x2 + textOffset , fieldheight*5, fieldsize, fieldheight, Qt::AlignLeft, i18n("Group 8")); 
//X 			break;
//X 		case PeriodicTableView::BLOCK:
//X 			p->fillRect(x1, fieldheight*2, square_w, square_h, m_colors["color_s"] ); 
//X 			p->fillRect(x1, fieldheight*3, square_w, square_h, m_colors["color_p"] ); 
//X 			p->fillRect(x1, fieldheight*4, square_w, square_h, m_colors["color_d"] ); 
//X 			p->fillRect(x1, fieldheight*5, square_w, square_h, m_colors["color_f"] ); 
//X 			
//X 			p->drawText(x1 + textOffset, fieldheight*2, fieldsize, fieldheight, Qt::AlignLeft, i18n("s-Block") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*3, fieldsize, fieldheight, Qt::AlignLeft, i18n("p-Block") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*4, fieldsize, fieldheight, Qt::AlignLeft, i18n("d-Block") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*5, fieldsize, fieldheight, Qt::AlignLeft, i18n("f-Block") ); 
//X 			break;
//X 		case PeriodicTableView::ACIDIC:
//X 			p->fillRect(x1, fieldheight*2, square_w, square_h, m_colors["color_ba"] ); 
//X 			p->fillRect(x1, fieldheight*3, square_w, square_h, m_colors["color_neu"] );
//X 			p->fillRect(x1, fieldheight*4, square_w, square_h, m_colors["color_ac"] ); 
//X 			p->fillRect(x1, fieldheight*5, square_w, square_h, m_colors["color_amp"] );
//X 			
//X 			p->drawText(x1 + textOffset, fieldheight*2, fieldsize, fieldheight, Qt::AlignLeft, i18n("Basic") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*3, fieldsize, fieldheight, Qt::AlignLeft, i18n("Neutral") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*4, fieldsize, fieldheight, Qt::AlignLeft, i18n("Acidic") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*5, fieldsize, fieldheight, Qt::AlignLeft, i18n("both acidic and basic behaviour","Amphoteric") ); 
//X 			break;
//X 		case PeriodicTableView::FAMILY:
//X 			p->fillRect( x1, fieldheight*2, square_w, square_h, m_colors["c_alkaline"] ); 
//X 			p->fillRect( x2, fieldheight*2, square_w, square_h, m_colors["c_rare"] ); 
//X 			p->fillRect( x1, fieldheight*3, square_w, square_h, m_colors["c_nonmetal"] ); 
//X 			p->fillRect( x2, fieldheight*3, square_w, square_h, m_colors["c_alkalie"] ); 
//X 			p->fillRect( x1, fieldheight*4, square_w, square_h, m_colors["c_other_metal"] ); 
//X 			p->fillRect( x2, fieldheight*4, square_w, square_h, m_colors["c_halogene"] ); 
//X 			p->fillRect( x1, fieldheight*5, square_w, square_h, m_colors["c_transition"] );
//X 			p->fillRect( x2, fieldheight*5, square_w, square_h, m_colors["c_noble_gas"] ); 
//X 			p->fillRect( x1, fieldheight*6, square_w, square_h, m_colors["c_metalloid"] ); 
//X 			
//X 			p->drawText( x1 + textOffset , fieldheight*2, fieldsize, fieldheight, Qt::AlignLeft, i18n("Alkaline") ); 
//X 			p->drawText( x2 + textOffset , fieldheight*2, fieldsize, fieldheight, Qt::AlignLeft, i18n("Rare Earth")); 
//X 			p->drawText( x1 + textOffset , fieldheight*3, fieldsize, fieldheight, Qt::AlignLeft, i18n("Non-Metals")); 
//X 			p->drawText( x2 + textOffset , fieldheight*3, fieldsize, fieldheight, Qt::AlignLeft, i18n("Alkalie-Metals")); 
//X 			p->drawText( x1 + textOffset , fieldheight*4, fieldsize, fieldheight, Qt::AlignLeft, i18n("Other Metal")); 
//X 			p->drawText( x2 + textOffset , fieldheight*4, fieldsize, fieldheight, Qt::AlignLeft, i18n("Halogene")); 
//X 			p->drawText( x1 + textOffset , fieldheight*5, fieldsize, fieldheight, Qt::AlignLeft, i18n("Transition Metal")); 
//X 			p->drawText( x2 + textOffset , fieldheight*5, fieldsize, fieldheight, Qt::AlignLeft, i18n("Noble Gas")); 
//X 			p->drawText( x1 + textOffset , fieldheight*6, fieldsize, fieldheight, Qt::AlignLeft, i18n("Metalloid")); 
//X 			break;
//X 		case PeriodicTableView::CRYSTAL:
//X 			p->fillRect(x1, fieldheight*2, square_w, square_h, Qt::cyan ); 
//X 			p->fillRect(x1, fieldheight*3, square_w, square_h, Qt::red ); 
//X 			p->fillRect(x1, fieldheight*4, square_w, square_h, Qt::yellow ); 
//X 			p->fillRect(x1, fieldheight*5, square_w, square_h, Qt::green ); 
//X 			p->fillRect(x1, fieldheight*6, square_w, square_h, Qt::white ); 
//X 
//X 			p->drawText(x1 + textOffset, fieldheight*2, fieldsize, fieldheight, Qt::AlignLeft, i18n("Own") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*3, fieldsize, fieldheight, Qt::AlignLeft, i18n("bcc, body centered cubic") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*4, fieldsize, fieldheight, Qt::AlignLeft, i18n("hdp, hexagonal") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*5, fieldsize, fieldheight, Qt::AlignLeft, i18n("ccp, cubic close packed") ); 
//X 			p->drawText(x1 + textOffset, fieldheight*6, fieldsize, fieldheight, Qt::AlignLeft, i18n("Unknown") ); 
//X 			break;
//X 	}
}

void PeriodicTableView::drawNumeration( QPainter* p )
{
//X 	if ( !p ) return;
//X 
//X 	switch(m_num){
//X 		case PeriodicTableView::NO:
//X 			return;
//X 		case PeriodicTableView::CAS:
//X 			for(int i = 0; i < 18 ; ++i )
//X 			{
//X 				p->drawText( i*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, QString::number(i+1));
//X 			}
//X 			break;
//X 		case PeriodicTableView::IUPAC:
//X 			for(int i = 0; i < 18 ; ++i )
//X 			{
//X 				p->drawText( i*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, m_IUPAClist[i]);
//X 			}
//X 			break;
//X 		case PeriodicTableView::IUPACOLD:
//X 			for(int i = 0; i < 18 ; ++i )
//X 			{
//X 				p->drawText( i*ELEMENTSIZE,0 ,ELEMENTSIZE,ELEMENTSIZE, Qt::AlignCenter, m_IUPACOLDlist[i]);
//X 			}
//X 			break;
//X 	}
}

	
void PeriodicTableView::drawSOMPeriodicTableView( QPainter* p )
{
//X 	EList::ConstIterator it = d->ElementList.begin();
//X 	const EList::ConstIterator itEnd = d->ElementList.end();
//X 
//X 	while ( it != itEnd )
//X 	{
//X 		( *it )->drawStateOfMatter( p, m_temperature );
//X 		++it;
//X 	}
}

void PeriodicTableView::slotTransientLabel()
{
//X 	QPoint point = ElementUnderMouse();
//X 
//X 	int X = point.x();
//X 	int Y = point.y();
//X 
//X 	const int num = ElementNumber( X, Y );
//X 	if ( num )
//X 		emit ToolTip( num );
//X 	else if ( pointerOnLegend( X, Y ) ) //show the tooltip for the lengend
//X 	{
//X 		m_showLegendTooltip = true;
//X 		update();
//X 	}
//X 	else
//X 		m_showLegendTooltip = false;
}

void PeriodicTableView::mousePressEvent( QMouseEvent *)
{
//X 	if( m_kalziumTip->isVisible() )
//X 		m_kalziumTip->hide();
}

void PeriodicTableView::mouseMoveEvent( QMouseEvent * /*mouse*/ )
{
//X 	//JH: only update() if we were showing a tooltip
//X 	if ( m_tooltipElementNumber || m_showLegendTooltip )
//X 	{
//X 		//this invalidates the number. If the mouse
//X 		//is moved, the number is invalid. 
//X 		m_tooltipElementNumber = 0;
//X 		m_showLegendTooltip = false;
//X 		update();
//X 	}
//X 
//X 	if( m_kalziumTip->isVisible() )
//X 	{
//X //		kdDebug()<< "visible" << endl;
//X 		QPoint point = ElementUnderMouse();
//X 
//X 		int X = point.x();
//X 		int Y = point.y();
//X 
//X 		const int num = ElementNumber( X, Y );
//X 
//X 		if ( num != 0 )
//X 			emit ToolTip( num );
//X 		else
//X 			m_kalziumTip->hide();
//X 	}
//X 
//X 	HoverTimer.start(  500, true ); //JH: true = run timer once, not continuously
//X 	MouseoverTimer.start(  200, true ); //JH: true = run timer once, not continuously
}

bool PeriodicTableView::pointerOnLegend(int X, int Y)
{
//X 	if ( X > 2 && X < 13 )
//X 	{
//X 		if ( Y < 4 && Y > 0 )
//X 		{
//X 			return true;
//X 		}
//X 	}
//X 	
//X 	return false;
}

void PeriodicTableView::mouseReleaseEvent( QMouseEvent *mouse )
{
//X 	///first: find out the position
//X 	int X = mouse->x()/ELEMENTSIZE;
//X 	
//X 	//for the y-position I need to substract ELEMENTSIZE pixel because
//X 	//the whole table doesn't start at (0,0) but at (0,ELEMENTSIZE)
//X 	int Y = mouse->y()-ELEMENTSIZE;
//X 
//X 	// ignore clicks on the small gap over rare earth
//X 	if (Y >= (ELEMENTSIZE*7) && Y < (ELEMENTSIZE*7+ELEMENTSIZE/3+1)) return;
//X 
//X 	// mind the gap!
//X 	if (Y > (ELEMENTSIZE*7)) Y -= ELEMENTSIZE/3;
//X 	Y /= ELEMENTSIZE;
//X 		
//X 	X += 1;
//X 	Y += 1;
//X 
//X 	QPoint point( X,Y );
//X 	emit tableClicked( point );
//X 	
//X 	const int num = ElementNumber( X, Y );
//X 
//X 	//kdDebug() << "Element clicked: " << num << endl;
//X 
//X 	//If no element was clicked ElementNumber() will return 0
//X 	if ( num ) {
//X 		emit ElementClicked( num );
//X 		selectPoint( point );
//X 	}
}

int PeriodicTableView::ElementNumber( int X, int Y )
{
//X 	//from this on I can use X and Y. Both contain the position of an element in the
//X 	//complete PeriodicTableView. Eg, He is 1,18 and Na is 2,1
//X 	
//X 	CList::ConstIterator it = d->CoordinateList.begin();
//X 	const CList::ConstIterator itEnd = d->CoordinateList.end();
//X 
//X 	int counter = 1;
//X 	while ( it != itEnd )
//X 	{//iterate through the list of coordinates and compare the x/y values.
//X 	 //finally, if the 20'es iterator has the same coordinates Element 20
//X 	 //has been clicked.
//X 	
//X 		coordinate c = *it;
//X 		if ( c.x == X )
//X 		{
//X 			if ( c.y == Y )
//X 			{//coordinates match. Return the number of the element.
//X 				return counter;
//X 			}
//X 		}
//X 		++it;
//X 		++counter;
//X 	}
//X 
	return 0;
}


void PeriodicTableView::unSelect()
{
//X 	m_currentPoint = QPoint(-1, -1);
//X 
//X 	// Full draw needed to redraw the select mark.
//X 	setFullDraw();
//X 	update();
}

void PeriodicTableView::selectPoint( const QPoint& point )
{
//X 	kdDebug() << "PeriodicTableView::selectPoint " << point << endl;
//X 
//X 	m_currentPoint = point;
//X 
//X 	// Full draw needed to redraw the select mark.
//X 	setFullDraw();
//X 	update();
}

void PeriodicTableView::selectElement( int num )
{
//X 	kdDebug() << "PeriodicTableView::selectElement " << num << endl;
//X 
//X //	selectPoint( d->element( num )->coords() );
}

void PeriodicTableView::drawPeriodicTableView( QPainter* p, bool isCrystal )
{
//X 	EList::ConstIterator it = d->ElementList.begin();
//X 	const EList::ConstIterator itEnd = d->ElementList.end();
//X 
//X 	bool simple = Prefs::pselook();
//X 	/**
//X 	 * this loop iterates through all elements. The Elements
//X 	 * draw themselfs, the PeriodicTableView only tells them to do so
//X 	 */
//X 	while ( it != itEnd )
//X 	{
//X 		( *it )->drawSelf( p, simple, isCrystal );
//X 		++it;
//X 	}
}

//CN This is called for *every* drawing of the table. This means
//a lot overload... I would be better to chache the values in
//member variables an only check if they need an update. 
void PeriodicTableView::calculateGradient( QPainter *p )
{
//X 	EList::ConstIterator it = d->ElementList.begin();
//X 	const EList::ConstIterator itEnd = d->ElementList.end();
//X 
//X 	QList<double> tmpList;
//X 	switch ( m_gradientType )
//X 	{
//X 		case Element::ATOMICRADIUS:
//X 			for (; it != itEnd; ++it )
//X 			{
//X 				tmpList.append( ( *it )->radius(Element::ATOMIC) );
//X 			}
//X 			break;
//X 		case Element::COVALENTRADIUS:
//X 			for (; it != itEnd; ++it )
//X 			{
//X 				tmpList.append( ( *it )->radius(Element::COVALENT) );
//X 			}
//X 			break;
//X 		case Element::VDWRADIUS:
//X 			for (; it != itEnd; ++it )
//X 			{
//X 				tmpList.append( ( *it )->radius(Element::VDW) );
//X 			}
//X 			break;
//X 		case Element::MASS:
//X 			for (; it != itEnd; ++it )
//X 			{
//X 				tmpList.append( ( *it )->data( ChemicalDataObject::mass ).value().toDouble() );
//X 			}
//X 			break;
//X 		case Element::DENSITY:
//X 			for (; it != itEnd; ++it )
//X 			{
//X 				tmpList.append( ( *it )->density() );
//X 			}
//X 			break;
//X 		case Element::BOILINGPOINT:
//X 			for (; it != itEnd; ++it )
//X 			{
//X 				tmpList.append( ( *it )->boiling() );
//X 			}
//X 			break;
//X 		case Element::MELTINGPOINT:
//X 			for (; it != itEnd; ++it )
//X 			{
//X 				tmpList.append( ( *it )->melting() );
//X 			}
//X 			break;
//X 		case Element::EN:
//X 			for (; it != itEnd; ++it )
//X 			{
//X 				tmpList.append( ( *it )->electroneg() );
//X 			}
//X 			break;
//X 		case Element::EA:
//X 			for (; it != itEnd; ++it )
//X 			{
//X 				tmpList.append( ( *it )->electroaf() );
//X 			}
//X 			break;
//X 	}
//X 
//X 	QList<double>::iterator dit = tmpList.begin();
//X 	const QList<double>::iterator ditEnd = tmpList.end();
//X 
//X 	double tmpMin = *dit;
//X 	double tmpMax = *dit;
//X 	for ( ; dit != ditEnd; ++dit )
//X 	{
//X 		if ((  *dit ) == 0 ) continue;
//X 		if ( ( *dit ) < tmpMin )
//X 			tmpMin = *dit;
//X 		if ( ( *dit ) > tmpMax )
//X 			tmpMax = *dit;
//X 	}
//X 	
//X 	//now draw the gradient-table
//X 	drawGradientPeriodicTableView( p, tmpMin, tmpMax );
}



void PeriodicTableView::drawGradientPeriodicTableView( QPainter *p, const double min, const double max )
{
//X 	QString title = QString::null;
//X 	
//X 	const double var = max-min;
//X 	EList::ConstIterator it = d->ElementList.begin();
//X 	const EList::ConstIterator itEnd = d->ElementList.end();
//X 
//X 	/**
//X 	 * this loop iterates through all elements. The Elements
//X 	 * draw themselves, the PeriodicTableView only tells them to do so
//X 	 */
//X 	it = d->ElementList.begin();
//X 	switch ( m_gradientType )
//X 	{
//X 		case Element::ATOMICRADIUS:
//X 			title = i18n( "Gradient: Atomic Radius" );
//X 			while ( it != d->ElementList.end() )
//X 			{
//X 				double value = ( *it )->radius( Element::ATOMIC );
//X 				double coeff = ( value - min )/var;
//X 
//X 				drawGradientButton( p, *it, coeff, value, min );
//X 				
//X 				++it;
//X 			}
//X 			break;
//X 		case Element::VDWRADIUS:
//X 			title = i18n( "Gradient: van der Waals Radius" );
//X 			while ( it != d->ElementList.end() )
//X 			{
//X 				double value = ( *it )->radius( Element::VDW );
//X 				double coeff = ( value - min )/var;
//X 				
//X 				drawGradientButton( p, *it, coeff, value, min );
//X 
//X 				++it;
//X 			}
//X 			break;
//X 		case Element::COVALENTRADIUS:
//X 			title = i18n( "Gradient: Covalent Radius" );
//X 			while ( it != d->ElementList.end() )
//X 			{
//X 				double value = ( *it )->radius( Element::COVALENT );
//X 				double coeff = ( (*it)->radius(Element::COVALENT) - min )/var;
//X 				
//X 				drawGradientButton( p, *it, coeff, value, min );
//X 
//X 				++it;
//X 			}
//X 			break;
//X 		case Element::MASS:
//X 			title = i18n( "Gradient: Atomic Mass" );
//X 			while ( it != d->ElementList.end() )
//X 			{
//X 				double coeff = ( (*it)->data( ChemicalDataObject::mass ).value().toDouble() - min )/var;
//X 				drawGradientButton( p, *it, coeff, ( *it )->data( ChemicalDataObject::mass ).value().toDouble(), min );
//X 
//X 				++it;
//X 			}
//X 			break;
//X 		case Element::DENSITY:
//X 			title = i18n( "Gradient: Atomic Density" );
//X 			while ( it != d->ElementList.end() )
//X 			{
//X 				double coeff = ( (*it)->density() - min )/var;
//X 
//X 				drawGradientButton( p, *it, coeff, ( *it )->density(), min );
//X 				++it;
//X 			}
//X 			break;
//X 		case Element::BOILINGPOINT:
//X 			title = i18n( "Gradient: Boiling point" );
//X 			while ( it != d->ElementList.end() )
//X 			{
//X 				double coeff = ( (*it)->boiling() - min )/var;
//X 				drawGradientButton( p, *it, coeff, ( *it )->boiling(), min );
//X 
//X 				++it;
//X 			}
//X 			break;
//X 		case Element::MELTINGPOINT:
//X 			title = i18n( "Gradient: Melting point" );
//X 			while ( it != d->ElementList.end() )
//X 			{
//X 				double coeff = ( (*it)->melting() - min )/var;
//X 				drawGradientButton( p, *it, coeff, ( *it )->melting(), min );
//X 
//X 				++it;
//X 			}
//X 			break;
//X 		case Element::EN:
//X 			title = i18n( "Gradient: Electronegativity" );
//X 			while ( it != d->ElementList.end() )
//X 			{
//X 				double coeff = ( (*it)->electroneg() - min )/var;
//X 				drawGradientButton( p, *it, coeff, ( *it )->electroneg(), min );
//X 
//X 				++it;
//X 			}
//X 			break;
//X 		case Element::EA:
//X 			title = i18n( "Gradient: Electron affinity" );
//X 			double tmpVar = -1*(min - max);
//X 			while ( it != d->ElementList.end() )
//X 			{	
//X 				if( (*it)->electroaf() == 0.0)
//X 					drawGradientButton( p, *it,-1.0, (*it )->electroaf(), min );
//X 				else
//X 				{
//X 					double coeff = ( max - (*it)->electroaf() )/tmpVar;
//X 					drawGradientButton( p, *it, coeff, (*it )->electroaf(), min );
//X 				}
//X 	
//X 				++it;
//X 			}
//X 			break;
//X 	}
//X 
//X 	// Now draw the legend.
//X 	int  x = ELEMENTSIZE*2+5;
//X 	int  y = 5;
//X 	//int  w = ELEMENTSIZE*10-5;
//X 	//int  h = ELEMENTSIZE*4-5;
//X 
//X 	// Create the gradient image.
//X 	QSize s( ELEMENTSIZE*7+20, 20 );
//X 	QImage img = KImageEffect::gradient ( s, Qt::white, Qt::red, 
//X 										  KImageEffect::HorizontalGradient );
//X 	QPixmap pm( img );
//X 
//X 	p->drawText( x+5, y+50, ELEMENTSIZE*10,20, Qt::AlignCenter, title ); 
//X 	p->drawPixmap( x+50, y+80, pm );
//X 
//X 	if ( m_gradientType == Element::EA )
//X 	{
//X 		// FIXME: In the lines below, "30" is the max allowed text
//X 		//        height.  This should be calculated from the font 
//X 		//        metrics, not hard coded.
//X 		p->drawText( x+50,y+100,ELEMENTSIZE*7+20,30, Qt::AlignRight, QString::number( min ) ); 
//X 		p->drawText( x+50,y+100,ELEMENTSIZE*7+20,30, Qt::AlignLeft, QString::number( max ) );
//X 	}
//X 	else
//X 	{
//X 		p->drawText( x+50,y+100,ELEMENTSIZE*7+20,30, Qt::AlignRight, QString::number( max ) ); 
//X 		p->drawText( x+50,y+100,ELEMENTSIZE*7+20,30, Qt::AlignLeft, QString::number( min ) );
//X 
//X 	} 
}

QPoint PeriodicTableView::ElementUnderMouse()
{
//X 	int X = mapFromGlobal( QCursor::pos() ).x()/ELEMENTSIZE;
//X 	int Y = mapFromGlobal( QCursor::pos() ).y( )-ELEMENTSIZE;
//X 
//X 	// mind the gap over rare earth!
//X 	if (Y >= (ELEMENTSIZE*7) && Y < (ELEMENTSIZE*7+ELEMENTSIZE/3+1)) return QPoint( 0, 0 );
//X 
//X 	if (Y > (ELEMENTSIZE*7)) Y -= ELEMENTSIZE/3;
//X 
//X 	Y /= ELEMENTSIZE;
//X 	
//X 	X += 1;
//X 	Y += 1;
//X 	
//X 	return QPoint( X,Y );
}


void PeriodicTableView::slotMouseover()
{
//X 	QPoint point = ElementUnderMouse();
//X 
//X 	int num = ElementNumber( point.x(), point.y() );
//X 	if ( num )
//X 		emit MouseOver( num );
}

void PeriodicTableView::drawGradientButton( QPainter *p, Element* e, double coeff, double value, double minValue )
{
//X 	if ( value >= minValue && coeff != -1.0)
//X 	{
//X 		QColor c = calculateColor( coeff );
//X 		e->drawGradient( p, QString::number( KalziumUtils::strippedValue( value ) ), c );
//X 	}
//X 	else
//X 		e->drawGradient( p, i18n("It means: Not Available. Translators: keep it as short as you can!", "N/A"), Qt::lightGray );
}

QColor PeriodicTableView::calculateColor( const double coeff )
{
//X 	const QColor color2 = Qt::white;
//X 	const QColor color1 = Qt::red;
//X 
//X 	int red = (int)( (color1.red() - color2.red()) * coeff + color2.red() );
//X 	int green = (int)( (color1.green() - color2.green()) * coeff + color2.green() );
//X 	int blue = (int)( (color1.blue() - color2.blue()) * coeff + color2.blue() );
//X 
//X 	return QColor( red, green, blue );
}

void PeriodicTableView::setLook( PeriodicTableView::SCHEMETYPE type, int which )
{
//X 	m_currentScheme = type;
//X 	
//X 	EList::ConstIterator it = d->ElementList.begin();
//X 	const EList::ConstIterator itEnd = d->ElementList.end();
//X 
//X 	switch ( type )
//X 	{
//X 		case NOCOLOUR:
//X 		{
//X 			const QColor color = Prefs::noscheme();
//X 			while ( it != itEnd )
//X 			{
//X 				( *it )->setElementColor( color );
//X 				++it;
//X 			}
//X 			setGradient( false );
//X 			break;
//X 		}
//X 		case PeriodicTableView::GROUPS:  // group view
//X 		{
//X 			QString group;
//X 
//X 			while ( it != itEnd )
//X 			{
//X 				group = ( *it )->group();
//X 				
//X 				if (group == QString("1")) {
//X 					( *it )->setElementColor( m_colors["color_1"] );
//X 				}
//X 				if (group == QString("2")){
//X 					( *it )->setElementColor( m_colors["color_2"] );
//X 				}
//X 				if (group == QString("3")){
//X 					( *it )->setElementColor( m_colors["color_3"] );
//X 				}
//X 				if (group == QString("4")){
//X 					( *it )->setElementColor( m_colors["color_4"] );
//X 				}
//X 				if (group == QString("5")){
//X 					( *it )->setElementColor( m_colors["color_5"] );
//X 				}
//X 				if (group == QString("6")){
//X 					( *it )->setElementColor( m_colors["color_6"] );
//X 				}
//X 				if (group == QString("7")){
//X 					( *it )->setElementColor( m_colors["color_7"] );
//X 				}
//X 				if (group == QString("8")){
//X 					( *it )->setElementColor( m_colors["color_8"] );
//X 				}
//X 
//X 				++it;
//X 			}
//X 			setGradient( false );
//X 			break;
//X 		}
//X 		case PeriodicTableView::BLOCK: //block view
//X 		{
//X 			static QString block;
//X 			while ( it != itEnd )
//X 			{
//X 				block = (*it)->block();
//X 	
//X 				if (block == QString("s")) {
//X 					(*it)->setElementColor( m_colors["color_s"] );
//X 				}
//X 				if (block == QString("p")) {
//X 					(*it)->setElementColor( m_colors["color_p"] );
//X 				}
//X 				if (block == QString("d")) {
//X 					(*it)->setElementColor( m_colors["color_d"] );
//X 				}
//X 				if (block == QString("f")) {
//X 					(*it)->setElementColor( m_colors["color_f"] );
//X 				}
//X 				++it;
//X 			}
//X 			setGradient( false );
//X 			break;
//X 		}
//X 		case PeriodicTableView::ACIDIC: //acidic beh
//X 		{
//X 			static QString acidicbeh;
//X 			while ( it != itEnd )
//X 			{
//X 				acidicbeh = ( *it )->acidicbeh();
//X 
//X 				if (acidicbeh == QString("0")) {
//X 					(*it)->setElementColor( m_colors["color_ac"] );
//X 				}
//X 				if (acidicbeh == QString("1")){
//X 					(*it)->setElementColor( m_colors["color_ba"] );
//X 				}
//X 				if (acidicbeh == QString("2")){
//X 					(*it)->setElementColor( m_colors["color_neu"] );
//X 				}
//X 				if (acidicbeh == QString("3")){
//X 					(*it)->setElementColor( m_colors["color_amp"] );
//X 				}
//X 				++it;
//X 			}
//X 			setGradient( false );
//X 			break;
//X 		}
//X 		case PeriodicTableView::FAMILY: //familiy of the element
//X 		{
//X 			static QString family;
//X 
//X 			while ( it != itEnd )
//X 			{
//X 				family = ( *it )->family();
//X 
//X 				if ( family == "Noblegas" ){
//X 					(*it)->setElementColor( m_colors["c_noble_gas"] );
//X 				}
//X 				if ( family == "Non-Metal" ){
//X 					(*it)->setElementColor( m_colors["c_nonmetal"] );
//X 				}
//X 				if ( family == "Rare_Earth" ){
//X 					(*it)->setElementColor( m_colors["c_rare"] );
//X 				}
//X 				if ( family == "Alkaline_Earth" ){
//X 					(*it)->setElementColor( m_colors["c_alkaline"] );
//X 				}
//X 				if ( family == "Alkali_Earth" ){
//X 					(*it)->setElementColor( m_colors["c_alkalie"] );
//X 				}
//X 				if ( family == "Transition" ){
//X 					(*it)->setElementColor( m_colors["c_transition"] );
//X 				}
//X 				if ( family == "Other_Metal" ){
//X 					(*it)->setElementColor( m_colors["c_other_metal"] );
//X 				}
//X 				if ( family == "Metalloids" ){
//X 					(*it)->setElementColor( m_colors["c_metalloid"] );
//X 				}
//X 				if ( family == "Halogene" ){
//X 					(*it)->setElementColor( m_colors["c_halogene"] );
//X 				}
//X 			
//X 				++it;
//X 			}
//X 			setGradient( false );
//X 			break;
//X 		}
//X 		case CRYSTAL:
//X 		{
//X 			QString structure;
//X 			while ( it != itEnd )
//X 			{
//X 				structure = ( *it )->crystalstructure();
//X 				if ( structure == "own")
//X 					(*it)->setElementColor( Qt::cyan );
//X 				else if ( structure == "bcc" )
//X 					(*it)->setElementColor( Qt::red );
//X 				else if ( structure == "hdp" )
//X 					(*it)->setElementColor( Qt::yellow );
//X 				else if ( structure == "ccp" )
//X 					(*it)->setElementColor( Qt::green );
//X 				else
//X 					(*it)->setElementColor( Qt::white );
//X 				++it;
//X 			}
//X 			setGradient( false );
//X 			break;
//X 		}
//X 		case GRADIENT:
//X 		{
//X 			setGradient( true );
//X 			setGradientType( which );
//X 			break;
//X 		}
//X 		default:
//X 			;
//X 	}
//X 
//X 	Prefs::setColorschemebox( type );
//X 	Prefs::writeConfig();
//X 
//X 	//JH: redraw the full table next time
//X 	setFullDraw();
//X 	update();
}

#include "periodictableview.moc"
