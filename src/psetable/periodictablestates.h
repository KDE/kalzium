/*
    Copyright (C) 2011  Rebetez Etienne <etienne.rebetez@oberwallis.ch>

    Kalzium is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    Kalzium is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef PERIODICTABLESTATES_H
#define PERIODICTABLESTATES_H

#include <QParallelAnimationGroup>
#include <QStateMachine>

#include "kalziumelementproperty.h"

#include "elementitem.h"
#include "numerationitem.h"
#include "statemachine.h"

/**
 * @class PeriodicTableStates
 * In this class all states of element items and numeration items are defined.
 * The position and animations of the items are set.
 *
 * @short Setting up the pse states
 * @author Etienne Rebetez
 *
 */

class PeriodicTableStates
{
public:
    /**
     * Constructor need list of Items.
     * @param elementItemList List of the Element items in the table.
     * @param numerationItemList List of the Numeration items in the table.
     */
    PeriodicTableStates(const QList<ElementItem *> &elementItemList,
                        const QList<NumerationItem *> &numerationItemList);

    virtual ~PeriodicTableStates();

    /**
     * Get the rectangle of the given periodic table.
     * @param tableIndex Index of the table @see pseTables.
     * @return Rectangle in floating point precision.
     */
    QRectF pseRect(const int &tableIndex) const;

    /**
     * Set the table index
     * @param tableIndex Index of the table @see pseTables..
     */
    void setTableState(const int &tableIndex) ;

private:
    void setNumerationItemPositions(const int &tableIndex);
    void hideAllNumerationItems(const int &tableIndex);
    int maxNumerationItemXCoordinate(const int &tableIndex);

    void addElementAnimation(QGraphicsObject *object, int factor = 1);

    void setElementItemPositions(const int &tableIndex);

    QPoint hiddenPoint() const;

    QStateMachine m_states;
    QParallelAnimationGroup *m_group;
    StateSwitcher *m_stateSwitcher;

    QList<ElementItem *> m_elementItemList;
    QList<NumerationItem *> m_numerationItemList;

    int m_width;
    int m_height;

    QList<QState *> m_tableStatesList;
};

#endif // PERIODICTABLESTATES_H
