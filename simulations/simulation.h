#ifndef SIMULATION_H
#define SIMULATION_H
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
#include <QTimer>
#include <QStringList>
#include <QPixmap>
#include <QByteArray>

#include "stone.h"
#include "simulationfield.h"

class Field;
class Move;
class Simulation;

/**
 * @author Carsten Niehaus
 * simulationsfactory
 */
class SimulationsFactory
{
	public:
		/**
		 * Get the instance of this factory.
		 **/
		static SimulationsFactory* instance();

		/**
		 * Returns the Gmae with the @p id specified.
		 * It will gives 0 if none found.
		 */
		Simulation* build( int id ) const;

		/**
		 * Returns the Simulation whose name is the @p id specified.
		 * It will gives 0 if none found.
		 */
		Simulation* build( const QByteArray& id ) const;

		/**
		 * Returns a list with the names of the gradients we support.
		 */
		QStringList simulations() const;

	private:
		SimulationsFactory();

		QList<Simulation*> m_simulations;
};

/**
 * @author Carsten Niehaus
 * @brief Baseclass for all simulations
 * Baseclass for all simulations. Inherit it and add it to the SimulationsFactory
 * to make it available
 */
class Simulation : public QObject
{
	Q_OBJECT
 	public:
		/**
		 * Destructor
		 */
		virtual ~Simulation();

		static Simulation* instance();

		/**
		 * @return the design of the tokens
		 */
		SimulationfieldWidget::TokenDesign design(){
			return m_design;
		}

		/**
		 * roll the dices. This means: start a new turn 
		 */
		virtual void rollDice() = 0;

		/**
		 * set the field to @p field
		 * @param field the Field of the Simulation
		 */
		void setField( Field* field )
		{
			m_field = field;
		}

		/**
		 * @return the field of the Simulation
		 */
		virtual Field* field() const{
			return m_field;
		}

		/**
		 * Creates a QPixmap of the move. Is will be like a screenshot of the 
		 * big widget
		 *
		 * @return a QPixmap of the move
		 * @param movenumber the number of the move of which the QPixmap will be created. The first move is 1.
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
		 * @return the current Move
		 */
		virtual Move* currentMove(){
			return m_moves.last();
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
		 * @return the statistics widget of the Simulation
		 */
		virtual QWidget * statisticsWidget() const;

		/**
		 * Returns the rules of the Simulation as a descriptive QString.
		 * @return the rules
		 */
		virtual QString rules() const = 0;

	signals:
		/**
		 * the Simulation has ended
		 */
		void simulationOver();

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
		 * Activate the Simulation by using a QTimer. In this case, every @p ms milliseconds
		 * a new turn will be done
		 * @see rollDice
		 */
		virtual void startWithTimer( int ms ){
			start();
			m_timer.start( ms );
		}
		
		/**
		 * halt the Simulation
		 */
		virtual void stopSimulation(){
			m_timer.stop();
		}

		/**
		 * Start the simulations, everything is set to the start-conditions
		 */
		virtual void start() = 0;

	protected:
		/**
		 * the field of the Simulation
		 */
		Field* m_field;

		/**
		 * the statisticwidget of the simulation
		 */
		QWidget* m_statWidget;
		
		/**
		 * needed for automated simulations
		 */
		QTimer m_timer;
 		
		/**
		 * the number of performed moves
		 */
		int m_numberOfMoves;

		/**
		 * Some simulations only allow a certain number of moves. This value
		 * defines that number. For other games, m_maxMoves will be -1.
		 */
		int m_maxMoves;

		/**
		 * The constructor
		 */
		Simulation();

		/**
		 * a list of moves. With this list you can follow
		 * the Simulation backward and forward.
		 */
		QList<Move*> m_moves;

		/**
		 * finish the move, emit the signal
		 */
		void finishMove();
		
		/**
		 * the design of the tokens
		 */
		SimulationfieldWidget::TokenDesign m_design;

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

#endif // SIMULATION_H

