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
#ifndef GAMESIMPLEMENTATION_H
#define GAMESIMPLEMENTATION_H

#include "simulation.h"

#include "ui_raform.h"


/**
 * @author Carsten Niehaus
 *
 * Rules of the game: The game starts with an all white board. In each turn, a 
 * Stone on the board will be selected randomly. If the stone is white, it will
 * turn black. This represents the radioactive decay.
 * The game becomes interesting if you alter the number of turns it runs. The 
 * probablilty that a stone swap the colour from white to black decreases with
 * each turn. It is proportional to the halflife period.
 */
class RAGame : public Simulation
{
	public:
		RAGame();

		static RAGame* instance();

		QByteArray name() const;
		
		QString description() const;
		
		void rollDice();

		QString rules() const;

		virtual QWidget * statisticsWidget() const;

	public slots:
		void start();

	private:
		int m_counter;
		int m_number;

		Ui::RAForm* m_statForm;

	protected:
		Field* m_field;
};

/**
 * @author Carsten Niehaus
 */
class CrystallizationGame : public Simulation
{
	public:
		CrystallizationGame();
		
		static CrystallizationGame* instance();
		
		virtual QWidget * statisticsWidget() const
		{return new QWidget();}

		void rollDice();
		
		QString rules() const;
		
		QByteArray name() const;
		
		QString description() const;
		
	public slots:
		/**
		 * starts the game
		 */
		void start();

	private:
		int m_number;

	protected:
		Field* m_field;

		/**
		 * @return the number of stones of the other player
		 */
		virtual int neighboursNum( Stone* stone );
		
		/**
		 * @return the number of stones of the player
		 */
		virtual int neighboursTeam( Stone* stone );

		/**
		 * Exange the Stone at the postion @p point with one stone
		 * of the other team. That other Stone has to be in orthogonal
		 * contact with the Stone in @p point
		 */
		virtual void exchangeStones( const QPoint& point );
};

/**
 * @author Carsten Niehaus
 */
class DecompositionSimulation : public CrystallizationGame
{
	public:
		DecompositionSimulation();
		
		static DecompositionSimulation* instance();
		
		virtual QWidget * statisticsWidget() const
		{return new QWidget();}

		void rollDice();
		
		QString rules() const;
		
		QByteArray name() const;
		
		QString description() const;
		
	public slots:
		/**
		 * starts the game
		 */
		void start();

	private:
		int m_number;

	protected:
		Field* m_field;

		//FIXME Why have the methods neighboursNum, neighboursTeam and exchangeStones
		//		to be reimplemented? If I don't do that they don't work because
		//		the stones are not found... Don't understand it...
		
		/**
		 * @return the number of stones of the other player
		 */
		virtual int neighboursNum( Stone* stone );
		
		/**
		 * @return the number of stones of the player
		 */
		virtual int neighboursTeam( Stone* stone );

		/**
		 * Exange the Stone at the postion @p point with one stone
		 * of the other team. That other Stone has to be in orthogonal
		 * contact with the Stone in @p point
		 */
		virtual void exchangeStones( const QPoint& point );
};

/**
 * @author Carsten Niehaus
 */
class BoltzmannSimulation : public Simulation
{
	public:
		BoltzmannSimulation();
		
		static BoltzmannSimulation* instance();
		
		virtual QWidget * statisticsWidget() const
		{}

		bool changeToDo;

		void rollDice();
		
		QString rules() const;
		
		QByteArray name() const;
		
		QString description() const;
		
	public slots:
		/**
		 * starts the game
		 */
		void start();

	private:
		int m_number;
		
		void changePosition( Stone * stone , bool lower );

	protected:
		Field* m_field;
};

/**
 * @author Carsten Niehaus
 */
class LightabsorptionSimulation : public Simulation
{
	public:
		LightabsorptionSimulation();
		
		static LightabsorptionSimulation* instance();
		
		virtual QWidget * statisticsWidget() const
		{return new QWidget();}

		void rollDice();
		
		QString rules() const;
		
		QByteArray name() const;
		
		QString description() const;
		
	public slots:
		/**
		 * starts the game
		 */
		void start();

	private:
		int m_number;

		int m_col;

	protected:
		Field* m_field;
};

#endif // GAMESIMPLEMENTATION_H
