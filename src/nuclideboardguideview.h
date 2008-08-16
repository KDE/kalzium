#ifndef NUCLIDEBOARDGUIDEVIEW_H
#define NUCLIDEBOARDGUIDEVIEW_H
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
#include <QGraphicsView>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QPixmap>

#include "nuclideboardview.h"

class IsotopeScene;

class IsotopeGuideView : public QGraphicsView
{
    Q_OBJECT

    public:
        IsotopeGuideView( QWidget * parent = 0);
        void setGuidedView( IsotopeView *guidedView );
        void drawItems( QPainter * painter, int numItems, QGraphicsItem ** items, const QStyleOptionGraphicsItem * options );

    protected:
        void drawForeground( QPainter *painter, const QRectF &rect );

    private:
    	IsotopeScene *m_scene;
        IsotopeView *m_guidedView;
        double m_zoomLevel;
        double m_scale;
        QPolygonF m_visibleSceneRect;
        bool m_pixmapRepaintRequested;
        QPixmap *m_pixmap;
        QPoint m_lastMousePos;
        bool m_dragEvent;
    
        void resizeEvent( QResizeEvent *event );
        void mousePressEvent( QMouseEvent *event );
        void mouseReleaseEvent( QMouseEvent *event );
        void mouseMoveEvent( QMouseEvent *event );

    private slots:
        void setZoomLevel( double zoomLevel );
        void setVisibleSceneRect( const QPolygonF &sceneRect );
};
#endif
	
