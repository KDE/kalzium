#ifndef GAMESDIALOG_IMPL_H
#define GAMESDIALOG_IMPL_H
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

#include "games.h"

class GamesDialog_Impl : public QWidget
{
	Q_OBJECT

	public:
		GamesDialog_Impl( QWidget * parent = 0 );
		~GamesDialog_Impl(){};
	
	private:
		Ui_Dialog ui;
		
};
#endif // GAMESDIALOG_IMPL_H
