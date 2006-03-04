/***************************************************************************
 *   Copyright (C) 2005,2006 by Carsten Niehaus,    cniehaus@kde.org       *
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
#include "simulation.h"
#include "stone.h"
#include "field.h"
#include "simulationsimplementations.h"

#include <QApplication>
#include <QPoint>

//SimulationsFactory
SimulationsFactory::SimulationsFactory()
{
	m_simulations << LightabsorptionSimulation::instance();
	m_simulations << RASimulation::instance();
	m_simulations << BoltzmannSimulation::instance();
	m_simulations << DecompositionSimulation::instance();
	m_simulations << CrystallizationSimulation::instance();
	m_simulations << VolterraSimulation::instance();
}

SimulationsFactory* SimulationsFactory::instance()
{
	static SimulationsFactory gf;
	return &gf;
}

Simulation* SimulationsFactory::build( int id ) const
{
	if (  (  id < 0 ) || (  id >= m_simulations.count() ) )
		return 0;

	return m_simulations.at( id );
}

Simulation* SimulationsFactory::build( const QByteArray& id ) const
{
	for (  int i = 0; i < m_simulations.count(); i++ )
	{
		if (  m_simulations.at(  i )->name() == id )
			return m_simulations.at( i );
	}

	// not found
	return 0;
}

QStringList SimulationsFactory::simulations() const
{
	QStringList l;
	for (  int i = 0; i < m_simulations.count(); i++ )
	{
		l << m_simulations.at( i )->description();
	}
	return l;
}

//Move
Move::Move( QByteArray array )
{
	m_array = array;
}

int Move::numberOfStones( Stone::PLAYER p )
{
	char letter;
	if ( p == Stone::First )
		letter = '1';
	else if ( p == Stone::Second )
		letter = '2';
	else if ( p == Stone::Third )
		letter = '3';
	else if ( p == Stone::Fourth )
		letter = '4';
	else 
		return 0;

	int num = 0;

	return m_array.count( letter );
}

//Simulation
Simulation::Simulation()
{
	m_numberOfMoves = 0;

	m_maxMoves = -1;	

	m_design = SimulationfieldWidget::CIRCLE;
	
	connect( &m_timer, SIGNAL( timeout() ), 
			this, SLOT( slotNextMove() ) );
}

Simulation* Simulation::instance()
{
	return 0;
}

QPixmap Simulation::pixmapOfMove( int movenumber )
{
	QPixmap px;

	if ( !movenumber > 1 && !movenumber < m_moves.count() )
		return QPixmap();

	//Move * move = m_moves.at( movenumber - 1 );
	
	return px;
}

Simulation::~Simulation(){}

void Simulation::slotNextMove()
{
	finishMove();
	m_numberOfMoves++;
	if ( m_maxMoves != -1 )//check if the maximum of moves should be checked
	{
		qDebug( "m_maxMoves: %d", m_maxMoves );
		if ( m_maxMoves <= m_numberOfMoves )
		{
			emit simulationOver();
			stopSimulation();
			return;
		} else
			rollDice();
	} else 
		rollDice();
}

void Simulation::finishMove()
{
	QByteArray ds;
	QChar c;
	
	for ( int y = 0; y < m_field->ySize()  ; ++y )
	{
		for ( int x = 0; x < m_field->xSize() ; ++x )
		{
			Stone* s = m_field->stoneAtPosition( QPoint( x,y ) );
			if ( s ) 
			{
				Stone::PLAYER p = s->player();
			
				if ( p == Stone::First )
					c = '1';
				else if ( p == Stone::Second )
					c = '2';
				else if ( p == Stone::Third )
					c = '3';
				else if ( p == Stone::Fourth )
					c = '4';
				ds.append( c );
			}
			else//emtpy field, append a space
				ds.append( ' ' );
		}
		ds.append( '\n' );
	}

	Move *move = new Move( ds );
	qDebug( "Move %d", m_numberOfMoves );
	qDebug( ds );
	m_moves.append( move );

	qApp->processEvents();

	emit turnOver( move );
	emit turnOver();
}

QWidget * Simulation::statisticsWidget() const
{
	return m_statWidget;
}

void Simulation::start()
{
	m_numberOfMoves = 0;
}

#include "moc_simulation.cpp"
