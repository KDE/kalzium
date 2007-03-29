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
#include <QLabel>
#include <QGridLayout>


LegendWidget::LegendWidget( QWidget *parent )
  : QWidget( parent )
{
    updateContent();
}

void LegendWidget::setMode( KalziumPainter::MODE m )
{
	m_mode = m;
}

void LegendWidget::updateContent()
{
    QList< QPair<QString, QBrush> > items;
    QGridLayout * layout = new QGridLayout( this );

    m_mode = KalziumPainter::SOM;

    switch ( m_mode )
    {
        case KalziumPainter::NORMAL:
            {//Taking care of the schemes

                break;
            }
        case KalziumPainter::SOM:
            {
                legendList items;
                items << qMakePair( i18n( "Solid" ), QBrush( Prefs::color_solid() ) );
                items << qMakePair( i18n( "Liquid" ), QBrush( Prefs::color_liquid() ) );
                items << qMakePair( i18n( "Vaporous" ), QBrush( Prefs::color_vapor() ) );
                items << qMakePair( i18n( "Unknown" ), QBrush( Qt::lightGray ) );
                items << qMakePair( i18n( "Solid" ), QBrush( Prefs::color_solid() ) );
                items << qMakePair( i18n( "Liquid" ), QBrush( Prefs::color_liquid() ) );
                items << qMakePair( i18n( "Vaporous" ), QBrush( Prefs::color_vapor() ) );
                items << qMakePair( i18n( "Unknown" ), QBrush( Qt::lightGray ) );
                items << qMakePair( i18n( "Solid" ), QBrush( Prefs::color_solid() ) );
                items << qMakePair( i18n( "Liquid" ), QBrush( Prefs::color_liquid() ) );
                items << qMakePair( i18n( "Vaporous" ), QBrush( Prefs::color_vapor() ) );
                items << qMakePair( i18n( "Unknown" ), QBrush( Qt::lightGray ) );

                int x = 0;
                int y = 0;

                foreach ( legendItem item, items )
                {
                    QLabel * label = new QLabel( item.first, this );
                    layout->addWidget(label , x, y );

                    x++;

                    if ( x >= 4 ) {
                        x = 0;
                        y++;
                    }
                }

                break;
            }
        case KalziumPainter::GRADIENT:
            {
//X                 // settings font
//X                 QFont legendFont = KGlobalSettings::generalFont();
//X                 legendFont.setPointSize( legendFont.pointSize() + 1 );
//X                 p.setFont( legendFont );
//X 
//X                 int padding = 6;
//X                 int sidepadding = 12;
//X                 QRect legendRect = QRect(0,0,width(), height() );
//X                 p.fillRect( legendRect, QBrush( Qt::lightGray ) );
//X 
//X                 QRect text = legendRect;
//X                 text.setHeight( text.height() / 2 - 2 * padding );
//X                 text.setWidth( text.width() - 2 * sidepadding );
//X                 text.translate( sidepadding, padding );
//X 
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

#include "legendwidget.moc"

