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
#include "KalziumGraph.moc"

//KDE-Includes
#include <kstandarddirs.h>
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
	grid = new QGridLayout ( this, 3, 4 ,8, -1, "GraphLayout" );
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
	kcb->insertItem( i18n( "Ionization Energy" ) );
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
	QPainter DC;
	DC.begin( this );

	int w_w = this->width()-10;         //this is the width of the widget in which the 
										//drawing happens
	
	int w_h = this->height();           //this is the height of the widget in which the
                                        //drawing happens
	
	int num = toRange_-fromRange_;      //the number of datapoints
	
	int real_w=w_w/num;                 //real_w is the width per datapoint

	DC.drawRect(10,10,w_w,w_h-10);
		
	double max = getMax();              //the maximum value of the drawed data

	double min = getMin();              //the minimum value of the drawed data

	for( int i = 0 ; i < num ; i++ )
	{
		double current = data->Data[ fromRange_+i ]/max;
		                                //at this point of time current is value of the data-
										//point but fitted into the widgetsize

		current*=w_h;                   //current is now the y-coordinate.
		
		int x=0, y=0, x_old=0, y_old=0;
		
		x=real_w*i+10;                  //x is now the correct x-coordinate
		
		y=w_h-( ( int )current )+10;    //y is now the correct y-coordinate

		x_old=real_w*( i-1 )+10;        //x_old is the x-coordinate for (i-1)
		
		if( i != 0 ) 
		{
			int temp = i; temp--;
      double old = (data->Data[ fromRange_+temp ]/max) * w_h;
      kdDebug () <<old;
      y_old = w_h- ((int) old) + 10;
			kdDebug() << y_old << endl;
		}

		DC.drawEllipse( x , y , 4 , 4 );//draw the datapoints
		

		if( i != 0 )
		{                               //draw the lines
			DC.drawLine( x_old,y_old,x,y );
		}
		
//X 		kdDebug() << "akt, y " << i << " " << y  << endl;
//X 		kdDebug() << "akt, x " << i << " " << x << endl;
//X 		kdDebug() << "alt, y " << i << " " << y_old << endl;
//X 		kdDebug() << "alt, x " << i << " " << x_old << endl;

	}

	DC.drawLine(10,this->height()/2,20,this->height()/2);
	DC.drawLine(10,this->height()/4,20,this->height()/4);
	DC.drawLine(10,this->height()/4*3,20,this->height()/4*3);
	
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

double KalziumGraph::getMin()
{
	double temp = data->Data[ fromRange_ ];
	for( int i = fromRange_ ; i < toRange_ ; i++ )
	{
		if ( data->Data[ i ] < temp )
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
