/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus                                 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/


#include "isotopeguideview.h"
#include "isotopescene.h"
#include <QScrollBar>
    
	IsotopeGuideView::IsotopeGuideView(QWidget *parent)
: QGraphicsView(parent)
{
    m_guidedView = 0;
    m_scale = 1.0;
    m_scene = new IsotopeScene(this);
    m_pixmapRepaintRequested = true;
    m_pixmap = 0;
    setScene(m_scene);
    setSceneRect(m_scene->itemsBoundingRect());
    ensureVisible(m_scene->sceneRect());

    setCursor( Qt::OpenHandCursor );
}

void IsotopeGuideView::setGuidedView(IsotopeView *guidedView)
{
    m_guidedView = guidedView;
    connect( m_guidedView, SIGNAL( zoomLevelChanged( double ) ),
             this,         SLOT( setZoomLevel( double ) ) );
    connect( m_guidedView, SIGNAL( visibleSceneRectChanged( const QPolygonF& ) ),
             this,         SLOT( setVisibleSceneRect( const QPolygonF& ) ) );
    m_zoomLevel = m_guidedView->zoomLevel();
}

void IsotopeGuideView::setVisibleSceneRect( const QPolygonF& sceneRect )
{
    m_visibleSceneRect = sceneRect;
    viewport()->update();
}

void IsotopeGuideView::drawItems( QPainter * painter, int numItems, QGraphicsItem ** items, const QStyleOptionGraphicsItem * options )
{
    if ( m_pixmapRepaintRequested ) {
        m_pixmap = new QPixmap( width(), height() );
        QPainter pixmapPainter( m_pixmap );
        pixmapPainter.fillRect( QRect( 0, 0, width(), height() ), Qt::white );
        //pixmapPainter.scale( m_scale, m_scale );
        pixmapPainter.setWorldTransform( painter->worldTransform() );
        pixmapPainter.setRenderHints( painter->renderHints() );
        QGraphicsView::drawItems( &pixmapPainter, numItems, items, options );
        m_pixmapRepaintRequested = false;
    }
    //painter->setClipRegion( QRegion() );
    painter->save();
    painter->resetTransform();
    painter->drawPixmap( 0, 0, *m_pixmap );
    painter->restore();
}

void IsotopeGuideView::drawForeground( QPainter *painter, const QRectF &rect )
{
    Q_UNUSED(rect)
    if ( m_guidedView )
    {
        painter->setPen( QPen( Qt::red ) );
        painter->drawPolygon( m_visibleSceneRect );
    }
}

void IsotopeGuideView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    double oldScale = m_scale;
    QSize size = event->size();
    double scaleX = size.width() / m_scene->width();
    double scaleY = size.height() / m_scene->height();
    m_scale = scaleX < scaleY ? scaleX : scaleY;

    double factor = m_scale / oldScale;
    scale( factor, factor );
    ensureVisible(m_scene->sceneRect());

    m_pixmapRepaintRequested = true;
}

void IsotopeGuideView::mousePressEvent(QMouseEvent *event)
{
    m_dragEvent = mapFromScene( m_visibleSceneRect ).boundingRect().contains( event->pos() );
    if ( m_dragEvent && event->buttons() & Qt::LeftButton ) {
        m_lastMousePos = event->pos();
        setCursor( Qt::ClosedHandCursor );
    }
}

void IsotopeGuideView::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragEvent = false;
    setCursor( Qt::OpenHandCursor );
}

void IsotopeGuideView::mouseMoveEvent(QMouseEvent *event)
{
    if ( m_dragEvent && event->buttons() & Qt::LeftButton ) {
        QPoint p1( m_guidedView->mapFromScene( mapToScene( m_lastMousePos ) ) );
        QPoint p2( m_guidedView->mapFromScene( mapToScene( event->pos() ) ) );
        m_guidedView->horizontalScrollBar()->setValue( m_guidedView->horizontalScrollBar()->value() + p2.x() - p1.x() );
        m_guidedView->verticalScrollBar()->setValue( m_guidedView->verticalScrollBar()->value() + p2.y() - p1.y() );
        m_lastMousePos = event->pos(); 

        m_visibleSceneRect = m_guidedView->visibleSceneRect();
        viewport()->update();
    }
}

void IsotopeGuideView::setZoomLevel( double zoomLevel )
{
    m_zoomLevel = zoomLevel;
}

#include "isotopeguideview.moc"
