/***************************************************************************
 *   Copyright (C) 2003 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef ELEMENTBUTTON_H
#define ELEMENTBUTTON_H

#include <qframe.h>
#include <qpainter.h>
#include <qstring.h>
#include "element.h"

class QMouseEvent;

/**
An ElementButton is the widget the users sees when looking at the table. 
It provides the drag&drop-actions and click-events. Furthermore it can change
it colors and hide/show itself.

@author Carsten Niehaus
*/
class ElementButton : public QFrame
{
	Q_OBJECT
	public:
		ElementButton(int number, Element *el, QWidget *parent = 0, const char *name = 0);

		~ElementButton();
		int ElementNumber();

		QString sym;

		/*
		 * the element this buttons represents
		 */
		Element *e;
		
		virtual void paintEvent( QPaintEvent* );

	private:
		/*
		 * the integer num represents the number of the element
		 */
		int m_ElementNumber;
		
		void mousePressEvent( QMouseEvent* );
		void mouseReleaseEvent( QMouseEvent* );

	signals:
		/*
		 * this signal emits the name ( the element-number )·
		 **/
		void num( int );

};

#endif
