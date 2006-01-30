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
#ifndef GAMES_H
#define GAMES_H

#include <QTimer>
#include <QStringList>
#include <QPixmap>

#include <krandomsequence.h>

#include "field.h"
#include "stone.h"

class KRandomSequence;
class Stone;
class Move;


/**
 * @author Carsten Niehaus
 * @brief Baseclass for all games
 */
class Game : public QObject
{
	Q_OBJECT
 	public:
		/**
		 * Destructor
		 */
		virtual ~Game();

		/**
		 * starts the game
		 */
		virtual void startGame();

		/**
		 * roll the dices. This means: start a new turn 
		 */
		virtual void rollDice() = 0;

		/**
		 * set the field to @p field
		 */
		void setField( Field* field )
		{
			m_field = field;
		}

		/**
		 * @return the field of the game
		 */
		virtual Field* field() const{
			return m_field;
		}

		/**
		 * Creates a QPixmap of the move. Is will be like a screenshot of the 
		 * big widget
		 *
		 * @return a QPixmap of the move
		 * @param the number of the move of which the QPixmap will be created. The first move is 1.
		 */
		virtual QPixmap pixmapOfMove( int movenumber );

	signals:
		/**
		 * the game has ended
		 */
		void gameOver();

		/**
		 * one turn is over
		 */
		void turnOver(Move*);

	public slots:
		/**
		 * Start the next draw/roll
		 */
		virtual void slotNextMove();

		/**
		 * Active the game by using a QTimer. In this case, every @p ms milliseconds
		 * a new turn will be done
		 * @see rollDice
		 */
		virtual void startWithTimer( int ms ){
			m_timer.start( ms );
		}
		
		/**
		 * halt the game
		 */
		virtual void stopGame(){
			m_timer.stop();
		}

	protected:
		/**
		 * the field of the game
		 */
		Field* m_field;
		
		/**
		 * needed for automated games
		 */
		QTimer m_timer;

		/**
		 * needed for the random numbers
		 */
		KRandomSequence random;

		/**
		 * The constructor
		 */
		Game();

		/**
		 * a list of moves. With this list you can follow
		 * the game backward and forward.
		 */
		QList<Move*> m_moves;

		/**
		 * finish the move, emit the signal
		 */
		void finishMove();
};

/**
 * @author Carsten Niehaus
 */
class Move
{
	public:
		/**
		 * Constructor
		 * @param list the list of stones
		 */
		Move( QStringList list ){
			m_list = list;
		}

		/**
		 * @return the number of stones of the PLAYER p
		 */
		int numberOfStones( Stone::PLAYER p );

	private:
		QStringList m_list;
};

#endif // GAMES_H
