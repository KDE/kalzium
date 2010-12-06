/***************************************************************************
 *   Copyright (C) 2005-2006 by Pino Toscano, toscano.pino@tiscali.it      *
 *   Copyright (C) 2003-2006 by Carsten Niehaus, cniehaus@kde.org          *
 *   Copyright (C) 2007-2009 by Marcus D. Hanwell                          *
 *   Copyright (C) 2010 by Etienne Rebetez, etienne.rebetez@oberwallis.ch  *
 *                                                                         *
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

#include <QSvgGenerator>

#include "psetables.h"
#include "statemachine.h"
#include "periodsystembase.h"
#include "numerationitem.h"
#include <prefs.h>

periodSystem::periodSystem(KalziumElementProperty *elProperty, QWidget *parent)
        : QGraphicsView(parent), m_width(42), m_height(42) // Some space between the elements (px40) looks nice.
{
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);

    setMouseTracking( true );

    m_tableTyp = Prefs::table();
    m_maxCoords = pseTables::instance()->getTabletype( m_tableTyp )->coordsMax();

    m_table = new PeriodicTableScene(this);
    m_table->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_table->setBackgroundBrush(Qt::white);

    foreach (int intElement, pseTables::instance()->getTabletype( 0 )->elements()) {
        ElementItem *item = new ElementItem(elProperty, intElement);
        connect(elProperty, SIGNAL(propertyChanged()), item, SLOT(redraw()));
        m_elementItems << item;
        m_table->addItem(item);
    }

    QPalette widgetPalette = palette();
    m_table->setBackgroundBrush(QBrush(widgetPalette.window()));

    setScene(m_table);

    connect(m_table, SIGNAL(freeSpaceClick()), this, SLOT(fitPseInView()));

    setupStatesAndAnimation();

    fitPseInView();
}

periodSystem::~periodSystem()
{
    delete scene();
    delete m_group;
    qDeleteAll(m_tableStates);
}

void periodSystem::setupStatesAndAnimation()
{
    StateSwitcher *stateSwitcher = new StateSwitcher(&m_states);
    m_group= new QParallelAnimationGroup;

    // Creating Nummerationitems here, we use the classic periodic table (0) as reference (18 in a row)
    QList<NumerationItem *> numerationItems;
    for (int j = 0; j < pseTables::instance()->getTabletype( 0 )->coordsMax().x(); ++j) {
        numerationItems << new NumerationItem( j );
        m_table->addItem(numerationItems.at( j ));
        connect(this, SIGNAL(numerationChange(int)), numerationItems.at(j), SLOT(setNumerationType(int)));
    }

    // For every Tabletyp the Position of the Elements are set up.
    for (int j = 0; j < pseTables::instance()->tables().count(); ++j)
    {
        m_tableStates << new QState(stateSwitcher);

        // First hide every numerationitem. It's easyer this way. Feel free to finde a better solution;)
        for (int i = 0; i < numerationItems.count(); ++i) {
            m_tableStates.at(j)->assignProperty(numerationItems.at( i ), "pos", QPointF( -m_width, -10 * m_width));
        }

        // Adding position of Nummeration for each tabletyp
        for (int i = 0; i < pseTables::instance()->getTabletype( j )->coordsMax().x() || i < numerationItems.count(); ++i) {
            int itemAtPos = pseTables::instance()->getTabletype( j )->numeration( i );

            if ( itemAtPos > 0 ) {
                m_tableStates.at(j)->assignProperty(numerationItems.at(itemAtPos - 1), "pos", QPointF( i * m_width, m_height / 4));
                // Animation for each element
                QPropertyAnimation *anim = new QPropertyAnimation(numerationItems.at(itemAtPos - 1), "pos");
                anim->setDuration( 1500 + i * 2);
                anim->setEasingCurve(QEasingCurve::InOutExpo);
                m_group->addAnimation(anim);
            }
        }

        // Values for each Element
        for (int i = 0; i < m_elementItems.count(); ++i) {
            ElementItem *item = m_elementItems.at(i);

            QPoint coords = pseTables::instance()->getTabletype( j )->elementCoords( i + 1 );

            // put the not needed elements a bit away
            if ( coords.x() == 0) {
                coords.setX(-1);
                coords.setY(-10);
            }

            m_tableStates.at(j)->assignProperty(item, "pos", QPointF((coords.x()-1) * m_width, (coords.y()) * m_height));

            // Animation for each element
            QPropertyAnimation *anim = new QPropertyAnimation(m_elementItems.at(i), "pos");
            anim->setDuration( 1600 + i * 2);
            anim->setEasingCurve(QEasingCurve::InOutExpo);
            m_group->addAnimation(anim);
        }
        // Finally add the states to the statemachine
        stateSwitcher->addState(m_tableStates.at(j), m_group, j);
    }

    connect(this , SIGNAL(tableChanged(int)), stateSwitcher, SLOT(slotSwitchState(int)));

    m_states.setInitialState(stateSwitcher);
    stateSwitcher->setInitialState(m_tableStates.at(m_tableTyp));
    m_states.start();
}


PeriodicTableScene* periodSystem::pseScene() const
{
    return  m_table;
}

int periodSystem::table() const
{
    return m_tableTyp;
}

void periodSystem::slotChangeTable(int table)
{
    Prefs::setTable(table);
    m_tableTyp = table;
    m_maxCoords = pseTables::instance()->getTabletype( m_tableTyp )->coordsMax();
    setBiggerSceneRect();
    emit tableChanged(m_tableTyp);
}

void periodSystem::slotSelectElement(int element)
{
    slotUnSelectElements();

    if (element > 0) {
        m_elementItems.at(--element)->setSelected(true);
    }
}

void periodSystem::slotUnSelectElements()
{
    foreach ( QGraphicsItem *item , m_table->selectedItems()) {
        item->setSelected( false );
    }
}

void periodSystem::setBiggerSceneRect()
{
    QRectF newRect(0, 0, m_table->sceneRect().width(), m_table->sceneRect().height());

    if (m_table->sceneRect().width() < m_maxCoords.x() * m_width) {
        newRect.setWidth((m_maxCoords.x() + 1) * m_width);
    }

    if ( m_table->sceneRect().height() < m_maxCoords.y() * m_height ) {
        newRect.setHeight((m_maxCoords.y() + 2) * m_height );
    }

    m_table->setSceneRect(newRect);
}

void periodSystem::fitPseInView()
{
    if (operator!=(m_table->sceneRect(), QRectF(0, 0, m_maxCoords.x() * m_width, (m_maxCoords.y() + 1) * m_height)) ) {
        m_table->setSceneRect(0, 0, m_maxCoords.x() * m_width, (m_maxCoords.y() + 1) * m_height);
    }
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

bool periodSystem::event(QEvent *e)
{
    return QGraphicsView::event(e);
}

void periodSystem::resizeEvent ( QResizeEvent * event )
{
    fitPseInView();
    QGraphicsView::resizeEvent(event);
}

void periodSystem::generateSvg(const QString& filename)
{
    QSvgGenerator *svgGen = new QSvgGenerator();
    svgGen->setFileName( filename );

    QPainter painter;
    painter.begin( svgGen );
    render( &painter , QRectF(0 , m_height, ( m_maxCoords.x() + 2.5) * m_width, (m_maxCoords.y() + 2.5) * m_height), QRect(0 , m_height, ( m_maxCoords.x() + 2.5) * m_width, (m_maxCoords.y() + 2.5) * m_height));
    painter.rotate(180);
    painter.end();

    delete svgGen;
}

#include "periodsystembase.moc"
