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


#include "nuclideboard.h"
#include "nuclideboardview.h"
#include "kalziumdataobject.h"
#include "ui_isotopedialog.h"
#include "kalziumpainter.h"
#include "legendwidget.h"

#include <element.h>
#include <isotope.h>

#include <klocale.h>

#include <kdebug.h>
#include <QStyleOptionGraphicsItem>
#include<prefs.h>

    IsotopeTableDialog::IsotopeTableDialog( QWidget* parent )
: KDialog( parent )
{
    setCaption(i18n("Isotope Table"));
    ui.setupUi( mainWidget() );
    ui.guide->setGuidedView( ui.gv );

    connect( ui.gv->scene(), SIGNAL( itemSelected(IsotopeItem*) ),
            this, SLOT( updateDockWidget( IsotopeItem*) )  );
    connect( ui.Slider,   SIGNAL( valueChanged( int ) ),
             this, SLOT( zoom ( int ) ));
    
    //Here comes the legend part         
    QList< QPair<QString, QBrush> > items;
    QVBoxLayout * layout = new QVBoxLayout;
    
    items << qMakePair( i18nc("alpha ray emission", "alpha" ), QBrush( Qt::red ) );
	items << qMakePair( i18nc("Electron capture method", "EC" ), QBrush( Qt::blue ) );
	items << qMakePair( i18nc("Many ways", "Multiple" ), QBrush( Qt::green ) );
	items << qMakePair( i18nc("Beta plus ray emission", "Beta +" ), QBrush( Qt::yellow ) );
	items << qMakePair( i18nc("Beta minus ray emission", "Beta -" ), QBrush( Qt::white ) );
	items << qMakePair( i18nc("Stable isotope", "Stable" ), QBrush( Qt::lightGray ) );
	items << qMakePair( i18nc("Default colour", "default" ), QBrush( Qt::	darkGray ) );
	
	int x = 0;
	foreach ( const legendPair &pair, items )
    {
		LegendItem *item = new LegendItem( pair );
		layout->addWidget(item , x );
	}
	ui.legendDisplay->setLayout(layout);
}

void IsotopeTableDialog::zoom (int level)
{
    double zoom = level/2.0;
    kDebug() << "level";
    (ui.gv)->setZoom( zoom );
}

void IsotopeTableDialog::updateDockWidget( IsotopeItem * item )
{
    QString empty ="";
    Isotope *s = item->isotope();

    QString header = i18n("<h1>%1 (%2)</h1>", s->parentElementSymbol(), s->parentElementNumber());
    QString mag = i18n("Magnetic moment: %1", (empty == s->magmoment())? i18n("Unknown"):s->magmoment() );

    QString halflife;
    if ( s -> halflife() > 0.0 ) {
        halflife = i18n ("Halflife: %1 %2", s->halflife(), s->halflifeUnit() );
    }
    else {
        halflife = i18n ("Halflife: Unknown");
    }

    QString abundance = i18n("Abundance: %1 %", empty != (s->abundance()) ? s->abundance() : "0" );
    QString nucleons = i18n("Number of nucleons: %1", s->nucleons() );
    QString spin = i18n("Spin: %1", (empty == s->spin())? i18n("Unknown"): s->spin() );
    QString exactMass = i18n("Exact mass: %1 u", s->mass() );

    QString html = header + "<br />" + nucleons + "<br />" + mag  + "<br />" + exactMass + "<br />" + spin +"<br />" +
        abundance + "<br />" + halflife;

    ui.label->setText(html);
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    IsotopeItem::IsotopeItem( Isotope * i, qreal x, qreal y, qreal width, qreal height,   QGraphicsItem *parent)
:  QAbstractGraphicsShapeItem(parent)
{
    m_rect = QRectF( x, y, width, height );
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
            b = QBrush( Qt::white );
            break;
        case stable:
            b = QBrush( Qt::lightGray );
            break;
        default:
            b = QBrush( Qt::darkGray );
            break;
    }
    setBrush(b);

    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, false);
}

void IsotopeItem::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget )
{
    Q_UNUSED(widget)
    // FIXME: Get rid of magic numbers and rather dynamically calculate them
    QRectF r1( m_rect.translated( 0.0, 2.5 ) );
    QRectF r2( m_rect.topLeft() + QPointF( 1.0, 0.5 ), m_rect.size() / 2.0 );
    QRectF r3( m_rect.topLeft() + QPointF( 6.0, 0.5 ) , m_rect.size() / 2.0 );

    const qreal lod = option->levelOfDetailFromTransform( painter->worldTransform() );
    if ( lod > 0.3 )
        painter->setPen( pen() );
    else
        painter->setPen( Qt::NoPen );
    painter->setBrush( brush() );
    painter->drawRect( m_rect );

    if ( lod >= 1.0 ) {
        painter->setFont( QFont( "Arial", 3 ,QFont::Bold) );
        painter->drawText( r1, Qt::AlignHCenter | Qt::TextDontClip, m_isotope->parentElementSymbol() );//, s->parentElementNumber()
        painter->setFont( QFont( "Arial", 1 ,QFont::Bold) );
        painter->drawText( r2, Qt::AlignHCenter | Qt::TextDontClip, QString::number( m_isotope->parentElementNumber() ) );
        painter->setFont( QFont( "Arial", 1 ,QFont::Bold) );
        painter->drawText( r3, Qt::AlignHCenter | Qt::TextDontClip, QString::number( m_isotope->nucleons() ) );
    }
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
    if (event->button() != Qt::RightButton) {
        event->ignore();
        return;
    }

    IsotopeScene *scene2 = static_cast<IsotopeScene*>(scene());
    scene2->updateContextHelp( this );
}

#include "nuclideboard.moc"

