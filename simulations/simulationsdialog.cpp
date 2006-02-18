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
//X #include <QComboBox>
#include <QLCDNumber>

#include <iostream>

GamesDialog::GamesDialog( Simulation * sim )
	: QDialog( 0 )
{
	setWindowTitle( "Kalzium Simulations" );

	m_simulation = sim;

	statsWidget = 0;

	vbox = new QVBoxLayout( this );

	m_controls = new GameControls_Impl( this );
	m_controls->ui.gf->setField( 0 );

	vbox->addWidget( m_controls );

	QStringList l = GamesFactory::instance()->games();
//X 	foreach( QString s, l ){
//X 		m_controls->ui.combo->addItem(s);
//X 	}
	
	connect(m_controls->ui.start, SIGNAL( clicked() ), 
			this, SLOT(startTheCurrentGame()) );
}

//X void GamesDialog::activateGame( int nr )
//X {
//X 	//FIXME This is not the best way I guess... But I have no
//X 	//clue how to improve it... Why is there no ->clear() call 
//X 	//or something?
//X 	foreach( QObject * o, vbox->children() )
//X 		vbox->removeWidget( ( QWidget* )o );
//X 	
//X 	//better safe than sorry
//X 	m_controls->ui.gf->setField( 0 );
//X 	
//X 	Simulation * g = GamesFactory::instance()->build( nr );
//X 
//X 	if ( !g ) return;
//X 
//X 	m_simulation = g;
//X 
//X 	statsWidget = m_simulation->statisticsWidget();
//X 	
//X 	vbox->addWidget( m_controls );
//X 	vbox->addWidget( statsWidget );
//X 	
//X 	int x = m_controls->ui.xsize->value();
//X 	int y = m_controls->ui.ysize->value();
//X 	
//X 	m_simulation->field()->setFieldXSize( x );
//X 	m_simulation->field()->setFieldYSize( y );
//X 	
//X 	m_controls->ui.gf->setField( m_simulation->field() );
//X //X 	m_controls->ui.label->setPlainText(m_simulation->rules());
//X 
//X 	m_controls->ui.gf->update();
//X 
//X 	resize( sizeHint() );
//X 
//X 	createConnetions();
//X }

void GamesDialog::slotStartWithTimer()
{
	m_simulation->startWithTimer( m_controls->ui.time->value() );
}

void GamesDialog::startTheCurrentGame()
{
//X 	int selection = m_controls->ui.combo->currentIndex();
//X 
//X 	activateGame( selection );
//X 
//X 	slotStartWithTimer();
}

void GamesDialog::createConnetions()
{
	connect(m_controls->ui.next, SIGNAL( clicked() ), 
			m_simulation, SLOT(slotNextMove()) );
	connect(m_controls->ui.stop, SIGNAL( clicked() ), 
			m_simulation, SLOT(stopGame()) );
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
