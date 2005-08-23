#ifndef NUCLIDEBOARD_H
#define NUCLIDEBOARD_H
/***************************************************************************
 *   Copyright (C) 2005 by Carsten Niehaus                                 *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <qwidget.h>
#include <qvaluelist.h>
#include <qpoint.h>
#include <qrect.h>

#include <kdialogbase.h>

class QColor;
class QPainter;
class QScrollView;
class Element;
class Isotope;

typedef QValueList<Isotope*> IsotopeList;
typedef QValueList<Element*> ElementList;

/**
 * This class is the drawing widget for the whole table
 *
 * @author Carsten Niehaus
 */
class IsotopeTableView : public QWidget
{
	friend class IsotopeTableDialog;
	
	Q_OBJECT

	public:
		IsotopeTableView( QWidget* parent = 0, QScrollView* scroll = 0, const char * name = 0 );
	
	public slots:
		/**
		 * Update the QMap of IsotopeAdapter. Only visible isotopes
		 * will be in the list. Therefore, in the paintEvent the class
		 * can simply iterate through all keys and paint them
		 */
		void updateIsoptopeRectList();

		/**
		 * Calculate the intersection of the selected region and the
		 * update the list of isotopes to be drawn
		 */
		void selectionDone( QRect selectedRect );
		
	private:
		QWidget *m_parent;
		QScrollView *m_scroll;

		int minNumberOfNucleons(
				QValueList<Element*>::ConstIterator it,
				QValueList<Element*>::ConstIterator itEnd );
		
		int maxNumberOfNucleons( 
				QValueList<Element*>::ConstIterator it,
				QValueList<Element*>::ConstIterator itEnd );

		int minNucleonOf( Element* el, int lowerbound = 0 ) const;

		int maxNucleonOf( Element* el, int upperbound = 500 ) const;

		QValueList<Isotope*> isotopesWithNucleonsInRange( Element* el, int lowerbound, int upperbound ) const;

		QRect getNewCoords( const QRect& rect ) const;

		/**
		 * The current size of a drawn isotope
		 */
		int m_rectSize;

		/**
		 * @return the color of the isotope @p isotope
		 */
		QColor isotopeColor( Isotope* isotope );

		QMap<Isotope*, QRect> m_IsotopeAdapterRectMap;

		QPoint m_firstPoint;

		void updateMinMaxValue();

		int m_firstElem;
		int m_lastElem;
		int m_firstElemNucleon;
		int m_lastElemNucleon;
		
		QRect m_selectedRegion;
		
		/**
		 * true if user is currently mouse the pressed mouse
		 */
		bool m_duringSelection;

	protected:
		virtual void paintEvent( QPaintEvent *e );
		
		virtual void resizeEvent( QResizeEvent *e );
		
		/**
		 * draw the x and y axis 
		 */
		void drawAxisLabels( QPainter *p );
		
		/**
		 * draw the isotope widgets
		 */
		void drawIsotopeWidgets( QPainter *p );
};

/**
 * The dialog representing the isotope table.
 *
 * @author Martin Pfeiffer
 * @author Carsten Niehaus
 */
class IsotopeTableDialog : public KDialogBase
{
	Q_OBJECT
	public:
		IsotopeTableDialog( QWidget* parent, const char* name = 0 );
		~IsotopeTableDialog(){};

	private:
		IsotopeTableView* m_view;

	protected slots:
		/**
		 * invokes the help for this widget
		 */
		virtual void slotHelp();

	signals:
		void selectionDone( QRect );
		
	protected:
		virtual bool eventFilter( QObject *obj, QEvent *ev );
};


/**
 * Simply widget that represents the legend of the isotope table.
 *
 * @author Martin Pfeiffer
 */
class NuclideLegend : public QWidget
{
	public:
		NuclideLegend( QWidget* parent, const char* name = 0 );
		~NuclideLegend() {};
		
	protected:
		virtual void paintEvent( QPaintEvent* );
};

#endif // NUCLIDEBOARD_H
