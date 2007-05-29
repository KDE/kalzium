#ifndef NUCLIDEBOARD_H
#define NUCLIDEBOARD_H
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

#include <QWidget>
#include <QList>
#include <QPixmap>
#include <QPoint>
#include <QRect>
#include <QScrollArea>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
 #include <QGraphicsPixmapItem>

#include "ui_isotopedialog.h"

#include <kdialog.h>

class QColor;
class QPainter;
class IsotopeScrollArea;
class KActionCollection;
class Element;
class Isotope;
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;

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
};

class IsotopeScene : public QGraphicsScene
{
    Q_OBJECT

    public:
        IsotopeScene( QObject * parent = 0);

    signals:
        void itemSelected(QGraphicsItem *item);

    private:
        void drawIsotopes();

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

	
class IsotopeItem : public QGraphicsRectItem
{
	public:
            IsotopeItem( qreal x, qreal y, qreal width, qreal height, QGraphicsItem *parent = 0);
	
	
};

#endif // NUCLIDEBOARD_H
