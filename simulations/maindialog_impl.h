#ifndef MAINDIALOG_IMPL_H
#define MAINDIALOG_IMPL_H

/***************************************************************************
*    copyright            : (C) 2006 by Carsten Niehaus
*    email                : cniehaus@kde.org
*    
***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <QDialog>

#include "maindialog.h"

/**
 * The main dialog of the simulations
 */
class MainDialog_Impl : public QDialog
{
	Q_OBJECT

	public:
		MainDialog_Impl( QWidget * parent = 0 );
		~MainDialog_Impl(){};

		Ui_MainDialog ui;

	private slots:
		/**
		 * Starts the selected Simulatino
		 */
		void startSelectedSimulation();
		
		void listRowChanged( int );
};

#endif // MAINDIALOG_IMPL_H
