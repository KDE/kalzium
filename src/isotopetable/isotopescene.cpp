/***************************************************************************
 *   Copyright (C) 2005, 2006, 2007, 2008 by Carsten Niehaus               *
 *   cniehaus@kde.org                                                      *
 *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.             *
 ***************************************************************************/


#include "isotopescene.h"
#include "isotopeitem.h"
#include "kalziumdataobject.h"

#include <element.h>
#include <isotope.h>

#include <klocale.h>

    IsotopeScene::IsotopeScene(QObject *parent)
: QGraphicsScene(parent)
{
    m_isotopeGroup = new QGraphicsItemGroup();
    m_isotopeGroup->setHandlesChildEvents(false);
    addItem( m_isotopeGroup );

    m_itemSize = 10;
    drawIsotopes();
    //m_isotopeGroup->scale(-1, 1);
}

void IsotopeScene::updateContextHelp( IsotopeItem * item )
{
    emit itemSelected( item );
}

void IsotopeScene::drawIsotopes()
{
    QList<Element*> elist = KalziumDataObject::instance()->ElementList;
    int mode =0;

    foreach ( Element * e, elist ) {
        int elementNumber = e->dataAsVariant( ChemicalDataObject::atomicNumber ).toInt();

        QList<Isotope*> ilist = KalziumDataObject::instance()->isotopes( elementNumber );
        foreach (Isotope *i , ilist )
        {
            int x = elementNumber * m_itemSize;
            int y =  ( 300 - i->nucleons() ) * m_itemSize;

            if (mode == 0) {
                //One part to the side of the other
                int threshold = 60;
                if ( elementNumber > threshold ) {
                    y += 120 * m_itemSize;
                    x += 5 * m_itemSize;
                }
            }
            else if (mode == 1) {
                //one part above the other part
                int threshold = 0;
                if ( elementNumber > threshold ) {
                    y -= 20 * m_itemSize;
                    x -= threshold * m_itemSize;
                }
            }
            else if (mode == 2) {
                //Both parts continuous
            }

            IsotopeItem *item = new IsotopeItem( i, x, y, m_itemSize,m_itemSize);
            item->setToolTip( i18n("Isotope of Element %1 (%2)", i->parentElementNumber() ,i->parentElementSymbol() ) );
            m_isotopeGroup->addToGroup( item );
        }
    }
}

#include "isotopescene.moc"
