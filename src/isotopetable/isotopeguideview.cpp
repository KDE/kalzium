/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus <cniehaus@kde.org>              *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 ***************************************************************************/

#include "isotopeguideview.h"

#include "isotopescene.h"
#include <QScrollBar>

IsotopeGuideView::IsotopeGuideView(QWidget *parent) : QGraphicsView(parent)
{
    m_guidedView = nullptr;
    m_scale = 1.0;

    setCursor(Qt::OpenHandCursor);
}

void IsotopeGuideView::setGuidedView(IsotopeView *guidedView)
{
    m_guidedView = guidedView;
    connect(m_guidedView, &IsotopeView::zoomLevelChanged,
            this, &IsotopeGuideView::setZoomLevel);
    connect(m_guidedView, &IsotopeView::visibleSceneRectChanged,
            this, &IsotopeGuideView::setVisibleSceneRect);
    m_zoomLevel = m_guidedView->zoomLevel();

    setScene(m_guidedView->scene());
    setSceneRect(scene()->itemsBoundingRect());
    ensureVisible(scene()->sceneRect());
}

void IsotopeGuideView::setVisibleSceneRect(const QPolygonF& sceneRect)
{
    m_visibleSceneRect = sceneRect;
    viewport()->update();
}

void IsotopeGuideView::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (m_guidedView && m_visibleSceneRect.boundingRect().intersects(rect)) {
        painter->setPen(QPen(Qt::red));
        painter->drawRect(m_visibleSceneRect.boundingRect().adjusted(0, 0, -1, -1));
    }
}

void IsotopeGuideView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_scale = qMin(qreal(viewport()->width()) / scene()->width(),
                   qreal(viewport()->height()) / scene()->height());
    setTransform(QTransform::fromScale(m_scale, m_scale));
}

void IsotopeGuideView::mousePressEvent(QMouseEvent *event)
{
    m_dragEvent = mapFromScene(m_visibleSceneRect).boundingRect().contains(event->pos());
    if (m_dragEvent && event->buttons() & Qt::LeftButton) {
        m_lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void IsotopeGuideView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    m_dragEvent = false;
    setCursor(Qt::OpenHandCursor);
}

void IsotopeGuideView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragEvent && event->buttons() & Qt::LeftButton) {
        QPoint p1(m_guidedView->mapFromScene(mapToScene(m_lastMousePos)));
        QPoint p2(m_guidedView->mapFromScene(mapToScene(event->pos())));
        m_guidedView->horizontalScrollBar()->setValue(m_guidedView->horizontalScrollBar()->value()
                                                      + p2.x() - p1.x());
        m_guidedView->verticalScrollBar()->setValue(m_guidedView->verticalScrollBar()->value()
                                                      + p2.y() - p1.y());
        m_lastMousePos = event->pos();

        m_visibleSceneRect = m_guidedView->visibleSceneRect();
        viewport()->update();
    }
}

void IsotopeGuideView::setZoomLevel(double zoomLevel)
{
    m_zoomLevel = zoomLevel;
}

