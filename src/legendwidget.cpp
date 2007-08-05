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
#include <kimageeffect.h>
#include <kglobalsettings.h>

#include <QPainter>
#include <QPixmap>
#include <QBrush>
#include <QColor>
#include <QFont>
#include <QLabel>
#include <QGridLayout>
#include <QFrame>

LegendWidget::LegendWidget( QWidget *parent )
  : QWidget( parent )
{
    setMinimumHeight(80);
}

void LegendWidget::setGradientType( KalziumGradientType * type )
{
    kDebug() << "setGradientType";
    m_gradientType = type;
    updateContent();
}
void LegendWidget::setTableType( KalziumTableType * type )
{
    m_tableType = type;
    updateContent();
}

void LegendWidget::setScheme( KalziumSchemeType * type )
{
    m_scheme = type;
    updateContent();
}

void LegendWidget::setMode( KalziumPainter::MODE m )
{
    m_mode = m;
    updateContent();
}

void LegendWidget::updateContent()
{
    QList< QPair<QString, QBrush> > items;

    switch ( m_mode )
    {
        case KalziumPainter::NORMAL://nothing to do here, all logic done in SOM
        case KalziumPainter::SOM:
            {
                if ( !m_scheme ) return;

                QList<legendPair> items;
                if ( m_mode == KalziumPainter::SOM )
                {
                    items << qMakePair( i18nc("one of the three states of matter (solid, liquid, vaporous or unknown)", "Solid" ), QBrush( Prefs::color_solid() ) );
                    items << qMakePair( i18nc("one of the three states of matter (solid, liquid, vaporous or unknown)", "Liquid" ), QBrush( Prefs::color_liquid() ) );
                    items << qMakePair( i18nc("one of the three states of matter (solid, liquid, vaporous or unknown)", "Vaporous" ), QBrush( Prefs::color_vapor() ) );
                    items << qMakePair( i18nc("one of the three states of matter (solid, liquid, vaporous or unknown)", "Unknown" ), QBrush( Qt::lightGray ) );
                } else {
                    items = m_scheme->legendItems();
                }

                updateLegendItemLayout( items );

                break;
            }
        case KalziumPainter::GRADIENT:
            {
                QList<legendPair> items;
                items << qMakePair( i18n( "Gradient: %1" ,m_gradientType->description() ), QBrush() );
                items << qMakePair( i18nc( "Minimum value of the gradient" , "Minimum: %1" , m_gradientType->minValue() ), QBrush( m_gradientType->firstColor() ) );
                items << qMakePair( i18nc( "Maximum value of the gradient" , "Maximum: %1" , m_gradientType->maxValue() ), QBrush( m_gradientType->secondColor() ) );

                updateLegendItemLayout( items );
                break;
            }
        case KalziumPainter::TIME:
            {
                break;
            }
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

    foreach ( legendPair pair, list )
    {
        LegendItem *item = new LegendItem( pair );

        m_legendItemList.append(item);

        layout->addWidget(item , x, y );

        x++;

        if ( x >= 4 ) {
            x = 0;
            y++;
        }
    }

    setLayout( layout );
}

LegendItem::LegendItem(const QPair<QString, QBrush>& pair, QWidget * parent)
{
    Q_UNUSED(parent);
    m_pair = pair;

    update();
}

void LegendItem::paintEvent( QPaintEvent * /* e */ )
{
    QPainter p;
    p.begin(this);
    QRect rect(0, 0, height(), height() );
    p.fillRect( rect , QBrush( m_pair.second ) );

    QRect textRect( height() + 10 , 0 , width() - 10 , height() );

    p.drawText( textRect, Qt::AlignLeft | Qt::AlignVCenter , m_pair.first );
    p.end();
}

#include "legendwidget.moc"

