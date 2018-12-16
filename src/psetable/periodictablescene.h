/**********************************************************************
 PeriodicTableScene - Periodic Table Graphics Scene for Kalzium

 Copyright (C) 2005-2006 by Pino Toscano, toscano.pino@tiscali.it
 Copyright (C) 2003-2006 by Carsten Niehaus, cniehaus@kde.org
 Copyright (C) 2007-2009 by Marcus D. Hanwell, marcus@cryos.org
 Copyright (C) 2010 by Etienne Rebetez, etienne.rebetez@oberwallis.ch

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

#ifndef PERIODICTABLESCENE_H
#define PERIODICTABLESCENE_H

#include <QGraphicsScene>
#include <QPointF>
#include <QTimer>

#include "elementitem.h"

/**
 * @class PeriodicTableScene
 * @author Marcus D. Hanwell
 * @brief This class encapsulates the scene, all items are contained in it.
 *
 * This class implements a QGraphicsScene that holds all of the element items.
 * Any items owned by this class are automatically deleted by it.
 */
class PeriodicTableScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit PeriodicTableScene(QObject *parent = nullptr);
    ~PeriodicTableScene() override;

Q_SIGNALS:
    /**
     * This signal is emitted when an element item is released.
     */
    void elementChanged(int element);
    /**
     * This signal is emitted when an element item is hovered.
     */
    void elementHovered(int element);
    /**
     * This signal is emitted when no element was clicked.
     */
    void freeSpaceClick();

private Q_SLOTS:
    void slotMouseover();

private:
    QTimer m_hoverTimer;
    int m_prevHoverElement;
    QPointF m_eventPos;

protected:
    bool event(QEvent *e) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // PERIODICTABLESCENE_H
