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
#include <QByteArray>

#include <krandomsequence.h>
#include <kdebug.h>

#include "field.h"
#include "stone.h"

class KRandomSequence;
class Stone;
class Move;
class Game;

/**
 * @author Carsten Niehaus
 * Gamesfactory
 */
class GamesFactory
{
	public:
		/**
		 * Get the instance of this factory.
		 **/
		static GamesFactory* instance();

		/**
		 * Returns the KalziumGradientType with the @p id specified.
		 * It will gives 0 if none found.
		 */
		Game* build( int id ) const;

		/**
		 * Returns the Game whose name is the @p id specified.
		 * It will gives 0 if none found.
		 */
		Game* build( const QByteArray& id ) const;

		/**
		 * Returns a list with the names of the gradients we support.
		 */
		QStringList games() const;

	private:
		GamesFactory();

		QList<Game*> m_games;
};

/**
 * @author Carsten Niehaus
 * @brief Baseclass for all games
 * Baseclass for all games. Inherit it and add it to the GamesFactory
 * to make it available
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

		static Game* instance();

		/**
		 * roll the dices. This means: start a new turn 
		 */
		virtual void rollDice() = 0;

		/**
		 * set the field to @p field
		 * @param field the Field of the game
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

		/**
		 * @return the number of moves already performed
		 */
		virtual int numberOfMoves() const {
			return m_numberOfMoves;
		}

		/**
		 * @return the Move with the number @p number
		 * @param number the number of the searched move
		 */
		virtual Move* move( int number ){
			if ( number < 1 )
				return 0;
			if ( number > m_moves.count() )
				return 0;

			return m_moves.at(number-1);
		}

		/**
		 * Returns the ID of this gradient.
		 * Mainly used when saving/loading.
		 */
		virtual QByteArray name() const = 0;
		
		/**
		 * Returns the description of this gradient.
		 * Used in all the visible places.
		 */
		virtual QString description() const = 0;

		/**
		 * Returns the rules of the game as a descriptive QString.
		 * @return the rules
		 */
		virtual QString rules() const = 0;

	signals:
		/**
		 * the game has ended
		 */
		void gameOver();

		/**
		 * one turn is over
		 */
		void turnOver(Move*);

		/**
		 * one turn is over
		 */
		void turnOver();

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
		 * the number of performed moves
		 */
		int m_numberOfMoves;

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
