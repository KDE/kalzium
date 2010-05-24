/***************************************************************************
 *   Copyright (C) 2007 by Carsten Niehaus                                 *
 *   cniehaus@kde.org                                                      *
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
#include "legendwidget.h"
#include "prefs.h"
#include "kalziumschemetype.h"
#include "kalziumgradienttype.h"
#include "kalziumpainter.h"

#include <kdebug.h>
#include <klocale.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>

LegendWidget::LegendWidget( QWidget *parent )
        : QWidget( parent )
{
    m_update = true;
    m_dockArea = Qt::BottomDockWidgetArea;
    setMinimumSize(130, 50);
}

void LegendWidget::setGradientType( KalziumGradientType * type )
{
    kDebug() << "setGradientType LegendWidget";
    m_gradientType = type;
    updateContent();
}
void LegendWidget::setTableType( KalziumTableType * type )
{
    kDebug() << "setTableType LegendWidget";
    m_tableType = type;
    updateContent();
}

void LegendWidget::setScheme( KalziumSchemeType * type )
{
    kDebug() << "setScheme LegendWidget";
    m_scheme = type;
    updateContent();
}

void LegendWidget::setMode( KalziumPainter::MODE m )
{
    kDebug() << "setMode LegendWidget";
    m_mode = m;
    updateContent();
}

void LegendWidget::setDockArea( Qt::DockWidgetArea newDockArea )
{
    kDebug() << "dock Area changed" << newDockArea;
    m_dockArea = newDockArea;
    updateContent();
}

void LegendWidget::LockWidget()
{
    m_update = false;
}

void LegendWidget::UnLockWidget()
{
    m_update = true;
}

void LegendWidget::updateContent()
{
    if (m_update) {
        QString gradientDesc;
        QList< QPair<QString, QColor> > items;
        switch ( m_mode ) {
        case KalziumPainter::SOM:
            items << qMakePair( i18nc("one of the three states of matter (solid, liquid, vaporous or unknown)", "Solid" ), QColor( Prefs::color_solid() ));
            items << qMakePair( i18nc("one of the three states of matter (solid, liquid, vaporous or unknown)", "Liquid" ), QColor( Prefs::color_liquid() ) );
            items << qMakePair( i18nc("one of the three states of matter (solid, liquid, vaporous or unknown)", "Vaporous" ), QColor( Prefs::color_vapor() ) );
            items << qMakePair( i18nc("one of the three states of matter (solid, liquid, vaporous or unknown)", "Unknown" ), QColor( Qt::lightGray ) );
            break;

        case KalziumPainter::NORMAL_GRADIENT:
        case KalziumPainter::GRADIENT:
            if (m_gradientType->logarithmicGradient())
                gradientDesc = i18nc("one of the two types of gradients available", "logarithmic");
            else
                gradientDesc = i18nc("one of the two types of gradients available", "linear");
            items << qMakePair( i18n( "%1 (%2)" ,m_gradientType->description(), gradientDesc ), QColor() );
            items << qMakePair( i18nc( "Minimum value of the gradient" , "Minimum: %1" , m_gradientType->minValue() ), QColor( m_gradientType->firstColor() ));
            items << qMakePair( i18nc( "Maximum value of the gradient" , "Maximum: %1" , m_gradientType->maxValue() ), QColor( m_gradientType->secondColor() ));

        case KalziumPainter::TIME:
            // case KalziumPainter::NORMAL:
        default:
            items << qMakePair( i18n( "Scheme: %1" ,m_scheme->description() ), QColor() );
            items << m_scheme->legendItems();
            break;
        }
        updateLegendItemLayout( items );
    }
}

void LegendWidget::updateLegendItemLayout( const QList<legendPair>& list )
{
    if (layout()) {
        delete layout();
    }
    foreach ( LegendItem * i, m_legendItemList ) {
        delete i;
    }

    m_legendItemList.clear();

    QGridLayout * layout = new QGridLayout( this );

    int x = 0;
    int y = 0;

    foreach ( const legendPair &pair, list )
    {
        LegendItem *item = new LegendItem( pair );
        m_legendItemList.append(item);

        if ((m_dockArea == Qt::BottomDockWidgetArea || m_dockArea == Qt::TopDockWidgetArea) ) {
            if (!pair.second.isValid() ) {
                y++;
                x = 0;
            }
            if ( x >= 3) {
                y++;
                x = 1;
            }
        }
        layout->addWidget(item , x, y);
        x++;
    }
    setLayout( layout );
}



LegendItem::LegendItem(const QPair<QString, QColor>& pair, QWidget * parent)
{
    Q_UNUSED(parent);
    QHBoxLayout *ItemLayout = new QHBoxLayout(this);
    QLabel * LegendLabel = new QLabel(this);

    if ( pair.second.isValid() ) {
        QPixmap LegendPixmap(20, 20);
        LegendPixmap.fill(pair.second);
        QLabel * LabelPixmap = new QLabel(this);
        LabelPixmap->setPixmap(LegendPixmap);
        ItemLayout->addWidget(LabelPixmap);
    }
    LegendLabel->setText(pair.first);
    ItemLayout->addWidget(LegendLabel);
    ItemLayout->setAlignment(Qt::AlignLeft);
    setLayout(ItemLayout);
}

#include "legendwidget.moc"

