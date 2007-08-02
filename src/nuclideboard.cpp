/***************************************************************************
 *   Copyright (C) 2005, 2006 by Carsten Niehaus                                 *
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


#include "nuclideboard.h"
#include "kalziumdataobject.h"
#include "kalziumutils.h"
#include "ui_isotopedialog.h"

#include <element.h>
#include <isotope.h>

#include <QEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QFile>
#include <QGraphicsTextItem>
#include <QGraphicsItemGroup>

#include <kaction.h>
#include <kactioncollection.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <ktoggleaction.h>
#include <ktoolinvocation.h>
#include <kicon.h>
#include <kstandarddirs.h>

    IsotopeTableDialog::IsotopeTableDialog( QWidget* parent )
: KDialog( parent )
{
    ui.setupUi( mainWidget() );
    IsotopeScene *scene = new IsotopeScene();

    connect( ui.pixelSpin, SIGNAL(valueChanged(int)),
            scene, SLOT( slotSetItemSize(int)));

    connect( ui.guides, SIGNAL(stateChanged(int)),
            scene, SLOT(slotToggleVisualGuides(int)) );
    
    connect( ui.infowidget, SIGNAL(stateChanged(int)),
            scene, SLOT(slotToggleInfowidget(int)) );

    scene->slotSetItemSize( ui.pixelSpin->value() );
    
    ui.gv->setScene(scene);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    IsotopeScene::IsotopeScene(QObject *parent)
: QGraphicsScene(parent)
{
    m_isotopeGroup = new QGraphicsItemGroup();
    m_isotopeGroup->setHandlesChildEvents(false);
    addItem( m_isotopeGroup );

    m_infoItem = new InformationItem( 40 , 40, 100, 100 );
    addItem( m_infoItem );

    m_itemSize = 10;
    drawIsotopes();
    m_isotopeGroup->scale(1, -1);
}

void IsotopeScene::slotToggleInfowidget(int state)
{
    kDebug() << "IsotopeScene::slotToggleInfowidget()";
    if ( state == Qt::Checked ) {
        m_infoItem->setVisible( true );
    } else {
        m_infoItem->setVisible( false );
    }
}

void IsotopeScene::slotToggleVisualGuides(int state)
{
    if ( state == Qt::Checked ) {
    } else {
    }
}

void IsotopeScene::updateContextHelp( IsotopeItem * item )
{
    m_infoItem->setIsotope( item->isotope() );
}

void IsotopeScene::drawIsotopes()
{
    QList<Element*> elist = KalziumDataObject::instance()->ElementList;

    foreach ( Element * e, elist ) {
        const int elementNumber = e->dataAsVariant( ChemicalDataObject::atomicNumber ).toInt();

        QList<Isotope*> ilist = KalziumDataObject::instance()->isotopes( elementNumber );
        foreach (Isotope *i , ilist )
        {
            IsotopeItem *item = new IsotopeItem( i, elementNumber*m_itemSize ,i->nucleons()*m_itemSize, m_itemSize,m_itemSize);
            item->setToolTip( i18n("Isotope of Element %1 (%2)", i->parentElementNumber() ,i->parentElementSymbol() ) );
            m_isotopeGroup->addToGroup( item );
        }
    }
}

void IsotopeScene::slotSetItemSize(int itemsize)
{
    m_itemSize = itemsize;

    foreach (QGraphicsItem *i, items() )
    {
        //check if the item is an IsotopeItem. If not: do nothing
        if (qgraphicsitem_cast<IsotopeItem *>(i)) 
        {
            IsotopeItem *ii = static_cast<IsotopeItem*>(i);

            QRect newRect( ii->isotope()->parentElementNumber() * m_itemSize, ii->isotope()->nucleons() * m_itemSize , m_itemSize, m_itemSize );
            ii->setRect( newRect );
        }
    }
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    IsotopeItem::IsotopeItem( Isotope * i, qreal x, qreal y, qreal width, qreal height,   QGraphicsItem *parent)
:  QGraphicsRectItem(x,y,width,height,parent)
{
    m_isotope = i;
    
    m_type = getType( m_isotope );

    QBrush b;
    switch (m_type) {
        case alpha:
            b = QBrush( Qt::red );
            break;
        case ec:
            b = QBrush( Qt::blue );
            break;
        case multiple:
            b = QBrush( Qt::green );
            break;
        case bplus:
            b = QBrush( Qt::yellow );
            break;
        case bminus:
            b = QBrush( Qt::lightGray );
            break;
        case stable:
            b = QBrush( Qt::darkGray );
            break;
        default:
            b = QBrush( Qt::darkGray );
            break;
    }
    setBrush(b);

    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

IsotopeItem::IsotopeType IsotopeItem::getType( Isotope * isotope )
{
    //TODO Here I need a clever way to find out *what* to return. 
    if (isotope->alphalikeliness() > 60.0 )
        return IsotopeItem::alpha;
    if (isotope->betaminuslikeliness() > 60.0 )
        return IsotopeItem::bminus;
    if (isotope->betapluslikeliness() > 60.0 )
        return IsotopeItem::bminus;
    if (isotope->eclikeliness() > 60.0 )
        return IsotopeItem::ec;
    else
        return IsotopeItem::stable;
}

void IsotopeItem::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
    if (event->button() != Qt::RightButton)
        return;
    
    IsotopeScene *scene2 = static_cast<IsotopeScene*>(scene());
    scene2->updateContextHelp( this );
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    InformationItem::InformationItem( qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent)
:  QGraphicsRectItem(x,y,width,height,parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);

    setBrush( QBrush( Qt::yellow ) );
    
    m_textitem = new QGraphicsTextItem( this );
    m_textitem->setPos( 0, 0 );
}

void InformationItem::setIsotope( Isotope * i )
{
    QString html = i18n("<h1>%1</h1> Number: %2", i->parentElementSymbol(), i->parentElementNumber());

    m_textitem->setHtml( html );
}


#include "nuclideboard.moc"

