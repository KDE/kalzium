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
                    items << qMakePair( i18n( "Solid" ), QBrush( Prefs::color_solid() ) );
                    items << qMakePair( i18n( "Liquid" ), QBrush( Prefs::color_liquid() ) );
                    items << qMakePair( i18n( "Vaporous" ), QBrush( Prefs::color_vapor() ) );
                    items << qMakePair( i18n( "Unknown" ), QBrush( Qt::lightGray ) );
                } else {
                    items = m_scheme->legendItems();
                }

                updateLegendItemLayout( items );

                break;
            }
        case KalziumPainter::GRADIENT:
            {
//X                 QSize imgsize( legendRect.width() - 2 * sidepadding, 20 );
//X //X                 QImage img = KImageEffect::gradient( imgsize, m_gradient->firstColor(), m_gradient->secondColor(), KImageEffect::HorizontalGradient );
//X //X 
//X //X                 QRect othertexts = text;
//X //X                 othertexts.moveTo( text.bottomLeft() + QPoint( 0, padding + 4 + imgsize.height() ) );
//X //X 
//X //X                 p.drawText( text, Qt::AlignHCenter | Qt::AlignBottom, i18n( "Gradient: %1", m_gradient->description() ) );
//X //X                 p.drawPixmap( text.bottomLeft() + QPoint( 0, padding ), QPixmap::fromImage( img ) );
//X //X 
//X //X                 p.drawText( othertexts, Qt::AlignRight, QString::number( m_gradient->maxValue() ) );
//X //X                 p.drawText( othertexts, Qt::AlignLeft, QString::number( m_gradient->minValue() ) );

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

