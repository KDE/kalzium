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

#include <qpainter.h>

#include <kplotobject.h>

PlotWidget::PlotWidget( double x1,
		double x2,
		double y1,
		double y2,
		QWidget *parent )
		: KPlotWidget( x1, x2, y1, y2, parent )
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
	for ( QList<KPlotObject*>::ConstIterator it = ObjectList.begin(); it != ObjectList.constEnd(); ++it ) 
	{
		KPlotObject* po = *it;
		// skip empty plot objects
		if ( po->points()->count() == 0 ) continue;
		// skip non-point plot objects
		if (po->type() != KPlotObject::POINTS) continue;

		// draw the connecting lines
		p->setPen( po->color() );
		for ( QList<QPointF*>::ConstIterator dpit = po->points()->begin(); dpit != po->points()->constEnd(); ++dpit )
		{
			QPoint q = mapToPoint( **dpit );

			if ( first ) 
				first = false;
			else
				p->drawLine(old, q);

			old = q;
		}
	}
}

