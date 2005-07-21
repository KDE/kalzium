#ifndef NUCLIDEBOARD_H
#define NUCLIDEBOARD_H
/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
 *   
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

#include <kdialog.h>
#include <qwidget.h>
#include <qpainter.h>
#include <isotope.h>
#include <qvaluelist.h>
#include "element.h"

class IsotopeWidget;

/**
 * @author Jörg Buchwald
 * @author Carsten Niehaus
 * 
 */
class NuclideBoard : public QWidget
{
	Q_OBJECT

	public:
		NuclideBoard(QValueList<Element*> list, QWidget* parent = 0, const char* name = 0);

		~NuclideBoard(){};

	private:
		QValueList<Element*> m_list;

		QValueList<IsotopeWidget*> m_isotopeWidgetList;

		void updateList();

		int highestNeutronCount();
		
		int lowestNeutronCount();

		int m_start;

		int m_stop;

	public slots:
		void setStart( int v ){
			m_start = v;
			updateList();
			update();
		}

		void setStop( int v ){
			m_stop = v;
			updateList();
			update();
		}

	protected:
		virtual void paintEvent(QPaintEvent*);
};

class IsotopeWidget
{
public:
		IsotopeWidget( Isotope* isotope );
		~IsotopeWidget(){};

		void setSize( int size ){
			m_size = size;
		}

		void setPoint( QPoint p ){
			m_point = p;
		}

		void drawSelf( QPainter* p );

private:
		Isotope* m_isotope;

		QPoint m_point;
		int m_size;
};

class Decay
{
public:
	Decay(QValueList<IsotopeWidget*> list){
		m_list = list;
	};

	~Decay(){};

private:
	QValueList<IsotopeWidget*> m_list;

private:
};


#endif // NUCLIDEBOARD_H
