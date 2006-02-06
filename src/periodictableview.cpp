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

#include <QApplication>
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
#include <QDrag>

PeriodicTableView::PeriodicTableView( QWidget *parent )
  : QWidget( parent )
{
	// No selection
	m_currentElement = 0;

	HoverTimer.setSingleShot( true );
	MouseoverTimer.setSingleShot( true );

	connect( this,        SIGNAL( ToolTip( int ) ), 
	         this,        SLOT( slotToolTip( int ) ) );

	connect( &HoverTimer, SIGNAL( timeout() ), 
	         this,        SLOT( slotTransientLabel() ) );
	
	connect( &MouseoverTimer, SIGNAL( timeout() ), 
	         this,        SLOT( slotMouseover() ) );

	setMouseTracking( true );

	//JH: eliminates flicker on redraw
	setAttribute( Qt::WA_NoBackground, true );

	m_showTooltip = true;
	m_showLegendTooltip = Prefs::tooltip();
	m_tooltipsEnabled = Prefs::tooltip();

	table = new QPixmap();
	table2 = new QPixmap();

	m_kalziumTip = new KalziumTip( this );

	//JH: Start with a full draw
	doFullDraw = true;

	m_startDrag = QPoint();

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
		delete table;
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
//X 	kDebug() << "PeriodicTableView::drawLegendToolTip()" << endl;
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

void PeriodicTableView::mousePressEvent( QMouseEvent * event )
{
	if ( m_kalziumTip->isVisible() )
		m_kalziumTip->hide();

	int elementnumber = m_painter->currentTableType()->elementAtCoords( event->pos() );
	
	if ( ( elementnumber > 0 ) && ( event->button() == Qt::LeftButton ) )
	{
		m_startDrag = event->pos();
	}
}

void PeriodicTableView::mouseMoveEvent( QMouseEvent *event )
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
	int num = m_painter->currentTableType()->elementAtCoords( m_startDrag );

	if ( ( QApplication::mouseButtons() & Qt::LeftButton ) && ( ( event->pos() - m_startDrag ).manhattanLength() > QApplication::startDragDistance() ) && ( num > 0 ) )
	{
		if ( m_kalziumTip->isVisible() )
		{
			m_kalziumTip->hide();
		}

		Element* pointedElement = KalziumDataObject::instance()->element( num );

		QDrag *drag = new QDrag( this );
		QMimeData *mimeData = new QMimeData;

		mimeData->setText( pointedElement->dataAsString( ChemicalDataObject::name ) );
		drag->setMimeData( mimeData );

		QRect elrect = m_painter->currentTableType()->elementRect( num );
		elrect.translate( -elrect.topLeft() );
		QPixmap pix( elrect.size() + QSize( 1, 1 ) );
		m_painter->begin( &pix );
		m_painter->drawElement( num, elrect );
		m_painter->end();

		drag->setPixmap( pix );

		drag->start( Qt::CopyAction | Qt::MoveAction );

		m_startDrag = QPoint();
	}
	else
	{
		if ( num > 0 )
			emit ToolTip( num );
		else if ( m_kalziumTip->isVisible() )
		{
			m_kalziumTip->hide();
		}

		HoverTimer.start( 500 );
		MouseoverTimer.start( 200 );
	}
}

void PeriodicTableView::mouseReleaseEvent( QMouseEvent *mouse )
{
	int elem = m_painter->currentTableType()->elementAtCoords( mouse->pos() );
	if ( elem > 0 )
	{
		emit ElementClicked( elem );
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

void PeriodicTableView::setValueLimit( int value, ChemicalDataObject::BlueObelisk type )
{
	//remove the value of the type if possible
	m_sliderValueList.remove( type );

	//now add the new type/value
	m_sliderValueList.insert( type, value );
}

void PeriodicTableView::toggleLimit( bool toggle, ChemicalDataObject::BlueObelisk type )
{
}

int PeriodicTableView::sliderValue( ChemicalDataObject::BlueObelisk type )
{
	QMapIterator<ChemicalDataObject::BlueObelisk, int> i( m_sliderValueList );

	while ( i.hasNext() ) {
		if ( i.key() == type )
			return i.value();
	}
	return 0;
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
	setFullDraw();
	update();
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

void PeriodicTableView::setFullDraw()
{
	doFullDraw = true;
}

#include "periodictableview.moc"
