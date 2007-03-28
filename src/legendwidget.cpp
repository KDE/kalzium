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

#include <QCursor>
#include <QKeyEvent>
#include <QSizePolicy>
#include <QPainter>
#include <QPixmap>
#include <QBrush>
#include <QColor>
#include <QFont>


LegendWidget::LegendWidget( QWidget *parent )
  : QWidget( parent )
{
}

void LegendWidget::setMode( KalziumPainter::MODE m )
{
	m_mode = m;
}

void LegendWidget::paintEvent( QPaintEvent * /*e*/ )
{
	m_pixmap = QPixmap( width(), height() );
	m_pixmap.fill( this, width(), height() );

	QPainter p;
	p.begin( &m_pixmap );

    switch ( m_mode )
    {
        case KalziumPainter::NORMAL:
        case KalziumPainter::SOM:
            {
                kDebug() << "SOM SOM SOM" << endl;
                // settings font
                QFont legendFont = KGlobalSettings::generalFont();
                legendFont.setPointSize( legendFont.pointSize() + 1 );
                p.setFont( legendFont );

                int padding = 6;
                QRect legendRect = QRect(0,0,width(), height() );
                p.fillRect( legendRect, QBrush( Qt::lightGray ) );
                int itemheight = ( legendRect.height() - 6 * padding ) / 5;
                int squareside = itemheight - 2;

                legendList items;
                items << qMakePair( i18n( "Solid" ), QBrush( Prefs::color_solid() ) );
                items << qMakePair( i18n( "Liquid" ), QBrush( Prefs::color_liquid() ) );
                items << qMakePair( i18n( "Vaporous" ), QBrush( Prefs::color_vapor() ) );
                items << qMakePair( i18n( "Unknown" ), QBrush( Qt::lightGray ) );

                // we allow max 10 items in the legend
                int numitems = qMin( items.count(), 10 );
                int itemwidth = legendRect.width() - 2 * padding;
                if ( numitems > 5 )
                    itemwidth = ( itemwidth - padding ) / 2;

                for ( int i = 0; i < numitems; i++ )
                {
                    int x = legendRect.left() + padding + ( i > 4 ? itemwidth + padding : 0 );
                    int y = legendRect.top() + padding + ( padding + itemheight ) * ( i % 5 );
                    QRect sq( x + 1, y + 1, squareside, squareside );
                    p.fillRect( sq, QBrush( items.at(i).second ) );
                    p.drawRect( sq );
                    QRect text( x + squareside + 5, y, itemwidth - squareside - 5, itemheight);
                    p.drawText( text, Qt::AlignLeft | Qt::AlignVCenter, items.at(i).first );
                }

                break;
            }
        case KalziumPainter::GRADIENT:
            {
                // settings font
                QFont legendFont = KGlobalSettings::generalFont();
                legendFont.setPointSize( legendFont.pointSize() + 1 );
                p.setFont( legendFont );

                int padding = 6;
                int sidepadding = 12;
                QRect legendRect = QRect(0,0,width(), height() );
                p.fillRect( legendRect, QBrush( Qt::lightGray ) );

                QRect text = legendRect;
                text.setHeight( text.height() / 2 - 2 * padding );
                text.setWidth( text.width() - 2 * sidepadding );
                text.translate( sidepadding, padding );

                QSize imgsize( legendRect.width() - 2 * sidepadding, 20 );
//X                 QImage img = KImageEffect::gradient( imgsize, m_gradient->firstColor(), m_gradient->secondColor(), KImageEffect::HorizontalGradient );
//X 
//X                 QRect othertexts = text;
//X                 othertexts.moveTo( text.bottomLeft() + QPoint( 0, padding + 4 + imgsize.height() ) );
//X 
//X                 p.drawText( text, Qt::AlignHCenter | Qt::AlignBottom, i18n( "Gradient: %1", m_gradient->description() ) );
//X                 p.drawPixmap( text.bottomLeft() + QPoint( 0, padding ), QPixmap::fromImage( img ) );
//X 
//X                 p.drawText( othertexts, Qt::AlignRight, QString::number( m_gradient->maxValue() ) );
//X                 p.drawText( othertexts, Qt::AlignLeft, QString::number( m_gradient->minValue() ) );

                break;
            }
        case KalziumPainter::TIME:
            {
                break;
            }
    }

	p.end();

	QPainter p2(this);
	p2.drawPixmap(0, 0, m_pixmap);
}

#include "legendwidget.moc"

