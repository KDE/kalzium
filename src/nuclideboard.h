#ifndef NUCLIDEBOARD_H
#define NUCLIDEBOARD_H
/***************************************************************************
 *   Copyright (C) 2007, 2008 by Carsten Niehaus                           *
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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>

class IsotopeItem;
class Isotope;

#include "ui_isotopedialog.h"

#include <kdialog.h>

class Isotope;
class InformationItem;

class QGraphicsSceneMouseEvent;
class QGraphicsTextItem;

/**
 * This class is the drawing widget for the whole table
 *
 * @author Pino Toscano
 * @author Carsten Niehaus
 */
class IsotopeTableDialog : public KDialog
{
    Q_OBJECT

    public:
        explicit IsotopeTableDialog( QWidget* parent = 0 );

    private:
        Ui::isotopeWidget ui;

    private slots:
        void updateDockWidget( IsotopeItem * );
        void zoom (int);
        
};

/**
 * The class represtens the items which is drawn on the QGraphicsScene. Each such item represents on
 * Isotope.
 * @author Carsten Niehaus
 */
class IsotopeItem : public QAbstractGraphicsShapeItem
{
	public:
            /**
             * there are several types of decay for an isotope.
             */
            enum IsotopeType { alpha, ec, multiple, bplus, bminus, stable };

            enum { Type = UserType + 1 };

            /**
             * @param isotope The Isotope represented
             */
            IsotopeItem(Isotope * isotope,  qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);

            /**
             * @return the Isotope the item represents
             */
            Isotope* isotope() const{
                return m_isotope;
            }

            QRectF boundingRect() const {
                return m_rect;
            }

            /**
             * @return the Type of the item
             */
            int type() const{
                return Type;
            }

            void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );


        private:
            IsotopeType m_type;
            Isotope* m_isotope;
            QRectF m_rect;

            /**
             * @return the IsotopeType of the Isotope
             */
            IsotopeType getType( Isotope * );

        protected:
            void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

class InformationItem : public QGraphicsRectItem
{
    public:
        enum { Type = UserType + 2 };

        /**
         * @param isotope The Isotope represented
         */
        InformationItem( qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);

        /**
         * @return the Type of the item
         */
        int type() const{
            return Type;
        }

        void setIsotope( IsotopeItem * item );

    private:
        QGraphicsTextItem *m_textitem;
};


class IsotopeScene : public QGraphicsScene
{
    Q_OBJECT

    public:
        IsotopeScene( QObject * parent = 0);

        void updateContextHelp( IsotopeItem * item );

    private:
        void drawIsotopes(void);

        //the size of each item
        int m_itemSize;

        ///this group stores all IsotopeItems
        QGraphicsItemGroup *m_isotopeGroup;

    signals:
        void itemSelected(IsotopeItem *item);
};

	
#endif // NUCLIDEBOARD_H
