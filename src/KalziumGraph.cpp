/***************************************************************************

                          KalziumGraph.cpp  -  description
                             -------------------
    begin                : Wed Oct 23 2002
    copyright            : (C) 2002 by Carsten Niehaus
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

#include "KalziumGraph.h"

//KDE-Includes
#include <kstddirs.h>
#include <ksimpleconfig.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kcombobox.h>
#include <kdebug.h>
#include <kconfig.h>
#include <klineedit.h>

//QT-Includes
#include <qdialog.h>
#include <qlayout.h>
#include <qframe.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qcanvas.h>
#include <qpainter.h>

#include <math.h>

KalziumGraphDialog::KalziumGraphDialog( QWidget *parent, const char *name) : KDialog( parent, name )
{
	this->setCaption( i18n( "Visualize Data" ));
	grid = new QGridLayout ( this, 5, 5 ,8, -1, "GraphLayout" );
	KPushButton *ok = new KPushButton( i18n( "Graph" ),this );
	QObject::connect(ok, SIGNAL( clicked() ), this, SLOT(slotokClicked() ));

	from = new KLineEdit("2", this );
	to = new KLineEdit("20", this );

	kcb = new KComboBox( this );
	kcb->insertItem( i18n( "Atomic Weight" ));
	kcb->insertItem( i18n( "Electronegativity" ) );
	kcb->insertItem( i18n( "Density" ) );
	kcb->insertItem( i18n( "Melting Point" ));
	kcb->insertItem( i18n( "Boiling Point" ) );
	kcb->insertItem( i18n( "Ionisation Energie" ) );
	kcb->insertItem( i18n( "Atomic Radius" ) );
	grid->addWidget( ok, 0,0 );
	grid->addWidget( kcb, 1,0 );
	grid->addWidget( from,0,1 );
	grid->addWidget( to,0,2 );
}

void KalziumGraphDialog::slotokClicked()
{
	int typ = kcb->currentItem();
	int fromRange =  from->text().toInt() ;
	int toRange =  to->text().toInt() ;

	KalziumGraphDataContainer *container = new KalziumGraphDataContainer( typ, fromRange,toRange );
 	graph = new KalziumGraph( fromRange,toRange,this, "graph" , container);
	graph->show();
 	
	grid->addMultiCellWidget( graph,2,3,0,2 );
}

KalziumGraph::KalziumGraph( int fromRange, int toRange,QWidget *parent, const char *name, KalziumGraphDataContainer *datacontainer) :
QFrame( parent, name ) 
{
	data = datacontainer;
	fromRange_ = fromRange;
	toRange_ = toRange;
}

void KalziumGraph::paintEvent( QPaintEvent * )
{
	int w_w = this->width();
	int w_h = this->height();
	int num = toRange_-fromRange_;
	int real_w=w_w/num;                       //real_w ist die Breite pro Datenpunkt

	
	QPainter DC;
	DC.begin( this );
	
//X 	int x1 = 0, y1=0, x2=0, y2=0;
	
	for( int i = 0 ; i < num ; i++ )
	{
//X 		x1 = real_w*i-1;
//X 		y1 = ( int )data->Data[ fromRange_+i-1 ];
//X 		x2 = real_w*i;
//X 		y2 = ( int )data->Data[ fromRange_+i ];
//X 
//X 		kdDebug() << "x1: " << x1 <<" y1: " << y1 <<"                     x2: " << x2 <<" y2: " << y2 << endl;
//X 	DC.drawLine( x1,y1,x2,y2 );

		double temp = getMax();

		double current = data->Data[ fromRange_+i ]/temp;
		current*=w_h;
		
		DC.drawPoint( real_w*i , w_h-( ( int )current ) );
	}
	DC.end();
}

double KalziumGraph::getMax()
{
	double temp = data->Data[ fromRange_ ];
	for( int i = fromRange_ ; i < toRange_ ; i++ )
	{
		if ( data->Data[ i ] > temp )
		{
			temp = data->Data[ i ];
		}
	}
	return temp;
}

KalziumGraphDataContainer::KalziumGraphDataContainer( int typ, int fromRange, int toRange )
{
	KSimpleConfig config (locate("data", "kalzium/kalziumrc"));

	//Weight 
	//EN 
	//MP == Meltingpoint 
	//BP == Boilingpoint 
	//Density 
	//IE == Ionizationenergie 
	//AR == atmomic radius 

	QString kind;

	switch ( typ ){
		case 0:
			kind = "Weight";
			break;
		case 1:
			kind = "EN";
			break;
		case 2:
			kind = "Density";
			break;
		case 3:
			kind = "MP";
			break;
		case 4:
			kind = "BP";
			break;
		case 5:
			kind = "IE";
			break;
		case 6:
			kind = "AR";
			break;
	}


	for( int i = fromRange; i < toRange ; i++ )
	{
		config.setGroup(QString::number( i ));
		Data[ i ]=config.readDoubleNumEntry( kind, -1 );
	}
}
