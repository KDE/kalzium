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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "kalziumdataobject.h"
#include "nuclideboard.h"

#include <qlabel.h>
#include <qlayout.h>
#include <qspinbox.h>
#include <qcursor.h>
#include <qstring.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qwidget.h>
#include <qmainwindow.h>
#include <qvbox.h>

#include <kdebug.h>
#include <klocale.h>
#include <kapplication.h>

#include "math.h"
	
int IsotopeTableView::m_maxIsoSize = 30;
int IsotopeTableView::m_minIsoSize = 20;

IsotopeTableView::IsotopeTableView( QWidget* parent, const char* name )
	: QWidget( parent, name )
{
	setBackgroundMode( NoBackground );
	m_duringSelection = false;

	m_list = KalziumDataObject::instance()->ElementList;
	m_highestNumberOfNeutrons = highestNeutronCount();
	m_lowestNumberOfNeutrons = lowestNeutronCount();

	m_startElementIterator = m_list.begin();
	m_stopElementIterator = m_list.end();

	m_topLeft = QPoint( 0, 0 );
	m_bottomRight = QPoint( 80, 30 );

	QTimer::singleShot( 50, this, SLOT( updateIsoptopeRectList() ) );
}

/**
 * in this method everything beside the drawing is correct.
 * For some reasons the QRect is not drawn, the dimentions
 * of it are absolutly correct
 */
void IsotopeTableView::paintEvent( QPaintEvent* /* e */ )
{
	QPixmap pm( width(), height() );
	pm.fill( Qt::black );

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

void IsotopeTableView::selectionDone( QRect selectedRect )
{
	m_selectedRegion = selectedRect;

	QRect tempRect = QRect( m_selectedRegion.x()/m_rectSize, 
			m_selectedRegion.y()/m_rectSize, 
			m_selectedRegion.width()/m_rectSize, 
			m_selectedRegion.height()/m_rectSize );

//	kdDebug() << tempRect << " ..::.. " << selectedRect << endl;

	//get the article which is selected
	QRect intersection = QRect( m_topLeft, m_bottomRight ).intersect( tempRect );

//	kdDebug( ) << "now the final rect: " << intersection << endl;

	m_topLeft = QPoint( intersection.x(), intersection.y() );
	m_bottomRight = QPoint( intersection.x()+intersection.width(),
				   		    intersection.y()+intersection.height());

	kdDebug() << m_topLeft << " ;; " << m_bottomRight << endl;
	
	updateIsoptopeRectList();
	update();
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
	QMap<IsotopeAdapter, QRect>::ConstIterator it = m_IsotopeAdapterRectMap.end();
	const QMap<IsotopeAdapter, QRect>::ConstIterator itEnd = m_IsotopeAdapterRectMap.begin();

	for ( ; it != itEnd ; --it )
	{
		Isotope* i = it.key().m_isotope;

		if ( i )
		{
			QColor color( isotopeColor( i ) ) ;
			p->setBrush( color );
			p->drawRect( it.data() );
		}
	}
	p->setBrush( Qt::black );
}

void IsotopeTableView::updateIsoptopeRectList()
{
	m_IsotopeAdapterRectMap.clear();

	/*
	 * this code will go from top left to bottom right.
	 * i = y-value
	 * j = x-value
	 */
			
	//calculate the number of shown elements
	const int numOfElements = m_bottomRight.y() - m_topLeft.y();
			
	double tmp1 = this->height()/m_bottomRight.y();
	double tmp2 = this->width()/m_bottomRight.x();

	//take the better size
	m_rectSize = (int) kMin( tmp1, tmp2 );

	for (int i = m_topLeft.y(); i < m_bottomRight.y(); ++i )
	{//first, the elements
		QValueList<Element*>::ConstIterator it = m_list.at( i );

		for (int j = m_topLeft.x(); j < m_bottomRight.x(); ++j )
		{//second, the isotopes
			//the istopes will be drawn from bottomleft to
			//topright. To do this I need to "flip" the
			//widget horizontally. The last line is always 
			//"numbOfElement". The first element is thus
			//numbOfElement - 0, the second numbOfElement - 1
			int realYValue = numOfElements - i;

			IsotopeAdapter adapter;

			adapter.m_point = QPoint( j, realYValue );
			if ( it != m_list.end() )
				adapter.m_isotope = ( *it )->isotopeByNucleons( j );
			else
				adapter.m_isotope = 0;

			QRect boundingRect = QRect( j*m_rectSize, 
					realYValue*m_rectSize, 
					m_rectSize, m_rectSize );

			m_IsotopeAdapterRectMap.insert(adapter, boundingRect);
		}
	}
}

int IsotopeTableView::highestNeutronCount()
{
	QValueList<Element*>::const_iterator it = m_startElementIterator;
	const QValueList<Element*>::const_iterator itEnd = m_stopElementIterator;

	IsotopeList isotopeList;
	IsotopeList::const_iterator isotope;
	IsotopeList::const_iterator isotopeEnd;

	int count = 0;

	for ( ; it != itEnd; ++it )
	{
		isotopeList = ( *it )->isotopes();

		if ( isotopeList.empty() )
			continue;

		isotopeEnd = isotopeList.end();
	
		for ( isotope = isotopeList.begin(); isotope != isotopeEnd; ++isotope )
		{
			if ( count < ( *isotope )->neutrons() )	
				count = ( *isotope )->neutrons();
		}
	}

	return count;
}

int IsotopeTableView::lowestNeutronCount()
{
	QValueList<Element*>::const_iterator it = m_list.begin();
	const QValueList<Element*>::const_iterator itEnd = m_list.end();

	IsotopeList isotopeList;
	IsotopeList::const_iterator isotope;
	IsotopeList::const_iterator isotopeEnd;

	int count = ( *it )->isotopes().first()->neutrons();
	
	for ( ; it != itEnd; ++it )
	{
		isotopeList = ( *it )->isotopes();
		if ( isotopeList.empty() )
			continue;

		isotopeEnd = isotopeList.end();

		for ( isotope = isotopeList.begin(); isotope != isotopeEnd; ++isotope )
		{
			if ( count > ( *isotope )->neutrons() )	
				count = ( *isotope )->neutrons();
		}
	}

	return count;
}

NuclideLegend::NuclideLegend( QWidget* parent, const char* name ) : QWidget( parent, name )
{
	setMinimumWidth( 300 );
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
	: KDialogBase(parent, name, true, i18n( "Isotope Table" ),
			KDialogBase::Apply|KDialogBase::Close|KDialogBase::Help, KDialogBase::Apply, true )
{
	QWidget *page = new QWidget( this );
	setMainWidget( page );
	
	QVBoxLayout *vbox = new QVBoxLayout(  page , 0, -1, "vbox" );
	

	QScrollView *helperSV = new QScrollView(page);
	QVBox *big_box = new QVBox( helperSV->viewport() );
	helperSV->addChild( big_box );
	vbox->addWidget( helperSV );
	
	m_view = new IsotopeTableView( big_box, "view" );

	connect( this, SIGNAL(selectionDone( QRect ) ), 
			m_view, SLOT(selectionDone( QRect ) ) );
	
	helperSV->viewport()->setPaletteBackgroundColor(paletteBackgroundColor());  
	helperSV->setFrameShape(QFrame::NoFrame);

	m_view->setMinimumSize( 800,800 );
	m_view->installEventFilter( this );
	
	setMinimumSize( 1000, 550 );
	resize( minimumSize() );

	update();
}

void IsotopeTableDialog::slotHelp()
{
	emit helpClicked();
	if ( kapp )
		kapp->invokeHelp ( "isotope_table", "kalzium" );
}

bool IsotopeTableDialog::eventFilter( QObject *obj, QEvent *ev )
{
	if (ev->type() == QEvent::MouseButtonPress )	
	{
		QMouseEvent *mev = (QMouseEvent*) ( ev );

		m_view->m_firstPoint = mev->pos();
		m_view->m_duringSelection = true;
		return true;
	}
	if (ev->type() == QEvent::MouseMove )	
	{
		QMouseEvent *mev = (QMouseEvent*) ( ev );

		QRect startPoint( m_view->m_firstPoint, m_view->m_firstPoint );
		QRect endPoint( mev->pos(), mev->pos() );

		m_view->m_selectedRegion = startPoint.unite( endPoint );
		m_view->update();
		return true;
	}
	if (ev->type() == QEvent::MouseButtonRelease )	
	{
		QMouseEvent *mev = (QMouseEvent*) ( ev );
		
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

