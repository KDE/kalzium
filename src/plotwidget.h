#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H
/***************************************************************************
    copyright            : (C) 2004 by Carsten Niehaus
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

class PlotWidget : public KPlotWidget
{
	Q_OBJECT

	public:
		PlotWidget( double x1 = 0.0, double x2 = 0.0, double y1 = 0.0, double y2 = 0.0, QWidget *parent=0 , const char *name =0 );

		void setConnection( bool b ){
			m_connectPoints = b;
		}

	private:
		bool m_connectPoints;

	protected:
		virtual void drawObjects( QPainter *p);

};

#endif // PLOTWIDGET_H

