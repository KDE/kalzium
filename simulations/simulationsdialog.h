#ifndef GAMESDIALOG_H
#define GAMESDIALOG_H
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
#include <QDialog>

class GameControls_Impl;
class Simulation;
class StatisticWidget;

class QVBoxLayout;

/**
 * @author Carsten Niehaus
 */
class GamesDialog : public QDialog
{
	Q_OBJECT
	
	public:
		GamesDialog( Simulation * sim );

	public slots:
		void slotStartWithTimer();

	private:
		QVBoxLayout * vbox;
	
		Simulation * m_simulation;
	
		GameControls_Impl * m_controls;

		QWidget *statsWidget;

		void setupWidgets();

	private slots:
		/**
		 * start the Simulation
		 */
		void startSimulation();

		/**
		 * create the connections (signal/slot)
		 */
		void createConnetions();
		
		void calculateStatistics();

		void displayStatistics();

};

#endif // GAMESDIALOG_H
