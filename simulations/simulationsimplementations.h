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

class QWidget;
namespace Ui
{
	class RAForm;
	class CrystalForm;
	class BoltzmannForm;
	class LightabsorptionForm;
	class DecompositionForm;
	class VolterraForm;
};

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
class RASimulation : public Simulation
{
	public:
		RASimulation();

		static RASimulation* instance();

		QByteArray name() const;
		
		QString description() const;
		
		void rollDice();

		QString rules() const;

		void updateStatistics();

	public slots:
		void start();
		void reset(){};

	private:
		int m_counter;

		Ui::RAForm* m_statForm;
};

/**
 * @author Carsten Niehaus
 */
class CrystallizationSimulation : public Simulation
{
	public:
		CrystallizationSimulation();
		
		static CrystallizationSimulation* instance();
		
		void rollDice();
		
		QString rules() const;
		
		QByteArray name() const;
		
		QString description() const;
		
		void updateStatistics();
		
	public slots:
		/**
		 * starts the game
		 */
		void start();
		void reset(){};

	private:
		Ui::CrystalForm* m_statForm;

	protected:
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
class DecompositionSimulation : public CrystallizationSimulation
{
	public:
		DecompositionSimulation();
		
		static DecompositionSimulation* instance();
		
		void rollDice();
		
		QString rules() const;
		
		QByteArray name() const;
		
		QString description() const;
		
		void updateStatistics();
		
	public slots:
		/**
		 * starts the game
		 */
		void start();
		void reset(){};

	private:
		Ui::DecompositionForm* m_statForm;

	protected:
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
		
		bool changeToDo;

		void rollDice();
		
		QString rules() const;
		
		QByteArray name() const;
		
		QString description() const;
		
		void updateStatistics();
		
	public slots:
		/**
		 * starts the game
		 */
		void start();
		void reset(){};

	private:
		void changePosition( Stone * stone , bool lower );
		
		Ui::BoltzmannForm* m_statForm;
};

/**
 * @author Carsten Niehaus
 */
class LightabsorptionSimulation : public Simulation
{
	public:
		LightabsorptionSimulation();
		
		static LightabsorptionSimulation* instance();
		
		void rollDice();
		
		QString rules() const;
		
		QByteArray name() const;
		
		QString description() const;
		
		void updateStatistics();
		
	public slots:
		/**
		 * starts the game
		 */
		void start();
		
		void reset(){};

	private:
		int m_col;

		QList<int> m_ypositions;

		Ui::LightabsorptionForm * m_statForm;
};

class VolterraSimulation : public Simulation
{
	public:
		VolterraSimulation();

		static VolterraSimulation* instance();

		void rollDice();
		
		QString rules() const;
		
		QByteArray name() const;
		
		QString description() const;
		
		void updateStatistics();
		
	public slots:
		/**
		 * starts the game
		 */
		void start();

		void reset();

	private:
		Ui::VolterraForm * m_statForm;

		Stone::PLAYER m_player;
};

#endif // GAMESIMPLEMENTATION_H
