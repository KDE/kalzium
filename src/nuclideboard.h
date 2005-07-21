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

#include <kdialogbase.h>
#include <qwidget.h>
#include <qpainter.h>
#include <isotope.h>
#include <qvaluelist.h>
#include "element.h"
#include "kalziumdataobject.h"

class IsotopeWidget;
class Decay;
class QColor;
class QSpinBox;

/**
 * @author Jörg Buchwald
 * @author Carsten Niehaus
 * 
 */
class NuclideBoard : public QWidget
{
	Q_OBJECT

	public:
		/**
		 * Constructor.
		 *
		 * @param list   the list of all elements
		 * @param parent parent widget for this one
		 * @param name   object name
		 */
		NuclideBoard(QValueList<Element*> list, QWidget* parent = 0, const char* name = 0);

		~NuclideBoard(){};

	private:
		QValueList<Element*> m_list;

		QValueList<IsotopeWidget*> m_isotopeWidgetList;
		
		QValueList<Decay*> m_decayList;

		void updateList();

		int highestNeutronCount();
		
		int lowestNeutronCount();

		int m_lowestNumberOfNeutrons;
		int m_highestNumberOfNeutrons;

		int m_start;

		int m_stop;

	public slots:
		/**
		 * defines the first isotope which will be displayed
		 * @param v the number of the element
		 */
		void setStart( int v ){
			if ( v > m_stop )
			{
				emitStartValue( m_start );
				return;
			}
			m_start = v;
			m_highestNumberOfNeutrons = highestNeutronCount();
			m_lowestNumberOfNeutrons = lowestNeutronCount();
			updateList();
		}

		/**
		 * defines the last isotope which will be displayed
		 * @param v the number of the element
		 */
	void setStop( int v ){
		if ( v < m_start )
		{
			emitStopValue( m_stop );
			return;
		}
		m_stop = v;
		m_highestNumberOfNeutrons = highestNeutronCount();
		m_lowestNumberOfNeutrons = lowestNeutronCount();
		updateList();
	}

	private slots:
		/**
		 * Draw the decay of an isotope
		 * @param the first istope in the row
		 */
		void slotDrawDecayRow( Isotope* isotope );

	signals:
		void emitStartValue( int );
		void emitStopValue( int );

	protected:
		virtual void paintEvent(QPaintEvent*);

		virtual void mousePressEvent( QMouseEvent* e );
};

/**
 * @author Carsten Niehaus
 */
class IsotopeWidget : public QWidget
{
	Q_OBJECT
	public:
		/**
		 * public constructor
		 * @param isotope the Isotope which this widget represents
		 * @param parent  parent widget for this widget
		 */
		IsotopeWidget( Isotope* isotope, QWidget *parent );
		~IsotopeWidget(){};

		/**
		 * defines the dimension of the widget
		 * @param size the size of the widget
		 */
		void setSize( int size ){
			m_size = size;
		}

		int size() const {
			return m_size;
		}

		/**
		 * define the coordinate of this widget
		 * @param p the coordinate
		 */
		void setPoint( QPoint p ){
			m_point = p;
		}

		/**
		 * in this method the widget paints itself
		 */
		void drawSelf( QPainter* p );

		/**
		 * if a IsotopeWidget is activated it will
		 * look a bit diffent. This is used to show
		 * and highligt row of decay
		 * @param a if true the widget will be activated
		 */
		void activate( bool a ){
			m_active = a;
		}

	private:
		Isotope* m_isotope;

		QColor m_color;

		QPoint m_point;
		int m_size;

		bool m_active;

	signals:
		void clicked( Isotope* );
};

/**
 * @author Carsten Niehaus
 */
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

class NuclideBoardDialog : public KDialogBase
{
	Q_OBJECT
	public:
		NuclideBoardDialog( KalziumDataObject *data, QWidget* parent, const char* name = 0 );
		~NuclideBoardDialog(){};

	private:
		QSpinBox *spin1, *spin2;
};



#endif // NUCLIDEBOARD_H
