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
	kcb->insertItem( i18n( "Density" ) );
	kcb->insertItem( i18n( "Melting Point" ));
	kcb->insertItem( i18n( "Boiling Point" ) );
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
	KalziumGraph *graph = new KalziumGraph( this, "graph" , container);
}

KalziumGraph::KalziumGraph( QWidget *parent, const char *name, KalziumGraphDataContainer *datacontainer) :
QWidget( parent, name ) 
{
	KalziumGraphDataContainer *data = datacontainer;
	QGridLayout *grid2 = new QGridLayout ( this, 6, 3 ,8, -1, "GraphLayout" );
	QLabel *one = new QLabel ( this );
	QLabel *two = new QLabel ( this );
	QLabel *three = new QLabel ( this );
	QLabel *four = new QLabel ( this );
	QLabel *five = new QLabel ( this );
	one->setText( "one" );
	two->setText( "two" );
	three->setText( "three" );
	four->setText( "four" );
	five->setText( "five" );
	grid2->addWidget( one ,1,0);
	grid2->addWidget( two ,2,0);
	grid2->addWidget( three ,3,0);
	grid2->addWidget( four ,4,0);
	grid2->addWidget( five ,5,0);

	for( int i = 0 ; i < 10 ; i++ )
	{
		kdDebug() << data->Data[ i ] << endl;
	}
}

KalziumGraphDataContainer::KalziumGraphDataContainer( int typ, int fromRange, int toRange )
{
	//for testing: only weight
    KSimpleConfig config (locate("data", "kalzium/kalziumrc"));
	for( int i = 0; i < 10 ; i++ )
	{
		config.setGroup(QString::number( i ));
		Data[ i ]=config.readEntry( "Weight", "0.0" );
	}
}
