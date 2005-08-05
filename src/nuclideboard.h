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
#include <qscrollview.h>
#include <qpainter.h>
#include <isotope.h>
#include <qvaluelist.h>
#include "element.h"

class IsotopeWidget;
class Decay;
class QColor;
class QSpinBox;

/**
 * @author J�rg Buchwald
 * @author Carsten Niehaus
 *
 */
class IsotopeTable : public QScrollView
{
	Q_OBJECT

	public:
		/**
		 * Constructor.
		 *
		 * @param parent parent widget for this one
		 * @param name   object name
		 */
		IsotopeTable(QWidget* parent = 0, const char* name = 0);

		~IsotopeTable(){};

		IsotopeWidget* getIsotopeWidget( Isotope* isotope );	

	private:
		QValueList<Element*> m_list;

		QValueList<IsotopeWidget*> m_isotopeWidgetList;
		
		Decay* m_decay;

		void updateList();

		int highestNeutronCount();
		int lowestNeutronCount();

		int m_lowestNumberOfNeutrons;
		int m_highestNumberOfNeutrons;

		int m_start;
		int m_stop;

		int m_isoWidth;	// width of a isotopeWidget on the board

	public slots:
		/**
		 * defines the first isotope which will be displayed
		 * @param value the number of the element
		 */
		void setStart( int value );

		/**
		 * defines the last isotope which will be displayed
		 * @param value the number of the element
		 */
		void setStop( int value );

	private slots:
		/**
		 * Draw the decay of an isotope
		 * @param isotope the first Isotope in the row
		 */
		void slotDrawDecayRow( Isotope* isotope );

	signals:
		void emitStartValue( int );
		void emitStopValue( int );

	protected:
		void drawContents( QPainter * p, int clipx, int clipy, int clipw, int cliph );
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
		~IsotopeWidget();

		/**
		 * if a IsotopeWidget is activated it will
		 * look a bit diffent. This is used to show
		 * and highligt row of decay
		 * @param a if true the widget will be activated
		 */
		void activate( bool a ){
			m_active = a;
			update();
		}

		/**
		 * @return the Istope of this widget
		 */
		Isotope* isotope()const{
			return m_isotope;
		}

/*	protected slots:
		virtual void mousePressEvent ( QMouseEvent * e );				*/

	protected:
		virtual void paintEvent(QPaintEvent*);

	private:
		Isotope* m_isotope;

		QColor m_color;

		bool m_active;
/*
	signals:
		void clicked( Isotope* );*/
};

/**
 * @author Carsten Niehaus
 */
class Decay
{
	public:
		Decay( IsotopeTable* parent, Isotope* isotope, Element* element );
		~Decay(){};

		void showDecay();
		void hideDecay();

	private:
		QValueList<IsotopeWidget*> m_list;
		QValueList<Element*> m_elements;
		Isotope* m_startIsotope;
		Element* m_startElement;
		IsotopeTable* m_parent;
	
		void buildDecayRow();
		Isotope* getIsotope( int protones, int neutrons );
};

class IsotopeTableDialog : public KDialogBase
{
	Q_OBJECT
	public:
		IsotopeTableDialog( QWidget* parent, const char* name = 0 );
		~IsotopeTableDialog(){};

	private:
		QSpinBox *spin1, *spin2;
	
	private slots:
		/**
		 * invokes the help for this widget
		 */
		void slotHelp();
};



#endif // NUCLIDEBOARD_H
