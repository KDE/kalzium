/***************************************************************************

                          plotwidget.cpp  -  description
                             -------------------
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

#include "plotwidget.h"

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
	for ( KPlotObject *po = ObjectList.first(); po; po = ObjectList.next() ) {

		if ( po->points()->count() ) {
			//draw the plot object
			p->setPen( QColor( po->color() ) );

			switch ( po->type() ) {
				case KPlotObject::POINTS :
				{
					p->setBrush( QColor( po->color() ) );

					for ( DPoint *dp = po->points()->first(); dp; dp = po->points()->next() ) 
					{
						QPoint q = dp->qpoint( PixRect, DataRect );
						int x1 = q.x() - po->size()/2;
						int y1 = q.y() - po->size()/2;

						switch( po->param() ) 
						{
							case KPlotObject::CIRCLE : 
								p->drawEllipse( x1, y1, po->size(), po->size() ); 
								break;
							case KPlotObject::SQUARE : p->drawRect( x1, y1, po->size(), po->size() ); break;
							case KPlotObject::LETTER : p->drawText( q, po->name().left(1) ); break;
							default: p->drawPoint( q );
						}
					}
					if (m_connectPoints)
					{
						int 	p1x, p1y, //the first point
							p2x, p2y; //the second point
						
						DPoint *dp = po->points()->first();
						
						while ( dp )
						{
							QPoint point = dp->qpoint( PixRect, DataRect );
							p1x = point.x();
							p1y = point.y();

							p->drawLine(p1x,p1y,p2x,p2y);

							p2x = p1x;
							p2y = p1y;

							dp = po->points()->next();
						}
					}
					kdDebug() << "nach der for-Schleife" << endl;

					p->setBrush( Qt::NoBrush );
					break;
				}

				case KPlotObject::CURVE :
				{
					p->setPen( QPen( QColor( po->color() ), po->size(), (QPen::PenStyle)po->param() ) );
					DPoint *dp = po->points()->first();
					p->moveTo( dp->qpoint( PixRect, DataRect ) );
					for ( dp = po->points()->next(); dp; dp = po->points()->next() )
						p->lineTo( dp->qpoint( PixRect, DataRect ) );
					break;
				}

				case KPlotObject::LABEL : //draw label centered at point in x, and slightly below point in y.
				{
					QPoint q = po->points()->first()->qpoint( PixRect, DataRect );
					p->drawText( q.x()-20, q.y()+6, 40, 10, Qt::AlignCenter | Qt::DontClip, po->name() );
					break;
				}

				case KPlotObject::POLYGON :
				{
					p->setPen( QPen( QColor( po->color() ), po->size(), (QPen::PenStyle)po->param() ) );
					p->setBrush( po->color() );

					QPointArray a( po->count() );

					unsigned int i=0;
					for ( DPoint *dp = po->points()->first(); dp; dp = po->points()->next() )
						a.setPoint( i++, dp->qpoint( PixRect, DataRect ) );

					p->drawPolygon( a );
					break;
				}

				case KPlotObject::UNKNOWN_TYPE : break;
			}
		}
	}
}

#include "plotwidget.moc"
