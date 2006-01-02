#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H
/***************************************************************************
    copyright            : (C) 2004, 2005 by Carsten Niehaus
    email                : cniehaus@kde.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kplotwidget.h>

/**
 * @short This class add the ability to connect the plotted data using a line
 *
 * PlotWidget is basically like a KPlotWidget; the unique feature it has more
 * it's the ability to connect the drawn points with a line.
 * Used as an handy way to connect points instead of adding ourselves the
 * connection lines as KPlotObjects.
 *
 * @author Carsten Niehaus
 */
class PlotWidget : public KPlotWidget
{
	Q_OBJECT

	public:
		PlotWidget( double x1 = 0.0, double x2 = 0.0, double y1 = 1.0, double y2 = 1.0, QWidget *parent = 0 );

		/**
		 * toggles if the points will be connected with 
		 * a line or not
		 * @param b if true the points will be connected
		 */
		void setConnection( bool b ){
			m_connectPoints = b;
		}

	private:
		/**
		 * if true, the points will be connected
		 */
		bool m_connectPoints;

	protected:
		/**
		 * draws the objects
		 */
		virtual void drawObjects( QPainter *p);

};

#endif // PLOTWIDGET_H

