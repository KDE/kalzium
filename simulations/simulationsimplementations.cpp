/***************************************************************************
 *   Copyright (C) 2006      by Carsten Niehaus,    cniehaus@kde.org       *
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

#include "simulationsimplementations.h"

#include "field.h"

#include "ui_raform.h"
#include "ui_boltzmannform.h"
#include "ui_crystalform.h"
#include "ui_lightabsorptionform.h"
#include "ui_decompositionform.h"
#include "ui_volterraform.h"

#include <time.h>
#include <stdlib.h>

void RASimulation::start()
{
	srand (  time( NULL ) );

	m_field->clear();

	//fill the field with x*y white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			m_field->addStone( new Stone( Stone::First, QPoint( x, y ) ) );
		}
	}
	Simulation::start();
}

RASimulation* RASimulation::instance()
{
	static RASimulation g;
	return &g;
}

QByteArray RASimulation::name() const
{
	return "RASimulation";
}

QString RASimulation::description() const
{
	return  "Radioactive decay";
}

QString RASimulation::rules() const
{
	return "The simulation starts with an all white board. In each turn, a Stone on the board will be selected randomly. If the stone is white, it will turn black. This represents the radioactive decay.  The game becomes interesting if you alter the number of turns it runs. The probablilty that a stone swap the colour from white to black decreases with each turn. It is proportional to the halflife period.";
}

void RASimulation::rollDice()
{
	//generating two random numbers
	const int x = ( int ) ( rand()%m_field->xSize() );
	const int y = ( int ) ( rand()%m_field->ySize() );

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	if ( !stone )
		return;

	//if there is a white stone make it black
	if ( stone->player() == Stone::First )
		stone->swap();
}

void RASimulation::updateStatistics()
{
	Move * m = lastMove();

	int w = m->numberOfStones( Stone::First );
	int h = m->numberOfStones( Stone::Second );
	double percentage = ( double )w/h * 100;

	m_statForm->white->setText(QString::number( w ));
	m_statForm->black->setText(QString::number( h ));
	m_statForm->percentage->setText( QString( "%1%" ).arg( percentage ) );
}

	RASimulation::RASimulation()
: Simulation()
{
	m_field = new Field();
	m_statForm = new Ui::RAForm();
	m_statWidget = new QWidget();
	m_statForm->setupUi( m_statWidget );
	m_design = SimulationfieldWidget::SQUARE;

	setField( m_field );

	m_counter = 0;
}

CrystallizationSimulation* CrystallizationSimulation::instance()
{
	static CrystallizationSimulation g;
	return &g;
}

QByteArray CrystallizationSimulation::name() const
{
	return "CrystallizationSimulation";
}

QString CrystallizationSimulation::description() const
{
	return "Crystallization";
}

QString CrystallizationSimulation::rules() const
{
	return "Explanation missing. It is about diffusion and forces in a crystal, you will always get a \"checkfield\" pattern";
}

void CrystallizationSimulation::rollDice()
{
	//generating two random numbers
	const int x = ( int ) ( rand()%m_field->xSize() );
	const int y = ( int ) ( rand()%m_field->ySize() );

	//the propability
	const int w = ( int ) rand()%2;

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	if ( !stone )
		return;

	int numTeam = m_field->neighboursTeam( stone ).count();
	int totalNum = m_field->neighbours( stone ).count();
	int numOtherTeam = totalNum - numTeam;

	if ( numTeam < numOtherTeam )
	{
		//do nothing
	}
	else if ( numTeam > numOtherTeam )
	{//exchange two stones (one from each team)
		m_field->exchangeStones( point );
	}
	else if ( numTeam == numOtherTeam )
	{//with a probability of 50% do as in the if-condition above
		if ( w%2 )
			m_field->exchangeStones( point );
	}
}

void CrystallizationSimulation::updateStatistics()
{
}

//Crystallizationgame
	CrystallizationSimulation::CrystallizationSimulation()
: Simulation()
{
	m_field = new Field();
	m_statForm = new Ui::CrystalForm();
	m_statWidget = new QWidget();
	m_statForm->setupUi( m_statWidget );
	setField( m_field );
}

void CrystallizationSimulation::start()
{
	srand (  time( NULL ) );

	m_field->clear();

	//fill the field with x*y black and white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			if ( y < m_field->ySize()/2 )
				m_field->addStone( new Stone( Stone::First, QPoint( x, y ) ) );
			else
				m_field->addStone( new Stone( Stone::Second, QPoint( x, y ) ) );
		}
	}
	Simulation::start();
}


//////////////////////////////////////////////////////////////////////
//DecompositionSimulation
DecompositionSimulation* DecompositionSimulation::instance()
{
	static DecompositionSimulation g;
	return &g;
}

QByteArray DecompositionSimulation::name() const
{
	return "DecompositionSimulation";
}

QString DecompositionSimulation::description() const
{
	return "Decomposition";
}

QString DecompositionSimulation::rules() const
{
	return "Explanation missing. It is about diffusion and forces in a crystal, you will always big areas";
}

void DecompositionSimulation::rollDice()
{
	//generating two random numbers
	const int x = ( int ) ( rand()%m_field->xSize() );
	const int y = ( int ) ( rand()%m_field->ySize() );

	//the propability
	const int w = ( int ) rand()%2;

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	if ( !stone )
		return;

	int numTeam = m_field->neighboursTeam( stone ).count();
	int totalNum = m_field->neighbours( stone ).count();
	int numOtherTeam = totalNum - numTeam;

	if ( numTeam < numOtherTeam )
	{//exchange two stones (one from each team)
		m_field->exchangeStones( point );
	}
	else if ( numTeam > numOtherTeam )
	{
		//do nothing
	}
	else if ( numTeam == numOtherTeam )
	{//with a probability of 50% do as in the if-condition above

		if ( w%2 )
			m_field->exchangeStones( point );
	}
}

void DecompositionSimulation::updateStatistics()
{
}


	DecompositionSimulation::DecompositionSimulation()
: CrystallizationSimulation()
{
	m_field = new Field();
	setField( m_field );
}

void DecompositionSimulation::start()
{
	srand (  time( NULL ) );

	m_field->clear();

	//fill the field with x*y black and white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			if ( y%2 )
			{
				if ( x%2 )
					m_field->addStone( new Stone( Stone::Second, QPoint( x, y ) ) );
				else
					m_field->addStone( new Stone( Stone::First, QPoint( x, y ) ) );
			} else {
				if ( x%2 )
					m_field->addStone( new Stone( Stone::First, QPoint( x, y ) ) );
				else
					m_field->addStone( new Stone( Stone::Second, QPoint( x, y ) ) );
			}
		}
	}
	Simulation::start();
}

///BoltzmannSimulation
BoltzmannSimulation::BoltzmannSimulation()
{
	m_field = new Field();
	setField( m_field );
	m_field->setFieldSize( 15,9 );
	m_statForm = new Ui::BoltzmannForm();
	m_statWidget = new QWidget();
	m_statForm->setupUi( m_statWidget );
	m_design = SimulationfieldWidget::DENSITY;

	changeToDo = true;
}

BoltzmannSimulation* BoltzmannSimulation::instance()
{
	static BoltzmannSimulation g;
	return &g;
}

QString BoltzmannSimulation::rules() const
{
	return "to be written";
}

QString BoltzmannSimulation::description() const
{
	return "Sedimation Equilibrum";
}

QByteArray BoltzmannSimulation::name() const
{
	return "Boltzmann";
}

void BoltzmannSimulation::rollDice()
{
	changeToDo = true;

	int numOfStones = m_field->stones().count();

	while ( changeToDo )
	{
		const int si = ( int ) ( rand()%numOfStones );
		Stone* stone = m_field->stones().at( si );

		int yPos = stone->position().y();
		int yMax = m_field->ySize() - 1; //if the field has a ySize of 6 it is
		//internally 5. I don't want to confuse this

		if ( m_numberOfMoves%2 ) //make the y-position one higher
		{
			if ( yPos == 0 )
			{//cannot make it higher, it is already at the maximum
			}
			else
			{
				changePosition( stone, false );
			}
		}
		else
		{//lower the token by 1
			if ( yPos == yMax )
			{ //cannot lower it

			}
			else
			{
				changePosition( stone, true );
			}
		}
	}
}

void BoltzmannSimulation::changePosition( Stone * stone, bool lower )
{
	QPoint p;
	p.setX( stone->position().x() );

	if ( lower )
		p.setY( stone->position().y()+1 );
	else
		p.setY( stone->position().y()-1 );

	stone->setPosition(p);
	changeToDo = false;
}

void BoltzmannSimulation::start()
{
	srand (  time( NULL ) );

	m_field->clear();

	//start on E=1
	int ypos = m_field->ySize() - 3;

	//fill the field with x*y white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
		for ( int foo = 0; foo < 20 ; ++foo )
			m_field->addStone( new Stone( Stone::First, QPoint( x, ypos ) ) );

	Simulation::start();
}

void BoltzmannSimulation::updateStatistics()
{
}

///LightabsorptionSimulation
LightabsorptionSimulation::LightabsorptionSimulation()
{
	m_field = new Field();
	m_field->setFieldSize( 9,20 );
	setField( m_field );

	m_col = 0;
}

LightabsorptionSimulation* LightabsorptionSimulation::instance()
{
	static LightabsorptionSimulation g;
	return &g;
}

QString LightabsorptionSimulation::rules() const
{
	return "On the left photons arive. For n rows, the probability that a photon reaches the next column is 1/n.";
}

QString LightabsorptionSimulation::description() const
{
	return "Lightabsorption Simulation";
}

QByteArray LightabsorptionSimulation::name() const
{
	return "Lightabsorption";
}

void LightabsorptionSimulation::rollDice()
{
	//we will remove n times
	int n = m_field->ySize();

	for (int i = 0 ; i < n ; ++i )
	{//go from top to bottom and roll for each token (n rows --> n rolls)
		const int y = ( int ) ( rand()%n );

		foreach( int row, m_ypositions )
		{//check if we want to remove a stone
			if (row == y)
				m_ypositions.removeAt(row);

			continue;
		}
	}

	m_field->clear();

	m_col++;

	foreach( int row, m_ypositions )
	{
		m_field->addStone( new Stone( Stone::First, QPoint( m_col, row ) ) );
	}
}

void LightabsorptionSimulation::start()
{
	srand( time( NULL ) );

	m_field->clear();

	//fill the first row with stones
	for ( int y = 0 ; y < m_field->ySize() ; ++y )
	{
		m_field->addStone( new Stone( Stone::First, QPoint( 0, y ) ) );
		m_ypositions.append( y );
	}

	m_maxMoves = m_field->ySize();
	Simulation::start();
}

void LightabsorptionSimulation::updateStatistics()
{
}

///VolterraSimulation
VolterraSimulation::VolterraSimulation()
{
	m_field = new Field( );
	m_field->setFieldSize( 9,9 );
	
	m_statForm = new Ui::VolterraForm();
	m_statWidget = new QWidget();
	m_statForm->setupUi( m_statWidget );

	m_player = Stone::First;
	setField( m_field );

	m_rain = false;
	m_food = false;
	m_temperature = true;
	m_toggle = false;
}

VolterraSimulation* VolterraSimulation::instance()
{
	static VolterraSimulation g;
	return &g;
}

QString VolterraSimulation::rules() const
{
	return "Rules: * 1. Predator on Prey --> Prey dead, Predator takes the place * 2. Predator on empty or Prey --> Field becomes void (not enough Prey for the Predator) * 3. Prey on empty field --> Prey takes the place * 4. Prey on Prey --> Prey takes neighbourfield * 5. Prey on Predator --> Prey dies, nothing happens * 6. Both Pray and Predator continue to live even when there are no entities left";
}

QString VolterraSimulation::description() const
{
	return "VolterraSimulation";
}

QByteArray VolterraSimulation::name() const
{
	return "VolterraSimulation";
}

void VolterraSimulation::rollDice()
{
	if ( 0 == m_numberOfMoves%5 )//toggle the condition with 50% likelyhood every 5 moves
	{
		const int to = ( int ) ( rand()%2 );
		if ( to  == 0 )
		{
			m_toggle ? m_toggle = false : m_toggle = true;
		}
	}

	//generating two random numbers
	const int x = ( int ) ( rand()%m_field->xSize() );
	const int y = ( int ) ( rand()%m_field->ySize() );

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	if ( !stone )
	{//the field was empty
		if ( m_player == Stone::First )
			m_field->addStone( new Stone( m_player, QPoint( x, y ) ) );
		else if ( m_player == Stone::Second )
		{//Predator on empty field: do nothing
		}
	}
	//Stone::First: Prey
	//Stone::Second: Predator
	else if ( m_player == Stone::Second )
	{//Predators run
		if (stone->player() == Stone::First)
		{//predator eats prey
			stone->setPlayer( Stone::Second );
			emit( removedStone() );//the stone changed the player, this needs an update of the field!
		}
		else if ( stone->player() == Stone::Second )
		{//Predator on Predator, both die
			m_field->removeStone( stone );
			emit( removedStone() );
		}
	}
	else
	{//Prey's turn
		if (stone->player() == Stone::First)
		{//There is already a bug, so place a bug next to the field (prey on prey)
			if ( !m_rain && !m_temperature && !m_food )
			{//simple ruleset
				QPoint newP = m_field->randomFreeNeighbourCell( point );
				qDebug("Adding one new ladybird!");
				if ( point != newP )
					m_field->addStone( new Stone( Stone::First, newP ));
			}
			else
			{
				if ( m_rain )
				{
					if ( m_toggle )
					{//good rainconditions, not one but two (!) bugs will be added
						qDebug("Adding two new ladybirds!");

						QPoint newP = m_field->randomFreeNeighbourCell( point );
						if ( point != newP )
							m_field->addStone( new Stone( Stone::First, newP ));
						newP = m_field->randomFreeNeighbourCell( point );
						if ( point != newP )
							m_field->addStone( new Stone( Stone::First, newP ));
					}
					else
					{//extreme rain! Two bugs are lost, no matter what!
						Stone * s = m_field->randomStone( Stone::First );
						if ( s )
 							m_field->removeStone( s );
						
						s = m_field->randomStone( Stone::First );
						if ( s )
 							m_field->removeStone( s );
					}
					
				}
				if ( m_temperature )
				{
					if ( m_toggle )
					{//good temperatures, not one but two (!) bugs will be added
						qDebug("Adding two new ladybirds!");
						
						QPoint newP = m_field->randomFreeNeighbourCell( point );
						if ( point != newP )
							m_field->addStone( new Stone( Stone::First, newP ));
						newP = m_field->randomFreeNeighbourCell( point );
						if ( point != newP )
							m_field->addStone( new Stone( Stone::First, newP ));
					}
					else
					{//bad conditions, one bug is gone because of the bad temperature!
						Stone * s = m_field->randomStone( Stone::First );
						if ( s )
						{
							qDebug("Removing one ladybird!");
 							m_field->removeStone( s );
						}
					}
				}
				if ( m_food )
				{
					if ( m_toggle )
					{//good conditions, the drawn and all surrounding free fields will becomes bugs!
					}
					else
					{//bad conditions, the drawn and all surrounding bugs will die!
						QList<Stone*> nei = m_field->neighboursTeam( stone );
						qDebug("removing %d stones!", nei.count());
						foreach( Stone*st,nei )
							m_field->removeStone( st );

						//and finaly remove the stone which has been drawn itself
						m_field->removeStone( stone );
					}
				}
			}
		}else
		{//prey is eaten by the predator already there, so do nothing
		}
	}

	m_player == Stone::First ? m_player = Stone::Second : m_player = Stone::First;

	m_field->debugOutput();
}

void VolterraSimulation::start()
{
	srand (  time( NULL ) );

	reset();

	Simulation::start();
}

void VolterraSimulation::updateStatistics()
{
	Move * m = lastMove();

	int w = m->numberOfStones( Stone::First );
	int h = m->numberOfStones( Stone::Second );
	double percentage = ( double )h/w * 100;

	int numfields=m_field->xSize()*m_field->ySize();

	m_statForm->freefields->setText(QString::number( numfields-w-h ));
	m_statForm->ladybird->setText( QString::number(  w ) );
	m_statForm->mantis->setText( QString::number(  h ) );
}

void VolterraSimulation::reset()
{
	m_field->clear();

	//fill the field with x*y black and white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			if ( y < m_field->ySize()/2 )
				m_field->addStone( new Stone( Stone::First, QPoint( x, y ) ) );
			else
				m_field->addStone( new Stone( Stone::Second, QPoint( x, y ) ) );
		}
	}
}
