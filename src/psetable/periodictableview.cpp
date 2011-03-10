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

#include "periodictableview.h"
#include "psetables.h"
#include <prefs.h>

PeriodicTableView::PeriodicTableView(QWidget *parent)
        : QGraphicsView(parent), m_width(42), m_height(42) // Some space between the elements (px40) looks nice.
{
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setCacheMode(QGraphicsView::CacheBackground);

    setMouseTracking( true );

    m_currentTableInex = Prefs::table();

    m_tableScene = new PeriodicTableScene(this);
    setScene(m_tableScene);
    connect(m_tableScene, SIGNAL(freeSpaceClick()), this, SLOT(fitPseInView()));

    m_tableStates = new PeriodicTableStates(
        createElementItems(),
        createNumerationItems(),
        m_width, m_height, this);

    fitPseInView();
}

QList<NumerationItem*> PeriodicTableView::createNumerationItems() const
{
    // Creating Nummerationitems here, we use the classic periodic table as reference (18 elements in a row)
    const int xMax = 18;

    QList<NumerationItem*> numerationItemList;

    for (int i = 0; i < xMax; ++i) {
        numerationItemList << new NumerationItem( i );
        m_tableScene->addItem( numerationItemList.at( i ) );
        connect(this, SIGNAL(numerationChange(int)), numerationItemList.at( i ), SLOT(setNumerationType(int)));
    }

    return numerationItemList;
}

QList<ElementItem*> PeriodicTableView::createElementItems() const
{
    QList<ElementItem*> elementItemList;
    KalziumElementProperty *elProperty = KalziumElementProperty::instance();

    foreach (int intElement, pseTables::instance()->getTabletype( 0 )->elements()) {
        ElementItem *item = new ElementItem(elProperty, intElement);

        connect(elProperty, SIGNAL(propertyChanged()), item, SLOT(redraw()));
        m_tableScene->addItem(item);
        elementItemList << item;
    }

    return elementItemList;
}

PeriodicTableScene* PeriodicTableView::pseScene() const
{
    return  m_tableScene;
}

int PeriodicTableView::table() const
{
    return m_currentTableInex;
}

void PeriodicTableView::slotChangeTable(int table)
{
    m_currentTableInex = table;
    Prefs::setTable( m_currentTableInex );

    setBiggerSceneRect();
    emit tableChanged( m_currentTableInex );
}

void PeriodicTableView::slotSelectOneElement(int element)
{
    slotUnSelectElements();
    slotSelectAdditionalElement( element );
}

void PeriodicTableView::slotSelectAdditionalElement(int element)
{
    if (element > 0)
        m_tableScene->items().at( --element )->setSelected( true );
}

void PeriodicTableView::slotUnSelectElements()
{
    foreach ( QGraphicsItem *item , m_tableScene->selectedItems())
    item->setSelected( false );
}

void PeriodicTableView::setBiggerSceneRect()
{
    QRectF currentRect(sceneRect());

    if ( currentRect.width() < currentPseRect().width() )
        currentRect.setWidth( currentPseRect().width() );

    if ( currentRect.height() < currentPseRect().width() )
        currentRect.setHeight( currentPseRect().width() );

    setSceneRect(currentRect);
}

QRectF PeriodicTableView::currentPseRect() const
{
    const QPoint maxTableCoords = pseTables::instance()->getTabletype( m_currentTableInex )->tableSize();

    const int x = maxTableCoords.x();

    // adding one for the numeration row.
    const int y = maxTableCoords.y() + 1;

    return QRectF(0, -m_height, x * m_width, y * m_height);
}

void PeriodicTableView::resizeEvent ( QResizeEvent * event )
{
    fitPseInView();
    QGraphicsView::resizeEvent(event);
}

void PeriodicTableView::fitPseInView()
{
    if ( operator!=( sceneRect(), currentPseRect() ) )
        setSceneRect( currentPseRect() );

    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

bool PeriodicTableView::event(QEvent *e)
{
    return QGraphicsView::event(e);
}

void PeriodicTableView::generateSvg(const QString& filename)
{
    QSvgGenerator *svgGen = new QSvgGenerator();
    svgGen->setFileName( filename );

    QPainter painter;
    painter.begin( svgGen );
    painter.rotate(180);
    render( &painter );
    painter.end();

    delete svgGen;
}

PeriodicTableView::~PeriodicTableView()
{
    delete scene();
    delete m_tableStates;
}

#include "periodictableview.moc"
