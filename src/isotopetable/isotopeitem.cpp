/***************************************************************************
 *   Copyright (C) 2005-2008 by Carsten Niehaus <cniehaus@kde.org>         *
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

#include "isotopeitem.h"

#include "isotopescene.h"

#include <element.h>
#include <isotope.h>

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include <KLocalizedString>

IsotopeItem::IsotopeItem(Isotope *i, qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
    : QAbstractGraphicsShapeItem(parent)
{
    m_rect = QRectF(0, 0, width, height);
    setPos(x, y);
    m_isotope = i;

    m_type = getType(m_isotope);

    QBrush b;
    switch (m_type) {
    case alpha:
        b = QBrush(Qt::red);
        break;
    case ec:
        b = QBrush(Qt::blue);
        break;
    case multiple:
        b = QBrush(Qt::green);
        break;
    case bplus:
        b = QBrush(Qt::yellow);
        break;
    case bminus:
        b = QBrush(Qt::white);
        break;
    case unknown:
        b = QBrush(Qt::darkGray);
        break;
    case stable:
        b = QBrush(Qt::magenta);
        break;
    default:
        b = QBrush(Qt::darkGray);
        break;
    }
    setBrush(b);

    m_symbolFont = QFont(QStringLiteral("Arial"), 3 ,QFont::Bold);
    m_otherFont = QFont(QStringLiteral("Arial"), 1 ,QFont::Bold);

    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    setToolTip(i18n("Isotope of Element %1 (%2)",
                    m_isotope->parentElementNumber(),
                    m_isotope->parentElementSymbol()));
}

void IsotopeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)

    const qreal lod = option->levelOfDetailFromTransform(painter->worldTransform());
    if (lod > 0.3) {
        painter->setPen(pen());
    } else {
        painter->setPen(Qt::NoPen);
    }
    painter->setBrush(brush());
    painter->drawRect(m_rect);

    if (lod >= 1.0) {
        // FIXME: Get rid of magic numbers and rather dynamically calculate them
        QRectF r1(m_rect.translated(0.0, 2.5));

        painter->setFont(m_symbolFont);
        painter->drawText(r1, Qt::AlignHCenter | Qt::TextDontClip, m_isotope->parentElementSymbol());//, s->parentElementNumber()

        if (lod >= 4.0) {
            QRectF r2(m_rect.topLeft() + QPointF(1.0, 0.5), m_rect.size() / 2.0);
            QRectF r3(m_rect.topLeft() + QPointF(6.0, 0.5) , m_rect.size() / 2.0);

            painter->setFont(m_otherFont);
            painter->drawText(r2,
                              Qt::AlignHCenter | Qt::TextDontClip,
                              QString::number(m_isotope->parentElementNumber()));
            painter->drawText(r3,
                              Qt::AlignHCenter | Qt::TextDontClip,
                              QString::number(m_isotope->nucleons()));
        }
    }
}

IsotopeItem::IsotopeType IsotopeItem::getType(Isotope * isotope)
{
    //TODO Here I need a clever way to find out *what* to return.
    if (isotope->alphalikeliness() > 60.0) {
        return IsotopeItem::alpha;
    }
    if (isotope->betaminuslikeliness() > 60.0 || isotope->betaminusneutronlikeliness() > 60.0 || isotope->betaminusfissionlikeliness() > 60.0) {
        return IsotopeItem::bminus;
    }
    if (isotope->betapluslikeliness() > 60.0 || isotope->betaplusalphalikeliness() > 60.0 || isotope->betaplusalphalikeliness() > 60.0) {
        return IsotopeItem::bplus;
    }
    if (isotope->eclikeliness() > 60.0) {
        return IsotopeItem::ec;
    }
    if (!(isotope->halflife() > 0.0) && !(isotope->abundance().isEmpty())) {
        return IsotopeItem::stable;
    } else {
        return IsotopeItem::unknown;
    }
}

void IsotopeItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() != Qt::RightButton) {
        event->ignore();
        return;
    }

    IsotopeScene *scene2 = static_cast<IsotopeScene*>(scene());
    scene2->updateContextHelp(this);
}
