/**********************************************************************
 PeriodicTableScene - Periodic Table Graphics Scene for Avogadro

 Copyright (C) 2007-2009 by Marcus D. Hanwell

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

#ifndef PERIODICTABLESCENE_P_H
#define PERIODICTABLESCENE_P_H

#include <QGraphicsScene>
#include <QTimer>
#include <QPointF>

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
    /**
     * Constructor.
     */
    PeriodicTableScene(QObject *parent = 0);
    ~PeriodicTableScene();

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
    /**
     * Generic event handler, currently defaults to calling parent class
     * (included for future compatibility)
     */
    bool event(QEvent *e);

    /**
     * Handles the mouse press events to change the active element.
     */
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    /**
     * Not used at present.
     */
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    /**
     * Opens the Detailed view of the element.
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  };


#endif // PERIODICTABLESCENE_P_H
