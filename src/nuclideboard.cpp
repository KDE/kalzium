/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *   
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include "nuclideboard.h"

#include "element.h"
#include "isotope.h"
#include "kalziumdataobject.h"
#include "kalziumutils.h"

#include <qevent.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qpolygon.h>
#include <qsizepolicy.h>

#include <kaction.h>
#include <kactioncollection.h>
#include <kcursor.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kstdaction.h>
#include <ktoolbar.h>
#include <ktoolinvocation.h>

#include <math.h>
#include <stdlib.h>

static const int MaxIsotopeSize = 60;
static const int MinIsotopeSize = 12;

static const int MaxElementDisplayed = 111;
static const int MinElementDisplayed = 1;

IsotopeTableView::IsotopeTableView( QWidget* parent, IsotopeScrollArea *scroll )
	: QWidget( parent ), m_parent( parent ), m_scroll( scroll )
{
	setBackgroundMode( Qt::NoBackground );
//	setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
//	setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
	setMinimumSize( 200, 200 );
	// resizing to a fake size
	resize( sizeHint() );
	m_pix.resize( size() );

	m_duringSelection = false;
	m_isMoving = false;

	m_rectSize = -1;

	m_firstElem = MinElementDisplayed;
	m_lastElem = MaxElementDisplayed;
	m_firstElemNucleon = minNucleonOf( KalziumDataObject::instance()->element( m_firstElem ) );
	m_lastElemNucleon = maxNucleonOf( KalziumDataObject::instance()->element( m_lastElem ) );

kdDebug() << k_funcinfo << m_firstElemNucleon << " ... " << m_lastElemNucleon << endl;

	// updating the list of isotope rects...
	updateIsoptopeRectList();
	// ... and repainting
	drawInternally();
	repaint();
}

void IsotopeTableView::paintEvent( QPaintEvent* /* e */ )
{
	QPixmap pm( size() );
	bitBlt( &pm, 0, 0, &m_pix );

	if ( m_duringSelection )
	{
		//draw the selection-rectangle
		QPainter p;
		p.begin( &pm, this );
	
		p.setPen( QPen( Qt::black, 1, Qt::DotLine ) );
		p.drawRect( m_selectedRegion );

		p.end();
	}

	bitBlt( this, 0, 0, &pm );
}

QRect IsotopeTableView::getNewCoords( const QRect& rect ) const
{
	QRect ret;

	int i = 0;
	int a = 0;

kdDebug() << "ORIG RECT: " << rect << endl
          << "OUR SIZE:  " << size() << endl;

	i = 0;
	a = height() - rect.top() - 30;
	if ( a > 0 )
	{
		while ( i * ( m_rectSize - 1 ) < a ) i++;
kdDebug() << "TOP: " << i << endl;
		ret.setTop( m_firstElem + i - 1 );
	}
	else
		ret.setTop( m_firstElem + 2 );

	i = 0;
	while ( i * ( m_rectSize - 1 ) < rect.right() - 30 ) i++;
kdDebug() << "RIGHT: " << i << endl;
	ret.setRight( m_firstElemNucleon + i - 1 );

	i = 0;
	a = height() - rect.bottom() - 30;
	if ( a > 0 )
	{
		while ( i * ( m_rectSize - 1 ) < a ) i++;
kdDebug() << "BOTTOM: " << i << endl;
		ret.setBottom( m_firstElem + i - 1 );
	}
	else
		ret.setBottom( m_firstElem );

	i = 0;
	while ( i * ( m_rectSize - 1 ) < rect.left() - 30 ) i++;
kdDebug() << "LEFT: " << i << endl;
	ret.setLeft( m_firstElemNucleon + i - 1 );

	// normalize the rect - needed to get valid coordinates
	ret = ret.normalize();

kdDebug() << "RECT: " << ret << endl;

	return ret;
}

void IsotopeTableView::selectionDone( const QRect& selectedRect )
{
	QRect r = getNewCoords( selectedRect );
	// I know it's strange, but somehow sometimes the bottom and the
	// top are swapped...
	m_firstElem = kMin( r.bottom(), r.top() );
	m_lastElem = kMax( r.bottom(), r.top() );
	m_firstElemNucleon = kMin( r.left(), r.right() );
	m_lastElemNucleon = kMax( r.left(), r.right() );

	// tell to the updateIsoptopeRectList() to recalc the size of every
	// element rect
	m_rectSize = -1;

	//now update the list of isotopes to be displayed
	updateIsoptopeRectList();
}

void IsotopeTableView::updateIsoptopeRectList( bool redoSize )
{
	m_IsotopeAdapterRectMap.clear();
	
	const int numOfElements = m_lastElem - m_firstElem + 1;
	const int numOfNucleons = m_lastElemNucleon - m_firstElemNucleon + 1;

	kdDebug () << m_firstElem << " ... " << m_lastElem << endl;
	kdDebug () << "Nucleons: " << m_firstElemNucleon << " ... " << m_lastElemNucleon << endl;

	if ( m_rectSize < 1 ) // need to recalc the size
	{
		if ( m_scroll )
		{
			m_rectSize = (int)kMin( ( m_scroll->viewport()->height() - 30 ) / (double)numOfElements,
			                        ( m_scroll->viewport()->width() - 30 ) / (double)numOfNucleons );
		}
		else
		{
			m_rectSize = (int)kMin( ( height() - 30 ) / (double)numOfElements,
			                        ( width() - 30 ) / (double)numOfNucleons );
		}
		redoSize = true;
	}
/*
kdDebug() << "width(): " << width() << " - height(): " << height()
          << " - noe: " << numOfElements << " - non: " << numOfNucleons
//          << " - a: " << a << " - b: " << b
          << " - size(): " << size()
          << " - m_rectSize: " << m_rectSize << endl;
//*/
	if ( redoSize )
	{
		if ( m_rectSize < MinIsotopeSize )
		{
			m_rectSize = MinIsotopeSize;
		}
		else if ( m_rectSize > MaxIsotopeSize )
		{
			m_rectSize = MaxIsotopeSize;
		}
		const int newsizex = numOfNucleons * ( m_rectSize - 1 ) + 2 + 30;
		const int newsizey = numOfElements * ( m_rectSize - 1 ) + 2 + 30;
		resize( newsizex, newsizey );
		m_pix.resize( size() );
//kdDebug() << "size(): " << size() << endl;
	}
	
	QList<Isotope*> isotopeList;
	QList<Isotope*>::ConstIterator isotope;
	QList<Isotope*>::ConstIterator isotopeEnd;

	int id = 0;

	for ( int i = m_firstElem; i <= m_lastElem; i++ )
	{
//kdDebug() << k_funcinfo << "i: " << i << endl;
		Element* el = KalziumDataObject::instance()->element( i );
		if ( !el ) continue;

//		kdDebug() << "el: " << el->elname() << " (" << el->number() <<")" << endl;
		isotopeList = isotopesWithNucleonsInRange( el, m_firstElemNucleon, m_lastElemNucleon );
		isotopeEnd = isotopeList.constEnd();
		isotope = isotopeList.constBegin();
//kdDebug() << k_funcinfo << "isolist: " << isotopeList.count() << endl;
		
		for ( ; isotope != isotopeEnd; ++isotope )
		{
			Isotope* iso = *isotope;
			if ( !iso ) continue;

			int Xpositon = iso->nucleons() - m_firstElemNucleon;

			QRect boundingRect = QRect( 
					Xpositon*(m_rectSize-1)+1, 
					( numOfElements - id - 1 ) * ( m_rectSize - 1 ) + 1,
					m_rectSize - 1, m_rectSize - 1 );
//kdDebug() << k_funcinfo << boundingRect << endl;

 			m_IsotopeAdapterRectMap.insert(iso, boundingRect);
		}
		++id;
	}

	drawInternally();
	update();
}

int IsotopeTableView::minNucleonOf( Element* el, int lowerbound ) const
{
	if ( !el ) return lowerbound;

	int minNumber = 1000;

	QList<Isotope*> isotopeList = el->isotopes();
	QList<Isotope*>::const_iterator isoIt = isotopeList.constBegin();
	QList<Isotope*>::const_iterator isoItEnd = isotopeList.constEnd();

	for ( ; isoIt != isoItEnd; ++isoIt )
	{
		if ( ( ( *isoIt )->nucleons() < minNumber ) &&
		     ( ( *isoIt )->nucleons() >= lowerbound ) )
			minNumber = ( *isoIt )->nucleons();
	}
	return minNumber;
}

int IsotopeTableView::maxNucleonOf( Element* el, int upperbound ) const
{
	if ( !el ) return upperbound;

	int maxNumber = 0;

	QList<Isotope*> isotopeList = el->isotopes();
	QList<Isotope*>::const_iterator isoIt = isotopeList.constBegin();
	QList<Isotope*>::const_iterator isoItEnd = isotopeList.constEnd();

	for ( ; isoIt != isoItEnd; ++isoIt )
	{
		if ( ( ( *isoIt )->nucleons() > maxNumber ) &&
		     ( ( *isoIt )->nucleons() <= upperbound ) )
			maxNumber = ( *isoIt )->nucleons();
	}
	return maxNumber;
}

QList<Isotope*> IsotopeTableView::isotopesWithNucleonsInRange( Element* el, int lowerbound, int upperbound ) const
{
	QList<Isotope*> isolist;

	if ( !el ) return isolist;

//kdDebug() << "isotopesWithNucleonsInRange(): " << el << " - low: " << lowerbound
//          << " - up: " << upperbound << endl;

	QList<Isotope*> isotopeList = el->isotopes();
	QList<Isotope*>::ConstIterator isoIt = isotopeList.constBegin();
	QList<Isotope*>::ConstIterator isoItEnd = isotopeList.constEnd();

	for ( ; isoIt != isoItEnd; ++isoIt )
	{
		if ( ( ( *isoIt )->nucleons() >= lowerbound ) &&
		     ( ( *isoIt )->nucleons() <= upperbound ) )
		{
			isolist.append( ( *isoIt ) );
		}
	}

	return isolist;
}


QPair<QColor, QColor> IsotopeTableView::isotopeColor( Isotope* isotope )
{
	QPair<QColor, QColor> def = qMakePair( QColor( Qt::magenta ), QColor( Qt::magenta ) );
	QPair<QColor, QColor> c = qMakePair( QColor(), QColor() );
	if ( !isotope ) return def;

	if ( !isotope->betaminusdecay() && !isotope->betaplusdecay() &&
	     !isotope->alphadecay() && !isotope->ecdecay() )
		c = def;
	else
	{
		if ( isotope->betaminusdecay() )
			if ( c.first.isValid() )
				c.second = Qt::cyan;
			else
				c.first = Qt::cyan;
		if ( isotope->betaplusdecay() )
			if ( c.first.isValid() )
				c.second = Qt::red;
			else
				c.first = Qt::red;
		if ( isotope->alphadecay() )
			if ( c.first.isValid() )
				c.second = Qt::yellow;
			else
				c.first = Qt::yellow;
		if ( isotope->ecdecay() )
			if ( c.first.isValid() )
				c.second = Qt::green;
			else
				c.first = Qt::green;

		if ( !c.second.isValid() )
			c.second = c.first;
	}

	return c;
}

void IsotopeTableView::drawInternally()
{
	m_pix.fill( Qt::lightGray );

	QPainter p( &m_pix );

	drawIsotopeWidgets( &p );

	drawLegends( &p );

	p.end();
}

void IsotopeTableView::drawIsotopeWidgets( QPainter *p )
{
	p->translate( 30, 0 );

	QMap<Isotope*, QRect>::ConstIterator it = m_IsotopeAdapterRectMap.constBegin();
	const QMap<Isotope*, QRect>::ConstIterator itEnd = m_IsotopeAdapterRectMap.constEnd();

	QFont f = p->font();
	f.setPointSize( KalziumUtils::maxSize( "Mn", QRect( 0, 0, m_rectSize, m_rectSize ), f, p ) );
	p->setFont( f );

	for ( ; it != itEnd ; ++it )
	{
		Isotope* i = it.key();
	
		if ( i )
		{
			QPair<QColor, QColor> colors( isotopeColor( i ) ) ;
			if ( colors.first.name() == colors.second.name() )
			{
				p->setBrush( colors.first );
				p->drawRect( it.data() );
			}
			else
			{
				QPen oldpen = p->pen();
				p->setPen( Qt::NoPen );
				QPolygon poly( 3 );
				poly.setPoint( 1, it.data().topRight() + QPoint( 1, 0 ) );
				poly.setPoint( 2, it.data().bottomLeft() + QPoint( 0, 1 ) );

				poly.setPoint( 0, it.data().topLeft() );
				p->setBrush( colors.first );
				p->drawPolygon( poly );

				poly.setPoint( 0, it.data().bottomRight() + QPoint( 1, 1 ) );
				p->setBrush( colors.second );
				p->drawPolygon( poly );

				p->setPen( oldpen );
				QBrush oldbrush = p->brush();
				p->setBrush( Qt::NoBrush );
				p->drawRect( it.data() );
				p->setBrush( oldbrush );
			}
			
			//For debugging, lets add the information
//			p->drawText( it.data() ,Qt::AlignCenter, QString::number( it.key()->neutrons() ) );
			p->drawText( it.data(), Qt::AlignCenter, KalziumDataObject::instance()->element( it.key()->protones() )->symbol() );
		}
	}
	p->setBrush( Qt::black );

	p->translate( -30, 0 );
}

void IsotopeTableView::drawLegends( QPainter *p )
{
	QFont oldfont = p->font();

	const int numElems = m_lastElem - m_firstElem + 1;
	const int numNucleons = m_lastElemNucleon - m_firstElemNucleon + 1;
	int i;
	QRect rect;

	QFont f = p->font();
	f.setPointSize( KalziumUtils::maxSize( "Mn", QRect( 0, 0, 30, m_rectSize ), f, p ) );
	p->setFont( f );

	// elements
	for ( i = 0; i < numElems; i++ )
	{
		rect = QRect( 0, ( numElems - i - 1 ) * ( m_rectSize - 1 ),
		              30, m_rectSize - 1 );
		p->drawText( rect, Qt::AlignCenter,
		             KalziumDataObject::instance()->element( m_firstElem + i )->symbol() );
	}

	f = p->font();
	f.setPointSize( KalziumUtils::maxSize( "222", QRect( 0, 0, m_rectSize, 30 ), f, p ) );
	p->setFont( f );

	// number of protons
	for ( i = 0; i < numNucleons; i++ )
	{
		rect = QRect( i * ( m_rectSize - 1 ) + 30, height() - 30,
		              m_rectSize - 1, 30 );
		p->drawText( rect, Qt::AlignCenter, QString::number( m_firstElemNucleon + i ) );
	}
/*
	int Xpositon = iso->nucleons() - m_firstElemNucleon;

	QRect boundingRect = QRect( 
		Xpositon*(m_rectSize-1)+1, 
		( numOfElements - id - 1 ) * ( m_rectSize - 1 ) + 1,
		m_rectSize - 1, m_rectSize - 1 );
*/
	p->setFont( oldfont );
}

void IsotopeTableView::mousePressEvent( QMouseEvent *e )
{
	if ( e->button() == Qt::LeftButton )
	{
		if ( m_duringSelection )
		{
			m_firstPoint = e->pos();
		}
		else
		{
			m_isMoving = true;
			m_firstPoint = m_scroll->mapToParent( e->pos() );
			setCursor( KCursor::handCursor() );
		}
	}
}

void IsotopeTableView::mouseReleaseEvent( QMouseEvent *e )
{
	if ( e->button() == Qt::LeftButton )
	{
		if ( m_duringSelection )
		{
			QRect startPoint( m_firstPoint, m_firstPoint );
			QRect endPoint( e->pos(), e->pos() );

			// ensure to have a valid QRect
			QRect ourrect = startPoint.unite( endPoint ).normalize();

			// ensure to zoom in a valid region
			if ( ( ourrect.width() > m_rectSize ) &&
			     ( ourrect.height() > m_rectSize ) )
			{
				m_duringSelection = false;

				emit toggleZoomAction( false );

				selectionDone( ourrect );
			}
			else
				update();
		}
		else
		{
			m_isMoving = false;
			setCursor( KCursor::arrowCursor() );
		}
	}
}

void IsotopeTableView::mouseMoveEvent( QMouseEvent *e )
{
	if ( m_duringSelection )
	{
		QRect startPoint( m_firstPoint, m_firstPoint );
		QRect endPoint( e->pos(), e->pos() );

		m_selectedRegion = startPoint.unite( endPoint );
		update();
	}
	if ( m_isMoving )
	{
		QPoint now = m_scroll->mapToParent( e->pos() );
		QPoint diff = m_firstPoint - now;
//kdDebug() << "m_firstPoint: " << m_firstPoint << " - now: " << now << endl;
		m_scroll->scrollBy( diff.x(), diff.y() );
		m_firstPoint = now;
	}
}

void IsotopeTableView::slotZoomIn()
{
	m_rectSize = (int)( m_rectSize * 1.2 );
	updateIsoptopeRectList( true );
	drawInternally();
	repaint();
}

void IsotopeTableView::slotZoomOut()
{
	m_rectSize = (int)( m_rectSize * 0.8 );
	updateIsoptopeRectList( true );
	drawInternally();
	repaint();
}

void IsotopeTableView::slotToogleZoomMode( bool state )
{
	m_duringSelection = state;
}



NuclideLegend::NuclideLegend( QWidget* parent )
	: QWidget( parent )
{
	setMinimumSize( 300, 50 );
	setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum );
	resize( minimumSizeHint() );
}

void NuclideLegend::paintEvent( QPaintEvent* /*e*/ )
{
	QPainter p( this );
	QString text;

	p.fillRect( 10, 10, 10, 10, Qt::cyan );
	text = i18n( "%1- Decay" ).arg( QChar( 946 ) );
	p.drawText( 30, 20, text );

	p.fillRect( 10, 30, 10, 10, Qt::red );
	text =  i18n( "%1+ Decay" ).arg( QChar( 946) );
	p.drawText( 30, 40, text );

	p.fillRect( 120, 10, 10, 10, Qt::yellow );
	text =  i18n( "%1 Decay" ).arg( QChar( 945 ) );
	p.drawText( 140, 20, text );

	p.fillRect( 120, 30, 10, 10, Qt::green );
	text = i18n( "Acronym of Electron Capture Decay", "EC Decay" );
	p.drawText( 140, 40, text );

	p.fillRect( 230, 10, 10, 10, Qt::magenta );
	text =  i18n( "Stable" );
	p.drawText( 250, 20, text );
}

IsotopeScrollArea::IsotopeScrollArea( QWidget* parent )
	: QScrollArea( parent )
{
	viewport()->setPaletteBackgroundColor( parent->paletteBackgroundColor() );
	setFrameShape( QFrame::NoFrame );
	setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
}

void IsotopeScrollArea::scrollBy( int x, int y )
{
	scrollContentsBy( x, y );
}

IsotopeTableDialog::IsotopeTableDialog( QWidget* parent )
	: KDialogBase( parent, "isotope-table", true, i18n( "Isotope Table" ), Help|Close, Close, true )
{
	QWidget *page = new QWidget( this );
	setMainWidget( page );

	QVBoxLayout *vbox = new QVBoxLayout( page , 0, -1 );

	IsotopeScrollArea *scrollarea = new IsotopeScrollArea( page );
	m_view = new IsotopeTableView( scrollarea, scrollarea );
	scrollarea->setWidget( m_view );

	NuclideLegend *legend = new NuclideLegend( page );

	m_ac = new KActionCollection( this );
	KToolBar *toolbar = new KToolBar( page, "toolbar", true, false );
	toolbar->setIconSize( 22 );
	KAction *a = KStdAction::zoomIn( m_view, SLOT( slotZoomIn() ), m_ac, "zoomin" );
	a->plug( toolbar );
	a = KStdAction::zoomOut( m_view, SLOT( slotZoomOut() ), m_ac, "zoomout" );
	a->plug( toolbar );
	KToggleAction *ta = new KToggleAction( i18n( "Select Zoom Area" ), "viewmagfit", 0, 0, 0, m_ac, "zoomselect" );
	connect( ta, SIGNAL( toggled( bool ) ), m_view, SLOT( slotToogleZoomMode( bool ) ) );
	connect( m_view, SIGNAL( toggleZoomAction( bool ) ), ta, SLOT( setChecked( bool ) ) );
	ta->plug( toolbar );

	vbox->addWidget( toolbar );
	vbox->addWidget( scrollarea );
	vbox->addWidget( legend );

	setMinimumSize( 750, 500 );
	resize( minimumSize() );

	update();
}

void IsotopeTableDialog::slotHelp()
{
	KToolInvocation::invokeHelp( QLatin1String( "isotope_table" ), QLatin1String( "kalzium" ) );
}

KActionCollection* IsotopeTableDialog::actionCollection()
{
	return m_ac;
}

#include "nuclideboard.moc"

