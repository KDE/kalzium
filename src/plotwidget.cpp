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
		, m_connectPoints( false )
{
}

void PlotWidget::drawObjects( QPainter *p )
{
	// draw the connecting lines before, so the lines won't overlay over
	// the texts
	if ( m_connectPoints )
	{
		QPoint old; // used to remember last coordinates
		bool first = true;
		for ( QList<KPlotObject*>::ConstIterator it = ObjectList.constBegin(); it != ObjectList.constEnd(); ++it ) 
		{
			KPlotObject* po = *it;
			// skip:
			// - empty plot objects
			// - non-point plot objects
			if ( ( po->count() == 0 ) || ( po->type() != KPlotObject::POINTS ) )
				continue;

			// draw the connecting lines
			p->setPen( po->color() );
			for ( QList<QPointF*>::ConstIterator dpit = po->points()->constBegin(); dpit != po->points()->constEnd(); ++dpit )
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
	// then, let the KPlotWidget draw the stuff as usual
	KPlotWidget::drawObjects( p );
}

