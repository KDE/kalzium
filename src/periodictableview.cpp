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

#include <QApplication>
#include <QPixmap>
#include <QPoint>
#include <QCursor>
#include <QMouseEvent>
#include <QPainter>
#include <QRect>
#include <QDrag>
#include <QSvgGenerator>

PeriodicTableView::PeriodicTableView( QWidget *parent )
  : QWidget( parent )
{
    // No selection
    m_currentElement = 0;

    setMouseTracking( true );

    //eliminates flicker on redraw
    setAttribute( Qt::WA_OpaquePaintEvent, true );

    table = 0;
    table2 = new QPixmap();

    //Start with a full draw
    doFullDraw = true;

    m_startDrag = QPoint();

    m_hoverTimer.setSingleShot( true );
    connect( &m_hoverTimer, SIGNAL( timeout() ), this, SLOT( slotMouseover() ) );
    m_prevHoverElement = 0;

    KalziumTableType *tt = KalziumTableTypeFactory::instance()->build( "Short" );
    m_painter = new KalziumPainter( tt );

    setMinimumSize( tt->size() );

    update();
}

PeriodicTableView::~PeriodicTableView()
{
	delete table;
	delete table2;

	delete m_painter;
}

void PeriodicTableView::activateColorScheme( const int nr )
{
    m_painter->setScheme( nr );

    emit SchemeChanged( m_painter->scheme() ); 

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

    emit ModeChanged( m );

	setFullDraw();
	update();
}

KalziumPainter::MODE PeriodicTableView::mode() const
{
	return m_painter->mode();
}

void PeriodicTableView::generateSvg(const QString& filename)
{
        QSvgGenerator *svgGen = new QSvgGenerator();

        svgGen->setFileName( filename );

        m_painter->begin( svgGen );
        m_painter->drawAll();
        m_painter->end();
}

void PeriodicTableView::paintEvent( QPaintEvent * e )
{
	if ( doFullDraw )
	{
		delete table;
		table = new QPixmap( size() );

		m_painter->begin( table );
		m_painter->drawAll();
		m_painter->end();

		doFullDraw = false;
	}

	*table2 = *table;

	if ( m_currentElement > 0 )
	{
		m_painter->begin( table2 );
		m_painter->drawElementSelector( m_currentElement );
		m_painter->end();
	}

	QPainter p( this );
	p.drawPixmap( e->rect().topLeft(), *table2, e->rect() );
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

int PeriodicTableView::sliderValue( ChemicalDataObject::BlueObelisk type )
{
	return m_sliderValueList.contains( type ) ? m_sliderValueList.value( type ) : 0;
}

QBrush PeriodicTableView::brushForElement( int element ) const
{
	return m_painter->brushForElement( element );
}

KalziumTableType* PeriodicTableView::tableType() const
{
	return m_painter->currentTableType();
}

void PeriodicTableView::slotChangeTable(int table)
{
    KalziumTableType *tt = KalziumTableTypeFactory::instance()->build( table );
    m_painter->setTableType(tt);

    emit TableTypeChanged( tt );

    setMinimumSize( tt->size() );
    setFullDraw();
    update();
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

    emit GradientTypeChanged( m_painter->gradient() );
    setFullDraw();
    update();
}

void PeriodicTableView::setTemperature( int temp )
{
	m_painter->setTemperature( temp );
	setFullDraw();
	update();
}

void PeriodicTableView::setTime( int time )
{
	m_painter->setTime( time );
	setFullDraw();
	update();
}

void PeriodicTableView::setFullDraw()
{
	doFullDraw = true;
}

#include "periodictableview.moc"
