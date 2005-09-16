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

#include "plotwidget.h"
#include "plotwidget.moc"

//KDE-Includes
#include <kdebug.h>

//QT-Includes
#include <qpainter.h>

PlotWidget::PlotWidget( double x1,
		double x2,
		double y1,
		double y2,
		QWidget *parent,
		const char* name)
		: KPlotWidget( x1, x2, y1, y2, parent, name )
{
	m_connectPoints = false;
}

void PlotWidget::drawObjects( QPainter *p )
{
	// let the KPlotWidget draw the default stuff first
	KPlotWidget::drawObjects(p);
	// then draw the connecting lines 
	if (!m_connectPoints) return; // bail out if connect points is not enabled

	QPoint old; // used to remember last coordinates
	bool first = true;
	for ( KPlotObject *po = ObjectList.first(); po; po = ObjectList.next() ) 
	{
		// skip empty plot objects
		if ( po->points()->count() == 0 ) continue;
		// skip non-point plot objects
		if (po->type() != KPlotObject::POINTS) continue;

		// draw the connecting lines
		p->setPen( QColor( po->color() ) );
		p->setBrush( QColor( po->color() ) );
		for ( DPoint *dp = po->points()->first(); dp; dp = po->points()->next() ) 
		{
			QPoint q = dp->qpoint( PixRect, DataRect );

			if ( first ) 
				first = false;
			else
				p->drawLine(old.x(), old.y(), q.x(), q.y());

			old.setX( q.x() );
			old.setY( q.y() );
		}
		p->setBrush( Qt::NoBrush );
	}
}

