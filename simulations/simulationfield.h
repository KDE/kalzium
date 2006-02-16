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
 */
class SimulationfieldWidget : public QFrame
{
	Q_OBJECT
	
	public:
		SimulationfieldWidget( QWidget * parent = 0 );
		virtual ~SimulationfieldWidget(){};

		void setField( Field * field ){
			m_field = field;
		}

		virtual QSize sizeHint() const;

	public slots:
		void slotUpdate( Move * move ){
			update();	
		}

	private:
		Field * m_field;

	protected:
		virtual void paintEvent( QPaintEvent * e );
};
#endif // SIMULATIONFIELD_H
