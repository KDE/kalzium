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
#include <QComboBox>

#include <iostream>

GamesDialog::GamesDialog()
	: QDialog( 0 )
{
	setWindowTitle( "Kalzium Simulations" );

	statsWidget = 0;

	vbox = new QVBoxLayout( this );

	m_controls = new GameControls_Impl( this );
	m_controls->ui.gf->setField( 0 );

	vbox->addWidget( m_controls );

	QStringList l = GamesFactory::instance()->games();
	foreach( QString s, l ){
		m_controls->ui.combo->addItem(s);
	}
	
	connect(m_controls->ui.start, SIGNAL( clicked() ), 
			this, SLOT(startTheCurrentGame()) );
}

void GamesDialog::activateGame( int nr )
{
	//better safe than sorry
	m_controls->ui.gf->setField( 0 );
	
	Simulation * g = GamesFactory::instance()->build( nr );

	if ( !g ) return;

	m_game = g;

	// removing the old widget
	vbox->removeWidget( statsWidget );
	// getting the new one and adding it to the dialog
	statsWidget = m_game->statisticsWidget();
	vbox->addWidget( statsWidget );
	
	int x = m_controls->ui.xsize->value();
	int y = m_controls->ui.ysize->value();
	
	m_game->field()->setFieldXSize( x );
	m_game->field()->setFieldYSize( y );
	
	m_controls->ui.gf->setField( m_game->field() );
	m_controls->ui.label->setPlainText(m_game->rules());

	m_controls->ui.gf->update();

	resize( sizeHint() );

	createConnetions();
}

void GamesDialog::slotStartWithTimer()
{
	m_game->startWithTimer( m_controls->ui.time->value() );
}

void GamesDialog::startTheCurrentGame()
{
	int selection = m_controls->ui.combo->currentIndex();

	activateGame( selection );

	slotStartWithTimer();
}

void GamesDialog::createConnetions()
{
	connect(m_controls->ui.next, SIGNAL( clicked() ), 
			m_game, SLOT(slotNextMove()) );
	connect(m_controls->ui.stop, SIGNAL( clicked() ), 
			m_game, SLOT(stopGame()) );
	connect(m_game, SIGNAL( turnOver(Move*) ), 
			m_controls->ui.gf, SLOT(slotUpdate(Move*)) );
	connect(m_game, SIGNAL( turnOver() ), 
			this, SLOT(calculateStatistics() ) );
}

void GamesDialog::calculateStatistics()
{
	if ( !m_game ) return;
}

void GamesDialog::displayStatistics()
{
}

#include "moc_simulationsdialog.cpp"
