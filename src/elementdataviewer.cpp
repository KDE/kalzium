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

#include "elementdataviewer.h"

#include <element.h>
#include <kplotaxis.h>
#include <kplotobject.h>

#include <klocale.h>
#include <kdebug.h>
#include <kactioncollection.h>
#include <kstandardaction.h>
#include <ktoolinvocation.h>

//QT-Includes
#include <QKeyEvent>
#include <QPen>
#include <QTimer>

AxisData::AxisData( AXISTYPE type) : currentDataType(-1)
{
    m_type = type;
}

double AxisData::value( int element ) const
{
    if ( ( element < 1 ) || ( element > dataList.count() ) )
        return 0.0;

    return dataList[ element-1 ];
}


ElementDataViewer::ElementDataViewer( QWidget *parent )
  : KDialog( parent ),
    m_yData( new AxisData( AxisData::Y ) ) ,
    m_xData( new AxisData( AxisData::X ) )
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
	KStandardAction::quit( this, SLOT( close() ), m_actionCollection );

	connect( m_timer, SIGNAL( timeout() ), 
          this, SLOT( drawPlot() ) );
	connect( ui.KCB_y, SIGNAL( activated(int) ),
	         this, SLOT( rangeChanged()) );
	connect( ui.KCB_x, SIGNAL( activated(int) ),
	         this, SLOT( rangeChanged()) );
	connect( ui.comboElementLabels, SIGNAL( activated( int ) ),
	         this, SLOT( rangeChanged()) );
	connect( ui.from, SIGNAL( valueChanged( int ) ),
	         this, SLOT( rangeChanged() ) );
	connect( ui.to, SIGNAL( valueChanged( int ) ),
	         this, SLOT( rangeChanged() ) );
	connect( this, SIGNAL( helpClicked() ), this, SLOT( slotHelp() ) );
	drawPlot();

	resize( 650, 500 );
}

ElementDataViewer::~ElementDataViewer()
{
	delete m_yData;
	delete m_xData;
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


void ElementDataViewer::setLimits()
{
    kDebug() << "ElementDataViewer::setLimits()";

    double x1 = 0.0, x2 = 0.0, y1 = 0.0, y2 = 0.0;

    getMinMax(x1, x2, m_xData);
    getMinMax(y1, y2, m_yData);

    kDebug() << x1 << " :: " << x2 << " ----- "  << y1 << " :: " << y2;

		//JH: add some padding to show all points
		double dx = 0.05*(x2-x1);
		double dy = 0.05*(y2-y1);
		x1 -= dx;
		x2 += dx;
		y1 -= dy;
		y2 += dy;

//X     // try to put a small padding to make the points on the axis visible
//X     double dx = ( to - from + 1 ) / 25 + 1.0;
//X     double dy = ( maxY - minY ) / 10.0;
//X     // in case that dy is quite small (for example, when plotting a single
//X     // point)
//X     if ( dy < 1e-7 )
//X         dy = maxY / 10.0;
//X     ui.plotwidget->setLimits( from - dx, to + dx, minY - dy, maxY + dy );
    
    ui.plotwidget->setLimits( x1, x2, y1, y2 );
}

void ElementDataViewer::getMinMax(double& min, double& max, AxisData * data)
{
    int firstElement = ui.from->value();
    int lastElement  = ui.to->value();
    
    double minValue = data->value(firstElement);
    double maxValue = data->value(firstElement);

    kDebug() << "Taking elements from " << firstElement << " to " << lastElement;

    for ( int _currentVal = firstElement; _currentVal <= lastElement; _currentVal++ )
    {//go over all selected elements
        double v = data->value( _currentVal );

        if( minValue > v )
            minValue = v;
        if( maxValue < v)
            maxValue = v;
    }
    
    kDebug() << "The value are ]"<< minValue << " , " << maxValue << "[.";

    min = minValue;
    max = maxValue;
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

void ElementDataViewer::setupAxisData( AxisData * data )
{
    DoubleList l;

    int selectedData = 0;
    if ( data->type() == AxisData::X )
        selectedData = ui.KCB_x->currentIndex();
    else
        selectedData = ui.KCB_y->currentIndex();

    data->currentDataType = selectedData;

    // init to _something_
    ChemicalDataObject::BlueObelisk kind = ChemicalDataObject::mass;
    QString caption;
    switch(selectedData)
    {
        case AxisData::NUMBER:
            {
                kind = ChemicalDataObject::atomicNumber;
                caption = i18n( "Atomic Number" );
                break;
            }
        case AxisData::MASS:
            {
                kind = ChemicalDataObject::mass;
                caption = i18n( "Atomic Mass [u]" );
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
    KalziumDataObject *kdo = KalziumDataObject::instance();

    foreach (Element * element, kdo->ElementList) {
        double value = element->dataAsVariant( kind ).toDouble();
        l << ( value > 0.0 ? value : 0.0 );
    }

    data->dataList.clear();
    data->dataList << l;
    data->kind = kind;

    if ( data->type() == AxisData::X )
    {
        ui.plotwidget->axis(KPlotWidget::BottomAxis)->setLabel( caption );
    }
    else
    {
        ui.plotwidget->axis(KPlotWidget::LeftAxis)->setLabel( caption );
        ui.plotwidget->axis(KPlotWidget::RightAxis)->setLabel( caption );
    }
}

void ElementDataViewer::drawPlot()
{
    /*
     * to be 100% safe delete the old list
     */
    ui.plotwidget->removeAllPlotObjects();

    /*
     * spare the next step in case everything is already set and done
     */
    if( m_yData->currentDataType != ui.KCB_y->currentIndex() )
        initData();
    
    if( m_xData->currentDataType != ui.KCB_x->currentIndex() )
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
    int num = to-from+1;

    setLimits();

    /*
     * check if the users wants to see the elementnames or not
     */
    int whatShow = ui.comboElementLabels->currentIndex();

    KPlotObject* dataPointGreen = 0;
    KPlotObject* dataPointRed = 0;

    double av_x = 0.0;
    double max_x = m_xData->value(from);
    double min_x = m_xData->value(from);
    double av_y = 0.0;
    double max_y = m_yData->value(from);
    double min_y = m_yData->value(from);

    /*
     * iterate for example from element 20 to 30 and contruct
     * the KPlotObjects
     */
		dataPointGreen = new KPlotObject(
                Qt::green,
                KPlotObject::Points,
                4,
                KPlotObject::Star );
        dataPointGreen->setLabelPen( QPen( Qt::blue ) );

		dataPointRed = new KPlotObject(
                Qt::red,
                KPlotObject::Points,
                4,
                KPlotObject::Star );	//Star can be replaced with a cross
        dataPointRed->setLabelPen( QPen( Qt::blue ) );

    for( int i = from; i < to+1 ; i++ )
    {
        double value_y = m_yData->value( i );
        double value_x = m_xData->value( i );

		bool known = ((value_y)>0.0)  ?   1   :   0;

		if (known)
		{
            av_x += value_x;
            av_y += value_y;

            if (value_x > max_x) {
                max_x = value_x;
            }
            if (value_y > max_y) {
                max_y = value_y;
            }
            if (value_x < min_x) {
                min_x = value_x;
            }
            if (value_y < min_y) {
                min_y = value_y;
            }
    				
    				QString lbl;
            if ( whatShow > 0 )//The users wants to see the labels
            {
    					lbl = whatShow == 1 ? names[i-1] : symbols[i-1];
            }

            dataPointGreen->addPoint( value_x, value_y, lbl );
		}
		else//unknown value
		{
		//num is required while finding the average, if an element is not
		//known it should not contribute to the average. Thus num = num - 1
			num--;
					QString lbl;
			if ( whatShow > 0 )//The users wants to see the labels
	        {
						lbl = whatShow == 1 ? names[i-1] : symbols[i-1];
	        }

	        dataPointRed->addPoint( value_x, value_y, lbl );
			//For an Unknown value, use a red point to mark the data-point.
		}
    }

		ui.plotwidget->addPlotObject( dataPointGreen );
		ui.plotwidget->addPlotObject( dataPointRed );

	if( num > 0 )
	{
    //now set the values for the min, max and avarage value
    ui.av_x->setText( QString::number( av_x / num ) );
    ui.minimum_x->setText( QString::number( min_x ) );
    ui.maximum_x->setText( QString::number( max_x ) );
    ui.av_y->setText( QString::number( av_y / num ) );
    ui.minimum_y->setText( QString::number( min_y ) );
    ui.maximum_y->setText( QString::number( max_y ) );
	}
	
	else
	{
	ui.av_x->setText( QString::number( 0.0 ) );
    ui.minimum_x->setText( QString::number( 0.0 ) );
    ui.maximum_x->setText( QString::number( 0.0 ) );
    ui.av_y->setText( QString::number( 0.0 ) );
    ui.minimum_y->setText( QString::number( 0.0 ) );
    ui.maximum_y->setText( QString::number( 0.0 ) );
	}
}

void ElementDataViewer::initData()
{
	setupAxisData(m_xData);
	setupAxisData(m_yData);
}

#include "elementdataviewer.moc"
