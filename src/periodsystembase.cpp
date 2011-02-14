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

periodSystem::periodSystem(QWidget *parent)
        : QGraphicsView(parent), m_width(42), m_height(42) // Some space between the elements (px40) looks nice.
{
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);

    setMouseTracking( true );

    m_currentTableInex = Prefs::table();
    m_hiddenPoint = QPoint(-40, -400);

    m_tableScene = new PeriodicTableScene(this);
    setScene(m_tableScene);
    connect(m_tableScene, SIGNAL(freeSpaceClick()), this, SLOT(fitPseInView()));

    createElementItems();

    createNumerationItems();

    setupStatesAndAnimation();

    fitPseInView();
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

void periodSystem::createElementItems()
{
    KalziumElementProperty *elProperty = KalziumElementProperty::instance();

    foreach (int intElement, pseTables::instance()->getTabletype( 0 )->elements()) {
        ElementItem *item = new ElementItem(elProperty, intElement);
        connect(elProperty, SIGNAL(propertyChanged()), item, SLOT(redraw()));
        m_tableScene->addObject(item);
    }
}

void periodSystem::setupStatesAndAnimation()
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

    connect(this , SIGNAL(tableChanged(int)), stateSwitcher, SLOT(slotSwitchState(int)));

    stateSwitcher->setInitialState( m_tableStatesList.at( m_currentTableInex ) );
    m_states.setInitialState(stateSwitcher);
    m_states.start();
}

void periodSystem::setNumerationItemPositions( int tableIndex )
{
    hideAllNumerationItems( tableIndex );

    for (int x = 0; x < maxNumerationItemXCoordinate( tableIndex ); ++x) {
        int itemAtPos = pseTables::instance()->getTabletype( tableIndex )->numeration( x );

        if ( itemAtPos > 0 ) {
            m_tableStatesList.at( tableIndex )->assignProperty(m_numerationItemList.at(itemAtPos - 1), "pos",
                    QPointF( x * m_width, m_height / 4));
            addElementAnimation( m_numerationItemList.at(itemAtPos - 1), x );
        }
    }
}

void periodSystem::hideAllNumerationItems(int tableIndex)
{
    foreach( NumerationItem *item, m_numerationItemList)
    m_tableStatesList.at( tableIndex )->assignProperty( item, "pos", QPointF( m_hiddenPoint ));
}

int periodSystem::maxNumerationItemXCoordinate(int tableIndex)
{
    return pseTables::instance()->getTabletype( tableIndex )->coordsMax().x() > m_numerationItemList.count() ?
           pseTables::instance()->getTabletype( tableIndex )->coordsMax().x() : m_numerationItemList.count();
}

void periodSystem::addElementAnimation(QGraphicsObject *object, int factor)
{
    QPropertyAnimation *anim = new QPropertyAnimation( object, "pos");
    anim->setDuration( 1600 + factor * 2);
    anim->setEasingCurve(QEasingCurve::InOutExpo);
    m_group->addAnimation( anim );
}

void periodSystem::setElementItemPositions(int tableIndex)
{
    for (int i = 0; i < m_tableScene->objects().count(); ++i) {
        QPoint coords = pseTables::instance()->getTabletype( tableIndex )->elementCoords( i + 1 );

        // put the not needed elements a bit away
        if ( coords.x() == 0) {
            coords = m_hiddenPoint;
        }

        m_tableStatesList.at( tableIndex )->assignProperty( m_tableScene->objects().at(i), "pos",
                QPointF( (coords.x()-1 ) * m_width, (coords.y()) * m_height));

        addElementAnimation( m_tableScene->objects().at( i ), i);
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

void periodSystem::slotSelectOneElement(int element)
{
    slotUnSelectElements();
    slotSelectAdditionalElement( element );
}

void periodSystem::slotSelectAdditionalElement(int element)
{
    if (element > 0)
        m_tableScene->items().at( --element )->setSelected( true );
}

void periodSystem::slotUnSelectElements()
{
    foreach ( QGraphicsItem *item , m_tableScene->selectedItems())
    item->setSelected( false );
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

periodSystem::~periodSystem()
{
    delete scene();
    delete m_group;
    qDeleteAll(m_tableStatesList);
}

#include "periodsystembase.moc"
