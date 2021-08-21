/*
    SPDX-FileCopyrightText: 2007 Carsten Niehaus <cniehaus@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

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

    if (event->angleDelta().y() > 0) {
        factor = event->angleDelta().y() / 100.0;
        m_zoomLevel *= factor;
    } else {
        factor = 1.0 / (-event->angleDelta().y() / 100.0);
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

