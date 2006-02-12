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

#include "gamesimplementation.h"

#include "ui_raform.h"

#include <time.h>
#include <stdlib.h>
#include <iostream>

void RAGame::start()
{
	srand (  time( NULL ) );
	
	m_field->stones().clear();

	//fill the field with x*y white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			m_field->addStone( new Stone( Stone::White, QPoint( x, y ) ) );
		}
	}
}

RAGame* RAGame::instance()
{
	static RAGame g;
	return &g;
}

QByteArray RAGame::name() const
{
	return "RAGame";
}

QString RAGame::description() const
{
	return  "Radioactive decay";
}

QWidget* RAGame::statisticsWidget() const
{
	return m_statWidget;
}

QString RAGame::rules() const
{
	return "The simulation starts with an all white board. In each turn, a Stone on the board will be selected randomly. If the stone is white, it will turn black. This represents the radioactive decay.  The game becomes interesting if you alter the number of turns it runs. The probablilty that a stone swap the colour from white to black decreases with each turn. It is proportional to the halflife period.";
}

void RAGame::rollDice()
{
	m_number++;
	
	//generating two random numbers
	const int x = ( int ) ( rand()%m_field->xSize() );
	const int y = ( int ) ( rand()%m_field->ySize() );

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	//increase if both dices have the same value. This
	//simulates the halflifetime
	if ( x == y )
		m_counter++;

	if ( !stone )
		return;

	//if there is a white stone make it black
	if ( stone->player() == Stone::White )
		stone->swap();
}

RAGame::RAGame()
	: Simulation()
{
	m_field = new Field();
	m_statForm = new Ui::RAForm();
	m_statWidget = new QWidget();
	m_statForm->setupUi( m_statWidget );

	setField( m_field );
	
	m_counter = 0;
	m_number = 0;
}

CrystallizationGame* CrystallizationGame::instance()
{
	static CrystallizationGame g;
	return &g;
}

QByteArray CrystallizationGame::name() const
{
	return "CrystallizationGame";
}

QString CrystallizationGame::description() const
{
	return "Crystallization";
}

QWidget* CrystallizationGame::statisticsWidget() const
{
	// TODO
	return new QWidget();
}

QString CrystallizationGame::rules() const
{
	return "Explanation missing. It is about diffusion and forces in a crystal, you will always get a \"checkfield\" pattern";
}

void CrystallizationGame::rollDice()
{
	m_number++;

	//generating two random numbers
	const int x = ( int ) ( rand()%m_field->xSize() );
	const int y = ( int ) ( rand()%m_field->ySize() );
	
	//the propability
	const int w = ( int ) rand()%2;

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	if ( !stone )
		return;

	int numTeam = neighboursTeam( stone );
	int totalNum = neighboursNum( stone );
	int numOtherTeam = totalNum - numTeam;
	
	if ( numTeam < numOtherTeam )
	{
		//do nothing
	}
	else if ( numTeam > numOtherTeam )
	{//exchange two stones (one from each team)
			exchangeStones( point );
	}
	else if ( numTeam == numOtherTeam )
	{//with a probability of 50% do as in the if-condition above
		if ( w%2 )
			exchangeStones( point );
	}
}

void CrystallizationGame::exchangeStones( const QPoint& point )
{
	Stone* stone = m_field->stoneAtPosition( point );
	
	QList<Stone*> otherTeamStones;
	
	Stone* stone1 = m_field->stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = m_field->stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = m_field->stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = m_field->stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 && stone1->player() != stone->player() )
		otherTeamStones << stone1;
	if ( stone2 && stone2->player() != stone->player()  )
		otherTeamStones << stone2;
	if ( stone3 && stone3->player() != stone->player()  )
		otherTeamStones << stone3;
	if ( stone4 && stone4->player() != stone->player()  )
		otherTeamStones << stone4;
	
	if ( otherTeamStones.count() < 1 )//well, there is nothing to exchange...
	{
		return;
	}
	

	//the stone to exchange...
	Stone *chosenStone = 0;
	
	if ( otherTeamStones.count() == 1 ) {
		chosenStone = otherTeamStones[ 0 ];//take the first (and only) Stone
	}
	else{
		const int choice = ( int ) ( rand()%otherTeamStones.count() );

		chosenStone = otherTeamStones[ choice ];
	}

	chosenStone->swap();
	stone->swap();
}

int CrystallizationGame::neighboursNum( Stone* stone )
{
	QPoint point( stone->position() );
	
	QList<Stone*> Stones;
	
	Stone* stone1 = m_field->stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = m_field->stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = m_field->stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = m_field->stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 ) Stones << stone1;
	if ( stone2 ) Stones << stone2;
	if ( stone3 ) Stones << stone3;
	if ( stone4 ) Stones << stone4;

	return Stones.count();
}

int CrystallizationGame::neighboursTeam( Stone* stone )
{
	QPoint point( stone->position() );
	
	QList<Stone*> TeamStones;
	
	Stone* stone1 = m_field->stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = m_field->stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = m_field->stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = m_field->stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 && stone1->player() == stone->player() )
		TeamStones << stone1;
	if ( stone2 && stone2->player() == stone->player()  )
		TeamStones << stone2;
	if ( stone3 && stone3->player() == stone->player()  )
		TeamStones << stone3;
	if ( stone4 && stone4->player() == stone->player()  )
		TeamStones << stone4;
	
	return TeamStones.count();
}

//Crystallizationgame
CrystallizationGame::CrystallizationGame()
	: Simulation()
{
	m_field = new Field();
	setField( m_field );

	m_number = 0;
}

void CrystallizationGame::start()
{
	srand (  time( NULL ) );

	m_field->stones().clear();
	
	//fill the field with x*y black and white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			if ( y < m_field->ySize()/2 )
				m_field->addStone( new Stone( Stone::White, QPoint( x, y ) ) );
			else
				m_field->addStone( new Stone( Stone::Black, QPoint( x, y ) ) );
		}
	}
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

QWidget* DecompositionSimulation::statisticsWidget() const
{
	// TODO
	return new QWidget();
}

QString DecompositionSimulation::rules() const
{
	return "Explanation missing. It is about diffusion and forces in a crystal, you will always big areas";
}

void DecompositionSimulation::rollDice()
{
	m_number++;

	//generating two random numbers
	const int x = ( int ) ( rand()%m_field->xSize() );
	const int y = ( int ) ( rand()%m_field->ySize() );
	
	//the propability
	const int w = ( int ) rand()%2;

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	if ( !stone )
		return;

	int numTeam = neighboursTeam( stone );
	int totalNum = neighboursNum( stone );
	int numOtherTeam = totalNum - numTeam;
	
	if ( numTeam < numOtherTeam )
	{//exchange two stones (one from each team)
		exchangeStones( point );
	}
	else if ( numTeam > numOtherTeam )
	{
		//do nothing
	}
	else if ( numTeam == numOtherTeam )
	{//with a probability of 50% do as in the if-condition above
		
		if ( w%2 )
			exchangeStones( point );
	}
}

DecompositionSimulation::DecompositionSimulation()
	: CrystallizationGame()
{
	m_field = new Field();
	setField( m_field );

	m_number = 0;
}

void DecompositionSimulation::start()
{
	srand (  time( NULL ) );

	m_field->stones().clear();
	
	//fill the field with x*y black and white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
	{
		for ( int y = 0; y < m_field->ySize() ; ++y )
		{
			if ( y%2 )
			{
				if ( x%2 )
					m_field->addStone( new Stone( Stone::Black, QPoint( x, y ) ) );
				else
					m_field->addStone( new Stone( Stone::White, QPoint( x, y ) ) );
			} else {
				if ( x%2 )
					m_field->addStone( new Stone( Stone::White, QPoint( x, y ) ) );
				else
					m_field->addStone( new Stone( Stone::Black, QPoint( x, y ) ) );
			}
		}
	}
}

void DecompositionSimulation::exchangeStones( const QPoint& point )
{
	Stone* stone = m_field->stoneAtPosition( point );
	
	QList<Stone*> otherTeamStones;
	
	Stone* stone1 = m_field->stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = m_field->stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = m_field->stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = m_field->stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 && stone1->player() != stone->player() )
		otherTeamStones << stone1;
	if ( stone2 && stone2->player() != stone->player()  )
		otherTeamStones << stone2;
	if ( stone3 && stone3->player() != stone->player()  )
		otherTeamStones << stone3;
	if ( stone4 && stone4->player() != stone->player()  )
		otherTeamStones << stone4;
	
	if ( otherTeamStones.count() < 1 )//well, there is nothing to exchange...
	{
		return;
	}
	

	//the stone to exchange...
	Stone *chosenStone = 0;
	
	if ( otherTeamStones.count() == 1 ) {
		chosenStone = otherTeamStones[ 0 ];//take the first (and only) Stone
	}
	else{
		const int choice = ( int ) ( rand()%otherTeamStones.count() );

		chosenStone = otherTeamStones[ choice ];
	}

	chosenStone->swap();
	stone->swap();
}

int DecompositionSimulation::neighboursNum( Stone* stone )
{
	QPoint point( stone->position() );
	
	QList<Stone*> Stones;
	
	Stone* stone1 = m_field->stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = m_field->stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = m_field->stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = m_field->stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 ) Stones << stone1;
	if ( stone2 ) Stones << stone2;
	if ( stone3 ) Stones << stone3;
	if ( stone4 ) Stones << stone4;

	return Stones.count();
}

int DecompositionSimulation::neighboursTeam( Stone* stone )
{
	QPoint point( stone->position() );
	
	QList<Stone*> TeamStones;
	
	Stone* stone1 = m_field->stoneAtPosition( QPoint(point.x()+1, point.y() ) );
	Stone* stone2 = m_field->stoneAtPosition( QPoint(point.x()-1, point.y() ) );
	Stone* stone3 = m_field->stoneAtPosition( QPoint(point.x(), point.y()+1 ) );
	Stone* stone4 = m_field->stoneAtPosition( QPoint(point.x(), point.y()-1 ) );

	if ( stone1 && stone1->player() == stone->player() )
		TeamStones << stone1;
	if ( stone2 && stone2->player() == stone->player()  )
		TeamStones << stone2;
	if ( stone3 && stone3->player() == stone->player()  )
		TeamStones << stone3;
	if ( stone4 && stone4->player() == stone->player()  )
		TeamStones << stone4;
	
	return TeamStones.count();
}

///BoltzmannSimulation
//
BoltzmannSimulation::BoltzmannSimulation()
{
	m_field = new Field();
	setField( m_field );

	changeToDo = true;

	m_number = 0;
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

QWidget* BoltzmannSimulation::statisticsWidget() const
{
	// TODO
	return new QWidget();
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
	m_number++;
	changeToDo = true;

	int numOfStones = m_field->stones().count();

	while ( changeToDo )
	{
		const int si = ( int ) ( rand()%numOfStones );
		Stone* stone = m_field->stones().at( si );

		int yPos = stone->position().y();
		int yMax = m_field->ySize() - 1; //if the field has a ySize of 6 it is
										 //internally 5. I don't want to confuse this

		if ( m_number%2 ) //make the y-position one higher
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
	
	m_field->stones().clear();

	//start on E=1
	int ypos = m_field->ySize() - 3;

	//fill the field with x*y white stones
	for ( int x = 0 ; x < m_field->xSize() ; ++x )
		for ( int foo = 0; foo < 20 ; ++foo )
			m_field->addStone( new Stone( Stone::White, QPoint( x, ypos ) ) );
}

///LightabsorptionSimulation
//
LightabsorptionSimulation::LightabsorptionSimulation()
{
	m_field = new Field();
	setField( m_field );

	m_number = 0;
	m_col = 0;
}
	
LightabsorptionSimulation* LightabsorptionSimulation::instance()
{
	static LightabsorptionSimulation g;
	return &g;
}

QString LightabsorptionSimulation::rules() const
{
	return "to be written";
}

QString LightabsorptionSimulation::description() const
{
	return "Lightabsorption Simulation";
}

QWidget* LightabsorptionSimulation::statisticsWidget() const
{
	// TODO
	return new QWidget();
}

QByteArray LightabsorptionSimulation::name() const
{
	return "Lightabsorption";
}

void LightabsorptionSimulation::rollDice()
{
	m_number++;

	//we will remove n times
	int n = m_field->ySize();

	for (int i = 0 ; i < n ; ++i )
	{
		const int y = ( int ) ( rand()%n );
		Stone * s = m_field->stoneAtPosition( QPoint(m_col, y) );

		if ( !s ) continue;

		int number = 0;
		int numberToRemove = 0;
		
		QList<Stone*> stones = m_field->stones();
		foreach( Stone * s , stones )
		{
			if ( s->position() == QPoint( m_col, y ) )
			{
				std::cout << "position equals, removing "<< m_col << ", " << s->position().y() << std::endl;
				numberToRemove = number;
			}
			else
				std::cout << "position different, nothing to remove "<< m_col << ", " << s->position().y() << std::endl;

			number++;
			numberToRemove++;
		}

		stones.removeAt( numberToRemove );	
		
		m_field->stones();
		foreach( Stone * s , stones )
		{
			std::cout << "Stone has position "<< m_col << ", " << s->position().y() << std::endl;
		}
	}
	
	m_col++;
}

void LightabsorptionSimulation::start()
{
	srand( time( NULL ) );
	
	m_field->stones().clear();

	//start on E=1
	int ypos = m_field->ySize() - 3;

	//fill the field with x*y white stones
	for ( int y = 0 ; y < m_field->ySize() ; ++y )
		m_field->addStone( new Stone( Stone::White, QPoint( 0, y ) ) );
}
