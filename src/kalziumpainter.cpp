/***************************************************************************
 *   Copyright (C) 2005, 2006      by Pino Toscano, toscano.pino@tiscali.it*
 *   Copyright (C) 2006, 2007      by Carsten Niehaus, cniehaus@kde.org    *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.         *
 ***************************************************************************/

#define ELEMENTSIZE 40
#include "kalziumpainter.h"
#include "kalziumdataobject.h"
#include "kalziumtabletype.h"
#include "kalziumschemetype.h"
#include "kalziumgradienttype.h"
#include "kalziumnumerationtype.h"
#include "kalziumutils.h"
#include "element.h"
#include "prefs.h"
#include "search.h"

#include <QApplication>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QPainter>
#include <QRect>
#include <QPainterPath>
#include <QDebug>

#include <kdebug.h>
#include <kglobalsettings.h>
#include <klocale.h>

KalziumPainter::KalziumPainter( KalziumTableType *ktt )
        : m_painter( 0 )
{
    m_ktt = ktt;

    setScheme( 1 );
    setGradient( 0 );
    setNumeration( 1 );

    m_temperature = 0;

    m_mode = NORMAL;
}

void KalziumPainter::begin( QPaintDevice *pd )
{
    end();
    m_painter = new QPainter( pd );
}

void KalziumPainter::end()
{
    if ( m_painter ) {
        if ( m_painter->isActive() )
            m_painter->end();
        delete m_painter;
        m_painter = 0;
    }
}

KalziumTableType* KalziumPainter::currentTableType() const
{
    return m_ktt;
}

void KalziumPainter::drawAll()
{
    drawElements();
    drawNumeration();
}

void KalziumPainter::drawElements()
{
    QPaintDevice *dev = m_painter->device();
    QRect r( 0, 0, dev->width(), dev->height() );
    m_painter->fillRect( r, QApplication::palette().background() );

    // now iterate over all elements the table contains
    int num = m_ktt->firstElement();
    while ( num != -1 ) {
        drawElement( num );
        num = m_ktt->nextOf( num );
    }
    if (m_mode == NORMAL_GRADIENT) {
        QPen prevPen = m_painter->pen();
        QPen pen;

        QRect rect;
        QBrush c;

        if (currentTableType()->name() == "Classic") {
            if (m_scheme->name() == "Blocks") {
                // (H) - 1
                rect = m_ktt->elementRect(1);
                c = m_scheme->elementBrush(1, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border, rect.x() + rect.width() - border,
                                    rect.y() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + rect.height() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + 7 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + 7 * rect.height() - border,
                                    rect.x() + border, rect.y() + 7 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 7 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (B) - 5
                rect = m_ktt->elementRect(5);
                c = m_scheme->elementBrush(5, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border, rect.x() + 5 * rect.width() + border,
                                    rect.y() + border);
                m_painter->drawLine(rect.x() + 5 * rect.width() + border, rect.y() + border,
                                    rect.x() + 5 * rect.width() + border, rect.y() - rect.height() + border);
                m_painter->drawLine(rect.x() + 5 * rect.width() + border, rect.y() - rect.height() + border,
                                    rect.x() + 6 * rect.width() - border, rect.y() - rect.height() + border);
                m_painter->drawLine(rect.x() + 6 * rect.width() - border, rect.y() - rect.height() + border,
                                    rect.x() + 6 * rect.width() - border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + 6 * rect.width() - border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + 4 * rect.width() - border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + 4 * rect.width() - border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + 4 * rect.width() - border, rect.y() + 6 * rect.height() - border);
                m_painter->drawLine(rect.x() + 4 * rect.width() - border, rect.y() + 6 * rect.height() - border,
                                    rect.x() + border, rect.y() + 6 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 6 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (Sc) - 21
                rect = m_ktt->elementRect(21);
                c = m_scheme->elementBrush(21, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + 10 * rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + 10 * rect.width() - border, rect.y() + border,
                                    rect.x() + 10 * rect.width() - border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + 10 * rect.width() - border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (La) - 57
                rect = m_ktt->elementRect(57);
                c = m_scheme->elementBrush(57, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (Ce) - 58
                rect = m_ktt->elementRect(58);
                c = m_scheme->elementBrush(58, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + 14 * rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + 14 * rect.width() - border, rect.y() + border,
                                    rect.x() + 14 * rect.width() - border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + 14 * rect.width() - border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);
            }
            else if (m_scheme->name() == "Family")
            {
                //  (H) - 1
                rect = m_ktt->elementRect(1);
                c = m_scheme->elementBrush(1, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (Li) - 3
                rect = m_ktt->elementRect(3);
                c = m_scheme->elementBrush(3, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (Be) - 4
                rect = m_ktt->elementRect(4);
                c = m_scheme->elementBrush(4, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (Sc) - 21
                rect = m_ktt->elementRect(21);
                c = m_scheme->elementBrush(21, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + 10 * rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + 10 * rect.width() - border, rect.y() + border,
                                    rect.x() + 10 * rect.width() - border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + 10 * rect.width() - border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (La) - 57
                rect = m_ktt->elementRect(57);
                c = m_scheme->elementBrush(57, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (Ac) - 89
                rect = m_ktt->elementRect(89);
                c = m_scheme->elementBrush(89, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (He) - 2
                rect = m_ktt->elementRect(2);
                c = m_scheme->elementBrush(2, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (F) - 9
                rect = m_ktt->elementRect(9);
                c = m_scheme->elementBrush(9, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (Al) - 13
                rect = m_ktt->elementRect(13);
                c = m_scheme->elementBrush(13, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + 2 * rect.height() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + 2 * rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + 2 * rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + 2 * rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + 3 * rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + 3 * rect.height() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 4 * rect.height() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 4 * rect.height() + border,
                                    rect.x() + 4 * rect.width() - border, rect.y() + 4 * rect.height() + border);
                m_painter->drawLine(rect.x() + 4 * rect.width() - border, rect.y() + 4 * rect.height() + border,
                                    rect.x() + 4 * rect.width() - border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + 4 * rect.width() - border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (B) - 5
                rect = m_ktt->elementRect(5);
                c = m_scheme->elementBrush(5, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + 2 * rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + 2 * rect.height() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 2 * rect.height() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 2 * rect.height() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() + border,
                                    rect.x() + 4 * rect.width() - border, rect.y() + 3 * rect.height() + border);
                m_painter->drawLine(rect.x() + 4 * rect.width() - border, rect.y() + 3 * rect.height() + border,
                                    rect.x() + 4 * rect.width() - border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + 4 * rect.width() - border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + 3 * rect.width() + border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + 3 * rect.width() + border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + 3 * rect.width() + border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + 3 * rect.width() + border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + 2 * rect.width() + border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() + border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + 2 * rect.width() + border, rect.y() + 3 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() + border, rect.y() + 3 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 3 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 3 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + rect.height() - border);

                //  (C) - 6
                rect = m_ktt->elementRect(6);
                c = m_scheme->elementBrush(6, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() - border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() - border,
                                    rect.x() + 2 * rect.width() + border, rect.y() + 3 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() + border, rect.y() + 3 * rect.height() - border,
                                    rect.x() + 2 * rect.width() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + rect.height() - border,
                                    rect.x() + border, rect.y() + rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (Ce) - 58
                rect = m_ktt->elementRect(58);
                c = m_scheme->elementBrush(58, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    14 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Th) - 90
                rect = m_ktt->elementRect(90);
                c = m_scheme->elementBrush(90, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    14 * rect.width() - border * 2, rect.height() - border * 2);
            }
            else if (m_scheme->name() == "Groups")
            {
                //  (H) - 1
                rect = m_ktt->elementRect(1);
                c = m_scheme->elementBrush(1, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 7 * rect.height() - border * 2);

                //  (Be) - 4
                rect = m_ktt->elementRect(4);
                c = m_scheme->elementBrush(4, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (B) - 5
                rect = m_ktt->elementRect(5);
                c = m_scheme->elementBrush(5, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (C) - 6
                rect = m_ktt->elementRect(6);
                c = m_scheme->elementBrush(6, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (N) - 7
                rect = m_ktt->elementRect(7);
                c = m_scheme->elementBrush(7, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (O) - 8
                rect = m_ktt->elementRect(8);
                c = m_scheme->elementBrush(8, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (F) - 9
                rect = m_ktt->elementRect(9);
                c = m_scheme->elementBrush(9, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (He) - 2
                rect = m_ktt->elementRect(2);
                c = m_scheme->elementBrush(2, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (Sc) - 21
                rect = m_ktt->elementRect(21);
                c = m_scheme->elementBrush(21, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Ti) - 22
                rect = m_ktt->elementRect(22);
                c = m_scheme->elementBrush(22, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (V) - 23
                rect = m_ktt->elementRect(23);
                c = m_scheme->elementBrush(23, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Cr) - 24
                rect = m_ktt->elementRect(24);
                c = m_scheme->elementBrush(24, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Mn) - 25
                rect = m_ktt->elementRect(25);
                c = m_scheme->elementBrush(25, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Fe) - 26
                rect = m_ktt->elementRect(26);
                c = m_scheme->elementBrush(26, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    3 * rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Cu) - 29
                rect = m_ktt->elementRect(29);
                c = m_scheme->elementBrush(29, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Zn) - 30
                rect = m_ktt->elementRect(30);
                c = m_scheme->elementBrush(30, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 3 * rect.height() - border * 2);

                //  (Ce) - 58
                rect = m_ktt->elementRect(58);
                c = m_scheme->elementBrush(58, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Pr) - 59
                rect = m_ktt->elementRect(59);
                c = m_scheme->elementBrush(59, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Nd) - 60
                rect = m_ktt->elementRect(60);
                c = m_scheme->elementBrush(60, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Pm) - 61
                rect = m_ktt->elementRect(61);
                c = m_scheme->elementBrush(61, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Sm) - 62
                rect = m_ktt->elementRect(62);
                c = m_scheme->elementBrush(62, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    3 * rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Tb) - 65
                rect = m_ktt->elementRect(65);
                c = m_scheme->elementBrush(65, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Dy) - 66
                rect = m_ktt->elementRect(66);
                c = m_scheme->elementBrush(66, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Ho) - 67
                rect = m_ktt->elementRect(67);
                c = m_scheme->elementBrush(67, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Er) - 68
                rect = m_ktt->elementRect(68);
                c = m_scheme->elementBrush(68, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Tm) - 69
                rect = m_ktt->elementRect(69);
                c = m_scheme->elementBrush(69, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Yb) - 70
                rect = m_ktt->elementRect(70);
                c = m_scheme->elementBrush(70, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);

                //  (Lu) - 71
                rect = m_ktt->elementRect(71);
                c = m_scheme->elementBrush(71, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 2 * rect.height() - border * 2);
            }
        }
        else if (currentTableType()->name() == "Short")
        {
            if (m_scheme->name() == "Blocks") {
                // (H) - 1
                rect = m_ktt->elementRect(1);
                c = m_scheme->elementBrush(1, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border, rect.x() + rect.width() - border,
                                    rect.y() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + rect.height() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + 7 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + 7 * rect.height() - border,
                                    rect.x() + border, rect.y() + 7 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 7 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (B) - 5
                rect = m_ktt->elementRect(5);
                c = m_scheme->elementBrush(5, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border, rect.x() + 5 * rect.width() + border,
                                    rect.y() + border);
                m_painter->drawLine(rect.x() + 5 * rect.width() + border, rect.y() + border,
                                    rect.x() + 5 * rect.width() + border, rect.y() - rect.height() + border);
                m_painter->drawLine(rect.x() + 5 * rect.width() + border, rect.y() - rect.height() + border,
                                    rect.x() + 6 * rect.width() - border, rect.y() - rect.height() + border);
                m_painter->drawLine(rect.x() + 6 * rect.width() - border, rect.y() - rect.height() + border,
                                    rect.x() + 6 * rect.width() - border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + 6 * rect.width() - border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);
            }
            else if (m_scheme->name() == "Family")
            {
                //  (H) - 1
                rect = m_ktt->elementRect(1);
                c = m_scheme->elementBrush(1, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (Li) - 3
                rect = m_ktt->elementRect(3);
                c = m_scheme->elementBrush(3, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (Be) - 4
                rect = m_ktt->elementRect(4);
                c = m_scheme->elementBrush(4, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (He) - 2
                rect = m_ktt->elementRect(2);
                c = m_scheme->elementBrush(2, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (F) - 9
                rect = m_ktt->elementRect(9);
                c = m_scheme->elementBrush(9, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (Al) - 13
                rect = m_ktt->elementRect(13);
                c = m_scheme->elementBrush(13, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + 2 * rect.height() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + 2 * rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + 2 * rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + 2 * rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + 3 * rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + 3 * rect.height() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 4 * rect.height() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 4 * rect.height() + border,
                                    rect.x() + border, rect.y() + 4 * rect.height() + border);
                m_painter->drawLine(rect.x() + border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (B) - 5
                rect = m_ktt->elementRect(5);
                c = m_scheme->elementBrush(5, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + rect.height() + border,
                                    rect.x() + 2 * rect.width() - border, rect.y() + 2 * rect.height() + border);
                m_painter->drawLine(rect.x() + 2 * rect.width() - border, rect.y() + 2 * rect.height() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 2 * rect.height() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 2 * rect.height() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() + border,
                                    rect.x() + 4 * rect.width() - border, rect.y() + 3 * rect.height() + border);
                m_painter->drawLine(rect.x() + 4 * rect.width() - border, rect.y() + 3 * rect.height() + border,
                                    rect.x() + 4 * rect.width() - border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + 4 * rect.width() - border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + 3 * rect.width() + border, rect.y() + 5 * rect.height() - border);
                m_painter->drawLine(rect.x() + 3 * rect.width() + border, rect.y() + 5 * rect.height() - border,
                                    rect.x() + 3 * rect.width() + border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + 3 * rect.width() + border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + 2 * rect.width() + border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() + border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + 2 * rect.width() + border, rect.y() + 3 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() + border, rect.y() + 3 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 3 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 3 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + rect.height() - border);

                //  (C) - 6
                rect = m_ktt->elementRect(6);
                c = m_scheme->elementBrush(6, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + border,
                                    rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() - border);
                m_painter->drawLine(rect.x() + 3 * rect.width() - border, rect.y() + 3 * rect.height() - border,
                                    rect.x() + 2 * rect.width() + border, rect.y() + 3 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() + border, rect.y() + 3 * rect.height() - border,
                                    rect.x() + 2 * rect.width() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + 2 * rect.width() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + rect.height() - border,
                                    rect.x() + border, rect.y() + rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + rect.height() - border,
                                    rect.x() + border, rect.y() + border);
            }
            else if (m_scheme->name() == "Groups")
            {
                //  (H) - 1
                rect = m_ktt->elementRect(1);
                c = m_scheme->elementBrush(1, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 7 * rect.height() - border * 2);

                //  (Be) - 4
                rect = m_ktt->elementRect(4);
                c = m_scheme->elementBrush(4, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);

                //  (B) - 5
                rect = m_ktt->elementRect(5);
                c = m_scheme->elementBrush(5, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (C) - 6
                rect = m_ktt->elementRect(6);
                c = m_scheme->elementBrush(6, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (N) - 7
                rect = m_ktt->elementRect(7);
                c = m_scheme->elementBrush(7, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (O) - 8
                rect = m_ktt->elementRect(8);
                c = m_scheme->elementBrush(8, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (F) - 9
                rect = m_ktt->elementRect(9);
                c = m_scheme->elementBrush(9, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 5 * rect.height() - border * 2);

                //  (He) - 2
                rect = m_ktt->elementRect(2);
                c = m_scheme->elementBrush(2, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 6 * rect.height() - border * 2);
            }
        }
        else if (currentTableType()->name() == "D")
        {
            if (m_scheme->name() == "Blocks") {
                //  (Sc) - 21
                rect = m_ktt->elementRect(21);
                c = m_scheme->elementBrush(21, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + 10 * rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + 10 * rect.width() - border, rect.y() + border,
                                    rect.x() + 10 * rect.width() - border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + 10 * rect.width() - border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (La) - 57
                rect = m_ktt->elementRect(57);
                c = m_scheme->elementBrush(57, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + border,
                                    rect.x() + rect.width() - border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() - border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);
            }
            else if (m_scheme->name() == "Family")
            {
                //  (Sc) - 21
                rect = m_ktt->elementRect(21);
                c = m_scheme->elementBrush(21, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawLine(rect.x() + border, rect.y() + border,
                                    rect.x() + 10 * rect.width() - border, rect.y() + border);
                m_painter->drawLine(rect.x() + 10 * rect.width() - border, rect.y() + border,
                                    rect.x() + 10 * rect.width() - border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + 10 * rect.width() - border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 4 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 4 * rect.height() - border,
                                    rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + rect.width() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + 2 * rect.height() - border);
                m_painter->drawLine(rect.x() + border, rect.y() + 2 * rect.height() - border,
                                    rect.x() + border, rect.y() + border);

                //  (La) - 57
                rect = m_ktt->elementRect(57);
                c = m_scheme->elementBrush(57, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (Ac) - 89
                rect = m_ktt->elementRect(89);
                c = m_scheme->elementBrush(89, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);
            }
            else if (m_scheme->name() == "Groups")
            {
                //  (Sc) - 21
                rect = m_ktt->elementRect(21);
                c = m_scheme->elementBrush(21, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Ti) - 22
                rect = m_ktt->elementRect(22);
                c = m_scheme->elementBrush(22, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (V) - 23
                rect = m_ktt->elementRect(23);
                c = m_scheme->elementBrush(23, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Cr) - 24
                rect = m_ktt->elementRect(24);
                c = m_scheme->elementBrush(24, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Mn) - 25
                rect = m_ktt->elementRect(25);
                c = m_scheme->elementBrush(25, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Fe) - 26
                rect = m_ktt->elementRect(26);
                c = m_scheme->elementBrush(26, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    3 * rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Cu) - 29
                rect = m_ktt->elementRect(29);
                c = m_scheme->elementBrush(29, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 4 * rect.height() - border * 2);

                //  (Zn) - 30
                rect = m_ktt->elementRect(30);
                c = m_scheme->elementBrush(30, rect);

                border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, 3 * rect.height() - border * 2);
            }
        }
        else if (currentTableType()->name() == "DZ")
        {
            if (m_scheme->name() == "Blocks") {
                //  (H) - 1
                rect = m_ktt->elementRect(1);
                c = m_scheme->elementBrush(1, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (He) - 2
                rect = m_ktt->elementRect(2);
                c = m_scheme->elementBrush(2, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (Be) - 3
                rect = m_ktt->elementRect(3);
                c = m_scheme->elementBrush(3, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (B) - 5
                rect = m_ktt->elementRect(5);
                c = m_scheme->elementBrush(5, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    6 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Ne) - 11
                rect = m_ktt->elementRect(11);
                c = m_scheme->elementBrush(11, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    2 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Al) - 13
                rect = m_ktt->elementRect(13);
                c = m_scheme->elementBrush(13, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    6 * rect.width() - border * 2, rect.height() - border * 2);

                //  (K) - 19
                rect = m_ktt->elementRect(19);
                c = m_scheme->elementBrush(19, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    2 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Sc) - 21
                rect = m_ktt->elementRect(21);
                c = m_scheme->elementBrush(21, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    10 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Ga) - 31
                rect = m_ktt->elementRect(31);
                c = m_scheme->elementBrush(31, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    6 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Rb) - 37
                rect = m_ktt->elementRect(37);
                c = m_scheme->elementBrush(37, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    2 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Y) - 39
                rect = m_ktt->elementRect(39);
                c = m_scheme->elementBrush(39, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    10 * rect.width() - border * 2, rect.height() - border * 2);

                //  (In) - 49
                rect = m_ktt->elementRect(49);
                c = m_scheme->elementBrush(49, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    6 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Cs) - 55
                rect = m_ktt->elementRect(55);
                c = m_scheme->elementBrush(55, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    2 * rect.width() - border * 2, rect.height() - border * 2);

                //  (La) - 57
                rect = m_ktt->elementRect(57);
                c = m_scheme->elementBrush(57, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    14 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Lu) - 71
                rect = m_ktt->elementRect(71);
                c = m_scheme->elementBrush(71, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (Hf) - 72
                rect = m_ktt->elementRect(72);
                c = m_scheme->elementBrush(72, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    9 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Ti) - 81
                rect = m_ktt->elementRect(81);
                c = m_scheme->elementBrush(81, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    6 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Fr) - 87
                rect = m_ktt->elementRect(87);
                c = m_scheme->elementBrush(87, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    2 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Ac) - 89
                rect = m_ktt->elementRect(89);
                c = m_scheme->elementBrush(89, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    14 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Lr) - 103
                rect = m_ktt->elementRect(103);
                c = m_scheme->elementBrush(103, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (Rf) - 104
                rect = m_ktt->elementRect(104);
                c = m_scheme->elementBrush(104, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    9 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Rf) - 104
                rect = m_ktt->elementRect(104);
                c = m_scheme->elementBrush(104, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    9 * rect.width() - border * 2, rect.height() - border * 2);

                //  (Uut) - 113
                rect = m_ktt->elementRect(113);
                c = m_scheme->elementBrush(113, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    4 * rect.width() - border * 2, rect.height() - border * 2);
            }
            else if (m_scheme->name() == "Family")
            {
                //  (H) - 1
                rect = m_ktt->elementRect(1);
                c = m_scheme->elementBrush(1, rect);

                int border = 2;
                pen = QPen(c.color());
                pen.setWidth(border * 2);
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (He) - 2
                rect = m_ktt->elementRect(2);
                c = m_scheme->elementBrush(2, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);

                //  (Be) - 3
                rect = m_ktt->elementRect(3);
                c = m_scheme->elementBrush(3, rect);

                pen.setColor(c.color());
                m_painter->setPen(pen);

                m_painter->drawRect(rect.x() + border, rect.y() + border,
                                    rect.width() - border * 2, rect.height() - border * 2);
            }
            else if (m_scheme->name() == "Groups")
            {

            }
        }
        m_painter->setPen(prevPen);
    }
}

void KalziumPainter::drawElement( int element, const QRect& r )
{
    if ( !m_scheme || !m_ktt ) return;

    const QRect rect = r.isNull() ? m_ktt->elementRect( element ) : r;

    if (rect.isNull() ) {
        return; //this element doesn't belong to the table it seems...
    }

    Element *el = KalziumDataObject::instance()->element( element );
    const QString symbol = el->dataAsString( ChemicalDataObject::symbol );

    bool selectedElement = KalziumDataObject::instance()->search()->matches( el );

    switch ( m_mode )
    {
    case NORMAL:	// Scheme
    {
        QBrush c;

        if ( selectedElement )
            c = QBrush( Qt::yellow, Qt::CrossPattern );
        else
            c = m_scheme->elementBrush( element, rect );

        //when drawing the iconic style we don't draw the text. That is
        //why I introduced this if-condition. (C Niehaus)
        if (m_scheme->name() == "Iconic") {
            m_painter->fillRect( rect, c );
            m_painter->drawRect( rect );
        } else if ( !c.texture().isNull() )
        {
            QRect symbolrect;
            QFont orig_font = m_painter->font();
            QFont font = m_painter->font();
            font.setPointSize( QFontInfo( font ).pointSize() - 2 );
            m_painter->setFont( font );
            m_painter->drawText( rect, Qt::AlignHCenter | Qt::AlignTop, symbol, &symbolrect );
            m_painter->setFont( orig_font );
            int symbolheight = symbolrect.height();
            QRect rect2 = rect.translated( 0, symbolheight );
            rect2.setHeight( rect2.height() - symbolheight - 1 );
            QPixmap pix = c.texture().scaled( rect2.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation );
            m_painter->drawPixmap( rect2.left() + ( rect2.width() - pix.width() ) / 2, rect2.top() + ( rect2.height() - pix.height() ) / 2, pix );
            m_painter->drawRect( rect );
        }
        else {
            // the brush doesn't have any texture,
            // so proceeding with normal colors and texts
            QColor textc = m_scheme->textColor( element );
            m_painter->setPen( textc );

            m_painter->fillRect( rect, c );
            m_painter->drawRect( rect );

            m_painter->drawText( rect, Qt::AlignCenter, symbol );
        }
        break;
    }
    case SOM:
    {
        QColor color;

        const double melting = el->dataAsVariant( ChemicalDataObject::meltingpoint ).toDouble();
        const double boiling = el->dataAsVariant( ChemicalDataObject::boilingpoint ).toDouble();
        const double mass = el->dataAsVariant( ChemicalDataObject::mass ).toDouble();

        if ( m_temperature < melting )
        {
            //the element is solid
            color= Prefs::color_solid();
        }
        else if ( ( m_temperature > melting ) && ( m_temperature < boiling ) )
        {
            //the element is liquid
            color= Prefs::color_liquid();
        }
        else if ( ( m_temperature > boiling ) && ( boiling > 0.0 ) )
        {
            //the element is vaporous
            color= Prefs::color_vapor();
        }
        else
            color = Qt::lightGray;

        m_painter->setPen( Qt::black );

        QFont orig_font = m_painter->font();
        QFont symbol_font = m_painter->font();
        symbol_font.setPointSize( 10 );
        QFont f = m_painter->font();
        f.setPointSize( 9 );
        m_painter->setFont( f );

        m_painter->fillRect( rect, QBrush( color ) );
        m_painter->drawRect( rect );

        m_painter->drawText( rect, Qt::AlignHCenter | Qt::AlignTop, QString::number( KalziumUtils::strippedValue( mass ) ) );

        m_painter->drawText( rect, Qt::AlignHCenter | Qt::AlignBottom, QString::number( element ) );

        m_painter->setFont( symbol_font );
        m_painter->drawText( rect, Qt::AlignCenter, symbol );

        m_painter->setFont( orig_font );
        break;
    }
    case GRADIENT:	// Gradient
    {
        m_painter->setPen( Qt::black );
        double coeff = m_gradient->elementCoeff( element );
        QBrush c = QBrush( m_gradient->calculateColor( coeff ) );

        m_painter->fillRect( rect, c );
        m_painter->drawRect( rect );

        m_painter->drawText( rect, Qt::AlignCenter, symbol );

        QFont orig_font = m_painter->font();
        QFont f = m_painter->font();
        f.setPointSize( 8 );
        m_painter->setFont( f );
        double value = m_gradient->value( element );
        QString strval = coeff != -1 ? QString::number( KalziumUtils::strippedValue( value ) ) : i18nc( "It means: Not Available. Translators: keep it as short as you can!", "N/A" );
        m_painter->drawText( rect, Qt::AlignHCenter | Qt::AlignBottom, strval );

        m_painter->setFont( orig_font );
        break;
    }
    case TIME:
    {
        QBrush c = brushForElement( element );

        m_painter->fillRect( rect, c );
        m_painter->drawRect( rect );

        m_painter->drawText( rect, Qt::AlignCenter, symbol );
        break;
    }
    case NORMAL_GRADIENT:
    {
        m_painter->setPen( Qt::black );
        double coeff = m_gradient->elementCoeff( element );
        QBrush c = QBrush( m_gradient->calculateColor( coeff ) );

        m_painter->fillRect( rect, c );
        m_painter->drawRect( rect );

        m_painter->drawText( rect, Qt::AlignCenter, symbol );

        QFont orig_font = m_painter->font();
        QFont f = m_painter->font();
        f.setPointSize( 8 );
        m_painter->setFont( f );
        double value = m_gradient->value( element );
        QString strval = coeff != -1 ? QString::number( KalziumUtils::strippedValue( value ) ) : i18nc( "It means: Not Available. Translators: keep it as short as you can!", "N/A" );
        m_painter->drawText( rect, Qt::AlignHCenter | Qt::AlignBottom, strval );

        m_painter->setFont( orig_font );
        break;
    }
    }
}

void KalziumPainter::drawNumeration()
{
    QStringList numitems = m_numeration->items();
    for ( int i = 0; i < numitems.count(); i++ )
    {
        QRect itemrect = m_ktt->numerationRect( i, m_numeration );
        if ( itemrect.isNull() ) continue;

        m_painter->drawText( itemrect, Qt::AlignCenter, numitems.at( i ) );
    }
}

void KalziumPainter::drawElementSelector( int element )
{
    if ( !m_ktt ) return;

    QRect elemrect = m_ktt->elementRect( element );

    QPen pen;

    m_painter->setRenderHint(QPainter::Antialiasing, true);
    // outer circle
    pen.setStyle( Qt::DotLine );
    pen.setWidth( 4 );
    pen.setColor( Qt::blue );
    m_painter->setPen( pen );
    m_painter->drawEllipse( elemrect.left() - 10, elemrect.top() - 10, elemrect.width() + 20, elemrect.height() + 20 );
    m_painter->setRenderHint(QPainter::Antialiasing, false);

    // inner circle
    pen.setWidth( 3 );
    pen.setColor( Qt::red );
    m_painter->setPen( pen );
    m_painter->drawEllipse( elemrect.left() - 5, elemrect.top() - 5, elemrect.width() + 10, elemrect.height() + 10 );
}

void KalziumPainter::setMode( MODE m )
{
    m_mode = m;
}

void KalziumPainter::setScheme( int s )
{
    KalziumSchemeType *tmp = KalziumSchemeTypeFactory::instance()->build( s );
    if ( tmp ) {
        m_scheme = tmp;
    }
}

void KalziumPainter::setScheme( const QByteArray& s )
{
    KalziumSchemeType *tmp = KalziumSchemeTypeFactory::instance()->build( s );
    if ( tmp ) {
        m_scheme = tmp;
    }
}

KalziumSchemeType* KalziumPainter::scheme() const
{
    return m_scheme;
}

void KalziumPainter::setGradient( int g )
{
    KalziumGradientType *tmp = KalziumGradientTypeFactory::instance()->build( g );
    if ( tmp ) {
        m_gradient = tmp;
    }
}

void KalziumPainter::setGradient( const QByteArray& g )
{
    KalziumGradientType *tmp = KalziumGradientTypeFactory::instance()->build( g );
    if ( tmp ) {
        m_gradient = tmp;
    }
}

KalziumGradientType* KalziumPainter::gradient() const
{
    return m_gradient;
}

void KalziumPainter::setNumeration( int n )
{
    KalziumNumerationType *tmp = KalziumNumerationTypeFactory::instance()->build( n );
    if ( tmp ) {
        m_numeration = tmp;
    }
}

void KalziumPainter::setNumeration( const QByteArray& n )
{
    KalziumNumerationType *tmp = KalziumNumerationTypeFactory::instance()->build( n );
    if ( tmp ) {
        m_numeration = tmp;
    }
}

void KalziumPainter::setTemperature( int temp )
{
    m_temperature = temp;
}

QBrush KalziumPainter::brushForElement( int element ) const
{
    if ( !m_scheme || !m_ktt ) return QBrush();

    QRect rect = m_ktt->elementRect( element );
    Element *el = KalziumDataObject::instance()->element( element );

    switch ( m_mode )
    {
    case NORMAL:
    {
        return m_scheme->elementBrush( element, rect );
        break;
    }
    case SOM:
    {
        QColor color;

        const double melting = el->dataAsVariant( ChemicalDataObject::meltingpoint ).toDouble();
        const double boiling = el->dataAsVariant( ChemicalDataObject::boilingpoint ).toDouble();

        if ( m_temperature < melting )
        {
            //the element is solid
            color = Prefs::color_solid();
        }
        else if ( ( m_temperature > melting ) && ( m_temperature < boiling ) )
        {
            //the element is liquid
            color = Prefs::color_liquid();
        }
        else if ( ( m_temperature > boiling ) && ( boiling > 0.0 ) )
        {
            //the element is vaporous
            color = Prefs::color_vapor();
        }
        else
            color = Qt::lightGray;

        return QBrush( color );
        break;
    }
    case GRADIENT:
    {
        double coeff = m_gradient->elementCoeff( element );
        return QBrush( m_gradient->calculateColor( coeff ) );
        break;
    }
    case TIME:
    {
        const int date = el->dataAsVariant( ChemicalDataObject::date ).toInt();

        qDebug() << date;

        if ( date == -1 ) //the element has not yet been recognized
            return QBrush( Qt::blue );

        if ( m_time >= date )
            return m_scheme->elementBrush( element, rect );
        else
            return QBrush( Qt::lightGray );
        break;
    }
    case NORMAL_GRADIENT:
    {
        double coeff = m_gradient->elementCoeff( element );
        return QBrush( m_gradient->calculateColor( coeff ) );
        break;
    }
    }
    // fallback
    return QBrush();
}

