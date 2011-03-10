/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2011  Rebetez Etienne <etienne.rebetez@oberwallis.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef PERIODICTABLESTATES_H
#define PERIODICTABLESTATES_H

#include <QtCore/QStateMachine>
#include <QtCore/QParallelAnimationGroup>

#include "kalziumelementproperty.h"

#include "elementitem.h"
#include "numerationitem.h"
#include "statemachine.h"

class PeriodicTableStates
{
public:
    PeriodicTableStates(const QList<ElementItem *> elementItemList,
                        const QList<NumerationItem *> numerationItemList);

    virtual ~PeriodicTableStates();

    QRectF currentPseRect( const int tableIndex ) const;

    void setTableState( const int tableIndex ) ;

private:
    /// Adding position of Nummeration for each tabletyp
    void setNumerationItemPositions(const int tableIndex);
    void hideAllNumerationItems(const int tableIndex);
    int maxNumerationItemXCoordinate(const int tableIndex);

    void addElementAnimation(QGraphicsObject *object, int factor = 1);

    void setElementItemPositions(const int tableIndex);

    QPoint hiddenPoint() const;

    QStateMachine m_states;
    QParallelAnimationGroup *m_group;
    StateSwitcher *m_stateSwitcher;

    QList<ElementItem *> m_elementItemList;
    QList<NumerationItem *> m_numerationItemList;

    int m_width, m_height;

    QList<QState *> m_tableStatesList;
};

#endif // PERIODICTABLESTATES_H
