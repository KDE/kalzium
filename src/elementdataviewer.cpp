/***************************************************************************
    copyright            : (C) 2004, 2005  by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "elementdataviewer.h"
#include "element.h"
#include "plotsetupwidget.h"
#include "plotwidget.h"
#include "kalziumdataobject.h"

#include <klocale.h>
#include <knuminput.h>
#include <kcombobox.h>
#include <kdebug.h>
#include <kactioncollection.h>
#include <kapplication.h>

//QT-Includes
#include <qlayout.h>
#include <qcheckbox.h>
#include <qlabel.h>

ElementDataViewer::ElementDataViewer( QWidget *parent, const char* name )
	: KDialogBase( KDialogBase::Plain, 
			i18n( "Plot Data") , 
			Help | User1 | Close,
			User1,
			parent, name )
{
	d = KalziumDataObject::instance();

	yData = new AxisData();
	
	QHBoxLayout *layout = new QHBoxLayout(plainPage(), 0, KDialog::spacingHint() );

	m_pPlotSetupWidget = new PlotSetupWidget( plainPage(), "plotsetup" );	
	m_pPlotSetupWidget->from->setMaxValue( d->numberOfElements() - 1 );
	m_pPlotSetupWidget->to->setMaxValue( d->numberOfElements() );
	m_pPlotWidget = new PlotWidget( 0.0, 12.0 ,0.0 ,22.0, plainPage(), "plotwidget" );
	m_pPlotWidget->setYAxisLabel(" ");
	m_pPlotWidget->setMinimumWidth( 200 );
	m_pPlotWidget->resize( 400, m_pPlotWidget->height() );

	layout->addWidget( m_pPlotSetupWidget );
	layout->addWidget( m_pPlotWidget );
	layout->setStretchFactor( m_pPlotSetupWidget, 0 );
	layout->setStretchFactor( m_pPlotWidget, 1 );

	// setup the list of names
	EList::iterator it = d->ElementList.begin();
	const EList::iterator itEnd = d->ElementList.end();
	for( ; it != itEnd ; ++it )
	{
		names.append( (*it)->elname() );
	}

	resize(500, 500);

	setButtonText( User1, i18n("&Plot") );

	m_actionCollection = new KActionCollection(this);
	KStdAction::quit(this, SLOT(slotClose()), m_actionCollection);	

	connect ( m_pPlotSetupWidget->KCB_y,         SIGNAL( activated(int) ),
			  this,                              SLOT( drawPlot()) );

	connect ( m_pPlotSetupWidget->connectPoints, SIGNAL( toggled(bool) ),
			  this,                              SLOT( drawPlot()) );
	connect ( m_pPlotSetupWidget->showNames,     SIGNAL( toggled(bool) ),
			  this,                              SLOT( drawPlot()) );

	// Draw the plot so that the user doesn't have to press the "Plot"
	// button to seee anything.
	drawPlot();
}

void ElementDataViewer::slotHelp()
{
	emit helpClicked();
	if ( kapp )
		kapp->invokeHelp ( "plot_data", "kalzium" );
}

// Reimplement slotUser1 from KDialogBase

void ElementDataViewer::slotUser1()
{
	kdDebug() << "slotUser1" << endl;

	drawPlot();
}


void ElementDataViewer::setLimits(int f, int t)
{
	kdDebug() << "setLimits()" << endl;
	
	double minY = yData->value(f);
	double maxY = yData->value(f);

	for ( int _currentVal = f; _currentVal <= t; _currentVal++ )
	{
		double v = yData->value( _currentVal );
		
		if( minY > v )
			minY = v;
		if( maxY < v)
			maxY = v;
	}
	
	// try to put a small padding to make the points on the axis visible
	double dx = ( t - f + 1 ) / 25 + 1.0;
	double dy = ( maxY - minY ) / 10.0;
	// in case that dy is quite small (for example, when plotting a single
	// point)
	if ( dy < 1e-7 )
		dy = maxY / 10.0;
	m_pPlotWidget->setLimits( f - dx, t + dx, minY - dy, maxY + dy );
}

void ElementDataViewer::paintEvent(QPaintEvent*)
{
	m_pPlotWidget->update();
}

void ElementDataViewer::keyPressEvent(QKeyEvent *e)
{
	switch ( e->key() )
	{
		case Key_Plus:
		case Key_Equal:
			slotZoomIn();
			break;
		case Key_Minus:
		case Key_Underscore:
			slotZoomOut();
			break;
		case Key_Escape:
			close();
			break;
	}
}

void ElementDataViewer::slotZoomIn(){}
void ElementDataViewer::slotZoomOut(){}

void ElementDataViewer::setupAxisData()
{
	DoubleList l;

	const int selectedData = m_pPlotSetupWidget->KCB_y->currentItem();

	//this should be somewhere else, eg in its own method
	yData->m_currentDataType = selectedData;

	EList::iterator it = d->ElementList.begin();
	const EList::iterator itEnd = d->ElementList.end();
	switch(selectedData)
	{
		case AxisData::MASS:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->mass();
				if( value > 0.0 )
				  l.append( value );
				else
				  l.append( 0.0 );
			}
			m_pPlotWidget->setYAxisLabel(i18n("Atomic Mass [u]"));
			break;
		case AxisData::MEANWEIGHT:
			for( ; it != itEnd ; ++it ) {
				double value =(*it)->meanmass();
				if( value > 0.0 )
				  l.append( value );
				else
				  l.append( 0.0 );
			}
			m_pPlotWidget->setYAxisLabel(i18n("Mean Mass [u]"));
			break;
		case AxisData::DENSITY:
			for( ; it != itEnd ; ++it ) {
				double value =(*it)->density();
				if( value > 0.0 )
				  l.append( value );
				else
				  l.append( 0.0 );
			}
			m_pPlotWidget->setYAxisLabel(i18n("Density"));
			break;
		case AxisData::EN:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->electroneg();
				if( value > 0.0 )
				  l.append( value );
				else
				  l.append( 0.0 );
			}
			m_pPlotWidget->setYAxisLabel(i18n("Electronegativity"));
			break;
		case AxisData::MELTINGPOINT:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->melting();
				if( value > 0.0 )
				  l.append( value );
				else
				  l.append( 0.0 );
			}
			m_pPlotWidget->setYAxisLabel(i18n("Melting Point [K]"));
			break;
		case AxisData::BOILINGPOINT:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->boiling();
				if( value > 0.0 )
				  l.append( value );
				else
				  l.append( 0.0 );
			}
			m_pPlotWidget->setYAxisLabel(i18n("Boiling Point [K]"));
			break;
		case AxisData::ATOMICRADIUS:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->radius( Element::ATOMIC );
				if( value > 0.0 )
				  l.append( value );
				else
				  l.append( 0.0 );
			}
			m_pPlotWidget->setYAxisLabel(i18n("Atomic Radius [pm]"));
			break;
		case AxisData::COVALENTRADIUS:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->radius( Element::COVALENT );
				if( value > 0.0 )
				  l.append( value );
				else
				  l.append( 0.0 );
			}
			m_pPlotWidget->setYAxisLabel(i18n("Covalent Radius [pm]"));
			break;
	}

	yData->setDataList( l );
}

void ElementDataViewer::drawPlot()
{
	kdDebug() << "drawPlot()" << endl;

	/*
	 * to be 100% safe delete the old list
	 */
	m_pPlotWidget->clearObjectList();

	/*
	 * spare the next step in case everything is already set and done
	 */
	if( yData->currentDataType() != m_pPlotSetupWidget->KCB_y->currentItem() )
		initData();

	/*
	 * if the user selected the elements 20 to 30 the list-values are 19 to 29!!!
	 */
	const int from = m_pPlotSetupWidget->from->value();
	const int to = m_pPlotSetupWidget->to->value();
	
	/*
	 * The number of elements. #20 to 30 are 30-20+1=11 Elements
	 */
	const int num = to-from+1;
	
	setLimits(from,to);

	/*
	 * check if the users wants to see the elementnames or not
	 */
	bool showNames = m_pPlotSetupWidget->showNames->isChecked();
	bool connectPoints = m_pPlotSetupWidget->connectPoints->isChecked();

	if ( connectPoints )
		m_pPlotWidget->setConnection( true );
	else
		m_pPlotWidget->setConnection( false );
		
	/*
	 * reserve the memory for the KPlotObjects
	 */
	//TODO QT4 replace QMemArray with QVector
	QMemArray<KPlotObject*> dataPoint(num);
	QMemArray<KPlotObject*> dataPointLabel(num);

	int number = 0;

	double max = 0.0, av = 0.0;
	double min = yData->value( 1 );

	/*
	 * iterate for example from element 20 to 30 and contruct
	 * the KPlotObjects
	 */
	for( int i = from; i < to+1 ; i++ )
	{
		double v = yData->value( i );

		if ( v >= 0.0 )
		{
			if ( v < min )
				min = v;
			if ( v > max )
				max = v;
			av += v;

			dataPoint[number] = new KPlotObject( "whocares", "Blue", KPlotObject::POINTS, 4, KPlotObject::CIRCLE );
			dataPoint[number]->addPoint( new DPoint( (double)i , v ) );
			m_pPlotWidget->addObject( dataPoint[ number ] );

			if (showNames)
			{
				dataPointLabel[number] = new KPlotObject( *(names.at(i-1)), "Red", KPlotObject::LABEL );
				dataPointLabel[number]->addPoint( new DPoint( (double)i , yData->value( i ) ) );
				m_pPlotWidget->addObject( dataPointLabel[number] );
			}
		}
		number++;
	}

	//now set the values for the min, max and avarage value
	m_pPlotSetupWidget->aValue->setText( QString::number( av/number ) );
	m_pPlotSetupWidget->minValue->setText( QString::number( min ) );
	m_pPlotSetupWidget->maxValue->setText( QString::number( max ) );
}

void ElementDataViewer::initData()
{
	setupAxisData();
}

///////////////////////////////////////////////

AxisData::AxisData()
{}

#include "elementdataviewer.moc"
