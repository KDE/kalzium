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

#ifndef LEGENDWIDGET_H
#define LEGENDWIDGET_H

#include <QLabel>

#include "kalziumelementproperty.h"

class LegendItem;

/**
 * @author Carsten Niehaus
 *
 * The LegendWidget displays the explanations of what the user is currently
 * seeing in the table
 */
class LegendWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LegendWidget(QWidget *parent);

    ~LegendWidget();

    void LockWidget();
    void UnLockWidget();

Q_SIGNALS:
    void elementMatched(int element);
    void resetElementMatch();

public slots:
    void updateContent();

    void setDockArea(Qt::DockWidgetArea newDockArea);

    /// is triggered when a LegenItem is Hoovered.
    void legendItemAction(QColor color);

private:
    bool isElementMatch(int element, QColor &color);

    bool m_update;

    QPixmap m_pixmap;

    QList<LegendItem*> m_legendItemList;

    Qt::DockWidgetArea m_dockArea;

    void updateLegendItemLayout(const QList<legendPair>& list);
};

/**
 * A LegendItem is displayed as one small rectangle which represents the
 * color in the table with a short explanation for it.
 *
 * @author Carsten Niehaus
 */
class LegendItem : public QLabel
{
    Q_OBJECT

public:
    LegendItem(const QPair<QString, QColor>& pair, QWidget * parent = nullptr);
    ~LegendItem() {}

Q_SIGNALS:
    void legenItemHoovered(QColor color);

private:
    QColor legendItemColor;

protected:
    void enterEvent(QEvent * event) override;
    void leaveEvent(QEvent * event) override;

};

#endif // LEGENDWIDGET_H
