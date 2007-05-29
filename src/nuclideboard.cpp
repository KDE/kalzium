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

#include <element.h>
#include <isotope.h>

#include "kalziumdataobject.h"
#include "kalziumutils.h"
#include "ui_isotopedialog.h"

#include <QEvent>
#include <QMenu>
#include <QMouseEvent>
#include <QLabel>
#include <QLayout>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QPolygon>
#include <QSizePolicy>
#include <QToolBar>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QPainter>
#include <QSvgRenderer>
#include <QFile>

#include <kaction.h>
#include <kactioncollection.h>
#include <kcursor.h>
#include <kdebug.h>
#include <kglobal.h>
#include <klocale.h>
#include <kstandardaction.h>
#include <ktoggleaction.h>
#include <ktoolinvocation.h>
#include <kicon.h>
#include <kstandarddirs.h>

#include <math.h>
#include <stdlib.h>


    IsotopeTableDialog::IsotopeTableDialog( QWidget* parent )
: KDialog( parent )
{
    ui.setupUi( mainWidget() );
    IsotopeScene *scene = new IsotopeScene();
    ui.gv->setScene(scene);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    IsotopeScene::IsotopeScene(QObject *parent)
: QGraphicsScene(parent)
{
    kDebug() << "IsotopeScene::IsotopeScene(QObject *parent)" << endl;
    drawIsotopes();
}

void IsotopeScene::drawIsotopes()
{
    QList<Element*> elist = KalziumDataObject::instance()->ElementList;

    foreach ( Element * e, elist ) {
        const int elementNumber = e->dataAsVariant( ChemicalDataObject::atomicNumber ).toInt();

        QList<Isotope*> ilist = KalziumDataObject::instance()->isotopes( elementNumber );
        foreach (Isotope *i , ilist )
        {
            kDebug() << "   Isotope of " << i->parentElementSymbol() << " with a mass of " << i->mass() << " and " << i->nucleons() << " nucleons." << endl;
            IsotopeItem::IsotopeType type = getType( i );
            IsotopeItem *item = new IsotopeItem(type, elementNumber*10 ,i->nucleons()*10, 10,10);
            addItem(item);
        }
    }
}


void IsotopeScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void IsotopeScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
}

void IsotopeScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

IsotopeItem::IsotopeType IsotopeScene::getType( Isotope * isotope )
{
    return IsotopeItem::bminus;
}





//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    IsotopeItem::IsotopeItem( IsotopeType type, qreal x, qreal y, qreal width, qreal height,   QGraphicsItem *parent)
:  QGraphicsRectItem(x,y,width,height,parent)
{
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}




#include "nuclideboard.moc"

