
/***************************************************************************
    copyright            : (C) 2006 by Carsten Niehaus
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

#include "maindialog_impl.h"
#include "simulation.h"
#include "simulationsdialog.h"

#include <QApplication>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>

MainDialog_Impl::MainDialog_Impl( QWidget * parent )
	: QDialog( parent )
{
	ui.setupUi( this );

	QStringList l = SimulationsFactory::instance()->simulations();
	foreach( QString s, l )
	{
		new QListWidgetItem( s, ui.simulationList );
	}

	connect( ui.startButton, SIGNAL(clicked()),
	         this, SLOT( startSelectedSimulation() ) );
	connect( ui.quitButton, SIGNAL(clicked()),
	         qApp, SLOT( quit() ) );
	connect( ui.simulationList, SIGNAL( currentRowChanged( int ) ),
	         this, SLOT( listRowChanged( int ) ) );
}

void MainDialog_Impl::startSelectedSimulation()
{
	Simulation *sim = SimulationsFactory::instance()->build( ui.simulationList->currentRow() );
	if ( sim )
	{
		GamesDialog *dlg = new GamesDialog( sim );
		dlg->exec();
	}
}

void MainDialog_Impl::listRowChanged( int newCurrent )
{
	Simulation *sim = SimulationsFactory::instance()->build( newCurrent );
	ui.labelDescription->setText( sim ? sim->rules() : QString() );
}

#include "moc_maindialog_impl.cpp"
