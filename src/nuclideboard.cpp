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
#include <q3scrollview.h>
#include <qsizepolicy.h>
#include <qtimer.h>
#include <q3vbox.h>

#include <kapplication.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>

#include <math.h>
#include <stdlib.h>

static const int MaxIsotopeSize = 60;
static const int MinIsotopeSize = 12;

static const int MaxElementDisplayed = 111;
static const int MinElementDisplayed = 1;

IsotopeTableView::IsotopeTableView( QWidget* parent, Q3ScrollView *scroll, const char* name )
	: QWidget( parent, name ), m_parent( parent ), m_scroll( scroll )
{
	setBackgroundMode( Qt::NoBackground );
//	setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
//	setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
	setMinimumSize( 200, 200 );
	// resizing to a fake size
	resize( sizeHint() );

	m_duringSelection = false;

	m_rectSize = -1;

	m_firstElem = MinElementDisplayed;
	m_lastElem = MaxElementDisplayed;
	m_firstElemNucleon = minNucleonOf( KalziumDataObject::instance()->element( m_firstElem ) );
	m_lastElemNucleon = maxNucleonOf( KalziumDataObject::instance()->element( m_lastElem ) );

kdDebug() << k_funcinfo << m_firstElemNucleon << " ... " << m_lastElemNucleon << endl;

	// updating the list of isotope rects...
	updateIsoptopeRectList();
	// ... and repainting
	repaint();
}

void IsotopeTableView::paintEvent( QPaintEvent* /* e */ )
{
	QPixmap pm( width(), height() );
	pm.fill( Qt::lightGray );

	QPainter p;
	p.begin( &pm, this );
	
	drawIsotopeWidgets( &p );

	if ( m_duringSelection )
	{//draw the selection-rectangle
#warning QPainter::rasterOp()
//		p.setRasterOp( Qt::XorROP );
		p.setPen( QPen( Qt::white, 1, Qt::DotLine ) );
		p.drawRect( m_selectedRegion );
#warning QPainter::rasterOp()
//		p.setRasterOp( Qt::CopyROP );
	}

	p.end();
	bitBlt( this, 0, 0, &pm );
}

void IsotopeTableView::resizeEvent( QResizeEvent */*e*/ )
{
	m_rectSize = -1;

	updateIsoptopeRectList();
	update();
}

QRect IsotopeTableView::getNewCoords( const QRect& rect ) const
{
	QRect ret;

	int i = 0;
	int a = 0;

kdDebug() << "ORIG RECT: " << rect << endl
          << "OUR SIZE:  " << size() << endl;

	i = 0;
	a = height() - rect.top();
	while ( i * ( m_rectSize - 1 ) < a ) i++;
kdDebug() << "TOP: " << i << endl;
	ret.setTop( m_firstElem + i - 1 );

	i = 0;
	while ( i * ( m_rectSize - 1 ) < rect.right() ) i++;
kdDebug() << "RIGHT: " << i << endl;
	ret.setRight( m_firstElemNucleon + i - 1 );

	i = 0;
	a = height() - rect.bottom();
	while ( i * ( m_rectSize - 1 ) < a ) i++;
kdDebug() << "BOTTOM: " << i << endl;
	ret.setBottom( m_firstElem + i - 1 );

	i = 0;
	while ( i * ( m_rectSize - 1 ) < rect.left() ) i++;
kdDebug() << "LEFT: " << i << endl;
	ret.setLeft( m_firstElemNucleon + i - 1 );

	// normalize the rect - needed to get valid coordinates
	ret = ret.normalize();

kdDebug() << "RECT: " << ret << endl;

	return ret;
}

void IsotopeTableView::selectionDone( QRect selectedRect )
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

void IsotopeTableView::updateIsoptopeRectList()
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
			m_rectSize = (int)kMin( m_scroll->viewport()->height() / (double)numOfElements,
			                        m_scroll->viewport()->width() / (double)numOfNucleons );
		}
		else
		{
			m_rectSize = (int)kMin( height() / (double)numOfElements,
			                        width() / (double)numOfNucleons );
		}
/*
kdDebug() << "width(): " << width() << " - height(): " << height()
          << " - noe: " << numOfElements << " - non: " << numOfNucleons
//          << " - a: " << a << " - b: " << b
          << " - size(): " << size()
          << " - m_rectSize: " << m_rectSize << endl;
//*/
		if ( m_rectSize < MinIsotopeSize )
		{
			m_rectSize = MinIsotopeSize;
		}
		else if ( m_rectSize > MaxIsotopeSize )
		{
			m_rectSize = MaxIsotopeSize;
		}
		const int newsizex = numOfNucleons * ( m_rectSize - 1 ) + 2;
		const int newsizey = numOfElements * ( m_rectSize - 1 ) + 2;
		resize( newsizex, newsizey );
		if ( m_scroll )
			m_scroll->resizeContents( newsizex, newsizey );
		if ( m_parent )
			m_parent->resize( newsizex, newsizey );
//kdDebug() << "size(): " << size() << endl;
	}
	
	IsotopeList isotopeList;
	IsotopeList::ConstIterator isotope;
	IsotopeList::ConstIterator isotopeEnd;

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
					m_rectSize, m_rectSize);
//kdDebug() << k_funcinfo << boundingRect << endl;

 			m_IsotopeAdapterRectMap.insert(iso, boundingRect);
		}
		++id;
	}

	update();
}

int IsotopeTableView::minNucleonOf( Element* el, int lowerbound ) const
{
	if ( !el ) return lowerbound;

	int minNumber = 1000;

	IsotopeList isotopeList = el->isotopes();
	IsotopeList::const_iterator isoIt = isotopeList.constBegin();
	IsotopeList::const_iterator isoItEnd = isotopeList.constEnd();

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

	IsotopeList isotopeList = el->isotopes();
	IsotopeList::const_iterator isoIt = isotopeList.constBegin();
	IsotopeList::const_iterator isoItEnd = isotopeList.constEnd();

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

	IsotopeList isotopeList = el->isotopes();
	IsotopeList::ConstIterator isoIt = isotopeList.constBegin();
	IsotopeList::ConstIterator isoItEnd = isotopeList.constEnd();

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


///FIXME there are more than just one decay possible...
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

void IsotopeTableView::drawIsotopeWidgets( QPainter *p )
{
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
				poly.setPoint( 1, it.data().topRight() );
				poly.setPoint( 2, it.data().bottomLeft() );

				poly.setPoint( 0, it.data().topLeft() );
				p->setBrush( colors.first );
				p->drawPolygon( poly );

				poly.setPoint( 0, it.data().bottomRight() );
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
}

NuclideLegend::NuclideLegend( QWidget* parent, const char* name ) : QWidget( parent, name )
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

IsotopeTableDialog::IsotopeTableDialog( QWidget* parent, const char* name )
	: KDialogBase( parent, name, true, i18n( "Isotope Table" ), Help|Close, Close, true )
{
	QWidget *page = new QWidget( this );
	setMainWidget( page );

	QVBoxLayout *vbox = new QVBoxLayout(  page , 0, -1, "vbox" );

	Q3ScrollView *helperSV = new Q3ScrollView(page);
	helperSV->viewport()->setPaletteBackgroundColor(paletteBackgroundColor());
	helperSV->setFrameShape(QFrame::NoFrame);
	helperSV->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	Q3VBox *big_box = new Q3VBox( helperSV->viewport() );
	helperSV->addChild( big_box );
	vbox->addWidget( helperSV );

	m_view = new IsotopeTableView( big_box, helperSV, "view" );
	m_view->installEventFilter( this );

	NuclideLegend *legend = new NuclideLegend( page, "legend" );
	vbox->addWidget( legend );

	connect( this, SIGNAL(selectionDone( QRect ) ),
	         m_view, SLOT(selectionDone( QRect ) ) );

	setMinimumSize( 750, 500 );
	resize( minimumSize() );

	update();
}

void IsotopeTableDialog::slotHelp()
{
	if ( kapp )
		kapp->invokeHelp ( "isotope_table", "kalzium" );
}

bool IsotopeTableDialog::eventFilter( QObject *obj, QEvent *ev )
{
	if (ev->type() == QEvent::MouseButtonPress )	
	{
		QMouseEvent *mev = static_cast<QMouseEvent*>( ev );

		m_view->m_firstPoint = mev->pos();
		m_view->m_duringSelection = true;
		return true;
	}
	else if (ev->type() == QEvent::MouseMove )
	{
		QMouseEvent *mev = static_cast<QMouseEvent*>( ev );

		QRect startPoint( m_view->m_firstPoint, m_view->m_firstPoint );
		QRect endPoint( mev->pos(), mev->pos() );

		m_view->m_selectedRegion = startPoint.unite( endPoint );
		m_view->update();
		return true;
	}
	else if (ev->type() == QEvent::MouseButtonRelease )
	{
		QMouseEvent *mev = static_cast<QMouseEvent*>( ev );
		
		QRect startPoint( m_view->m_firstPoint, m_view->m_firstPoint );
		QRect endPoint( mev->pos(), mev->pos() );

		m_view->m_duringSelection = false;
		
		// ensure to have a valid QRect
		QRect ourrect = startPoint.unite( endPoint ).normalize();

		emit selectionDone( ourrect );
		return true;
	}
	KDialogBase::eventFilter(obj,ev);
	return false;
}

#include "nuclideboard.moc"

