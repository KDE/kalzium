#ifndef SIMULATIONFIELD_H
#define SIMULATIONFIELD_H
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
#include <QFrame>

class QPaintEvent;

class Field;
class Move;

/**
 * @author Carsten Niehaus
 * @brief the graphical representation of the field
 */
class SimulationfieldWidget : public QFrame
{
	Q_OBJECT
	
	public:
		/**
		 * public Constructor
		 */
		SimulationfieldWidget( QWidget * parent = 0 );
		
		/**
		 * Destructor
		 */
		virtual ~SimulationfieldWidget(){};

		void setField( Field * field ){
			m_field = field;
			update();
		}
		
		/**
		 * defines the look of the tokens
		 */
		enum TokenDesign
		{
			CIRCLE = 0/**<Circles*/,
			SQUARE/**<Squares*/
		};

		/**
		 * Sets the design of the field
		 * @param design The TokenDesign which will be used in the paintEvent
		 */
		void setDesign( TokenDesign design ){
			m_design = design;
		}

	public slots:
		/**
		 * redraw the field
		 */
		void slotUpdate( Move * move ){
			( void ) move;
			update();	
		}

	private:
		Field * m_field;

		TokenDesign m_design;

	protected:
		virtual void paintEvent( QPaintEvent * e );
};
#endif // SIMULATIONFIELD_H
