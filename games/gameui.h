#ifndef GAMEUI_H
#define GAMEUI_H
/***************************************************************************
*    copyright            : (C) 2006 by Carsten Niehaus
*    email                : cniehaus@kde.org
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

class GameWidget : public QWidget
{
	Q_OBJECT

	public:
		GameWidget( QWidget * parent );

		void setField( Field * field ){
			m_field = field;
		}

	private:
		Field * m_field;

	protected:
		virtual void paintEvent( QPaintEvent * e );
};
#endif // GAMEUI_H
