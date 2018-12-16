/**********************************************************************
 NumerationItem - Numeration Item, part of the Periodic Table Graphics View
 for Kalzium

 Copyright (C) 2007-2009 by Marcus D. Hanwell marcus@cryos.org
 Some portions (C) 2010  by Konstantin Tokarev
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

#include "numerationitem.h"

#include <prefs.h>

#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QPainter>
#include <QStyleOption>

#include <KLocalizedString>

#include "kalziumnumerationtype.h"

NumerationItem::NumerationItem(int xPosition) : m_width(40), m_height(20),
        m_xPosition(xPosition)
{
    setNumerationType(Prefs::numeration());
}

NumerationItem::~NumerationItem()
{
}

QRectF NumerationItem::boundingRect() const
{
    return QRectF(0, 0, m_width, m_height);
}

QPainterPath NumerationItem::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, m_width, m_height);
    return path;
}

void NumerationItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    QPen pen;
    QLinearGradient grad(QPointF(0, 0), QPointF(0, m_height));
    grad.setColorAt(0, m_color);
    grad.setColorAt(1, m_color.darker());
    painter->setBrush(grad);
    pen.setColor(m_color.dark(1000));
    painter->setPen(pen);

    QRectF rect(0, 0, m_width, m_height);
    painter->drawRoundedRect(rect, m_width / 10, m_width / 10);
    painter->drawText(rect, Qt::AlignCenter, m_numeration);
}

void NumerationItem::setNumerationType(int type)
{
   type == 0 ? m_color = QColor(Qt::transparent) : m_color = QColor(Qt::white);

   m_numeration = KalziumNumerationTypeFactory::instance()->build(type)->item(m_xPosition);

   update();
}

