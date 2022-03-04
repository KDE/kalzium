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
    // Center the bottom left at the beginning
    // centerOn(0, sceneRect().height());
    // Zoom in a bit 
    // setZoom(3);
    // Makes sure that you always zoom to the mouse if you use the scroll wheel
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
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
    Q_EMIT visibleSceneRectChanged(visibleSceneRect);

    QGraphicsView::mouseMoveEvent(event);
}

void IsotopeView::wheelEvent(QWheelEvent *event)
{
    double oldZoomLevel = m_zoomLevel;
    double factor = event->angleDelta().y() / 1000.0;
    m_zoomLevel = oldZoomLevel + oldZoomLevel * factor;

    if (m_zoomLevel < 0.3) {
        m_zoomLevel = 0.3;
    } else if (m_zoomLevel > 10.0) {
        m_zoomLevel = 10.0;
    }

    if (oldZoomLevel != m_zoomLevel) {
        factor = m_zoomLevel / oldZoomLevel;
        scale(factor, factor);
        Q_EMIT zoomLevelChanged(m_zoomLevel);
    }
    QPolygonF visibleSceneRect = mapToScene(viewport()->rect());
    Q_EMIT visibleSceneRectChanged(visibleSceneRect);

    event->accept();
}

void IsotopeView::setZoom(double zoom)
{
    double oldZoomLevel = m_zoomLevel;
    double factor;

    m_zoomLevel = zoom;

    if (m_zoomLevel < 0.3) {
        m_zoomLevel = 0.3;
    } else if (m_zoomLevel > 10.0) {
        m_zoomLevel = 10.0;
    }

    factor = m_zoomLevel / oldZoomLevel;
    scale(factor,factor);
    Q_EMIT zoomLevelChanged(m_zoomLevel);
    QPolygonF visibleSceneRect = mapToScene(viewport()->rect());
    Q_EMIT visibleSceneRectChanged(visibleSceneRect);
}
