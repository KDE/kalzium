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
#include <prefs.h>

periodSystem::periodSystem(KalziumElementProperty *elProperty, QWidget *parent)
        : QGraphicsView(parent), m_width(42), m_height(42) // Some space between the elements (px40) looks nice.
{
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);

    setMouseTracking( true );

    m_currentTableInex = Prefs::table();
    m_hiddenPoint = QPoint(-40, -400);

    m_tableScene = new PeriodicTableScene(this);
    m_tableScene->setItemIndexMethod(QGraphicsScene::NoIndex);
    m_tableScene->setBackgroundBrush(Qt::white);

    foreach (int intElement, pseTables::instance()->getTabletype( 0 )->elements()) {
        ElementItem *item = new ElementItem(elProperty, intElement);
        connect(elProperty, SIGNAL(propertyChanged()), item, SLOT(redraw()));
        m_tableScene->addObject(item);
    }

    QPalette widgetPalette = palette();
    m_tableScene->setBackgroundBrush(QBrush(widgetPalette.window()));

    setScene(m_tableScene);

    connect(m_tableScene, SIGNAL(freeSpaceClick()), this, SLOT(fitPseInView()));

    setupStatesAndAnimation();

    fitPseInView();
}

periodSystem::~periodSystem()
{
    delete scene();
    delete m_group;
    qDeleteAll(m_tableStatesList);
//     qDeleteAll(m_numerationItemList);
}

void periodSystem::setupStatesAndAnimation()
{
    StateSwitcher *stateSwitcher = new StateSwitcher(&m_states);
    m_group= new QParallelAnimationGroup;

    createNumerationItems();

    // For every Tabletyp the Position of the Elements are set up.
    for (int tableIndex = 0; tableIndex < pseTables::instance()->tables().count(); ++tableIndex) {

        m_tableStatesList << new QState(stateSwitcher);

        // First hide every numerationitem. It's easyer this way. Feel free to finde a better solution;)
        hideAllNumerationItems( tableIndex );

        // Adding position of Nummeration for each tabletyp
        for (int i = 0; i < pseTables::instance()->getTabletype( tableIndex )->coordsMax().x() || i < m_numerationItemList.count(); ++i) {
            int itemAtPos = pseTables::instance()->getTabletype( tableIndex )->numeration( i );

            if ( itemAtPos > 0 ) {
                m_tableStatesList.at( tableIndex )->assignProperty(m_numerationItemList.at(itemAtPos - 1), "pos", QPointF( i * m_width, m_height / 4));
                // Animation for each element
                QPropertyAnimation *anim = new QPropertyAnimation(m_numerationItemList.at(itemAtPos - 1), "pos");
                anim->setDuration( 1500 + i * 2);
                anim->setEasingCurve(QEasingCurve::InOutExpo);
                m_group->addAnimation(anim);
            }
        }

        // Values for each Element
        for (int i = 0; i < m_tableScene->objects().count(); ++i) {
            QPoint coords = pseTables::instance()->getTabletype( tableIndex )->elementCoords( i + 1 );

            // put the not needed elements a bit away
            if ( coords.x() == 0) {
                coords = m_hiddenPoint;
            }

            m_tableStatesList.at( tableIndex )->assignProperty( m_tableScene->objects().at(i), "pos",
                    QPointF((coords.x()-1) * m_width, (coords.y()) * m_height));

            // Animation for each element
            QPropertyAnimation *anim = new QPropertyAnimation( m_tableScene->objects().at(i), "pos" );
            anim->setDuration( 1600 + i * 2);
            anim->setEasingCurve(QEasingCurve::InOutExpo);
            m_group->addAnimation(anim);
        }
        // Finally add the states to the statemachine
        stateSwitcher->addState(m_tableStatesList.at( tableIndex ), m_group, tableIndex);
    }

    connect(this , SIGNAL(tableChanged(int)), stateSwitcher, SLOT(slotSwitchState(int)));

    m_states.setInitialState(stateSwitcher);
    stateSwitcher->setInitialState( m_tableStatesList.at( m_currentTableInex ) );
    m_states.start();
}

void periodSystem::createNumerationItems() {
    // Creating Nummerationitems here, we use the classic periodic table (0) as reference (18 in a row)
    const int xMax = pseTables::instance()->getTabletype( 0 )->coordsMax().x();

    for (int i = 0; i < xMax; ++i) {
        m_numerationItemList << new NumerationItem( i );
        m_tableScene->addItem( m_numerationItemList.at( i ) );
        connect(this, SIGNAL(numerationChange(int)), m_numerationItemList.at( i ), SLOT(setNumerationType(int)));
    }
}

void periodSystem::hideAllNumerationItems(int tableIndex) {
    foreach( NumerationItem *item, m_numerationItemList) {
        m_tableStatesList.at( tableIndex )->assignProperty( item, "pos", QPointF( m_hiddenPoint ));
    }
}


PeriodicTableScene* periodSystem::pseScene() const
{
    return  m_tableScene;
}

int periodSystem::table() const
{
    return m_currentTableInex;
}

void periodSystem::slotChangeTable(int table)
{
    m_currentTableInex = table;
    Prefs::setTable( m_currentTableInex );

    setBiggerSceneRect();
    emit tableChanged( m_currentTableInex );
}

void periodSystem::slotSelectElement(int element)
{
    slotUnSelectElements();

    if (element > 0) {
        m_tableScene->items().at(--element)->setSelected(true);
    }
}

void periodSystem::slotUnSelectElements()
{
    foreach ( QGraphicsItem *item , m_tableScene->selectedItems()) {
        item->setSelected( false );
    }
}

bool periodSystem::event(QEvent *e)
{
    return QGraphicsView::event(e);
}

void periodSystem::setBiggerSceneRect()
{
    QRectF newRect(0, 0, sceneRect().width(), sceneRect().height());
    const QPoint maxCoords = pseTables::instance()->getTabletype( m_currentTableInex )->coordsMax();

    if (sceneRect().width() < maxCoords.x() * m_width) {
        newRect.setWidth((maxCoords.x() + 1) * m_width);
    }

    if ( sceneRect().height() < maxCoords.y() * m_height ) {
        newRect.setHeight((maxCoords.y() + 1) * m_height );
    }

    setSceneRect(newRect);
}

void periodSystem::resizeEvent ( QResizeEvent * event )
{
    fitPseInView();
    QGraphicsView::resizeEvent(event);
}

void periodSystem::fitPseInView()
{
    const QPoint maxCoords = pseTables::instance()->getTabletype( m_currentTableInex )->coordsMax();
    if (operator!=(sceneRect(), QRectF(0, 0, maxCoords.x() * m_width, (maxCoords.y() + 1) * m_height)) ) {
        setSceneRect(0, 0, maxCoords.x() * m_width, (maxCoords.y() + 1) * m_height);
    }
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void periodSystem::generateSvg(const QString& filename)
{
    QSvgGenerator *svgGen = new QSvgGenerator();
    svgGen->setFileName( filename );

    const QPoint maxCoords = pseTables::instance()->getTabletype( m_currentTableInex )->coordsMax();

    QPainter painter;
    painter.begin( svgGen );
    render( &painter ,
            QRectF(0 , m_height, ( maxCoords.x() + 2.5) * m_width, (maxCoords.y() + 2.5) * m_height),
            QRect(0 , m_height, ( maxCoords.x() + 2.5) * m_width, (maxCoords.y() + 2.5) * m_height));
    painter.rotate(180);
    painter.end();

    delete svgGen;
}

#include "periodsystembase.moc"
