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
#include "games.h"
#include "gamecontrols_impl.h"
#include "stone.h"
#include "field.h"

#include <QPoint>
#include <QLayout>

#include <kdebug.h>
#include <klocale.h>

#include <math.h>
#include <time.h>

//Move
int Move::numberOfStones( Stone::PLAYER p )
{
	QString letter;
	if ( p == Stone::White )
		letter = "W";
	else if ( p == Stone::Black )
		letter = "B";
	else 
		return 0;

	int num = 0;
	QStringList::Iterator it = m_list.begin();
	const QStringList::Iterator itEnd = m_list.end();
	for (; it != itEnd; ++it)	
	{
		QString s = *it;
		num += s.count( letter );
	}

	return num;
}

//Game
Game::Game()
{
	connect( &m_timer, SIGNAL( timeout() ), 
			this, SLOT( slotNextMove() ) );
}

Game::~Game(){}

void Game::slotNextMove()
{
	finishMove();
	rollDice();
}

void Game::startGame()
{
	random.setSeed( time(0) );
}

void Game::finishMove()
{
	QString ds = QString();
	
	QStringList sl;
	
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
		sl.append( ds );
	}

	Move *move = new Move( sl );
	m_moves.append( move );

	emit turnOver( move );
}

#include "games.moc"
