/***************************************************************************
 *   Copyright (C) 2005      by Carsten Niehaus,    cniehaus@kde.org       *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.          *
 ***************************************************************************/

#include "gamesimplementation.h"
#include "gamecontrols_impl.h"
#include "simulationsdialog.h"
#include "simulation.h"
#include "simulationfield.h"
#include "field.h"

#include <QLayout>
#include <QLCDNumber>

#include <iostream>

GamesDialog::GamesDialog( Simulation * sim )
	: QDialog( 0 )
{
	m_simulation = sim;
	m_controls = new GameControls_Impl( this );
	
	setWindowTitle( "Kalzium Simulations -- " + m_simulation->description() );

	if ( sim )
	{
		setupWidgets();
		createConnetions();
	}
	
	connect(m_controls->ui.start, SIGNAL( clicked() ), 
			this, SLOT(startSimulation()) );
}

void GamesDialog::setupWidgets()
{
 	statsWidget = m_simulation->statisticsWidget();

	int x = m_controls->ui.xsize->value();
	int y = m_controls->ui.ysize->value();
	m_simulation->field()->setFieldXSize( x );
	m_simulation->field()->setFieldYSize( y );
	
	m_controls->ui.gf->setField( m_simulation->field() );
	m_controls->ui.gf->update();

	resize( sizeHint() );

	vbox = new QVBoxLayout( this );

	m_controls->ui.gf->setField( 0 );

	vbox->addWidget( m_controls );
	vbox->addWidget( statsWidget );

	m_controls->ui.captionLabel->setText( m_simulation->description() );
}

void GamesDialog::slotStartWithTimer()
{
	m_simulation->startWithTimer( m_controls->ui.time->value() );
}

void GamesDialog::startSimulation()
{
	slotStartWithTimer();
}

void GamesDialog::createConnetions()
{
	connect(m_controls->ui.next, SIGNAL( clicked() ), 
			m_simulation, SLOT(slotNextMove()) );
	connect(m_controls->ui.stop, SIGNAL( clicked() ), 
			m_simulation, SLOT(stopSimulation()) );
	connect(m_simulation, SIGNAL( turnOver(Move*) ), 
			m_controls->ui.gf, SLOT(slotUpdate(Move*)) );
	connect(m_simulation, SIGNAL( turnOver() ), 
			this, SLOT(calculateStatistics() ) );
}

void GamesDialog::calculateStatistics()
{
	if ( !m_simulation ) return;

	m_controls->ui.lcd->display( m_simulation->numberOfMoves() );	
}

void GamesDialog::displayStatistics()
{
}

#include "moc_simulationsdialog.cpp"
