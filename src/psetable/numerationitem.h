/**********************************************************************
 NumerationItem - Numeration Item, part of the Periodic Table Graphics View
 for Kalzium

 Copyright (C) 2007-2009 by Marcus D. Hanwell marcus@cryos.org
 Copyright (C) 2010      by Etienne Rebetez etienne.rebetez@oberwallis.ch

 This file is part of the Avogadro molecular editor project.
 For more information, see <https://avogadro.cc/>

 Kalzium is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 Avogadro is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 02110-1301, USA.
 **********************************************************************/

#ifndef NUMERATIONITEM_H
#define NUMERATIONITEM_H

#include <QGraphicsItem>

#include "kalziumdataobject.h"
#include <chemicaldataobject.h>

/**
 * @class NumerationItem
 * @author Marcus D. Hanwell
 * @author Etienne Rebetez
 * @brief An Numeration item, intended to display a id of the numeration row.
 *
 */
class NumerationItem : public QGraphicsObject
{
    Q_OBJECT

public:
    /**
     * Constructor. Should be called with the element number for this item. The
     * constructor uses setData to set the element number using the key 0. This
     * is then used by PeriodicTable to figure out which element was clicked on.
     */
    NumerationItem(int xPosition = 0);

    /**
     * Destructor.
     */
    ~NumerationItem();

    /**
     * @return the bounding rectangle of the element item.
     */
    QRectF boundingRect() const override;

    /**
     * @return the painter path which is also a rectangle in this case.
     */
    QPainterPath shape() const override;

    /**
     * This is where most of the action takes place. The element box is drawn
     * along with its symbol.
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

public Q_SLOTS:
    void setNumerationType(int type);

private:
    /**
     * Width of the elements.
     */
    int m_width;

    /**
     * Height of the elements.
     */
    int m_height;

    /**
     * The row Position of the Numeration item
     */
    int m_xPosition;

    /**
     * The numeration symbol.
     */
    QString m_numeration;

    /**
     * The color of the element.
     */
    QColor m_color;

};

#endif // NUMERATIONITEM_H
