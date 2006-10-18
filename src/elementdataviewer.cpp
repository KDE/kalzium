/***************************************************************************
    copyright            : (C) 2004, 2005, 2006  by Carsten Niehaus
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

#include <element.h>
#include <kplotaxis.h>
#include <kplotobject.h>

#include <klocale.h>
#include <kdebug.h>
#include <kactioncollection.h>
#include <kstdaction.h>
#include <ktoolinvocation.h>

//QT-Includes
#include <QKeyEvent>
#include <QCheckBox>
#include <QLabel>
#include <QPointF>
#include <QTimer>

#include "elementdataviewer.h"

AxisData::AxisData() : currentDataType(-1)
{
}

double AxisData::value( int element ) const
{
    if ( ( element < 1 ) || ( element > dataList.count() ) )
        return 0.0;

    return dataList[ element-1 ];
}


ElementDataViewer::ElementDataViewer( QWidget *parent )
  : KDialog( parent ),
    yData( new AxisData() )
{
	setCaption( i18n( "Plot Data" ) );
	setButtons( Help | Close );
	setDefaultButton( Close );

	KalziumDataObject *kdo  = KalziumDataObject::instance();

  ui.setupUi( mainWidget() );

	m_timer = new QTimer( this );
	m_timer->setSingleShot( true );

	// setup the list of names
  foreach (Element * e, kdo->ElementList) {
      names << e->dataAsString( ChemicalDataObject::name );
      symbols << e->dataAsString( ChemicalDataObject::symbol );
  }

	m_actionCollection = new KActionCollection (this );
	KStdAction::quit( this, SLOT( close() ), m_actionCollection );

	connect( m_timer, SIGNAL( timeout() ), 
          this, SLOT( drawPlot() ) );
	connect( ui.KCB_y, SIGNAL( activated(int) ),
	         this, SLOT( drawPlot()) );
	connect( ui.comboElementLabels, SIGNAL( activated( int ) ),
	         this, SLOT( drawPlot()) );
	connect( ui.from, SIGNAL( valueChanged( int ) ),
	         this, SLOT( rangeChanged() ) );
	connect( ui.to, SIGNAL( valueChanged( int ) ),
	         this, SLOT( rangeChanged() ) );

	drawPlot();

	resize( 650, 500 );
}

ElementDataViewer::~ElementDataViewer()
{
	delete yData;
}

void ElementDataViewer::slotHelp()
{
	KToolInvocation::invokeHelp( "plot_data", "kalzium" );
}

void ElementDataViewer::rangeChanged()
{
	m_timer->stop();
	m_timer->start( 500 );
}


void ElementDataViewer::setLimits(int f, int t)
{
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
    ui.plotwidget->setLimits( f - dx, t + dx, minY - dy, maxY + dy );
}

void ElementDataViewer::keyPressEvent(QKeyEvent *e)
{
    switch ( e->key() )
    {
        case Qt::Key_Plus:
        case Qt::Key_Equal:
            slotZoomIn();
            break;
        case Qt::Key_Minus:
        case Qt::Key_Underscore:
            slotZoomOut();
            break;
        case Qt::Key_Escape:
            close();
            break;
    }
}

void ElementDataViewer::slotZoomIn(){}
void ElementDataViewer::slotZoomOut(){}

void ElementDataViewer::setupAxisData()
{
    DoubleList l;

    const int selectedData = ui.KCB_y->currentIndex();

    //this should be somewhere else, eg in its own method
    yData->currentDataType = selectedData;

    KalziumDataObject *kdo = KalziumDataObject::instance();

    // init to _something_
    ChemicalDataObject::BlueObelisk kind = ChemicalDataObject::mass;
    QString caption;
    switch(selectedData)
    {
        case AxisData::MASS:
            {
                kind = ChemicalDataObject::mass;
                caption = i18n( "Atomic Mass [u]" );
                break;
            }
        case AxisData::MEANWEIGHT:
            {//TODO not sure if mass is ok here...
                kind = ChemicalDataObject::mass;
                caption = i18n( "Mean Mass [u]" );
                break;
            }
        case AxisData::DENSITY:
            {
                kind = ChemicalDataObject::density;
                caption = i18n( "Density" );
                break;
            }
        case AxisData::EN:
            {
                kind = ChemicalDataObject::electronegativityPauling;
                caption = i18n( "Electronegativity" );
                break;
            }
        case AxisData::MELTINGPOINT:
            {
                kind = ChemicalDataObject::meltingpoint;
                caption = i18n( "Melting Point [K]" );
                break;
            }
        case AxisData::BOILINGPOINT:
            {
                kind = ChemicalDataObject::boilingpoint;
                caption = i18n( "Boiling Point [K]" );
                break;
            }
        case AxisData::ATOMICRADIUS:
            {
                kind = ChemicalDataObject::radiusVDW;
                caption = i18n( "Atomic Radius [pm]" );
                break;
            }
        case AxisData::COVALENTRADIUS:
            {
                kind = ChemicalDataObject::radiusCovalent;
                caption = i18n( "Covalent Radius [pm]" );
                break;
            }
    }

    foreach (Element * element, kdo->ElementList) {
        double value = element->dataAsVariant( kind ).toDouble();
        l << ( value > 0.0 ? value : 0.0 );
    }

    yData->dataList.clear();
    yData->dataList << l;
    yData->kind = kind;

    ui.plotwidget->axis(KPlotWidget::LeftAxis)->setLabel( caption );
}

void ElementDataViewer::drawPlot()
{
    /*
     * to be 100% safe delete the old list
     */
    ui.plotwidget->clearObjectList();

    /*
     * spare the next step in case everything is already set and done
     */
    if( yData->currentDataType != ui.KCB_y->currentIndex() )
        initData();

    /*
     * if the user selected the elements 20 to 30 the list-values are 19 to 29!!!
     */
    const int tmpfrom = ui.from->value();
    const int tmpto = ui.to->value();
    const int from = qMin( tmpfrom, tmpto );
    const int to = qMax( tmpfrom, tmpto );

    /*
     * The number of elements. #20 to 30 are 30-20+1=11 Elements
     */
    const int num = to-from+1;

    setLimits(from,to);

    /*
     * check if the users wants to see the elementnames or not
     */
    int whatShow = ui.comboElementLabels->currentIndex();

    KPlotObject* dataPoint = 0;
    KPlotObject* dataPointLabel = 0;

    double av = 0.0;
    double max;
    double min = max = yData->value( from );

    KalziumDataObject *kdo = KalziumDataObject::instance();

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
//X 
            //dataPoint = new KPlotObject( names[i-1] + QLatin1String( "\n" ) + KalziumUtils::prettyUnit( kdo->element( i ), yData->kind ), Qt::blue, KPlotObject::POINTS, 4, KPlotObject::CIRCLE );
            dataPoint = new KPlotObject( "Test", 
                    Qt::blue, 
                    KPlotObject::POINTS, 
                    4, 
                    KPlotObject::CIRCLE );
            dataPoint->addPoint( new QPointF( (double)i, v ) );
            ui.plotwidget->addObject( dataPoint );

            if ( whatShow > 0 )
            {
                QString lbl = whatShow == 1 ? names[i-1] : symbols[i-1];
                dataPointLabel = new KPlotObject( lbl, Qt::red, KPlotObject::LABEL );
                dataPointLabel->addPoint( new QPointF( (double)i, v ) );
                ui.plotwidget->addObject( dataPointLabel );
            }
        }
    }

    //now set the values for the min, max and avarage value
    ui.aValue->setText( QString::number( av / num ) );
    ui.minimum->setText( QString::number( min ) );
    ui.maximum->setText( QString::number( max ) );
}

void ElementDataViewer::initData()
{
	setupAxisData();
}

#include "elementdataviewer.moc"
