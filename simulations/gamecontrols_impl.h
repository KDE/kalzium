#ifndef GAMECONTROLS_IMPL_H
#define GAMECONTROLS_IMPL_H
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

#include <QWidget>
#include <QPainter>

#include "simulation.h"
#include "ui_gamecontrols.h"

/**
 * @author Carsten Niehaus
 */
class GameControls_Impl : public QWidget
{
	Q_OBJECT

	public:
		GameControls_Impl( QWidget * parent = 0 );
		~GameControls_Impl(){};
	
		Ui_SimulationDialog ui;

	public slots:
		void showSettings();
		
};
#endif // GAMECONTROLS_IMPL_H

