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

class IsotopeAdapter
{
	public:
		IsotopeAdapter()
		{
			QUuid uuid;
			mUID = uuid.toString();
		}

		///the istopte this widgets represents
		Isotope* m_isotope;

		///the postition (x,y) in the "table" of the isotopes.
		//If the grid is 20x30 for example, the x and y values
		//cannot be bigger than 20 or 30. 
		QPoint m_point;

		bool operator< ( const IsotopeAdapter &adapter ) const
		{
			return true;
		}
		
	private:
		QString mUID;
};

class IsotopeTableView : public QWidget
{
	friend class IsotopeTableDialog;
	
	Q_OBJECT

	public:
		IsotopeTableView( QWidget* parent = 0, const char * name = 0 );
	
	public slots:
		/**
		 *	Dieses Ding updated nach jedem zoom die Liste mit QMap<Isotope*, QRect>
		 *	D.h. dass in ihr all sichtbaren Isotope incl. Größe und Position drin.
		 *	Mit Schnittmengenberechnung kann bei zoom-in also geschaut werden,
		 *	welche Isotope in der Schnittmenge liegen. Die anderen fliegen raus.
		 */
		void updateIsoptopeRectList();

		void selectionDone( QRect selectedRect );
		
	private: IsotopeList m_isotopeList;

		int highestNeutronCount(); int lowestNeutronCount();

		QValueList<Element*>::ConstIterator m_startElementIterator;
		QValueList<Element*>::ConstIterator m_stopElementIterator;
		
		QValueList<Element*> m_list;

		//the lowest and highest number of neutrons, of all visible elements
		int m_lowestNumberOfNeutrons; int m_highestNumberOfNeutrons;

		static int m_minIsoSize;	// size of a isotopeWidget on the board
		static int m_maxIsoSize;	// size of a isotopeWidget on the board

		int m_rectSize;

		/** @return the color of the isotope
		 */
		QColor isotopeColor( Isotope* );

		QMap<IsotopeAdapter, QRect> m_IsotopeAdapterRectMap;

		QPoint m_firstPoint;

		QPoint m_topLeft;
		QPoint m_bottomRight;
		
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

	private slots:
		/**
		 * invokes the help for this widget
		 */
		void slotHelp();

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
