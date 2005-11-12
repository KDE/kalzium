/***************************************************************************
 *   Copyright (C) 2003-2005 by Carsten Niehaus                            *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.             *
 ***************************************************************************/

#ifndef PeriodicTableView_H
#define PeriodicTableView_H

class Element;
class KalziumTip;
class KalziumPainter;
class KalziumGradientType;

#include <QWidget>
#include <QTimer>
#include <QPoint>
#include <QPixmap>
#include <QList>
#include <QMap>

#include "kalziumpainter.h"
#include "chemicaldataobject.h"

/**
 * A PeriodicTableView is the widget on where we paint a periodic table.
 *
 * It does not contain any code for painting, as all the painting routines
 * belongs to KalziumPainter.
 *
 * @author Carsten Niehaus
 */
class PeriodicTableView : public QWidget
{
	Q_OBJECT

	public:
		/**
		 * Constructor
		 * @param parent parent widget
		 */
		PeriodicTableView( QWidget *parent = 0 );
		~PeriodicTableView();

		/**
		 * Sets the numeration type to the one with index @p which.
		 */
		void setNumeration( int which );

		/**
		 * @return whether the tooltips are enabled
		 */
		bool tooltipsEnabled() const{
			return m_tooltipsEnabled;
		}

		/**
		 * put the limit for the @p type specified, but do not actually (de)activate it
		 * @param type type of the @value
		 * @param value the value of the the @p type
		 */
		void setValueLimit( int value, ChemicalDataObject::BlueObelisk type );

		/**
		 * really activate the "graying" mode ( if @p toggle is true ), for the @p type specified
		 * or deactivate the "graying" mode  (if @p toggle is false)
		 */
		void toggleLimit( bool toggle, ChemicalDataObject::BlueObelisk type );


		/**
		 * if @p enabled is true the tooltips
		 * will be enabled
		 */
		void setTooltipsEnabled( bool enabled ){
			m_tooltipsEnabled = enabled;
		}

		/**
		 * Sets the scheme to use.
		 * @param nr is the index of the new scheme
		 */
		void activateColorScheme( const int nr);

		/**
		 * @return the short and descriptive name of this PeriodicTableView
		 */
		QString shortName() const{
			return m_ShortName;
		}

		/**
		 * if @p show is true the tooltip will be displayed
		 */
		void setShowTooltip( bool show ){
			m_showTooltip = show;
		}

		/**
		 * @return whether tooltips will be displayed
		 */
		bool showTooltip() const {
			return m_showTooltip;
		}

		/**
		 * @return the current gradient type
		 */
		KalziumGradientType* gradient() const;
		
		void setGradient( int which );

		/**
		 * if false, the user disabled tooltips
		 */
		bool m_tooltipsEnabled;

		bool m_showLegendTooltip;

		/**
		 * activates or deactivates the legend
		 */
		void showLegend( bool show );
		bool showLegend() const;

		void setMode( KalziumPainter::MODE m );
		KalziumPainter::MODE mode() const;

		/**
		 * @param type the type of value which is searched for
		 * @return the value of the @p type
		 */
		int sliderValue( ChemicalDataObject::BlueObelisk type );

	private:
		/**
		 * if true the tooltips will be displayed
		 */
		bool m_showTooltip;

		QMap <ChemicalDataObject::BlueObelisk, int> m_sliderValueList;
		
		KalziumTip* m_kalziumTip;
		
		/**
		 * timer used for the tooltip
		 */
		QTimer HoverTimer;
		QTimer MouseoverTimer;

		/**
		 * the number of the element the mouse-cursor is over
		 */
		int m_tooltipElementNumber;

		/**
		 * the currently selected element
		 */
		int m_currentElement;

		/**
		 * this is a short, descriptive name of the PeriodicTableView
		 */
		QString m_ShortName;

		/**
		 * implements double buffering of the widget.
		 */
		QPixmap *table;			// The basic PeriodicTableView
		QPixmap *table2;		// Basic PeriodicTableView + extra data such as tooltip, etc

		/**
		 * used for bitBlit. If true the complete table will be drawn
		 */
		bool doFullDraw;

		KalziumPainter *m_painter;

		QPoint m_startDrag;

	protected:
		virtual void paintEvent( QPaintEvent *e );
		virtual void mouseReleaseEvent( QMouseEvent* );
		virtual void mousePressEvent( QMouseEvent* event );
		virtual void mouseMoveEvent( QMouseEvent* );

		/**
		 * draw the tooltip for the legend
		 */
		virtual void drawLegendToolTip( QPainter *p );

		/**
		 * JH: Draw the full table next time
		 */
		void setFullDraw();

	public slots:
		/**
		 * This method sets the color for the buttons corresponding to
		 * the given temperature @p temp
		 * @param temp is the temperature to which all buttons will be set
		 */
		void setTemperature( int temp );

		/**
		 * this slot removes the selection of any point
		 */
		void unSelect();

	private slots:
		/**
		 * If called this slot will emit the signal MouseOver( num )
		 * where num is the number of the element the mouse if over.
		 * If the mouse is not over an element nothing will be emited
		 * @see MouseOver()
		 */
		void slotMouseover();
		
		/**
		 * start the calculation of the element over which the mouse-cursor
		 * is over. Finally the signal ToolTip( int ) is emitted
		 */
		void slotTransientLabel();
		
		/**
		 * this slot updates the currently selected point
		 */
		void selectPoint( const QPoint& point );

		/**
		 * this slot updates the element given in the @p num
		 * @param num The number of the selected element
		 */
		void selectElement( int num );

		/**
		 * sets the current element to @p number
		 * and updates the table
		 */
		void slotToolTip( int number );
		
	signals:
		/**
		 * this signal is emitted when an element is clicked
		 */
		void ElementClicked(int);

		/**
		 * this signal is emitted when the tooltip of an element
		 * has to be displayed
		 */
		void ToolTip(int);

		/**
		 * This signal is emited when the mouse pointer is
		 * over an element
		 */
		void MouseOver( int );
};


#endif
