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
#include "games.h"
#include "gamecontrols_impl.h"

#include <kapplication.h>

#include <QPoint>
#include <QLayout>

#include <kdebug.h>
#include <klocale.h>

#include <math.h>
#include <time.h>

///Stone
Stone::Stone( PLAYER player, const QPoint& point )
{
	m_player = player;
	m_position = point;
}

void Stone::swap()
{
	if ( m_player == Stone::White )
		m_player = Stone::Black;
	else
		m_player = Stone::White;
}

Stone* Field::stoneAtPosition( const QPoint& pos )
{
	Stone* stone;

	foreach( Stone* s, m_stones )
	{
		if ( s->position() == pos )
			return s;
	}

	return 0;
}

///Field
Field::Field( int x, int y )
{}

Field::~Field()
{}

//Game
Game::Game()
{
}

Game::~Game(){}

void Game::slotNextMove()
{
	rollDice();
}

void Game::startGame()
{
	random.setSeed( time(0) );
}

void Game::stopGame()
{}

//RAField
RAGame::RAField::RAField( int x, int y )
	: Field( x, y )
{
	m_size_x = x;
	m_size_y = y;
}

void RAGame::RAField::moveStoneTo( Stone* stone, const QPoint& newPosition )
{
}

void RAGame::RAField::addStone( Stone* stone )
{
	m_stones.append( stone );
}

void RAGame::rollDice()
{
	m_number++;
	
	//generating two random numbers
	const int x = ( int ) ( random.getLong( 6 ) );
	const int y = ( int ) ( random.getLong( 6 ) );

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

//RAgame
RAGame::RAGame()
	: Game()
{
	m_field = new RAField( 6,6 );

	m_counter = 0;
	m_number = 0;

	//fill the field with 6x6 white stones
	for ( int x = 0 ; x < 6 ; ++x )
	{
		for ( int y = 0; y < 6 ; ++y )
		{
			m_field->addStone( new Stone( Stone::White, QPoint( x, y ) ) );
		}
	}
	
	//now lets play 100 times!
	while ( m_counter < 80 && m_number < 80 )
	{
		rollDice();
	}
	
	QString ds = QString();
	for ( int x = 0; x < 6 ; ++x )
	{
		ds = QString();
		for ( int y = 0; y < 6 ; ++y )
		{
			Stone* s = m_field->stoneAtPosition( QPoint( x,y ) );
			if ( s->player() == Stone::White )
				ds += "W";
			else
				ds += "B";
		}
		kdDebug() << ds << endl;
	}
}

//CrystallizationField
CrystallizationGame::CrystallizationField::CrystallizationField( int x, int y )
	: Field( x, y )
{
	m_size_x = x;
	m_size_y = y;
}

void CrystallizationGame::CrystallizationField::moveStoneTo( Stone* stone, const QPoint& newPosition )
{
}

void CrystallizationGame::CrystallizationField::addStone( Stone* stone )
{
	m_stones.append( stone );
}

void CrystallizationGame::rollDice()
{
	m_number++;

	//generating two random numbers
	const int x = ( int ) random.getLong( 6 );
	const int y = ( int ) random.getLong( 6 );
	
	//the propability
	const int w = ( int ) random.getLong( 2 );

	QPoint point( x, y );

	Stone* stone = m_field->stoneAtPosition( point );

	if ( !stone )
		return;

	int numTeam = neighboursTeam( stone );
	int totalNum = neighboursNum( stone );
	int numOtherTeam = totalNum - numTeam;
	
	kdDebug() << "Doing point " << point << " numTeam: " << numTeam << " numOtherTeam: " << numOtherTeam << endl;

	if ( numTeam < numOtherTeam )
	{
		//do nothing
	}
	else if ( numTeam > numOtherTeam )
	{//exange two stones (one from each team)
			exchangeStones( point );
	}
	else if ( numTeam == numOtherTeam )
	{//with a probability of 50% do as in the if-condition above
		if ( w%2 )
			exchangeStones( point );
	}
	else
		kdDebug() << "error" << endl;
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
	
	kdDebug() << "Run: #" << m_number << " , # of stones: " << otherTeamStones.count() << " Stones: " << stone1 << " " << stone2 << " " << stone3 << " " << stone4<< endl;

	if ( otherTeamStones.count() < 1 )//well, there is nothing to exchange...
		return;

	//the stone to exchange...
	Stone *chosenStone = 0;
	
	if ( otherTeamStones.count() == 1 ) {
		kdDebug() << "Taking the only stone there is..." << endl;
		chosenStone = otherTeamStones[ 0 ];//take the first (and only) Stone
	}
	else{
		const int choice = ( int ) ( random.getLong( otherTeamStones.count() ) );
		kdDebug() << "Choice: " << choice << " (of " << otherTeamStones.count() << ")" << endl;

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
	: Game()
{
	m_field = new CrystallizationField( 6,6 );

	m_number = 0;

	//fill the field with 6x6 white stones
	for ( int x = 0 ; x < 6 ; ++x )
	{
		for ( int y = 0; y < 6 ; ++y )
		{
			if ( y < 3 )
				m_field->addStone( new Stone( Stone::White, QPoint( x, y ) ) );
			else
				m_field->addStone( new Stone( Stone::Black, QPoint( x, y ) ) );
		}
	}
	
	QString ds = QString();
	kdDebug() << "Before starting the game:" << endl;
	for ( int x = 0; x < 6 ; ++x )
	{
		ds = QString();
		for ( int y = 0; y < 6 ; ++y )
		{
			Stone* s = m_field->stoneAtPosition( QPoint( x,y ) );
			if ( s->player() == Stone::White )
				ds += "W";
			else
				ds += "B";
		}
		kdDebug() << ds << endl;
	}
	
}

void CrystallizationGame::startGame()
{
	Game::startGame();
	
	//now lets play 100 times!
	while ( m_number < 80 )
	{
		rollDice();
	}
	
	kdDebug() << "Result of the game:" << endl;

	QString ds = QString();
	for ( int x = 0; x < 6 ; ++x )
	{
		ds = QString();
		for ( int y = 0; y < 6 ; ++y )
		{
			Stone* s = m_field->stoneAtPosition( QPoint( x,y ) );
			if ( s->player() == Stone::White )
				ds += "W";
			else
				ds += "B";
		}
		kdDebug() << ds << endl;
	}
}

#include "games.moc"
