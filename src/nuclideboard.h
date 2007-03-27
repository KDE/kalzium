#ifndef NUCLIDEBOARD_H
#define NUCLIDEBOARD_H
/***************************************************************************
 *   Copyright (C) 2005, 2006 by Carsten Niehaus                                 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/

#include <QWidget>
#include <QList>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QScrollArea>

#include <kdialog.h>

class QColor;
class QPainter;
class IsotopeScrollArea;
class KActionCollection;
class Element;
class Isotope;

/**
 * This class is the drawing widget for the whole table
 *
 * @author Pino Toscano
 * @author Carsten Niehaus
 */
class IsotopeTableView : public QWidget
{
	Q_OBJECT

	public:
		IsotopeTableView( QWidget* parent = 0, IsotopeScrollArea* scroll = 0 );
	
	public slots:
		/**
		 * Update the QMap of IsotopeAdapter. Only visible isotopes
		 * will be in the list. Therefore, in the paintEvent the class
		 * can simply iterate through all keys and paint them
		 */
		void updateIsoptopeRectList( bool redoSize = false );

		/**
		 * Calculate the new area to show, using @p selectedRect ,
		 * and update the list of isotopes to be drawn
		 */
		void selectionDone( const QRect& selectedRect );

		void slotZoomIn();
		void slotZoomOut();
		void slotToogleZoomMode( bool );

	signals:
		void toggleZoomAction( bool );

	private:
		QWidget *m_parent;
		IsotopeScrollArea *m_scroll;

		QPixmap m_pix;

		/**
		 * Finds the isotope of the element @p el with the lower number
		 * of nucleons and returns that number of nucleons.
		 * The @p lowerbound parameter indicates which is the minimum
		 * index allowed.
		 */
		int minNucleonOf( Element* el, int lowerbound = 0 ) const;

		/**
		 * Finds the isotope of the element @p el with the higher number
		 * of nucleons and returns that number of nucleons.
		 * The @p upperbound parameter indicates which is the maximum
		 * index allowed.
		 */
		int maxNucleonOf( Element* el, int upperbound = 500 ) const;

		/**
		 * @returns a list of the isotopes of the element @p el whose
		 * nucleons are in the range [ @p lowerbound , @p upperbound ]
		 */
		QList<Isotope*> isotopesWithNucleonsInRange( Element* el, int lowerbound, int upperbound ) const;

		/**
		 * @returns a rect containing the new "coordinates" of the
		 * area to show
		 */
		QRect getNewCoords( const QRect& rect ) const;

		/**
		 * The current size of a drawn isotope
		 */
		int m_rectSize;

		/**
		 * @return the color(s) of the isotope @p isotope
		 */
		QList<QColor> isotopeColors( Isotope* isotope );

		QMap<Isotope*, QRect> m_IsotopeAdapterRectMap;

		QPoint m_firstPoint;

		int m_firstElem;
		int m_lastElem;
		int m_firstElemNucleon;
		int m_lastElemNucleon;
		
		QRect m_selectedRegion;
		
		/**
		 * true if user is currently mouse the pressed mouse
		 */
		bool m_duringSelection;

		bool m_isMoving;

	protected:
		virtual void paintEvent( QPaintEvent *e );
		
		virtual void mousePressEvent( QMouseEvent *e );
		virtual void mouseReleaseEvent( QMouseEvent *e );
		virtual void mouseMoveEvent( QMouseEvent *e );
		
		void drawInternally();
		/**
		 * draw the isotope widgets
		 */
		void drawIsotopeWidgets( QPainter *p );
		/**
		 * draw the two legends
		 */
		void drawLegends( QPainter *p );
};

/**
 * The dialog representing the isotope table.
 *
 * @author Martin Pfeiffer
 * @author Carsten Niehaus
 */
class IsotopeTableDialog : public KDialog
{
	Q_OBJECT
	public:
		IsotopeTableDialog( QWidget* parent );
		~IsotopeTableDialog(){}

		KActionCollection* actionCollection();

	private:
		IsotopeTableView* m_view;
		KActionCollection* m_ac;

	private slots:
		/**
		 * invokes the help for this widget
		 */
		virtual void slotHelp();
};


/**
 * Simply widget that represents the legend of the isotope table.
 *
 * @author Martin Pfeiffer
 */
class NuclideLegend : public QWidget
{
	public:
		NuclideLegend( QWidget* parent );
		~NuclideLegend() {}
		
	protected:
		virtual void paintEvent( QPaintEvent* );
};


/**
 * Small QScollArea derived class to allow the scrolling even outside
 * the class' methods.
 *
 * @author Pino Toscano
 */
class IsotopeScrollArea : public QScrollArea
{
	Q_OBJECT
	public:
		IsotopeScrollArea( QWidget* parent );
		~IsotopeScrollArea() {}

		void scrollBy( int x, int y );
};

#endif // NUCLIDEBOARD_H
