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

//QT-Includes
#include <qdialog.h>
#include <qlayout.h>
#include <qframe.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qhbox.h>
#include <qlabel.h>

KalziumGraphDialog::KalziumGraphDialog( QWidget *parent, const char *name) : KDialog( parent, name )
{
	this->setCaption( i18n( "Visualize Data" ));
	grid = new QGridLayout ( this, 4, 2 ,8, -1, "GraphLayout" );
	KPushButton *ok = new KPushButton( i18n( "Graph" ),this );
	kcb = new KComboBox( this );
	kcb->insertItem( i18n( "Atomic Weight" ));
	kcb->insertItem( i18n( "Electronegativity" ) );
	kcb->insertItem( i18n( "Density" ) );
	kcb->insertItem( i18n( "Melting Point" ));
	kcb->insertItem( i18n( "Boiling Point" ) );
	kcb->insertItem( i18n( "Ionisation Energie" ) );
	kcb->insertItem( i18n( "Atomic Radius" ) );
	QObject::connect(ok, SIGNAL( clicked() ), this, SLOT(slotokClicked() ));
	grid->addWidget( ok, 0,0 );
	grid->addWidget( kcb, 1,0 );
}

void KalziumGraphDialog::slotokClicked()
{
	int typ = kcb->currentItem();
	//for testing: only graph the first 10 Elements
	KalziumGraphDisplay *graph = new KalziumGraphDisplay( typ, 1,10, this, "graph" );
	graph->show();
	grid->addWidget( graph, 3 , 1 );
}


KalziumGraphDisplay::KalziumGraphDisplay( int typ, int fromRange, int toRange , QWidget *parent, const char *name ):QWidget( parent, name )
{
	KalziumGraphDataContainer *container = new KalziumGraphDataContainer( typ, fromRange, toRange );
	KalziumGraph *graph = new KalziumGraph( fromRange,toRange,this, "graph" , container);
}

KalziumGraph::KalziumGraph( int fromRange, int toRange,QWidget *parent, const char *name, KalziumGraphDataContainer *datacontainer) :
QWidget( parent, name ) 
{
	KalziumGraphDataContainer *data = datacontainer;
	QGridLayout *grid2 = new QGridLayout ( this, 6, 3 ,8, -1, "GraphLayout" );

	for( int i = fromRange ; i < toRange ; i++ )
	{
		kdDebug() << data->Data[ i ] << endl;
	}
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
