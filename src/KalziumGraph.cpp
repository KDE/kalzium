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
#include <kconfig.h>
#include <kstddirs.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kcombobox.h>

//QT-Includes
#include <qdialog.h>
#include <qlayout.h>
#include <qframe.h>
#include <qwidget.h>
#include <qpushbutton.h>
#include <qhbox.h>

KalziumGraphDialog::KalziumGraphDialog( QWidget *parent, const char *name) : KDialog( parent, name )
{
	QHBox *hbox = new QHBox( this );
	KPushButton *ok = new KPushButton( hbox );
	kcb = new KComboBox( hbox );
	QString test = "foo";
//X 	kcb->addItem( test  );
//X 	kcb->addItem( test  );
	QObject::connect(ok, SIGNAL( clicked() ), this, SLOT(slotokClicked() ));
}

void KalziumGraphDialog::slotokClicked()
{
	int typ = kcb->currentText().toInt();
	KalziumGraphDisplay *graph = new KalziumGraphDisplay( typ, 1,10, this, "graph" );
}


KalziumGraphDisplay::KalziumGraphDisplay( int typ, int fromRange, int toRange , QWidget *parent, const char *name ):KDialog( parent, name )
{
	KalziumGraphDataContainer *container = new KalziumGraphDataContainer( typ, fromRange, toRange );
	KalziumGraph *graph = new KalziumGraph( this, "graph" , container);
}

KalziumGraph::KalziumGraph( QWidget *parent, const char *name, KalziumGraphDataContainer *data) :
QWidget( parent, name ) 
{

}

KalziumGraphDataContainer::KalziumGraphDataContainer( int typ, int fromRange, int toRange )
{
}
