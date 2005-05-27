/***************************************************************************
                             -------------------
    copyright            : (C) 2004 by Carsten Niehaus
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
#include "plotsetupwidget.h"
#include <klocale.h>
#include <knuminput.h>
#include <kcombobox.h>
#include <kdebug.h>

//QT-Includes
#include <qframe.h>
#include <qlayout.h>
#include <qcheckbox.h>

ElementDataViewer::ElementDataViewer( KalziumDataObject *data, QWidget *parent, const char* name )
	: KDialogBase( KDialogBase::Plain, 
			i18n( "Visualize Data") , 
			User1 | Close,
			User1,
			parent, name )
{
	kdDebug() << "ElementDataViewer" << endl;
	
	d = data;

	yData = new AxisData();
	
	QFrame *page = plainPage();
	QHBoxLayout *hlay = new QHBoxLayout(page, 0, spacingHint() );
	QVBoxLayout *vlay = new QVBoxLayout(hlay, 0 );
	m_pPlotSetupWidget = new PlotSetupWidget( page, "psw" );	
	m_pPlotWidget = new PlotWidget( 0.0,12.0,0.0,22.0, page, "plotwidget" );

	hlay->addWidget( m_pPlotSetupWidget );
	vlay->addWidget( m_pPlotWidget);

	/*
	 * setup the list of names
	 **/
	EList::iterator it = d->ElementList.begin();
	const EList::iterator itEnd = d->ElementList.end();
	for( ; it != itEnd ; ++it )
	{
		names.append( (*it)->elname() );
	}

	resize(500, 500);

	setButtonText( User1, i18n("&Plot") );

	connect ( m_pPlotSetupWidget->showNames, SIGNAL( toggled(bool)), this, SLOT(slotUser1()));
}

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

	int _currentVal = f;
	int _to = t;
	
	while( _currentVal < _to )
	{
		double v = yData->value( _currentVal );
		
		if( minY > v )
			minY = v;
		if( maxY < v)
			maxY = v;

		_currentVal++;
	}
	
	m_pPlotWidget->setLimits( (double)f, (double)t, minY, maxY );
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
		case AxisData::WEIGHT:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->weight();
				if( value != -1 )
				  l.append( value );
				else
				  l.append( 0.0 );
				m_pPlotWidget->setYAxisLabel(i18n("Atomic Weight [u]"));
			}
			break;
		case AxisData::MEANWEIGHT:
			for( ; it != itEnd ; ++it ) {
				double value =(*it)->meanweight();
				if( value != -1 )
				  l.append( value );
				else
				  l.append( 0.0 );
				m_pPlotWidget->setYAxisLabel(i18n("Mean Weight [u]"));
			}
			break;
		case AxisData::DENSITY:
			for( ; it != itEnd ; ++it ) {
				double value =(*it)->density();
				if( value != -1 )
				  l.append( value );
				else
				  l.append( 0.0 );
				m_pPlotWidget->setYAxisLabel(i18n("Density"));
			}
			break;
//X 		case AxisData::IE1:
//X 			for( ; itEnd ; ++it ) {
//X 				double value = (*it)->ie();
//X 				if( value != -1 )
//X 				  l.append( value );
//X 				else
//X 				  l.append( 0.0 );
//X 				m_pPlotWidget->setYAxisLabel(i18n("1. Ionization Energy"));
//X 			}
//X 			break;
//X 		case AxisData::IE2:
//X 			for( ; itEnd ; ++it ) {
//X 				double value = (*it)->ie2();
//X 				if( value != -1 )
//X 				  l.append( value );
//X 				else
//X 				  l.append( 0.0 );
//X 				m_pPlotWidget->setYAxisLabel(i18n("2. Ionization Energy"));
//X 			}
//X 			break;
		case AxisData::EN:
			for( ; it != itEnd; ++it ) {
				double value = (*it)->electroneg();
				if( value != -1 )
				  l.append( value );
				else
				  l.append( 0.0 );
				m_pPlotWidget->setYAxisLabel(i18n("Electronegativity"));
			}
			break;
		case AxisData::MELTINGPOINT:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->melting();
				if( value != -1 )
				  l.append( value );
				else
				  l.append( 0.0 );
				m_pPlotWidget->setYAxisLabel(i18n("Melting Point [K]"));
			}
			break;
		case AxisData::BOILINGPOINT:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->electroneg();
				if( value != -1 )
				  l.append( value );
				else
				  l.append( 0.0 );
				m_pPlotWidget->setYAxisLabel(i18n("Boiling Point [K]"));
			}
			break;
		case AxisData::ATOMICRADIUS:
			for( ; it != itEnd ; ++it ) {
				double value = (*it)->radius();
				if( value != -1 )
				  l.append( value );
				else
				  l.append( 0.0 );
				m_pPlotWidget->setYAxisLabel(i18n("Atomic Radius [pm]"));
			}
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
	int from = m_pPlotSetupWidget->from->value();
	int to = m_pPlotSetupWidget->to->value();
	kdDebug() << "from :" << from << endl;
	kdDebug() << "to :" << to << endl;
	
	/*
	 * The number of elements. #20 to 30 are 30-20+1=11 Elements
	 */
	int num = to-from+1;
	
	setLimits(from,to);

	/*
	 * check if the users wants to see the elementnames or not
	 */
	bool showNames = m_pPlotSetupWidget->showNames->isChecked();
	bool connectPoints = m_pPlotSetupWidget->connectPoints->isChecked();

	if ( connectPoints )
	{
		m_pPlotWidget->setConnection( true );
	}
	else
	{
		m_pPlotWidget->setConnection( false );
	}
		
	/*
	 * reserve the memory for the KPlotObjects
	 */
	//TODO QT4 replace QMemArray with QVector
	QMemArray<KPlotObject*> dataPoint(num);
	QMemArray<KPlotObject*> dataPointLabel(num);

	int number = 0;

	/*
	 * iterate for example from element 20 to 30 and contruct
	 * the KPlotObjects
	 */
	for( int i = from; i < to+1 ; i++ )
	{
		dataPoint[number] = new KPlotObject( "whocares", "Blue", KPlotObject::POINTS, 4, KPlotObject::CIRCLE );
		dataPoint[number]->addPoint( new DPoint( (double)i , yData->value( i ) ) );
		m_pPlotWidget->addObject( dataPoint[ number ] );

		if (showNames)
		{
		  dataPointLabel[number] = new KPlotObject( *(names.at(i)), "Red", KPlotObject::LABEL );
		  dataPointLabel[number]->addPoint( new DPoint( (double)i , yData->value( i ) ) );
		  m_pPlotWidget->addObject( dataPointLabel[number] );
		}
		
		number++;
	}
}

void ElementDataViewer::initData()
{
	setupAxisData();
}

///////////////////////////////////////////////7

AxisData::AxisData()
{}

#include "elementdataviewer.moc"
