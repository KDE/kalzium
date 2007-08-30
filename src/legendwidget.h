#ifndef LEGENDWIDGET_H
#define LEGENDWIDGET_H
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

#include <kdialog.h>

#include "kalziumpainter.h"
#include "kalziumschemetype.h"

class LegendItem;
class KalziumGradientType;

/**
 * @author Carsten Niehaus
 *
 * The LegendWidget displays the explanations of what the user is currently 
 * seeing in the table
 */
class LegendWidget : public QWidget
{
    Q_OBJECT

    public:
        LegendWidget( QWidget *parent );

        ~LegendWidget(){}

    private:
        QPixmap m_pixmap;

        KalziumSchemeType * m_scheme;

        KalziumPainter::MODE m_mode;

        KalziumTableType * m_tableType;

        KalziumGradientType * m_gradientType;

        QList<LegendItem*> m_legendItemList;

        void updateLegendItemLayout( const QList<legendPair>& list );

    public slots:
        void updateContent();

        void setMode( KalziumPainter::MODE m );

        void setScheme( KalziumSchemeType * type );

        void setTableType( KalziumTableType * type );
        
        void setGradientType( KalziumGradientType * type );
};

/**
 * A LegendItem is displayed as one small rectangle which represents the
 * color or QBrush in the table with a short explanation for it.
 *
 * @author Carsten Niehaus
 */
class LegendItem : public QWidget
{
    Q_OBJECT

    public:
        LegendItem( const QPair<QString, QBrush>& pair, QWidget * parent = 0 );
        ~LegendItem(){}

    private:
        QPair<QString, QBrush> m_pair;

    protected:
        virtual void paintEvent( QPaintEvent * e );
};

#endif // LEGENDWIDGET_H
