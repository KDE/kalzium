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

#include "isotopeview.h"

#include "isotopescene.h"

IsotopeView::IsotopeView(QWidget *parent) : QGraphicsView(parent)
{
    m_scene = new IsotopeScene(this);
    m_zoomLevel = 1.0;
    setScene(m_scene);
    setSceneRect(m_scene->itemsBoundingRect());
    ensureVisible(m_scene->sceneRect());
}

IsotopeView::~IsotopeView()
{
    delete scene();
}

void IsotopeView::resizeEvent(QResizeEvent *event)
{
    //ensureVisible(QRectF(0,0,100,100),0,0);

    //event->accept();
    event->ignore();
}

void IsotopeView::mouseMoveEvent(QMouseEvent *event)
{
    QPolygonF visibleSceneRect = mapToScene(viewport()->rect());
    emit visibleSceneRectChanged(visibleSceneRect);

    QGraphicsView::mouseMoveEvent(event);
}

void IsotopeView::wheelEvent(QWheelEvent *event)
{
    double oldZoomLevel = m_zoomLevel;
    double factor;

    if (event->delta() > 0) {
        factor = event->delta() / 100.0;
        m_zoomLevel *= factor;
    } else {
        factor = 1.0 / (-event->delta() / 100.0);
        m_zoomLevel *= factor;
    }

    if (m_zoomLevel < 0.5) {
        m_zoomLevel = 0.5;
    } else if (m_zoomLevel > 10.0) {
        m_zoomLevel = 10.0;
    }

    if (oldZoomLevel != m_zoomLevel) {
        factor = m_zoomLevel / oldZoomLevel;
        scale(factor, factor);
        emit zoomLevelChanged(m_zoomLevel);
    }
    QPolygonF visibleSceneRect = mapToScene(viewport()->rect());
    emit visibleSceneRectChanged(visibleSceneRect);

    event->accept();
}

void IsotopeView::setZoom(double zoom)
{
    double oldZoomLevel = m_zoomLevel;
    double factor;

    m_zoomLevel = zoom;

    if (m_zoomLevel < 0.5) {
        m_zoomLevel = 0.5;
    } else if (m_zoomLevel > 10.0) {
        m_zoomLevel = 10.0;
    }

    factor = m_zoomLevel / oldZoomLevel;
    scale(factor,factor);
    emit zoomLevelChanged(m_zoomLevel);
    QPolygonF visibleSceneRect = mapToScene(viewport()->rect());
    emit visibleSceneRectChanged(visibleSceneRect);
}

