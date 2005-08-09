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
#include <qpoint.h>
#include <qrect.h>
#include <quuid.h>
#include "element.h"

class IsotopeWidget;
class QColor;
class QSpinBox;
class QEvent;

typedef QValueList<Isotope*> IsotopeList;
typedef QValueList<Element*> ElementList;

/**
 *@author Carsten Niehaus
 *This class is the drawing widget for the whole table
 */
class IsotopeTableView : public QWidget
{
	friend class IsotopeTableDialog;
	
	Q_OBJECT

	public:
		IsotopeTableView( QWidget* parent = 0, const char * name = 0 );
	
	public slots:
		/**
		 * update the QMap<> of of IsotopeAdapter. Only visible isotopes
		 * will be in the list. Therefor, in the paintEvent the class
		 * can simply iterate through all keys and paint them
		 */
		void updateIsoptopeRectList();

		/**
		 * Calculate the intersection of the selected region and the
		 * update the list of isotopes to be drawn
		 */
		void selectionDone( QRect selectedRect );
		
	private: 
		QValueList<Element*> m_list;

		static QPoint m_maxBottomRight;

		static int m_minIsoSize;	// min size of a isotopeWidget on the board
		static int m_maxIsoSize;	// max size of a isotopeWidget on the board

		int maxElementNumberDisplayed;
		int minElementNumberDisplayed;

		int minNumberOfNucleons(
				QValueList<Element*>::ConstIterator it,
				QValueList<Element*>::ConstIterator itEnd );
		
		int maxNumberOfNucleons( 
				QValueList<Element*>::ConstIterator it,
				QValueList<Element*>::ConstIterator itEnd );

		int m_rectSize; ///the current size of a drawn isotope

		/** @return the color of the isotope
		 */
		QColor isotopeColor( Isotope* );

		QMap<Isotope*, QRect> m_IsotopeAdapterRectMap;

		QPoint m_firstPoint;

		QPoint m_topLeft, m_oldTopLeft;
		QPoint m_bottomRight, m_oldBottomRight;
		
		QRect m_selectedRegion;
		
		bool m_duringSelection; ///true if user is currently mouse the pressed mouse

		 
	protected:
		virtual void paintEvent( QPaintEvent*e );
		
		/**
		 * draw the x and y axis 
		 */
		void drawAxisLabels( QPainter *p );
		
		/**
		 * draw the isotopewidgets
		 */
		void drawIsotopeWidgets( QPainter *p );
};

/**
 * @author Martin Pfeiffer
 * @author Carsten Niehaus
 *
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
