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

#include "periodictableview.h"
#include "prefs.h"
#include "element.h"
#include "kalziumtip.h"
#include "kalziumutils.h"
#include "kalziumdataobject.h"
#include "kalziumpainter.h"
#include "kalziumtabletype.h"

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
	// No selection
	m_currentElement = 0;

	connect( this,        SIGNAL( ToolTip( int ) ), 
	         this,        SLOT( slotToolTip( int ) ) );

	connect( &HoverTimer, SIGNAL( timeout() ), 
	         this,        SLOT( slotTransientLabel() ) );
	
	connect( &MouseoverTimer, SIGNAL( timeout() ), 
	         this,        SLOT( slotMouseover() ) );

	setMouseTracking( true );

	//JH: eliminates flicker on redraw
	setBackgroundMode( Qt::NoBackground );

	m_showTooltip = true;
	m_showLegendTooltip = Prefs::tooltip();
	m_tooltipsEnabled = Prefs::tooltip();

	table = new QPixmap();
	table2 = new QPixmap();

	m_kalziumTip = new KalziumTip( this );

	//JH: Start with a full draw
	doFullDraw = true;

	KalziumTableType *classic = KalziumTableTypeFactory::instance()->build( "Classic" );
	m_painter = new KalziumPainter( classic );
	m_painter->toggleLegend( Prefs::showlegend() );
	
	// according to carsten :)
	setMinimumSize( classic->size() );

	update();
}

void PeriodicTableView::slotToolTip( int number )
{
	if ( !m_showTooltip || !m_tooltipsEnabled )
		return; //don't update if the table is locked

	QWidget *p = parentWidget();
	if ( p )
	{
		m_kalziumTip->showTip( mapFromGlobal( QCursor::pos() ),
				KalziumDataObject::instance()->element( number ),
				p->width(), p->height() );
	}
}

PeriodicTableView::~PeriodicTableView()
{
}

void PeriodicTableView::activateColorScheme( const int nr )
{
	m_painter->setScheme( nr );
	setFullDraw();
	update();
}

void PeriodicTableView::setMode( KalziumPainter::MODE m )
{
	m_painter->setMode( m );
	setFullDraw();
	update();
}

KalziumPainter::MODE PeriodicTableView::mode() const
{
	return m_painter->mode();
}

void PeriodicTableView::paintEvent( QPaintEvent * /*e*/ )
{
	//JH: I have split the drawing into two pixmaps: table and table2.
	//table contains the "static" PeriodicTableView table, and does not change very often.
	//table2 contains the tooltips and any other dynamic overlays.
	//Usually, we can skip the code which renders the table, and just use the
	//image stored in table...when doFullDraw==false, the rendering code is skipped.
	if ( doFullDraw )
	{
		table = new QPixmap( size() );

		m_painter->begin( table );
		m_painter->drawAll();
		m_painter->end();

		doFullDraw = false;
	}

	//JH: Ok, now table contains the static PeriodicTableView table, and we may need to draw
	//a tooltip on it.  However, we don't want to ruin the stored table pixmap,
	//so let's copy it to table2 and add the tooltip there.
	*table2 = *table;

	if ( m_currentElement > 0 )
	{
		m_painter->begin( table2 );
		m_painter->drawElementSelector( m_currentElement );
		m_painter->end();
	}

	//JH: Finally, copy the table2 pixmap to the widget
	QPainter p( this );
	p.drawPixmap( 0, 0, *table2 );
	p.end();
}

void PeriodicTableView::drawLegendToolTip( QPainter* p )
{
	(void)p;
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

void PeriodicTableView::drawLegend( QPainter* p )
{
	(void)p;
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
	if ( m_kalziumTip->isVisible() )
		m_kalziumTip->hide();
}

void PeriodicTableView::mouseMoveEvent( QMouseEvent *mouse )
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
	const int num = m_painter->currentTableType()->elementAtCoords( mouse->pos() );

	if ( num > 0 )
		emit ToolTip( num );
	else if ( m_kalziumTip->isVisible() )
	{
		m_kalziumTip->hide();
	}

	HoverTimer.start( 500, true ); //JH: true = run timer once, not continuously
	MouseoverTimer.start( 200, true ); //JH: true = run timer once, not continuously
}

void PeriodicTableView::mouseReleaseEvent( QMouseEvent *mouse )
{
	int elem = m_painter->currentTableType()->elementAtCoords( mouse->pos() );
	if ( elem > 0 )
	{
#if 0
		emit ElementClicked( elem );
#endif
		selectElement( elem );
	}
	else
		unSelect();
}

void PeriodicTableView::unSelect()
{
	m_currentElement = 0;
	update();
}

void PeriodicTableView::selectPoint( const QPoint& point )
{
	m_painter->currentTableType()->elementAtCoords( point );
}

void PeriodicTableView::selectElement( int num )
{
	m_currentElement = num;
	update();
}

void PeriodicTableView::slotMouseover()
{
	int num = m_painter->currentTableType()->elementAtCoords( mapFromGlobal( QCursor::pos() ) );
	if ( num > 0 )
		emit MouseOver( num );
}

void PeriodicTableView::setNumeration( int which )
{
	m_painter->setNumeration( which );
	setFullDraw();
	update();
}

KalziumGradientType* PeriodicTableView::gradient() const
{
	return m_painter->gradient();
}

void PeriodicTableView::setGradient( int which )
{
	m_painter->setGradient( which );
	setFullDraw();
	update();
}

void PeriodicTableView::showLegend( bool show )
{
	m_painter->toggleLegend( show );
}

bool PeriodicTableView::showLegend() const
{
	return m_painter->legendShown();
}

void PeriodicTableView::setTemperature( int temp )
{
	m_painter->setTemperature( temp );
	setFullDraw();
	update();
}

void PeriodicTableView::setDate( int date )
{
	m_painter->setDate( date );
	setFullDraw();
	update();
}

int PeriodicTableView::date() const
{
	return m_painter->date();
}

void PeriodicTableView::setTimeline( bool timeline )
{
	m_painter->setTimeline( timeline );
	setFullDraw();
	update();
}

bool PeriodicTableView::timeline() const
{
	return m_painter->timeline();
}

void PeriodicTableView::setFullDraw()
{
	doFullDraw = true;
}

#include "periodictableview.moc"
