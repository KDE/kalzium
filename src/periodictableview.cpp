/***************************************************************************
 *   Copyright (C) 2003-2005, 2006 by Carsten Niehaus                            *
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

	setMouseTracking( true );

	//JH: eliminates flicker on redraw
	setAttribute( Qt::WA_OpaquePaintEvent, true );

	table = new QPixmap();
	table2 = new QPixmap();

	//JH: Start with a full draw
	doFullDraw = true;

	m_startDrag = QPoint();

	m_hoverTimer.setSingleShot( true );
	connect( &m_hoverTimer, SIGNAL( timeout() ), this, SLOT( slotMouseover() ) );
	m_prevHoverElement = 0;

	KalziumTableType *classic = KalziumTableTypeFactory::instance()->build( "Classic" );
	m_painter = new KalziumPainter( classic );
	m_painter->toggleLegend( Prefs::showlegend() );
	
	// according to carsten :)
	setMinimumSize( classic->size() );

	update();
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

KalziumSchemeType* PeriodicTableView::scheme() const
{
	return m_painter->scheme();
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

void PeriodicTableView::mousePressEvent( QMouseEvent * event )
{
	int elementnumber = m_painter->currentTableType()->elementAtCoords( event->pos() );
	
	if ( ( elementnumber > 0 ) && ( event->button() == Qt::LeftButton ) )
	{
		m_startDrag = event->pos();
	}
}

void PeriodicTableView::mouseMoveEvent( QMouseEvent *event )
{
	int num = m_painter->currentTableType()->elementAtCoords( m_startDrag );

	if ( ( QApplication::mouseButtons() & Qt::LeftButton ) && ( ( event->pos() - m_startDrag ).manhattanLength() > QApplication::startDragDistance() ) && ( num > 0 ) )
	{
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
		if ( m_hoverTimer.isActive() )
			m_hoverTimer.stop();
		m_hoverTimer.start( 200 );
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
	return m_sliderValueList.contains( type ) ? m_sliderValueList.value( type ) : 0;
}

QBrush PeriodicTableView::brushForElement( int element ) const
{
	return m_painter->brushForElement( element );
}

void PeriodicTableView::slotMouseover()
{
	int num = m_painter->currentTableType()->elementAtCoords( mapFromGlobal( QCursor::pos() ) );
	if ( ( num > 0 ) && ( num != m_prevHoverElement ) )
		emit MouseOver( num );
	m_prevHoverElement = num;
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

void PeriodicTableView::setSliderValue( ChemicalDataObject::BlueObelisk type, int value )
{
	m_painter->setSliderValue( type, value );
	setFullDraw();
	update();
}

void PeriodicTableView::setFullDraw()
{
	doFullDraw = true;
}

#include "periodictableview.moc"
