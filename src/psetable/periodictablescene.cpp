/**********************************************************************
 PeriodicTableScene - Periodic Table Graphics Scene for Kalzium

 Copyright (C) 2005-2006 by Pino Toscano, toscano.pino@tiscali.it
 Copyright (C) 2003-2006 by Carsten Niehaus, cniehaus@kde.org
 Copyright (C) 2007-2009 by Marcus D. Hanwell, marcus@cryos.org
 Copyright (C) 2010 by Etienne Rebetez, etienne.rebetez@oberwallis.ch

 This file is part of the Kalzium molecular editor project.

 Kalzium is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 Kalzium is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 02110-1301, USA.

 **********************************************************************/

#include "periodictablescene.h"

#include <QApplication>
#include <QDrag>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMimeData>
#include <QPainter>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>

PeriodicTableScene::PeriodicTableScene(QObject *parent)
        : QGraphicsScene(parent),
          m_prevHoverElement(-1)
{
    QPalette widgetPalette = palette();
    setBackgroundBrush(QBrush(widgetPalette.window()));

    setItemIndexMethod(QGraphicsScene::NoIndex);

    m_hoverTimer.setSingleShot(true);
    connect(&m_hoverTimer, &QTimer::timeout, this, &PeriodicTableScene::slotMouseover);
}

PeriodicTableScene::~PeriodicTableScene()
{}

bool PeriodicTableScene::event(QEvent *e)
{
    return QGraphicsScene::event(e);
}

void PeriodicTableScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    QGraphicsItem *item = QGraphicsScene::itemAt(event->scenePos(), QTransform());
    if (item->data(0).toInt() > 0 && item->data(0).toInt() < 119) {
        m_eventPos = event->scenePos();
    } else {
        emit freeSpaceClick();
    }

    QGraphicsScene::mousePressEvent(event);
}

void PeriodicTableScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem *item = QGraphicsScene::itemAt(m_eventPos, QTransform());

    if ((QApplication::mouseButtons() & Qt::LeftButton) &&
            (event->pos() - m_eventPos).manhattanLength() > QApplication::startDragDistance() &&
            item->data(0).toInt() > 0) {
        Element* pointedElement = KalziumDataObject::instance()->element(item->data(0).toInt());

        QDrag *drag = new QDrag(event->widget());
        QMimeData *mimeData = new QMimeData;

        mimeData->setText(pointedElement->dataAsString(ChemicalDataObject::name));
        drag->setMimeData(mimeData);

        QPixmap pix(item->boundingRect().width() + 1, item->boundingRect().height() + 1);
        pix.fill(palette().color(QPalette::Window));

        QPainter painter(&pix);
        item->paint(&painter, new QStyleOptionGraphicsItem());

        drag->setPixmap(pix);
        drag->start(Qt::CopyAction | Qt::MoveAction);

        m_eventPos = QPoint();
    } else {
        m_eventPos = event->scenePos();

        if (m_hoverTimer.isActive()) {
            m_hoverTimer.stop();
        }
        m_hoverTimer.start(100);
    }

    QGraphicsScene::mouseMoveEvent(event);
}


void PeriodicTableScene::slotMouseover()
{
    QGraphicsItem *item = QGraphicsScene::itemAt(m_eventPos, QTransform());

    if (item->data(0).toInt() > 0 && item->data(0).toInt() < 119) {
        int num = item->data(0).toInt();
        if ((num > 0) && (num != m_prevHoverElement)) {
            emit elementHovered(num);
        }
        m_prevHoverElement = num;
    }
}


void PeriodicTableScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    QGraphicsItem *item = QGraphicsScene::itemAt(event->scenePos(), QTransform());
    if (item->data(0).toInt() > 0 && item->data(0).toInt() < 119) {
        emit(elementChanged(item->data(0).toInt()));
    }
    QGraphicsScene::mouseReleaseEvent(event);
}


