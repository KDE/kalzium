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

#include <qlabel.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qscrollview.h>
#include <qsizepolicy.h>
#include <qtimer.h>
#include <qvbox.h>

#include <kapplication.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>

#include <math.h>

static const int MaxIsotopeSize = 45;
static const int MinIsotopeSize = 12;

static const int MaxElementDisplayed = 111;
static const int MinElementDisplayed = 1;

IsotopeTableView::IsotopeTableView( QWidget* parent, QScrollView *scroll, const char* name )
	: QWidget( parent, name ), m_parent( parent ), m_scroll( scroll )
{
	setBackgroundMode( NoBackground );
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

//	QTimer::singleShot( 0, this, SLOT( updateIsoptopeRectList() ) );
	updateIsoptopeRectList();

//	QTimer::singleShot( 100, this, SLOT( repaint() ) );
	repaint();
}

/**
 * in this method everything beside the drawing is correct.
 * For some reasons the QRect is not drawn, the dimentions
 * of it are absolutly correct
 */
void IsotopeTableView::paintEvent( QPaintEvent* /* e */ )
{
	QPixmap pm( width(), height() );
	pm.fill( Qt::lightGray );

	QPainter p;
	p.begin( &pm, this );
	
	drawAxisLabels( &p );
	drawIsotopeWidgets( &p );

	if ( m_duringSelection )
	{//draw the selection-rectangle
		p.setRasterOp( Qt::XorROP );
		p.setPen( QPen( Qt::white, 1, Qt::DotLine ) );
		p.drawRect( m_selectedRegion );
		p.setRasterOp( Qt::CopyROP );
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

QRect IsotopeTableView::getNewCoords( const QRect& rect1 ) const
{
	QRect ret;

	int i = 0;
	int a = 0;

	QRect rect( m_scroll->viewportToContents( rect1.topLeft() ),
	            m_scroll->viewportToContents( rect1.bottomRight() ) );

	i = 0;
kdDebug() << "ORIG RECT: " << rect
          << "OUR SIZE:  " << size() << endl;
	a = height() - rect.top();
	while ( i * m_rectSize < a ) i++;
kdDebug() << "TOP: " << i << endl;
	ret.setTop( m_firstElem + i );

	i = 0;
	a = width() - rect.right();
	while ( i * m_rectSize < a ) i++;
kdDebug() << "RIGHT: " << i << endl;
	ret.setRight( m_firstElemNucleon + i );

	i = 0;
	while ( i * m_rectSize < rect.bottom() ) i++;
kdDebug() << "BOTTOM: " << i << endl;
	ret.setBottom( m_firstElem + i - 1 );

	i = 0;
	while ( i * m_rectSize < rect.left() ) i++;
kdDebug() << "LEFT: " << i << endl;
	ret.setLeft( m_firstElemNucleon + i - 1 );

kdDebug() << "RECT: " << ret << endl;

	return ret;
}

void IsotopeTableView::selectionDone( QRect selectedRect )
{
/*
	//safe the old positions
	m_oldBottomRight = m_bottomRight;
	m_oldTopLeft = m_topLeft;
	
	//FIXME This is plain wrong. I need to calculate realive to the old
	//size...
	//for thie maxElementNumberDisplayed and minElementNumberDisplayed
	//have to be adjusted: The first and the last line which is displaying
	//and element. If line 1 show element 90 and the last line displays element
	//34 the will have the value 90 and 34
	m_topLeft =  QPoint( selectedRect.x(), selectedRect.y() );
	m_bottomRight = QPoint( selectedRect.x() + selectedRect.width(),
							selectedRect.y() + selectedRect.height() );

	kdDebug() << "links, x:" <<
		m_topLeft.x() - m_oldTopLeft.x() <<
		" :links, y: " <<
		m_topLeft.y() - m_oldTopLeft.y() <<
		" :rechts, x: " <<
		m_oldBottomRight.x() - m_bottomRight.x() <<
		" :rechts,y: " <<
		m_oldBottomRight.y() - m_bottomRight.y() <<
		endl;

	m_topLeft /= m_rectSize;
	m_bottomRight /= m_rectSize;

	updateMinMaxValue();
*/
/*
	int i = 0;
	int a = height() - selectedRect.bottom();
	while ( i * m_rectSize < a ) i++;
kdDebug() << "BOTTOM: " << i << endl;
	m_firstElem += i - 1;

	i = 0;
	while ( i * m_rectSize < selectedRect.left() ) i++;
kdDebug() << "LEFT: " << i << endl;
	m_firstElemNucleon += i - 1;

	i = 0;
	while ( i * m_rectSize < selectedRect.top() ) i++;
kdDebug() << "TOP: " << i << endl;
	m_lastElem -= i - 1;

	i = 0;
	a = width() - selectedRect.right();
	while ( i * m_rectSize < a ) i++;
kdDebug() << "RIGHT: " << i << endl;
	m_lastElemNucleon -= i - 1;
*/
	QRect r = getNewCoords( selectedRect );
	m_firstElem = r.bottom();
	m_lastElem = r.top();
	m_firstElemNucleon = r.left();
	m_lastElemNucleon = r.right();

	m_rectSize = -1;

	//now update the list of isotopes to be displayed
	updateIsoptopeRectList();
}

void IsotopeTableView::updateMinMaxValue()
{

}

void IsotopeTableView::updateIsoptopeRectList()
{
	kdDebug() << "IsotopeTableView::updateIsoptopeRectList()" << endl;
	m_IsotopeAdapterRectMap.clear();
	
//	const int numOfElements = m_bottomRight.y() - m_topLeft.y();
	const int numOfElements = m_lastElem - m_firstElem + 1;
	const int numOfNucleons = m_lastElemNucleon - m_firstElemNucleon + 1;

/*
	int firstElement = maxElementNumberDisplayed-m_bottomRight.y();
	int lastElement = maxElementNumberDisplayed-m_topLeft.y();

	kdDebug () << firstElement << " ... " << lastElement << endl;
*/
	kdDebug () << m_firstElem << " ... " << m_lastElem << endl;

/*
	if ( firstElement < 1 || firstElement > 111 ) return;
	if ( lastElement < 1 || lastElement > 111 ) return;
*/

/*	
	QValueList<Element*>::ConstIterator it = m_list.at( m_firstElem );
	const QValueList<Element*>::ConstIterator itEnd = m_list.at( m_lastElem );
	
	//get the min/max numbers of the isotopes.
	const int minNucleons = minNumberOfNucleons( it, itEnd );
	const int maxNucleons = maxNumberOfNucleons( it, itEnd );
*/

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
//*
kdDebug() << "width(): " << width() << " - height(): " << height()
          << " - noe: " << numOfElements << " - non: " << numOfNucleons
//          << " - a: " << a << " - b: " << b
          << " - size(): " << size()
          << " - m_rectSize: " << m_rectSize << endl;
//*/
//kdDebug() << "m_rectSize: " << m_rectSize << endl;
		if ( m_rectSize < MinIsotopeSize )
		{
			m_rectSize = MinIsotopeSize;
		}
		else if ( m_rectSize > MaxIsotopeSize )
		{
			m_rectSize = MaxIsotopeSize;
		}
		resize( numOfNucleons * m_rectSize, numOfElements * m_rectSize );
		if ( m_scroll )
			m_scroll->resizeContents( numOfNucleons * m_rectSize, numOfElements * m_rectSize );
		if ( m_parent )
			m_parent->resize( numOfNucleons * m_rectSize, numOfElements * m_rectSize );
kdDebug() << "size(): " << size() << endl;
/*
		m_rectSize = (int)kMin( (double)numOfElements / width(),
		                        (double)numOfNucleons / height() );
*/
	}
	
//	if ( minNucleons < 1 ) return;
	
//X 	int tempSizeH = ( lastElement-firstElement )/height();
//X 	int tempSizeW = ( maxNucleons - minNucleons )/width();
//X 
//X 	m_rectSize = kMin( tempSizeW, tempSizeH );
//X 
//X 	kdDebug() << "Size calculation: " << tempSizeH << " and " << tempSizeW << endl;
	
	IsotopeList isotopeList;
	IsotopeList::ConstIterator isotope;
	IsotopeList::ConstIterator isotopeEnd;

//	for ( int countY = numOfElements ; it != itEnd; ++it )
	for ( int i = m_firstElem; i <= m_lastElem; i++ )
	{
//kdDebug() << k_funcinfo << "i: " << i << endl;
		Element* el = KalziumDataObject::instance()->element( i );
		if ( !el ) continue;

//		kdDebug() << "el: " << el->elname() << " (" << el->number() <<")" << endl;
		isotopeList = isotopesWithNucleonsInRange( el, m_firstElemNucleon, m_lastElemNucleon );
//		isotopeList = el->isotopes();
		isotopeEnd = isotopeList.constEnd();
		isotope = isotopeList.constBegin();
//kdDebug() << k_funcinfo << "isolist: " << isotopeList.count() << endl;
		
		for ( ; isotope != isotopeEnd; ++isotope )
		{
			Isotope* iso = *isotope;
			if ( !iso ) continue;
			

//			int Xpositon = iso->nucleons() - minNucleons;
			int Xpositon = iso->nucleons() - m_firstElemNucleon;

/*
			QRect boundingRect = QRect( 
					Xpositon*m_rectSize, 
					countY*m_rectSize, 
					m_rectSize, m_rectSize);
*/
			QRect boundingRect = QRect( 
					Xpositon*m_rectSize, 
					( numOfElements - i ) * m_rectSize,
					m_rectSize, m_rectSize);
//kdDebug() << k_funcinfo << boundingRect << endl;
			
//X			kdDebug() << "boundRect of the adapter: " << boundingRect << " Isotope of the adapter with " << iso->neutrons() << " neutrons." << endl;
			
 			m_IsotopeAdapterRectMap.insert(iso, boundingRect);
		}
//		countY--;
	}

	update();
}

int IsotopeTableView::maxNumberOfNucleons( 
		QValueList<Element*>::ConstIterator it,
		QValueList<Element*>::ConstIterator itEnd )
{
	int maxNumber = 0;
	for ( ; it != itEnd; ++it )
	{
		Element* el = *it;

		if ( !el ) continue;

		IsotopeList isotopeList = el->isotopes();
		IsotopeList::const_iterator isoIt = isotopeList.constBegin();
		IsotopeList::const_iterator isoItEnd = isotopeList.constEnd();

		for ( ; isoIt != isoItEnd; ++isoIt )
		{
			if ( ( *isoIt )->nucleons() > maxNumber )
				maxNumber = ( *isoIt )->nucleons();
		}
	}
	return maxNumber;
}

int IsotopeTableView::minNumberOfNucleons( QValueList<Element*>::ConstIterator it,
		QValueList<Element*>::ConstIterator itEnd )
{
	int minNumber = 1000;

	for ( ; it != itEnd; ++it )
	{
		Element* el = *it;

		if ( !el ) continue;

		IsotopeList isotopeList = el->isotopes();
		IsotopeList::const_iterator isoIt = isotopeList.constBegin();
		IsotopeList::const_iterator isoItEnd = isotopeList.constEnd();
		
		for ( ; isoIt != isoItEnd; ++isoIt )
		{
			if ( ( *isoIt )->nucleons() < minNumber )
				minNumber = ( *isoIt )->nucleons();
		}
	}

	return minNumber;
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

QValueList<Isotope*> IsotopeTableView::isotopesWithNucleonsInRange( Element* el, int lowerbound, int upperbound ) const
{
	QValueList<Isotope*> isolist;

	if ( !el ) return isolist;

//kdDebug() << "isotopesWithNucleonsInRange(): " << el << " - low: " << lowerbound
//          << " - up: " << upperbound << endl;

	IsotopeList isotopeList = el->isotopes();
	IsotopeList::ConstIterator isoIt = isotopeList.constBegin();
	IsotopeList::ConstIterator isoItEnd = isotopeList.constEnd();

	for ( ; isoIt != isoItEnd; ++isoIt )
	{
//kdDebug() << "# NUCLEONS: " << ( *isoIt )->nucleons() << endl;
		if ( ( ( *isoIt )->nucleons() >= lowerbound ) &&
		     ( ( *isoIt )->nucleons() <= upperbound ) )
			isolist.append( ( *isoIt ) );
	}

	return isolist;
}


///FIXME there are more than just one decay possible...
QColor IsotopeTableView::isotopeColor( Isotope* isotope )
{
	if ( !isotope ) return Qt::magenta;

	if ( isotope->betaminusdecay() )
		return Qt::blue;
	else if ( isotope->betaplusdecay() )
		return Qt::red;
	else if ( isotope->alphadecay() )
		return Qt::yellow;
	else if (  isotope->ecdecay() )
		return Qt::green;
	else
		return Qt::magenta;
}

void IsotopeTableView::drawAxisLabels( QPainter *p )
{
	( void )p;
}
		
void IsotopeTableView::drawIsotopeWidgets( QPainter *p )
{
	QMap<Isotope*, QRect>::ConstIterator it = m_IsotopeAdapterRectMap.constBegin();
	const QMap<Isotope*, QRect>::ConstIterator itEnd = m_IsotopeAdapterRectMap.constEnd();
		
	QFont f = p->font();
	f.setPointSize( KalziumUtils::maxSize( "22", QRect( 0, 0, m_rectSize, m_rectSize ), f, p ) );
	p->setFont( f );

	for ( ; it != itEnd ; ++it )
	{
		Isotope* i = it.key();
	

		if ( i )
		{
			QColor color( isotopeColor( i ) ) ;
			p->setBrush( color );
			p->drawRect( it.data() );
			
			//For debugging, lets add the information
//			p->drawText( it.data() ,Qt::AlignCenter, QString::number( it.key()->neutrons() ) );
			p->drawText( it.data() ,Qt::AlignCenter, QString::number( it.key()->nucleons() ) );
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

	p.fillRect( 10, 10, 10, 10, Qt::blue );
	text = i18n( "%1- %2" ).arg( QChar( 946 ) ).arg( i18n( "Decay" ) );
	p.drawText( 30, 20, text );

	p.fillRect( 10, 30, 10, 10, Qt::red );
	text =  i18n( "%1+ %2" ).arg( QChar( 946) ).arg( i18n( "Decay" ) );
	p.drawText( 30, 40, text );

	p.fillRect( 120, 10, 10, 10, Qt::yellow );
	text =  i18n( "%1 %2" ).arg( QChar( 945 ) ).arg( i18n( "Decay" ) );
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

	QScrollView *helperSV = new QScrollView(page);
	helperSV->viewport()->setPaletteBackgroundColor(paletteBackgroundColor());
	helperSV->setFrameShape(QFrame::NoFrame);
	helperSV->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	QVBox *big_box = new QVBox( helperSV->viewport() );
	helperSV->addChild( big_box );
	vbox->addWidget( helperSV );

	m_view = new IsotopeTableView( big_box, helperSV, "view" );
//	m_view->setMinimumSize( 800,800 );
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

		emit selectionDone( startPoint.unite( endPoint ) );
		return true;
	}
	KDialogBase::eventFilter(obj,ev);
	return false;
}

#include "nuclideboard.moc"

