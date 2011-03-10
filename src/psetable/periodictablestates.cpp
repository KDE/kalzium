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


#include "periodictablestates.h"

#include "psetables.h"
#include "statemachine.h"

#include <prefs.h>

PeriodicTableStates::PeriodicTableStates(
    const QList< ElementItem* > elementItemList,
    const QList< NumerationItem* > numerationItemList,
    const int width, const int height,
    QObject* parent)
        : QObject(parent),
        m_elementItemList(elementItemList),
        m_numerationItemList(numerationItemList),
        m_width(width), m_height(height)
{
    StateSwitcher *stateSwitcher = new StateSwitcher(&m_states);
    m_group= new QParallelAnimationGroup;

    // For every Tabletyp the Position of the Elements are set up.
    for (int tableIndex = 0; tableIndex < pseTables::instance()->tables().count(); ++tableIndex) {
        m_tableStatesList << new QState(stateSwitcher);

        setNumerationItemPositions( tableIndex );

        setElementItemPositions( tableIndex );

        stateSwitcher->addState(m_tableStatesList.at( tableIndex ), m_group, tableIndex);
    }

    connect(parent , SIGNAL( tableChanged(int) ), stateSwitcher, SLOT( slotSwitchState(int) ) );

    stateSwitcher->setInitialState( m_tableStatesList.at( Prefs::table() ) );
    m_states.setInitialState(stateSwitcher);
    m_states.start();
}

void PeriodicTableStates::setNumerationItemPositions( int tableIndex )
{
    hideAllNumerationItems( tableIndex );

    for (int x = 0; x < maxNumerationItemXCoordinate( tableIndex ); ++x) {
        int numerationId = pseTables::instance()->getTabletype( tableIndex )->numerationAtPos( x );

        if ( numerationId >= 0 ) {
            m_tableStatesList.at( tableIndex )->assignProperty(m_numerationItemList.at(numerationId), "pos",
                    QPointF( x * m_width, - m_height * 3 / 4));
            addElementAnimation( m_numerationItemList.at(numerationId), x );
        }
    }
}

void PeriodicTableStates::hideAllNumerationItems(int tableIndex)
{
    foreach( NumerationItem *item, m_numerationItemList)
    m_tableStatesList.at( tableIndex )->assignProperty( item, "pos", QPointF( hiddenPoint() ));
}

QPoint PeriodicTableStates::hiddenPoint() const
{
    return QPoint(-40, -400);
}

int PeriodicTableStates::maxNumerationItemXCoordinate(int tableIndex)
{
    const int maxTableLenght = pseTables::instance()->getTabletype( tableIndex )->tableSize().x();

    return maxTableLenght > m_numerationItemList.count() ?
           maxTableLenght : m_numerationItemList.count();
}

void PeriodicTableStates::addElementAnimation(QGraphicsObject *object, int duration)
{
    QPropertyAnimation *anim = new QPropertyAnimation( object, "pos");
    anim->setDuration( 1600 + duration * 2);
    anim->setEasingCurve(QEasingCurve::InOutExpo);
    m_group->addAnimation( anim );
}

void PeriodicTableStates::setElementItemPositions(int tableIndex)
{
    for (int i = 0; i < m_elementItemList.size(); ++i) {
        const int elementNumber = m_elementItemList.at( i )->data(0).toInt();
        QPoint itemPosition = pseTables::instance()->getTabletype( tableIndex )->elementCoords( elementNumber );

        // put the not needed elements a bit away
        if ( itemPosition.x() < 0)
            itemPosition = hiddenPoint();

        m_tableStatesList.at( tableIndex )->assignProperty( m_elementItemList.at( i ), "pos",
                QPointF( (itemPosition.x() ) * m_width, (itemPosition.y()) * m_height));

        addElementAnimation( m_elementItemList.at( i ), i);
    }
}

PeriodicTableStates::~PeriodicTableStates()
{
    delete m_group;
    qDeleteAll(m_tableStatesList);
}

#include "periodictablestates.moc"
